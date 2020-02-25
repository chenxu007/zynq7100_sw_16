
/**
  ******************************************************************************
  * @par Copyright (c): ChengDu BinHong Science & Technology Co.,ltd
  * @file    FCSW_RS_PRTL.H
  * @author  chenxu
  * @version V2.0.0
  * @date    2017/07/05
  * @defgroup FCSW_RS_PRTL
  * @ingroup  FCSW_RS_PRTL  
  * @brief   Templet for header files. You can change this templet depend on your
  *          requirments.
  * @par History
  * Date          Owner         BugID/CRID        Contents
  * 2017/07/05    chenxu     	None              Draft
  ****************************************************************************** 
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FCSW_RS_PRTL_H__
#define __FCSW_RS_PRTL_H__

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "FCSW_Mgr\RS\Buf\AppBuf.h"

/** @addtogroup FCSW_RS_PRTL
  * @{
  */
  
/* Exported macro ------------------------------------------------------------*/
/* Frame PRM */
#define PRM_REQU            0x01    /**< 閿熸枻鎷烽敓绉告拝鎷烽敓鏂ゆ嫹閿燂拷            */
#define PRM_RESP            0x00    /**< 閿熸帴璁规嫹绔欓敓鏂ゆ嫹閿熸枻鎷�            */

/*Frame Type Sign*/
#define FRAME_MIDDLE        0x00    /**< 閿熸枻鎷峰抚閿熸枻鎷烽敓鍙》鎷峰抚           */
#define FRAME_END           0x01    /**< 閿熸枻鎷峰抚閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷峰抚           */
#define FRAME_FIRST         0x02    /**< 閿熸枻鎷峰抚閿熸枻鎷烽敓鏂ゆ嫹1甯�           */
#define FRAME_SINGLE        0x03    /**< 閿熸枻鎷峰抚                   */

/* AFN */
#define AFN_CONFIRM         0x00    /**< 纭敓鏂ゆ嫹/閿熸枻鎷烽敓鏂ゆ嫹              */
#define AFN_REGRW           0x01    /**< 閿熸枻鎷蜂綅                   */
#define AFN_CTLCMD          0x02    /**< 閿熸枻鎷疯矾閿熸帴鍙ｇ》鎷烽敓锟�         */
#define AFN_ACTIVEREPORT    0x03    /**< 閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹               */
#define AFN_FILETRANSMIT    0x04    /**< 閿熸枻鎷烽敓鐭鎷烽敓鏂ゆ嫹               */
#define AFN_QUERYPARAM      0x05    /**< 閿熸枻鎷疯閿熸枻鎷烽敓鏂ゆ嫹               */

/* FN */
/* AFN_CONFIRM閿涙N */
#define FN_CON_CONFIRM      0x01
#define FN_CON_DENY    		0x02
#define FN_CON_TERMWISE		0x03

/* AFN_REGRW閿涙N */
#define FN_REG_READ         0x01
#define FN_REG_WRITE        0x02
#define FN_REG_READ_VIR     0x03
#define FN_REG_WRITE_VIR    0x04
#define FN_REG_WRITE_BIT    0x05

/* AFN_CTLCMD閿涙N */
#define FN_CTL_LOGIC_CFG    0x01
#define FN_CTL_NET_CFG      0x02
#define FN_CTL_RTE_SINGLE   0x03
#define FN_CTL_RTE_MULTI    0x04
#define FN_CTL_RTE_BC       0x05
#define FN_CTL_RTE_DEL_BY_SERIAL 0x08
#define FN_CTL_RTE_DEL_SINGLE    0x09
#define FN_CTL_RTE_DEL_MULTI     0x0a
#define FN_CTL_RTE_DEL_BC        0x0b
#define FN_CTL_RTE_DEL_PORT      0x0c
#define FN_CTL_RTE_DEL_ALL       0x0d
#define FN_CTL_MON          0x0e
#define FN_CTL_MON_MSG      0x0f
#define FN_CTL_MON_DEL_MSG  0x10
#define FN_CTL_SYNC_SERVER  0x11
#define FN_CTL_SYNC_CLIENT  0x12
#define FN_CTL_MODE         0x13

/* AFN_QUERYPARAM閿涙N */
#define FN_QUERY_SWINFO     0x01
#define FN_QUERY_SWCFG      0x02
#define FN_QUERY_PORTCFG    0x03
#define FN_QUERY_PORTSTAT   0x04
#define FN_QUERY_CFGMODE    0x05
#define FN_QUERY_SYNC_COUNT 0x06

#define FCSW_RS_PRTL_BSN_DATA_OFF     6u

/* Exported types ------------------------------------------------------------*/

typedef int (*FUNCPTR) ();
typedef enum
{
    CHAN_TYPE_Eth0 = 0,
	CHAN_TYPE_Eth1,
    CHAN_TYPE_SIZE
}ChanType;

  
/* Exported constants --------------------------------------------------------*/


/* Exported Global Variables -------------------------------------------------*/
 /**  Description of global variable  */
extern u8 Flag_UpdateFileTrans;
/* Exported functions ------------------------------------------------------- */
/**  Description of this function  */
void FCSW_RS_PRTL_DO(AppBuf * appBuf); 


#ifdef __cplusplus
}
#endif

#endif /* FCSW_RS_PRTL.H */

/**
  * @}
  */

/*********************************End Of File *********************************/

