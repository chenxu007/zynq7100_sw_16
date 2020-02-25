


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
int CMD_QueryParam(AppBuf * appBuf);
int CMD_Query_SW_Info(AppBuf * appBuf);
void Response_Query_SW_Info(ChanType chType, u32 addr, u32 value);
int CMD_Query_SW_Config(AppBuf * appBuf);
void Response_Query_SW_Config(ChanType chType, u32 addr, u32 value);
int CMD_Query_Port_Config(AppBuf * appBuf);
void Response_Query_Port_Config(ChanType chType, u32 addr, u32 value);
int CMD_Query_Port_Error(AppBuf * appBuf);
void Response_Query_Port_Error(ChanType chType, u32 addr, u32 value);

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
int CMD_QueryParam(AppBuf * appBuf)
{
    u8  fn;
    /* 娴犲穳n瀵拷顬婂Λ锟界叀,閺嗗倹妞傞崣顏呮暜閹镐礁宕熸稉顏勫棘閺佺増鐓＄拠锟�/*/
	fn = *((u8 *)appBuf->pbuf->payLoad+6);
	switch(fn)
	{
	    case FN_QUERY_SWINFO:
			return(CMD_Query_SW_Info(appBuf));
		    break;
	    case FN_QUERY_SWCFG:
			return(CMD_Query_SW_Config(appBuf));
		    break;
	    case FN_QUERY_PORTCFG:
			return(CMD_Query_Port_Config(appBuf));
		    break;
	    case FN_QUERY_PORTSTAT:
			return(CMD_Query_Port_Error(appBuf));
		    break;
		case FN_QUERY_CFGMODE:
			break;			
		default:
		    return 1;
	}

	return 0;
	
}


int CMD_Query_SW_Info(AppBuf * appBuf)
{
    u16 len;
	u32 addr;
	u32 value;

    len = *(u16 *)((u8 *)appBuf->pbuf->payLoad+1);
	if(len != 8)
	{
	    return 1;
	}
	addr = *(u32 *)((u8 *)appBuf->pbuf->payLoad+7);
    if(FCSW_OK!=FCSW_Query_SW_Info(addr, &value, sw_dev))
    {
        return 1;
   	}
	FCSW_printf("Begin Response Query.\n");
	Response_Query_SW_Info(CHAN_TYPE_Eth0, addr, sw_dev);
	
    return 0;
}

void Response_Query_SW_Info(ChanType chType, u32 addr, u32 value)
{
    PBuf * ppbuf;
    AppBuf appBuf;
    
    PBUF_ALLOC(ppbuf);
    APPBUF_POAOUT_INIT((&appBuf), chType, ppbuf, AFN_QUERYPARAM, PRM_RESP, FRAME_SINGLE, 0, 0);

    ppbuf->payLoad[ppbuf->len++] = FN_QUERY_SWINFO;
    
	ppbuf->payLoad[appBuf.pbuf->len++] = (u8)(addr & 0x000000FF);
	ppbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 8) & 0x000000FF);
	ppbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 16) & 0x000000FF);
	ppbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 24) & 0x000000FF);		

	ppbuf->payLoad[appBuf.pbuf->len++] = (u8)(value & 0x000000FF);
	ppbuf->payLoad[appBuf.pbuf->len++] = (u8)((value >> 8) & 0x000000FF);
	ppbuf->payLoad[appBuf.pbuf->len++] = (u8)((value >> 16) & 0x000000FF);
	ppbuf->payLoad[appBuf.pbuf->len++] = (u8)((value >> 24) & 0x000000FF);

	FCSW_RS_PRTL_DO(&appBuf);
    
    PBUF_FREE(ppbuf);
    appBuf.pbuf = NULL;

}

int CMD_Query_SW_Config(AppBuf * appBuf)
{
    u16 len;
	u32 addr;
	u32 value;

    len = *(u16 *)((u8 *)appBuf->pbuf->payLoad+1);
	if(len != 8)
	{
	    return 1;
	}
	addr = *(u32 *)((u8 *)appBuf->pbuf->payLoad+7);
    if(FCSW_OK!=FCSW_Query_SW_Config(addr, &value, sw_dev))
    {
        return 1;
   	}
	
	Response_Query_SW_Config(CHAN_TYPE_Eth0, addr, value);
	
    return 0;
}

