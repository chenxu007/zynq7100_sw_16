

/**
  ******************************************************************************
  * @par Copyright (c): ChengDu BinHong Science & Technology Co.,ltd
  * @file    FCSW_RS_PRTL.C
  * @author  chenxu
  * @version V2.0.0
  * @date    2017/07/05
  * @defgroup FCSW_RS_PRTL
  * @ingroup  
  * @brief   Templet for header files. You can change this templet depend on your
  *          requirments.
  * @par History
  * Date      	Owner       BugID/CRID        Contents
  * 2017/06/01	chenxu
  ****************************************************************************** 
  */

/* Includes ------------------------------------------------------------------*/
#include "FCSW_Mgr\RS\FCSW_RS_PRTL.h"
#include "FCSW_Mgr\RS\Buf\AppBuf.h"
#include "FCSW_Type.h"
#include "FCSW_Mgr\Cfg\FCSW.h"
#include <string.h>

#pragma pack(push, 1)

/** @addtogroup FCSW_RS_PRTL
  * @{
  */
  
/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
typedef struct
{
    u8 Flag;               /* 0x68 */
    u16 Len      ;          /* len  */
    u8 Resv : 6;           /* reserve bits */
    u8 Prm : 1;             /* prm */
    u8 Dir : 1;             /* transfer direction bit */
    u8 AFN;                 /* application function code */
    u8 PSEQ : 4;
    u8 CON : 1;             /* need confirm flag */
    u8 FIR_FIN : 2;         /* the first frame flag FIR, the last frame flag FIN 
                                    00 - multi frame, the middle frame 
                                    01 - multi frame, the last frame
                                    10 - multi frame, the first frame
                                    11 - single frame */

}Header;

typedef struct
{
    Header header;

    u8 pfc;
    
    void (*plogin_callback)(void);            /**< 閿熸枻鎷峰綍搴旈敓鏂ゆ嫹姘愰敓鏂ゆ嫹閿熸枻鎷烽敓锟�                                                 */
    void (*phb_callback)(void);               /**< 閿熸枻鎷烽敓鏂ゆ嫹搴旈敓鏂ゆ嫹姘愰敓鏂ゆ嫹閿熸枻鎷烽敓锟�                                                 */
    
}FCSW_RS_PRTLCB;

#pragma pack(pop)

extern int CMD_Request(AppBuf * appBuf);
extern int CMD_RegRW(AppBuf * appBuf);
extern int CMD_CtrlCmd(AppBuf * appBuf);
extern int CMD_QueryParam(AppBuf * appBuf);
extern int CMD_FileTrans(AppBuf * appBuf, u8 con);


static FCSW_RS_PRTLCB FCSW_RS_PRTL;
    
static FUNCPTR AfnFunc[] = 
{
    (FUNCPTR)CMD_Request,              	/* 0x00 */
    (FUNCPTR)CMD_RegRW,              	/* 0x01 */
    (FUNCPTR)CMD_CtrlCmd,              /* 0x02 */
    NULL,         						/* 0x03 */
    (FUNCPTR)CMD_FileTrans,           	/* 0x04 */
    (FUNCPTR)CMD_QueryParam,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,         
    NULL,          
};

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

static void FCSW_RS_PRTL_OUTPUT(AppBuf * appBuf)
{
	  u32 i;
    u8 cs = 0u;
    Header *header = (Header *)appBuf->pbuf->payLoad;   
    
    header->Flag = 0x68;
#ifdef WORK_IN_PC
    header->Dir = 0x00;
#endif
#ifdef WORK_IN_BOARD
    header->Dir = 0x01;
#endif
    header->Resv = 0;
    header->AFN = appBuf->poa.out.afn;
    header->Prm = appBuf->poa.out.prm;
    header->FIR_FIN = appBuf->poa.out.fts;
    header->CON = appBuf->poa.out.con;
    /* 閿熸枻鎷烽敓濮愭姤閿熶茎绛规嫹閿熸枻鎷穕en閿熻娈电鎷峰� */

    /* 閿熸枻鎷烽敓鏂ゆ嫹鏍￠敓鏂ゆ嫹閿熺春S,閿熸枻鎷烽敓鑺傛唻鎷烽敓鏂ゆ嫹鏈熬閿熸枻鎷烽敓鏂ゆ嫹CS閿熼叺鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹 */       
    if(PRM_REQU == header->Prm)
    {
        header->PSEQ = (u8)(FCSW_RS_PRTL.pfc & 0x0F);
        FCSW_RS_PRTL.pfc++;
    }

    else if(PRM_RESP == header->Prm)
    {

        header->PSEQ = FCSW_RS_PRTL.header.PSEQ;
    }
    else
    {
    }

    /* ���㱨�ĳ���len�ֶε�ֵ */
    header->Len = appBuf->pbuf->len - 3;
    
    for (i = 0; i < header->Len; i++)
    {
        cs += *((u8 *) appBuf->pbuf->payLoad + 3 + i);
    }
    appBuf->pbuf->len += 2;

    *(((u8 *) appBuf->pbuf->payLoad + appBuf->pbuf->len) - 2) = cs;
    *(((u8 *) appBuf->pbuf->payLoad + appBuf->pbuf->len) - 1) = 0x16;

    /*//纭敓鏂ゆ嫹callback 娉ㄩ敓鏂ゆ嫹*/
    if(header->CON)
    {
        u8 fn = *((u8*)appBuf->pbuf->payLoad + FCSW_RS_PRTL_BSN_DATA_OFF);

    }
    FCSW_printf("Begin rs send\n");
    FCSW_RS_Send(appBuf);
}

static void FCSW_RS_PRTL_BSN_DO(AppBuf * appBuf)
{
    switch(*((u8 *)appBuf->pbuf->payLoad+4))
    {
        case AFN_QUERYPARAM:
        	FCSW_printf("Begin AFN QUERY\n");
            if(0!=CMD_QueryParam(appBuf))
                assert(0);
            break;
        case AFN_CTLCMD:
            if(0!=CMD_CtrlCmd(appBuf))
                assert(0);
            break;						
        case AFN_FILETRANSMIT:
            if(0!=CMD_FileTrans(appBuf, FCSW_RS_PRTL.header.CON))
                assert(0);
            break;  
        default:

            break;
    }
     
    


   
}

void FCSW_RS_PRTL_DO (AppBuf * appBuf)
{
    u8       i;
    
    
    if(APPBUF_POAOUT == appBuf->bufTyp)
    {
        FCSW_RS_PRTL_OUTPUT(appBuf);
    }

    if(APPBUF_POAIN == appBuf->bufTyp)
    {
        FCSW_RS_PRTL_BSN_DO(appBuf);
    }

    if(APPBUF_RAW == appBuf->bufTyp)
    {
        memcpy(&FCSW_RS_PRTL.header, appBuf->pbuf->payLoad, sizeof(FCSW_RS_PRTL.header));

        /*//閿熸枻鎷烽敓鎴掓柟閿熸枻鎷锋牎閿熸枻鎷�/*/
        if(0x01 == FCSW_RS_PRTL.header.Dir)
        {
            FCSW_RS_PRTL_OUTPUT(appBuf);
        }
        else if(0x00 == FCSW_RS_PRTL.header.Dir)
        {
            FCSW_RS_PRTL_BSN_DO(appBuf);
        }
    }
    
}



/**
  * @}
  */
/*********************************End Of File *********************************/


