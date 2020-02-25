

/**
  ******************************************************************************
  * @par Copyright (c): ChengDu BinHong Science & Technology Co.,ltd
  * @file    C_File_Templet.C
  * @author  chenxu
  * @version V2.0.0
  * @date    2017/07/05
  * @defgroup UTM
  * @ingroup  DAL
  * @brief   Templet for header files. You can change this templet depend on your
  *          requirments.
  * @par History
  * Date      	Owner       BugID/CRID        Contents
  * 2017/06/01	chenxu
  ****************************************************************************** 
  */

/* Includes ------------------------------------------------------------------*/
#include "FCSW_Type.h"
#include "FCSW_Mgr\RS\Buf\AppBuf.h"
#include "FCSW_Mgr\RS\FCSW_RS_PRTL.h"
#include "FCSW_Mgr\Cfg\FCSW.h"
#if defined WORK_IN_BOARD || defined WORK_IN_PC
/** @addtogroup UTM
  * @{
  */
  
/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
int CMD_CtrlCmd(AppBuf * appBuf);
int CMD_Ctrl_LogicCFG(AppBuf * appBuf);
void Response_Ctrl_LogicCFG(ChanType chType, u32 addr, u32 value);
int CMD_Ctrl_NetCFG(AppBuf * appBuf);
void Response_Ctrl_NetCFG(ChanType chType, u32 addr, u32 value);
int CMD_Ctrl_RTE_Single(AppBuf * appBuf);
void Response_Ctrl_RTE_Single(ChanType chType, u8 con);
int CMD_Ctrl_RTE_Multi(AppBuf * appBuf);
void Response_Ctrl_RTE_Multi(ChanType chType, u32 addr, u32 value);
int CMD_Ctrl_RTE_Broadcast(AppBuf * appBuf);
void Response_Ctrl_RTE_Broadcast(ChanType chType, u32 addr, u32 value);
int CMD_Ctrl_RTE_DelBySerial(AppBuf * appBuf);
void Response_Ctrl_RTE_DelBySerial(ChanType chType, u32 addr, u32 value);
int CMD_Ctrl_RTE_DelSingle(AppBuf * appBuf);
void Response_Ctrl_RTE_DelSingle(ChanType chType, u32 addr, u32 value);
int CMD_Ctrl_RTE_DelMulti(AppBuf * appBuf);
void Response_Ctrl_RTE_DelMulti(ChanType chType, u32 addr, u32 value);
int CMD_Ctrl_RTE_DelBroadcast(AppBuf * appBuf);
void Response_Ctrl_RTE_DelBroadcast(ChanType chType, u32 addr, u32 value);
int CMD_Ctrl_RTE_DelPort(AppBuf * appBuf);
void Response_Ctrl_RTE_DelPort(ChanType chType, u32 addr, u32 value);
int CMD_Ctrl_RTE_DelAll(AppBuf * appBuf);
void Response_Ctrl_RTE_DelAll(ChanType chType, u32 addr, u32 value);
int CMD_Ctrl_Mon(AppBuf * appBuf);
void Response_Ctrl_Mon(ChanType chType, u32 addr, u32 value);
int CMD_Ctrl_MonMsg(AppBuf * appBuf);
void Response_Ctrl_MonMsg(ChanType chType, u32 addr, u32 value);
int CMD_Ctrl_MonDelMsg(AppBuf * appBuf);
void Response_Ctrl_MonDelMsg(ChanType chType, u32 addr, u32 value);
int CMD_Ctrl_SyncServer(AppBuf * appBuf);
void Response_Ctrl_SyncServer(ChanType chType, u32 addr, u32 value);
int CMD_Ctrl_SyncClient(AppBuf * appBuf);
void Response_Ctrl_SyncClient(ChanType chType, u32 addr, u32 value);
int CMD_Ctrl_Mode(AppBuf * appBuf);
void Response_Ctrl_Mode(ChanType chType, u32 addr, u32 value);
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
int CMD_CtrlCmd(AppBuf * appBuf)
{
    u8  fn;
    /* 浠巉n寮�妫�煡 */
	fn = *((u8 *)appBuf->pbuf->payLoad+6);
	switch(fn)
	{
	    case FN_CTL_LOGIC_CFG:
			return(CMD_Ctrl_LogicCFG(appBuf));
		    break;
	    case FN_CTL_NET_CFG:
			return(CMD_Ctrl_NetCFG(appBuf));
		    break;
	    case FN_CTL_RTE_SINGLE:
			return(CMD_Ctrl_RTE_Single(appBuf));
		    break;
	    case FN_CTL_RTE_MULTI:
			return(CMD_Ctrl_RTE_Multi(appBuf));
		    break;
		case FN_CTL_RTE_BC:
			return(CMD_Ctrl_RTE_Broadcast(appBuf));
			break;
		case FN_CTL_RTE_DEL_BY_SERIAL:
			return(CMD_Ctrl_RTE_DelBySerial(appBuf));
			break;	
		case FN_CTL_RTE_DEL_SINGLE:
			return(CMD_Ctrl_RTE_DelSingle(appBuf));
			break;
		case FN_CTL_RTE_DEL_MULTI:
			return(CMD_Ctrl_RTE_DelMulti(appBuf));
			break;	
		case FN_CTL_RTE_DEL_BC:
			return(CMD_Ctrl_RTE_DelBroadcast(appBuf));
			break;
		case FN_CTL_RTE_DEL_PORT:
			return(CMD_Ctrl_RTE_DelPort(appBuf));
			break;	
		case FN_CTL_RTE_DEL_ALL:
			return(CMD_Ctrl_RTE_DelAll(appBuf));
			break;
		case FN_CTL_MON:
			return(CMD_Ctrl_Mon(appBuf));
			break;
		case FN_CTL_MON_MSG:
			return(CMD_Ctrl_MonMsg(appBuf));
			break;
		case FN_CTL_MON_DEL_MSG:
			return(CMD_Ctrl_MonDelMsg(appBuf));
			break;	
		case FN_CTL_SYNC_SERVER:
			return(CMD_Ctrl_SyncServer(appBuf));
			break;
		case FN_CTL_SYNC_CLIENT:
			return(CMD_Ctrl_SyncClient(appBuf));
			break;
		case FN_CTL_MODE:
			return(CMD_Ctrl_Mode(appBuf));
			break;

		default:
		    return 1;
	}

	return 0;
	
}


