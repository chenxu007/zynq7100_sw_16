
/**
  ******************************************************************************
  * @par Copyright (c): ChengDu BinHong Science & Technology Co.,ltd
  * @file    PBuf.C
  * @author  chenxu
  * @version V2.0.0
  * @date    2017/07/05
  * @defgroup USER
  * @ingroup  USER
  * @brief   Templet for header files. You can change this templet depend on your
  *          requirments.
  * @par History
  * Date      	Owner       BugID/CRID        Contents
  * 2017/06/01	chenxu		
  ****************************************************************************** 
  */

/* Includes ------------------------------------------------------------------*/
#include "FCSW_Mgr\RS\Buf\AppBuf.h"
#include "FCSW_Mgr\RS\FCSW_CheckFrame.h"
#include "FCSW_Mgr\RS\FCSW_RS_PRTL.h"
#include "FCSW_Mgr\Cfg\FCSW.h"
#include <assert.h>
#include <string.h>


/** @addtogroup USER
  * @{
  */
  
/* Private macro -------------------------------------------------------------*/
/*  structure of normal frame from the Data Link Layer

 Flag            ( 1 Byte 68H)
 Length          ( 2 Bytes)
 Control         ( 1 Byte )
 DATA            ( n Bytes, depend only from the length)
 CS              ( 1 Byte )
 Flag            ( 1 Byte 16H)
 */

/*  structure of encrypt frame from the Data Link Layer
 AFN             ( 1 Byte application layer function code )
 SEQ             ( 1 Byte frame sequence code )
 DA              ( 2 Bytes information point )
 DT              ( 2 Bytes information type )
 Data unit       ( n Bytes data unit)
 ...             ( DAn, DTn, Data units... )
 DA              ( 2 Bytes information point )
 DT              ( 2 Bytes information type )
 Data unit       ( n Bytes data unit)
 AUX             ( 8/22 Bytes  )

 */

#define FRAME_NO_DATA_LEN               7   /**< 閺冪姵鏆熼幑顔煎敶鐎硅婀侀弫鐗堝祦閺嶅洨銇氶惃鍕Г閺傚洤銇旈梹鍨 */

/* states for scanning incomming bytes from a bytestream */
#define FRAME_STATES_NULL               0    /**< no synchronisation */
#define FRAME_STATES_FLAG_FIR           1    /**< have the first Flag Byte received */
#define FRAME_STATES_LEN_1              2    /**< have the length byte */
#define FRAME_STATES_LEN_2              3    /**< have the length byte */
#define FRAME_STATES_CONTROL            7    /**< have the control byte */
#define FRAME_STATES_LINK_USER_DATA     9    /**< have the link user data bytes */
#define FRAME_STATES_CS                 10   /**< wait for the CS */
#define FRAME_STATES_END                11   /**< wait for the 16H */
#define FRAME_STATES_COMPLETE           12   /**< complete frame */


#define CHKFRAME_TIMEOUT                10   /**< 閹躲儲鏋冪搾鍛閺冨爼妫挎妯款吇10缁夛拷 */



/* Private variables ---------------------------------------------------------*/
static CheckFrameCB thechk[CHAN_TYPE_SIZE];
static u8 thechk_id = 0u;


/* Private typedef -----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/

/**
  * @brief   This function   
  * @param[in]   inArgName  input argument description. 
  * @param[out]  outArgName output argument description.  
  * @retval  FCSW_OK      Success 
  * @retval  ERROR   Failed
  * @par History
  * Date          Owner         BugID/CRID        Contents
  * 2015/02/05    hezongbin     None              Draft 
  */

static void
chk_frm(CheckFrameCB *chk,
        ChanType type,
        u8 *rxBuf,
        u32 rxLen);

CheckFrameCB *
CheckFrameInit(void)
{
    if (thechk_id >= CHAN_TYPE_SIZE)
    {
        return NULL;
    }
    thechk[thechk_id].frameState = FRAME_STATES_NULL;
/*   thechk[thechk_id].updateTime = time(NULL);*/
    thechk[thechk_id].overTime = CHKFRAME_TIMEOUT;
    thechk[thechk_id].dLen = 0;
    thechk[thechk_id].cfmLen = 0;
    thechk[thechk_id].ppbuf = NULL;    

    thechk[thechk_id].pchkfrm = chk_frm;
    thechk_id++;
    return &thechk[thechk_id - 1];
}


