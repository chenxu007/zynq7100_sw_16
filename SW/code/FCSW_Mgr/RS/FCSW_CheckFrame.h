/**
  ******************************************************************************
  * @par Copyright (c): ChengDu BinHong Science & Technology Co.,ltd
  * @file    CHECKFRAME.H
  * @author  chenxu
  * @version V2.0.0
  * @date    2017/07/05
  * @defgroup UTM
  * @ingroup  UTM  
  * @brief   Templet for header files. You can change this templet depend on your
  *          requirments.
  * @par History
  * Date          Owner         BugID/CRID        Contents
  * 2017/07/05    chenxu     	None              Draft
  ****************************************************************************** 
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CHKFRAME_H_
#define CHKFRAME_H_

/* Includes ------------------------------------------------------------------*/
#include "FCSW_Mgr\RS\FCSW_RS_PRTL.h"

/** @addtogroup UTM
  * @{
  */
  
/* Exported macro ------------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/
 
typedef struct CheckFrameCB_
{
    void (* pchkfrm)(struct CheckFrameCB_ *chk, ChanType type, u8 *recvBuf, u32 recvLen);

    u8 frameState;                       /* 鎶ユ枃鐘舵�                                  */
/*    time_t updateTime;                 /* 鏇存柊鏃堕棿                                  */
    u8 overTime;                         /* 鎶ユ枃绛夊緟瓒呮椂鏃堕棿                          */
    u32 dLen;                            /* 鐢ㄦ埛鏁版嵁闀垮害,瑙ｆ瀽鑷姤鏂囬暱搴﹀煙             */
    u32 cfmLen;                          /* 鐢ㄦ埛鏁版嵁纭闀垮害锛岀敤浜庤В鏋愭暟鎹暱搴︿复鏃剁敤  */

    ChanType chanType;                   /* 鎶ユ枃淇￠亾鏉ユ簮绫诲瀷                          */
    PBuf *ppbuf;                         /* 鎶ユ枃姹�                                   */
    u16 pbufPos;                         /* 鎶ユ枃鎺ユ敹瀛楄妭鍋忕Щ浣嶇疆                      */
    u8 cs;                               /* 鏍￠獙鍜�                                   */
} CheckFrameCB;

/* Exported constants --------------------------------------------------------*/


/* Exported Global Variables -------------------------------------------------*/
 /**  Description of global variable  */

/* Exported functions ------------------------------------------------------- */
/**  Description of this function  */
extern CheckFrameCB *CheckFrameInit(void);

#endif /* CHECKFRAME.H */

/**
  * @}
  */

/*********************************End Of File *********************************/