int CMD_Ctrl_LogicCFG(AppBuf * appBuf)
{

    return 0;
}

void Response_Ctrl_LogicCFG(ChanType chType, u32 addr, u32 value)
{


}

int CMD_Ctrl_NetCFG(AppBuf * appBuf)
{

    return 0;
}

void Response_Ctrl_NetCFG(ChanType chType, u32 addr, u32 value)
{


}
/* 閰嶇疆鍗曟挱璺敱鐨勬暟鎹牸寮�
0   1  2   3      4     5     6       7      8      9    10   11  12
68h L1 L2  C  02h(AFN) SEQ 03h(FN) inport outport did1 did2 did3 did4 CS 16h*/
int CMD_Ctrl_RTE_Single(AppBuf * appBuf)
{
    u16 len;
	u8  inport;
	u8  outport;
	u32 did;

    len = *((u8 *)appBuf->pbuf->payLoad+1);
	len += 0x100*(*(u8 *)(appBuf->pbuf->payLoad+2));
	if(len != 12)
	{
	    return 1;
	}
	/* 鑾峰彇鍙傛暟 */
	inport = *((u8 *)appBuf->pbuf->payLoad+7);
	outport = *((u8 *)appBuf->pbuf->payLoad+8);
	did = *(u8 *)(appBuf->pbuf->payLoad+9);
	did += 0x100*(*(u8 *)(appBuf->pbuf->payLoad+10));
	did += 0x10000*(*(u8 *)(appBuf->pbuf->payLoad+11));
	did += 0x1000000*(*(u8 *)(appBuf->pbuf->payLoad+12));

	/* 閰嶇疆鍙傛暟 */
    if(FCSW_OK!= FCSW_RTE_Config_Single(inport, outport, did, sw_dev))
	{
	    Response_Ctrl_RTE_Single(CHAN_TYPE_Eth0, FN_CON_DENY);
	    return 1;
	}
    Response_Ctrl_RTE_Single(CHAN_TYPE_Eth0, FN_CON_CONFIRM);
    
    return 0;
}