static void
chk_frm(CheckFrameCB *chk,
        ChanType chanType,
        u8 *rxBuf,
        u32 rxLen)
{
    /* 婵″倹鐏夊鑼病鐎瑰本鍨氶惃鍕�閸掓瑩鍣搁弬鏉跨磻婵拷*/
    if (chk->frameState == FRAME_STATES_COMPLETE)
    {
        chk->frameState = FRAME_STATES_NULL;
    }
    /* 婵″倹鐏夋穱锟犱壕閸欐ê瀵查崚娆撳櫢閺傛澘绱戞慨锟�/
    if (chanType != chk->chanType)
    {
        chk->chanType = chanType;
        chk->frameState = FRAME_STATES_NULL;
    }

    /* 婵″倹鐏夌搾鍛閸掓瑩鍣搁弬鏉跨磻婵拷*/
/*     if (abs(time(NULL) - chk->updateTime) > chk->overTime)
//     {
//         chk->frameState = FRAME_STATES_NULL;
//     }*/

    while (rxLen > 0)
    {

        switch (chk->frameState)
        {
            case FRAME_STATES_NULL:

                if (*rxBuf == 0x68)
                {
                    /* 婵″倹鐏夋稊瀣閻㈠疇顕潻鍥︾稻閺堫亝顥戝ù瀣╄礋鐎瑰本鏆ｉ崚娆忓帥free */
                    if (chk->ppbuf != NULL)
                    {
                        PBUF_FREE(chk->ppbuf);
                    }

                    /* 閹恒儲鏁圭敮褍绱戞慨瀣剁礉婵″倹鐏塸buf娑撹櫣鈹栭崚娆戞暤鐠囷拷*/
                    PBUF_ALLOC(chk->ppbuf);
                    
                    chk->pbufPos = 0;

                    chk->frameState = FRAME_STATES_LEN_1;
/*//                    chk->updateTime = time(NULL);*/
                    chk->dLen = 0;
                }
                break;

            case FRAME_STATES_LEN_1: /* 濡拷绁碙EN閻ㄥ嫪缍嗙�妤勫Ν */
                chk->frameState = FRAME_STATES_LEN_2;/* 娑撳搫鍚嬬�閫涘瘜缁旀瑤绗夊Λ锟界ゴ鐟欏嫮瀹崇猾璇茬� */
                chk->dLen = *rxBuf ;
                break;

            case FRAME_STATES_LEN_2: /* 濡拷绁碙EN閻ㄥ嫰鐝�妤勫Ν */
                chk->dLen += ((u32)*rxBuf << 8u);
                if (chk->dLen > (PBUF_BYTE_MAX - 8))
                {
                    chk->frameState = FRAME_STATES_NULL;
                    PBUF_FREE(chk->ppbuf);
                    FCSW_printf("Frame error. L1 ");
									/*  //assert(0);*/
									  
                }
                else
                {
                	FCSW_printf("dataLen=0x%02x",chk->dLen);
                    chk->frameState = FRAME_STATES_CONTROL;
                }
                break;


            case FRAME_STATES_CONTROL:
                chk->cs = *rxBuf;
                chk->frameState = FRAME_STATES_LINK_USER_DATA;/* 娑撳秷鍏樺Λ锟界ゴ閺傜懓鎮滈敍灞芥礈娑撹櫣楠囬懕鏃�箒娑撳﹨顢戦幎銉︽瀮 */
                break;

            case FRAME_STATES_LINK_USER_DATA:
                chk->cs += *rxBuf;
                if (chk->pbufPos == (2+chk->dLen))
                {
                    chk->frameState = FRAME_STATES_CS;
                }
                break;

            case FRAME_STATES_CS:
                if (*rxBuf == chk->cs)
                {
                    chk->frameState = FRAME_STATES_END;
                }
                else
                {
                    chk->frameState = FRAME_STATES_NULL;
                    FCSW_printf("chk->cs=0x%02x,rxBuf=0x%02x\n",chk->cs,*rxBuf);
                    PBUF_FREE(chk->ppbuf);
                    FCSW_printf("Frame error. CS ");
                  
									  /*//assert(0);*/
									  
                }
                break;

            case FRAME_STATES_END:
                if (*rxBuf == 0x16)
                {
                    chk->frameState = FRAME_STATES_COMPLETE;
                    FCSW_printf("Frame ok.\n");
                }
                else
                {
                    chk->frameState = FRAME_STATES_NULL;
                    PBUF_FREE(chk->ppbuf);
                    FCSW_printf("Frame error. END ");
									  /*//assert(0);*/
									  
                }
                break;
            default:
                break;
        }

        if (chk->frameState != FRAME_STATES_NULL)
        {
            memcpy((u8 *) chk->ppbuf->payLoad + chk->pbufPos, rxBuf, 1);
            chk->pbufPos++;
        }

        /* 鐎瑰本鏆ｉ幎銉︽瀮閿涘矁鐨熼悽銊ヮ樀閻炲棗鍤遍弫鐗堝复閸欙拷*/
        if (chk->frameState == FRAME_STATES_COMPLETE)
        {
        #ifdef WORK_IN_BOARD
            AppBuf rxAppBuf;
					
            chk->ppbuf->len = chk->pbufPos;

            APPBUF_RAW_INIT((&rxAppBuf), chanType, chk->ppbuf);

            FCSW_RS_PRTL_DO(&rxAppBuf);

            PBUF_FREE(chk->ppbuf);

		#endif

            chk->frameState = FRAME_STATES_NULL;
            chk->pbufPos = 0;
            chk->dLen = 0;
        }
        rxLen--;
        rxBuf++;
    }
    return;
}


/**
  * @}
  */
/*********************************End Of File *********************************/
