void Response_Query_SW_Config(ChanType chType, u32 addr, u32 value)
{
    PBuf * ppbuf;
    AppBuf appBuf;
    
    PBUF_ALLOC(ppbuf);
    APPBUF_POAOUT_INIT((&appBuf), chType, ppbuf, AFN_QUERYPARAM, PRM_RESP, FRAME_SINGLE, 0, 0);

    ppbuf->payLoad[ppbuf->len++] = FN_QUERY_SWCFG;

	ppbuf->payLoad[appBuf.pbuf->len++] = (u8)(addr & 0x000000FF);
	ppbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 8) & 0x000000FF);
	ppbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 16) & 0x000000FF);
	ppbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 24) & 0x000000FF);		

	ppbuf->payLoad[appBuf.pbuf->len++] = (u8)(value & 0x000000FF);
	ppbuf->payLoad[appBuf.pbuf->len++] = (u8)((value >> 8) & 0x000000FF);
	ppbuf->payLoad[appBuf.pbuf->len++] = (u8)((value >> 16) & 0x000000FF);
	ppbuf->payLoad[appBuf.pbuf->len++] = (u8)((value >> 24) & 0x000000FF);

	FCSW_RS_PRTL_DO(&appBuf);
    
    PBUF_FREE(ppbuf);
    appBuf.pbuf = NULL;

}

int CMD_Query_Port_Config(AppBuf * appBuf)
{
    u16 len;
	u32 addr;
	u32 value;

    len = *(u16 *)((u8 *)appBuf->pbuf->payLoad+1);
	if(len != 8)
	{
	    return 1;
	}
	addr = *(u32 *)((u8 *)appBuf->pbuf->payLoad+7);
    if(FCSW_OK!=FCSW_Query_Port_Config(addr, &value, sw_dev))
    {
        return 1;
   	}
	
	Response_Query_Port_Config(CHAN_TYPE_Eth0, addr, value);
	
    return 0;
}

void Response_Query_Port_Config(ChanType chType, u32 addr, u32 value)
{
    PBuf * ppbuf;
    AppBuf appBuf;
    
    PBUF_ALLOC(ppbuf);
    APPBUF_POAOUT_INIT((&appBuf), chType, ppbuf, AFN_QUERYPARAM, PRM_RESP, FRAME_SINGLE, 0, 0);

    ppbuf->payLoad[ppbuf->len++] = FN_QUERY_PORTCFG;

	ppbuf->payLoad[appBuf.pbuf->len++] = (u8)(addr & 0x000000FF);
	ppbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 8) & 0x000000FF);
	ppbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 16) & 0x000000FF);
	ppbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 24) & 0x000000FF);		

	ppbuf->payLoad[appBuf.pbuf->len++] = (u8)(value & 0x000000FF);
	ppbuf->payLoad[appBuf.pbuf->len++] = (u8)((value >> 8) & 0x000000FF);
	ppbuf->payLoad[appBuf.pbuf->len++] = (u8)((value >> 16) & 0x000000FF);
	ppbuf->payLoad[appBuf.pbuf->len++] = (u8)((value >> 24) & 0x000000FF);

	FCSW_RS_PRTL_DO(&appBuf);
    
    PBUF_FREE(ppbuf);
    appBuf.pbuf = NULL;

}



int CMD_Query_Port_Error(AppBuf * appBuf)
{
    u16 len;
	u32 addr;
	u32 value;

    len = *(u16 *)((u8 *)appBuf->pbuf->payLoad+1);
	if(len != 8)
	{
	    return 1;
	}
	addr = *(u32 *)((u8 *)appBuf->pbuf->payLoad+7);
    if(FCSW_OK!=FCSW_Query_Port_Error(addr, &value, sw_dev))
    {
        return 1;
   	}
	
	Response_Query_Port_Error(CHAN_TYPE_Eth0, addr, value);
	
    return 0;
}

void Response_Query_Port_Error(ChanType chType, u32 addr, u32 value)
{
    PBuf * ppbuf;
    AppBuf appBuf;
    
    PBUF_ALLOC(ppbuf);
    APPBUF_POAOUT_INIT((&appBuf), chType, ppbuf, AFN_QUERYPARAM, PRM_RESP, FRAME_SINGLE, 0, 0);

    ppbuf->payLoad[ppbuf->len++] = FN_QUERY_PORTSTAT;
	ppbuf->payLoad[appBuf.pbuf->len++] = FN_QUERY_PORTSTAT;
	ppbuf->payLoad[appBuf.pbuf->len++] = (u8)(addr & 0x000000FF);
	ppbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 8) & 0x000000FF);
	ppbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 16) & 0x000000FF);
	ppbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 24) & 0x000000FF);		

	ppbuf->payLoad[appBuf.pbuf->len++] = (u8)(value & 0x000000FF);
	ppbuf->payLoad[appBuf.pbuf->len++] = (u8)((value >> 8) & 0x000000FF);
	ppbuf->payLoad[appBuf.pbuf->len++] = (u8)((value >> 16) & 0x000000FF);
	ppbuf->payLoad[appBuf.pbuf->len++] = (u8)((value >> 24) & 0x000000FF);

	FCSW_RS_PRTL_DO(&appBuf);
    
    PBUF_FREE(ppbuf);
    appBuf.pbuf = NULL;

}


/**
  * @}
  */
/*********************************End Of File *********************************/

#endif