void Response_Ctrl_RTE_Single(ChanType chType, u8 con)
{
    PBuf * ppbuf;
    AppBuf appBuf;
    
    PBUF_ALLOC(ppbuf);
    APPBUF_POAOUT_INIT((&appBuf), chType, ppbuf, AFN_CTLCMD, PRM_RESP, FRAME_SINGLE, 0, 0);

    ppbuf->payLoad[ppbuf->len++] = con;

	FCSW_RS_PRTL_DO(&appBuf);
    
    PBUF_FREE(ppbuf);
    appBuf.pbuf = NULL;

}

int CMD_Ctrl_RTE_Multi(AppBuf * appBuf)
{

    return 0;
}

void Response_Ctrl_RTE_Multi(ChanType chType, u32 addr, u32 value)
{


}

int CMD_Ctrl_RTE_Broadcast(AppBuf * appBuf)
{

    return 0;
}

void Response_Ctrl_RTE_Broadcast(ChanType chType, u32 addr, u32 value)
{


}

int CMD_Ctrl_RTE_DelBySerial(AppBuf * appBuf)
{

    return 0;
}

void Response_Ctrl_RTE_DelBySerial(ChanType chType, u32 addr, u32 value)
{


}

int CMD_Ctrl_RTE_DelSingle(AppBuf * appBuf)
{

    return 0;
}

void Response_Ctrl_RTE_DelSingle(ChanType chType, u32 addr, u32 value)
{


}

int CMD_Ctrl_RTE_DelMulti(AppBuf * appBuf)
{

    return 0;
}

void Response_Ctrl_RTE_DelMulti(ChanType chType, u32 addr, u32 value)
{


}

int CMD_Ctrl_RTE_DelBroadcast(AppBuf * appBuf)
{

    return 0;
}

void Response_Ctrl_RTE_DelBroadcast(ChanType chType, u32 addr, u32 value)
{


}

int CMD_Ctrl_RTE_DelPort(AppBuf * appBuf)
{

    return 0;
}

void Response_Ctrl_RTE_DelPort(ChanType chType, u32 addr, u32 value)
{


}

int CMD_Ctrl_RTE_DelAll(AppBuf * appBuf)
{

    return 0;
}

void Response_Ctrl_RTE_DelAll(ChanType chType, u32 addr, u32 value)
{


}

int CMD_Ctrl_Mon(AppBuf * appBuf)
{

    return 0;
}

void Response_Ctrl_Mon(ChanType chType, u32 addr, u32 value)
{


}

int CMD_Ctrl_MonMsg(AppBuf * appBuf)
{

    return 0;
}

void Response_Ctrl_MonMsg(ChanType chType, u32 addr, u32 value)
{


}

int CMD_Ctrl_MonDelMsg(AppBuf * appBuf)
{

    return 0;
}

void Response_Ctrl_MonDelMsg(ChanType chType, u32 addr, u32 value)
{


}
int CMD_Ctrl_SyncServer(AppBuf * appBuf)
{

    return 0;
}

void Response_Ctrl_SyncServer(ChanType chType, u32 addr, u32 value)
{


}
int CMD_Ctrl_SyncClient(AppBuf * appBuf)
{

    return 0;
}

void Response_Ctrl_SyncClient(ChanType chType, u32 addr, u32 value)
{


}

int CMD_Ctrl_Mode(AppBuf * appBuf)
{

    return 0;
}

void Response_Ctrl_Mode(ChanType chType, u32 addr, u32 value)
{


}

#endif

