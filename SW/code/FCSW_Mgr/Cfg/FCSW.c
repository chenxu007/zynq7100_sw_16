#include "FCSW.h"
#include "FCSW_Mgr\RS\Buf\PBuf.h"
#include "FCSW_Mgr\RS\Buf\AppBuf.h"
#include "FCSW_Mgr\Reg\FCSW_REG.h"
#include "FCSW_Mgr\Rte\FCSW_RouteMgr.h"

#ifdef WORK_IN_BOARD
#include <vxWorks.h>


#include <stdlib.h>
#endif

#ifdef WORK_IN_PC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BOARD_PC_Recv 
#endif


FCSW_STRU* sw_dev;


FCSW_STRU* FCSW_Dev_Init(void)
{
#ifdef WORK_IN_PC
    FCSW_STRU     *sw_dev;

	sw_dev = malloc(sizeof(FCSW_STRU));
    
    strncpy_s(sw_dev->swipv4, sizeof(sw_dev->swipv4), SW_IPV4, sizeof(SW_IPV4));
	sw_dev->swport = SW_IP_PORT;

	return sw_dev;

#endif
#ifdef WORK_IN_BOARD

    FCSW_STRU     *sw_dev;

	u32            val;
	u32            port_total;
	u32            port_rte_total;
    


	sw_dev = malloc(sizeof(FCSW_STRU));
	

	sw_dev->swport = SW_IP_PORT;

	sw_dev->RegBase = 0x40000000;
	sw_dev->sw_path = FCSW_PATH_AXI;

	FCSW_Dbg_printf("FCSW test vitual reg Write and Read:\n");
	FCSW_Dbg_printf("FCSW write data 0x12345678 to addr 0x1060\n");
	if(FCSW_OK != FCSW_Reg_Write_Virtual(0x1060, 0x12345678, sw_dev))
	{
		FCSW_Dbg_printf("FCSW write virtual reg error\n");
	}
	else
	{
		FCSW_Dbg_printf("FCSW read data from addr 0\n");
		if(FCSW_OK != FCSW_Reg_Read_Virtual(0x1060, &val, sw_dev))
		{
			FCSW_Dbg_printf("FCSW read virtual reg error\n");
		}
		FCSW_Dbg_printf("FCSW read val=0x%08x\n",val);
	}
	
	

	
	
	sw_dev->sw_info = malloc(sizeof(FCSW_SW_INFO_STRU));
	if(FCSW_OK != FCSW_Query_SW_Info(FCSW_REG_DEV_ID_CDR, &val, sw_dev))
	{
	     FCSW_printf("Query SW info error\n");
		 return 0;		    
	}
	FCSW_Dbg_printf("FCSW_REG_DEV_ID_CDR val = 0x%08x\n",val);
	sw_dev->sw_info->deviceid = val/0x10000;
	sw_dev->sw_info->vendorid = val%0x10000;
	if(FCSW_OK != FCSW_Query_SW_Info(FCSW_REG_DEV_VERSION_CDR, &val, sw_dev))
	{
	     FCSW_printf("Query SW info error\n");
		 return 0;		    
	}
	sw_dev->sw_info->version[0] = val/0x1000000;
	sw_dev->sw_info->version[1] = (val/0x10000)%0x100;
	sw_dev->sw_info->version[2] = (val%0x10000)/0x100;
	if(FCSW_OK != FCSW_Query_SW_Info(FCSW_REG_DEV_FEAT_CDR, &val, sw_dev))
	{
	     FCSW_printf("Query SW info error\n");
		 return 0;		    
	}
	FCSW_Dbg_printf("FCSW_REG_DEV_FEAT_CDR val = 0x%08x\n",val);
	sw_dev->sw_info->sw_support = (u8)((val&&0x80000000)>>31);
	sw_dev->sw_info->speed = (u8)((val&&0x7C000000)>>26);
	sw_dev->sw_info->monitor_support = (u8)((val&&0x01000000)>>25);
	sw_dev->sw_info->bcast_support = (u8)((val&&0x00800000)>>24);
	sw_dev->sw_info->mcast_support = (u8)((val&&0x00400000)>>23);
	sw_dev->sw_info->bb_credit_support = (u8)((val&&0x00200000)>>22);
	sw_dev->sw_info->redundant_support = (u8)((val&&0x00100000)>>21);
	sw_dev->sw_info->priority_support = (u8)((val&&0x00080000)>>20);
	sw_dev->sw_info->f_port_support = (u8)((val&&0x00040000)>>19);
	sw_dev->sw_info->e_port_support = (u8)((val&&0x00020000)>>18);
	sw_dev->sw_info->synx_support = (u8)((val&&0x00010000)>>17);
	sw_dev->sw_info->syny_support = (u8)((val&&0x00008000)>>16);
	sw_dev->sw_info->synz_support = (u8)((val&&0x00004000)>>15);
	sw_dev->sw_info->err_capture_support = (u8)((val&&0x00002000)>>14);
	sw_dev->sw_info->clk_server_support = (u8)((val&&0x00001000)>>13);
	sw_dev->sw_info->clk_client_support = (u8)((val&&0x00000800)>>12);
	sw_dev->sw_info->isl_trunk_support = (u8)((val&&0x00000400)>>11);
	if(FCSW_OK != FCSW_Query_SW_Info(FCSW_REG_DEV_PORT_INF_CDR, &val, sw_dev))
	{
	     FCSW_printf("Query SW info error\n");
		 return 0;		    
	}
	FCSW_Dbg_printf("FCSW_REG_DEV_PORT_INF_CDR val = 0x%08x\n",val);
	sw_dev->sw_info->port_total = (u8)((val&&0x0000FF00)>>8);
	FCSW_Dbg_printf("SW Port Num = %d\n",sw_dev->sw_info->port_total);
	sw_dev->sw_info->monitor_port_num = (u8)((val&&0x000000FF)>>0);
	if(FCSW_OK != FCSW_Query_SW_Info(FCSW_REG_DEV_LUT_SIZE_CDR, &val, sw_dev))
	{
	     FCSW_printf("Query SW info error\n");
		 return 0;		    
	}
	FCSW_Dbg_printf("FCSW_REG_DEV_LUT_SIZE_CDR val = 0x%08x\n",val);
	sw_dev->sw_info->mc_lut_size = (u8)((val&&0x000000FF)>>0);
	FCSW_Dbg_printf("SW Port lut size = %d\n",sw_dev->sw_info->mc_lut_size);
	
	if((sw_dev->sw_info->deviceid != SW_DEVICE_ID)||(sw_dev->sw_info->vendorid != SW_VENDER_ID))
	{
	     FCSW_printf("SW ID error\n");
	     FCSW_printf("deviceid=0x%x,vendorid=0x%x\n",sw_dev->sw_info->deviceid,sw_dev->sw_info->vendorid);	    
	}

	
    sw_dev->sw_cfg = malloc(sizeof(FCSW_SW_CFG_FCSW_STATUS_STRU));
    	
	
    port_total = sw_dev->sw_info->port_total;
	sw_dev->sw_port_cfg = malloc(port_total*sizeof(FCSW_PORT_STAU_STRU));

	
	port_rte_total = sw_dev->sw_info->mc_lut_size;
	val = 0;
	FCSW_RTE_Manager_Init(sw_dev);
	
	FCSW_RS_Init();

    sw_dev->prepareOK_flag;
	return sw_dev;
#endif
}



FCSW_STATUS FCSW_Reg_Read(u32 addr, u32 *value, FCSW_STRU *sw_dev)
{
#ifdef WORK_IN_PC
	AppBuf      appBuf;
	PBuf *      ppbuffer;
	ChanType	chanType;
	u32 		evalue;
	u8			afn,fn;
	u32         raddr;


	chanType = sw_dev->sw_prtl_path;


	PBUF_ALLOC(ppbuffer);

	APPBUF_POAOUT_INIT((&appBuf), chanType, ppbuffer,
			AFN_REGRW, PRM_REQU, FRAME_SINGLE, 0, NULL);


	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = FN_REG_READ;
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)(addr & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 8) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 16) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 24) & 0x000000FF); 

	FCSW_RS_PRTL_DO(&appBuf);

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;


	PBUF_ALLOC(ppbuffer);

	if(FCSW_OK != FCSW_RS_Recv(ppbuffer->payLoad))
	{
		FCSW_printf("Recieve frame error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}			

	afn = *(u8 *)(ppbuffer->payLoad+ 4);
	fn = *(u8 *)(ppbuffer->payLoad+ 6); 
	

	if((afn != AFN_REGRW)||(fn != FN_REG_READ))
	{
		FCSW_printf("Recieve frame fn error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}


    raddr = *(u8 *)(ppbuffer->payLoad+7);
	raddr += 0x100*(*(u8 *)(ppbuffer->payLoad+8));
	raddr += 0x10000 * (*(u8 *)(ppbuffer->payLoad + 9));
	raddr += 0x1000000 * (*(u8 *)(ppbuffer->payLoad + 10));
	if(raddr!=addr)
	{
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;	
		return FCSW_ERROR;
	}
	evalue = *(u8 *)(ppbuffer->payLoad+11);
	evalue += 0x100*(*(u8 *)(ppbuffer->payLoad+12));
	evalue += 0x10000 * (*(u8 *)(ppbuffer->payLoad + 13));
	evalue += 0x1000000 * (*(u8 *)(ppbuffer->payLoad + 14));
	*value = evalue;
	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;
	return(FCSW_OK);
#endif

#ifdef WORK_IN_BOARD
    u32 val;

    if(sw_dev->sw_path == FCSW_PATH_LOCALBUS)
	{
	    val = FCSW_Reg_Read_LB(addr+sw_dev->RegBase);
	}
	else if(sw_dev->sw_path == FCSW_PATH_AXI)
	{
		val = FCSW_Reg_Read_AXI(addr+sw_dev->RegBase);
	}
	else
	{
	    return FCSW_ERROR;
	}
    *value = val;
	return FCSW_OK;
#endif

}
FCSW_STATUS FCSW_Reg_Write(u32 addr, u32 value, FCSW_STRU *sw_dev)
{
#ifdef WORK_IN_PC
	AppBuf      appBuf;
	PBuf *      ppbuffer;
	ChanType	chanType;
	u32 		evalue;
	u8			afn,fn;
	u32         raddr;


	chanType = sw_dev->sw_prtl_path;


	PBUF_ALLOC(ppbuffer);

	APPBUF_POAOUT_INIT((&appBuf), chanType, ppbuffer,
			AFN_REGRW, PRM_REQU, FRAME_SINGLE, 0, NULL);

	
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = FN_REG_WRITE;
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)(addr & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 8) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 16) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 24) & 0x000000FF); 
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)(value & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((value >> 8) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((value >> 16) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((value >> 24) & 0x000000FF); 

	FCSW_RS_PRTL_DO(&appBuf);

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;


	PBUF_ALLOC(ppbuffer);

	if(FCSW_OK != FCSW_RS_Recv(ppbuffer->payLoad))
	{
		FCSW_printf("Recieve frame error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}			

	

	afn = *(u8 *)(ppbuffer->payLoad+ 4);
	fn = *(u8 *)(ppbuffer->payLoad+ 6); 
	

	if((afn != AFN_REGRW)||(fn != FN_CON_CONFIRM))
	{
		FCSW_printf("Recieve frame fn error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;
	return(FCSW_OK);			

#endif
#ifdef WORK_IN_BOARD
    if(sw_dev->sw_path == FCSW_PATH_LOCALBUS)
	{
	    FCSW_Reg_Write_LB(addr+sw_dev->RegBase,value);
	}
	else if(sw_dev->sw_path == FCSW_PATH_AXI)
	{
		FCSW_Reg_Write_AXI(addr+sw_dev->RegBase,value);
	}
	else
	{
	    return FCSW_ERROR;
	}

	return FCSW_OK;

#endif

}

FCSW_STATUS FCSW_Reg_Read_Virtual(u32 addr, u32 *value, FCSW_STRU *sw_dev)
{
#ifdef WORK_IN_PC
	AppBuf      appBuf;
	PBuf *      ppbuffer;
	ChanType	chanType;
	u32 		evalue;
	u8			afn,fn;
	u32         raddr;


	chanType = sw_dev->sw_prtl_path;

	
	PBUF_ALLOC(ppbuffer);

	APPBUF_POAOUT_INIT((&appBuf), chanType, ppbuffer,
			AFN_REGRW, PRM_REQU, FRAME_SINGLE, 0, NULL);

	
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = FN_REG_READ_VIR;
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)(addr & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 8) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 16) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 24) & 0x000000FF); 

	FCSW_RS_PRTL_DO(&appBuf);

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;

	
	PBUF_ALLOC(ppbuffer);

	if(FCSW_OK != FCSW_RS_Recv(ppbuffer->payLoad))
	{
		FCSW_printf("Recieve frame error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}			

	

	afn = *(u8 *)(ppbuffer->payLoad+ 4);
	fn = *(u8 *)(ppbuffer->payLoad+ 6); 
	
	
	if((afn != AFN_REGRW)||(fn != FN_REG_READ_VIR))
	{
		FCSW_printf("Recieve frame fn error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}

	raddr = *(ppbuffer->payLoad + 7);
	raddr += 0x100 * (*(u8 *)(ppbuffer->payLoad + 8));
	raddr += 0x10000 * (*(u8 *)(ppbuffer->payLoad + 9));
	raddr += 0x1000000 * (*(u8 *)(ppbuffer->payLoad + 10));
	if(raddr!=addr)
	{
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;	
		return FCSW_ERROR;
	}
	evalue = *(ppbuffer->payLoad+11);
	evalue += 0x100 * (*(u8 *)(ppbuffer->payLoad + 12));
	evalue += 0x10000 * (*(u8 *)(ppbuffer->payLoad + 13));
	evalue += 0x1000000 * (*(u8 *)(ppbuffer->payLoad + 14));
	*value = evalue;
	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;
	return(FCSW_OK);		

#endif
#ifdef WORK_IN_BOARD
    u32 val;
    u32 tmp;
    if(FCSW_OK!=FCSW_Reg_Write(0x00000204,addr,sw_dev))
		return FCSW_ERROR;
	if(FCSW_OK!=FCSW_Reg_Write(0x00000200,1,sw_dev))
		return FCSW_ERROR;
	vxbUsDelay(1);

	if(FCSW_OK!=FCSW_Reg_Read(0x0000020c,&val,sw_dev))
		return FCSW_ERROR;
    *value = val;
	return FCSW_OK;
#endif

}
FCSW_STATUS FCSW_Reg_Write_Virtual(u32 addr, u32 value, FCSW_STRU *sw_dev)
{
#ifdef WORK_IN_PC
	AppBuf      appBuf;
	PBuf *      ppbuffer;
	ChanType	chanType;
	u32 		evalue;
	u8			afn,fn;
	u32         raddr;

	chanType = sw_dev->sw_prtl_path;

	PBUF_ALLOC(ppbuffer);

	APPBUF_POAOUT_INIT((&appBuf), chanType, ppbuffer,
			AFN_REGRW, PRM_REQU, FRAME_SINGLE, 0, NULL);


	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = FN_REG_WRITE_VIR;
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)(addr & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 8) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 16) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 24) & 0x000000FF); 
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)(value & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((value >> 8) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((value >> 16) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((value >> 24) & 0x000000FF); 

	FCSW_RS_PRTL_DO(&appBuf);

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;

	
	PBUF_ALLOC(ppbuffer);

	if(FCSW_OK != FCSW_RS_Recv(ppbuffer->payLoad))
	{
		FCSW_printf("Recieve frame error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}			

	afn = *(u8 *)(ppbuffer->payLoad+ 4);
	fn = *(u8 *)(ppbuffer->payLoad+ 6); 
		
	if((afn != AFN_REGRW)||(fn != FN_CON_CONFIRM))
	{
		FCSW_printf("Recieve frame fn error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;
	return(FCSW_OK);			

#endif
#ifdef WORK_IN_BOARD
    if(FCSW_OK!=FCSW_Reg_Write(0x00000204,addr,sw_dev))
		return FCSW_ERROR;
	if(FCSW_OK!=FCSW_Reg_Write(0x00000208,value,sw_dev))
		return FCSW_ERROR;
	if(FCSW_OK!=FCSW_Reg_Write(0x00000200,0,sw_dev))
		return FCSW_ERROR;	
    return FCSW_OK;
#endif

}

FCSW_STATUS FCSW_WriteReg_bit(u32 addr, u32 value, u32 validmask, FCSW_STRU *sw_dev)
{
#ifdef WORK_IN_PC
	AppBuf      appBuf;
	PBuf *      ppbuffer;
	ChanType	chanType;
	u32 		evalue;
	u8			afn,fn;
	u32         raddr;


	chanType = sw_dev->sw_prtl_path;

	PBUF_ALLOC(ppbuffer);

	APPBUF_POAOUT_INIT((&appBuf), chanType, ppbuffer,
			AFN_REGRW, PRM_REQU, FRAME_SINGLE, 0, NULL);

	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = FN_REG_WRITE_BIT;
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)(addr & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 8) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 16) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 24) & 0x000000FF); 
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)(value & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((value >> 8) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((value >> 16) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((value >> 24) & 0x000000FF); 
    appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)(validmask & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((validmask >> 8) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((validmask >> 16) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((validmask >> 24) & 0x000000FF); 
	
	FCSW_RS_PRTL_DO(&appBuf);

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;

	PBUF_ALLOC(ppbuffer);

	if(FCSW_OK != FCSW_RS_Recv(ppbuffer->payLoad))
	{
		FCSW_printf("Recieve frame error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}			

	afn = *(u8 *)(ppbuffer->payLoad+ 4);
	fn = *(u8 *)(ppbuffer->payLoad+ 6); 

	if((afn != AFN_REGRW)||(fn != FN_CON_CONFIRM))
	{
		FCSW_printf("Recieve frame fn error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;
	return(FCSW_OK);	

#endif
#ifdef WORK_IN_BOARD
    u32 wvalue;

    if(FCSW_OK != FCSW_Reg_Read(addr,&wvalue,sw_dev))
	 	return FCSW_ERROR;

	wvalue = wvalue&&(~validmask)+value&&validmask;
	 
    if(FCSW_OK != FCSW_Reg_Write(addr,wvalue,sw_dev))
		return FCSW_ERROR;

	return FCSW_OK;

	
#endif

}

FCSW_STATUS FCSW_RTE_Config_Single(u8 inPort, u8 outPort, u32 did, FCSW_STRU *sw_dev)
{
#ifdef WORK_IN_PC
	AppBuf		appBuf;
	PBuf *		ppbuffer;
	ChanType	chanType;
	u32 		evalue;
	u8			afn,fn;
	u32 		raddr;

	chanType = sw_dev->sw_prtl_path;

	PBUF_ALLOC(ppbuffer);

	APPBUF_POAOUT_INIT((&appBuf), chanType, ppbuffer,
			AFN_CTLCMD, PRM_REQU, FRAME_SINGLE, 0, NULL);

	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = FN_CTL_RTE_SINGLE;
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = inPort;
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = outPort;
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)(did & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((did >> 8) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((did >> 16) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((did >> 24) & 0x000000FF); 

	FCSW_RS_PRTL_DO(&appBuf);

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;

	PBUF_ALLOC(ppbuffer);

	if(FCSW_OK != FCSW_RS_Recv(ppbuffer->payLoad))
	{
		FCSW_printf("Recieve frame error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}			

	afn = *(u8 *)(ppbuffer->payLoad+ 4);
	fn = *(u8 *)(ppbuffer->payLoad+ 6); 
	
	if((afn != AFN_CTLCMD)||(fn != FN_CON_CONFIRM))
	{
		FCSW_printf("Recieve frame fn error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;
	return(FCSW_OK);	 	
	
#endif

#ifdef WORK_IN_BOARD
    u8 serial;
	FCSW_PORT_CFG_FCSW_STATUS_STRU* rte_list;
	u32 outPortMask0,outPortMask1;
	FCSW_RTE_MANAGER_STRU * rte_list_main;
	
	rte_list_main = (FCSW_RTE_MANAGER_STRU *)(((sw_dev->sw_port_cfg)+inPort)->rte_list);
	rte_list = rte_list_main->rte_list_single;	

    if((inPort==outPort)||(inPort>sw_dev->sw_info->port_total)
		||(outPort>sw_dev->sw_info->port_total)||(sw_dev==NULL))
		return FCSW_ERROR;

	if(0==FCSW_RTE_Manager_RTE_List_Is_Full(rte_list))
	{
		FCSW_printf("rte list full\n");
		return 4;	
	}	    
	serial = FCSW_RTE_Manager_Get_NewSerial(rte_list);
	

	if(FCSW_OK!=FCSW_Reg_Write_Virtual(FCSW_REG_DEV_MC_DID_COR(inPort),did,sw_dev))
	{
		FCSW_printf("reg read error\n");
		return FCSW_ERROR;	
	}

	if(outPort<32)
	{
	    outPortMask0 = 1<<outPort;
		outPortMask1 = 0;
	}
	else
	{
	    outPortMask0 = 0;
		outPortMask1 = 1<<(outPort-32);
	}
	if(FCSW_OK!=FCSW_Reg_Write_Virtual(FCSW_REG_DEV_MC_MASK0_COR(inPort),outPortMask0,sw_dev))
	{
		FCSW_printf("reg write error\n");
		return FCSW_ERROR;	
	}
	if(FCSW_OK!=FCSW_Reg_Write_Virtual(FCSW_REG_DEV_MC_MASK1_COR(inPort),outPortMask1,sw_dev))
	{
		FCSW_printf("reg write error\n");
		return FCSW_ERROR;	
	}
   
	if(FCSW_OK!=FCSW_Reg_Write_Virtual(FCSW_REG_DEV_MC_ADDR_COR(inPort),serial,sw_dev))
	{
		FCSW_printf("reg write error\n");
		return FCSW_ERROR;	
	}

	FCSW_RTE_Manager_Add(inPort,outPortMask0,outPortMask1,did,serial,rte_list);

	return FCSW_OK;

#endif

}

FCSW_STATUS FCSW_RTE_Config_Multi(u8 inPort, u32 outPortMask0, u32 outPortMask1, u32 did, FCSW_STRU *sw_dev)
{
#ifdef WORK_IN_PC
	AppBuf		appBuf;
	PBuf *		ppbuffer;
	ChanType	chanType;
	u32 		evalue;
	u8			afn,fn;
	u32 		raddr;

	chanType = sw_dev->sw_prtl_path;

	PBUF_ALLOC(ppbuffer);

	APPBUF_POAOUT_INIT((&appBuf), chanType, ppbuffer,
			AFN_CTLCMD, PRM_REQU, FRAME_SINGLE, 0, NULL);


	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = FN_CTL_RTE_MULTI;
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = inPort;
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)(outPortMask0 & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((outPortMask0 >> 8) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((outPortMask0 >> 16) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((outPortMask0 >> 24) & 0x000000FF); 
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)(outPortMask1 & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((outPortMask1 >> 8) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((outPortMask1 >> 16) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((outPortMask1 >> 24) & 0x000000FF); 
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)(did & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((did >> 8) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((did >> 16) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((did >> 24) & 0x000000FF); 

	FCSW_RS_PRTL_DO(&appBuf);

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;

	PBUF_ALLOC(ppbuffer);

	if(FCSW_OK != FCSW_RS_Recv(ppbuffer->payLoad))
	{
		FCSW_printf("Recieve frame error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}			

	afn = *(u8 *)(ppbuffer->payLoad+ 4);
	fn = *(u8 *)(ppbuffer->payLoad+ 6); 
	
	if((afn != AFN_CTLCMD)||(fn != FN_CON_CONFIRM))
	{
		FCSW_printf("Recieve frame fn error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;
	return(FCSW_OK);	 	

#endif
#ifdef WORK_IN_BOARD
    u8 serial;
	FCSW_PORT_CFG_FCSW_STATUS_STRU* rte_list;
	FCSW_RTE_MANAGER_STRU * rte_list_main;
	
	rte_list_main = (FCSW_RTE_MANAGER_STRU *)(((sw_dev->sw_port_cfg)+inPort)->rte_list);
	rte_list = rte_list_main->rte_list_multi;

    if((inPort>sw_dev->sw_info->port_total)
		||(sw_dev==NULL))
		return FCSW_ERROR;
	
	if(0==FCSW_RTE_Manager_RTE_List_Is_Full(rte_list))
	{
		FCSW_printf("rte list full\n");
		return 4;	
	}	    
	serial = FCSW_RTE_Manager_Get_NewSerial(rte_list);
	
	if(FCSW_OK!=FCSW_Reg_Write_Virtual(FCSW_REG_DEV_MC_DID_COR(inPort),did,sw_dev))
	{
		FCSW_printf("reg read error\n");
		return FCSW_ERROR;	
	}

	if(FCSW_OK!=FCSW_Reg_Write_Virtual(FCSW_REG_DEV_MC_MASK0_COR(inPort),outPortMask0,sw_dev))
	{
		FCSW_printf("reg write error\n");
		return FCSW_ERROR;	
	}
	if(FCSW_OK!=FCSW_Reg_Write_Virtual(FCSW_REG_DEV_MC_MASK1_COR(inPort),outPortMask1,sw_dev))
	{
		FCSW_printf("reg write error\n");
		return FCSW_ERROR;	
	}
    
	if(FCSW_OK!=FCSW_Reg_Write_Virtual(FCSW_REG_DEV_MC_ADDR_COR(inPort),serial,sw_dev))
	{
		FCSW_printf("reg write error\n");
		return FCSW_ERROR;	
	}

	FCSW_RTE_Manager_Add(inPort,outPortMask0,outPortMask1,did,serial,rte_list);

	return FCSW_OK;

#endif

}

FCSW_STATUS FCSW_RTE_Config_BroadCast(u8 inPort, u32 did, FCSW_STRU *sw_dev)
{
#ifdef WORK_IN_PC
	AppBuf		appBuf;
	PBuf *		ppbuffer;
	ChanType	chanType;
	u32 		evalue;
	u8			afn,fn;
	u32 		raddr;

	chanType = sw_dev->sw_prtl_path;

	PBUF_ALLOC(ppbuffer);

	APPBUF_POAOUT_INIT((&appBuf), chanType, ppbuffer,
			AFN_CTLCMD, PRM_REQU, FRAME_SINGLE, 0, NULL);


	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = FN_CTL_RTE_BC;
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = inPort;
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)(did & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((did >> 8) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((did >> 16) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((did >> 24) & 0x000000FF); 

	FCSW_RS_PRTL_DO(&appBuf);

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;

	PBUF_ALLOC(ppbuffer);

	if(FCSW_OK != FCSW_RS_Recv(ppbuffer->payLoad))
	{
		FCSW_printf("Recieve frame error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}			

	afn = *(u8 *)(ppbuffer->payLoad+ 4);
	fn = *(u8 *)(ppbuffer->payLoad+ 6); 
	
	if((afn != AFN_CTLCMD)||(fn != FN_CON_CONFIRM))
	{
		FCSW_printf("Recieve frame fn error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;
	return(FCSW_OK);	 	

#endif
#ifdef WORK_IN_BOARD
    u8 serial;
	FCSW_PORT_CFG_FCSW_STATUS_STRU* rte_list;
	FCSW_RTE_MANAGER_STRU * rte_list_main;
	
	rte_list_main = (FCSW_RTE_MANAGER_STRU *)(((sw_dev->sw_port_cfg)+inPort)->rte_list);
	rte_list = rte_list_main->rte_list_broadcast;

    if((inPort>sw_dev->sw_info->port_total)
		||(sw_dev==NULL))
		return FCSW_ERROR;
	
	if(0==FCSW_RTE_Manager_RTE_List_Is_Full(rte_list))
	{
		FCSW_printf("rte list full\n");
		return 4;	
	}	    
	serial = FCSW_RTE_Manager_Get_NewSerial(rte_list);
	
	if(FCSW_OK!=FCSW_Reg_Write_Virtual(FCSW_REG_DEV_MC_DID_COR(inPort),did,sw_dev))
	{
		FCSW_printf("reg read error\n");
		return FCSW_ERROR;	
	}

	if(FCSW_OK!=FCSW_Reg_Write_Virtual(FCSW_REG_DEV_MC_MASK0_COR(inPort),0xFFFFFFFF,sw_dev))
	{
		FCSW_printf("reg write error\n");
		return FCSW_ERROR;	
	}
	if(FCSW_OK!=FCSW_Reg_Write_Virtual(FCSW_REG_DEV_MC_MASK1_COR(inPort),0xFFFFFFFF,sw_dev))
	{
		FCSW_printf("reg write error\n");
		return FCSW_ERROR;	
	}

	if(FCSW_OK!=FCSW_Reg_Write_Virtual(FCSW_REG_DEV_MC_ADDR_COR(inPort),serial,sw_dev))
	{
		FCSW_printf("reg write error\n");
		return FCSW_ERROR;	
	}

	FCSW_RTE_Manager_Add(inPort,0xFFFFFFFF,0xFFFFFFFF,did,serial,rte_list);

	return FCSW_OK;

#endif

}

FCSW_STATUS FCSW_RTE_Config_ISL(u8 EportNum, u8 onoff, u16 domain_id, FCSW_STRU *sw_dev)
{
#ifdef WORK_IN_PC
#endif
#ifdef WORK_IN_BOARD
#endif

}
FCSW_STATUS FCSW_RTE_Delete_FPort_Num(u8 inPort, u8 serial, FCSW_STRU *sw_dev)
{
#ifdef WORK_IN_PC
	AppBuf		appBuf;
	PBuf *		ppbuffer;
	ChanType	chanType;
	u32 		evalue;
	u8			afn,fn;
	u32 		raddr;

	chanType = sw_dev->sw_prtl_path;

	PBUF_ALLOC(ppbuffer);

	APPBUF_POAOUT_INIT((&appBuf), chanType, ppbuffer,
			AFN_CTLCMD, PRM_REQU, FRAME_SINGLE, 0, NULL);


	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = FN_CTL_RTE_DEL_BY_SERIAL;
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = inPort;
	

	FCSW_RS_PRTL_DO(&appBuf);

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;

	PBUF_ALLOC(ppbuffer);

	if(FCSW_OK != FCSW_RS_Recv(ppbuffer->payLoad))
	{
		FCSW_printf("Recieve frame error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}			


	afn = *(u8 *)(ppbuffer->payLoad+ 4);
	fn = *(u8 *)(ppbuffer->payLoad+ 6); 
	
	if((afn != AFN_CTLCMD)||(fn != FN_CON_CONFIRM))
	{
		FCSW_printf("Recieve frame fn error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;
	return(FCSW_OK);	 	

#endif
#ifdef WORK_IN_BOARD
    FCSW_PORT_CFG_FCSW_STATUS_STRU* rte_list;    

	
    if((inPort>sw_dev->sw_info->port_total)
		||(sw_dev==NULL))
		return FCSW_ERROR;    
	
	rte_list = (FCSW_PORT_CFG_FCSW_STATUS_STRU*)((sw_dev->sw_port_cfg)+inPort)->rte_list;

	
	if(FCSW_OK!=FCSW_Reg_Write_Virtual(FCSW_REG_DEV_MC_MASK0_COR(inPort),0,sw_dev))
	{
		FCSW_printf("reg write error\n");
		return FCSW_ERROR;	
	}
	if(FCSW_OK!=FCSW_Reg_Write_Virtual(FCSW_REG_DEV_MC_MASK1_COR(inPort),0,sw_dev))
	{
		FCSW_printf("reg write error\n");
		return FCSW_ERROR;	
	}

	if(FCSW_OK!=FCSW_Reg_Write_Virtual(FCSW_REG_DEV_MC_ADDR_COR(inPort),serial,sw_dev))
	{
		FCSW_printf("reg write error\n");
		return FCSW_ERROR;	
	}

	return FCSW_OK;
    


#endif
}

FCSW_STATUS FCSW_RTE_Delete_Single_FPort_Did(u8 inPort, u32 did, FCSW_STRU *sw_dev)
{
#ifdef WORK_IN_PC
	AppBuf		appBuf;
	PBuf *		ppbuffer;
	ChanType	chanType;
	u32 		evalue;
	u8			afn,fn;
	u32 		raddr;

	chanType = sw_dev->sw_prtl_path;

	PBUF_ALLOC(ppbuffer);

	APPBUF_POAOUT_INIT((&appBuf), chanType, ppbuffer,
			AFN_CTLCMD, PRM_REQU, FRAME_SINGLE, 0, NULL);


	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = FN_CTL_RTE_DEL_SINGLE;
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = inPort;
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)(did & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((did >> 8) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((did >> 16) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((did >> 24) & 0x000000FF); 

	FCSW_RS_PRTL_DO(&appBuf);

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;

	PBUF_ALLOC(ppbuffer);

	if(FCSW_OK != FCSW_RS_Recv(ppbuffer->payLoad))
	{
		FCSW_printf("Recieve frame error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}			

	afn = *(u8 *)(ppbuffer->payLoad+ 4);
	fn = *(u8 *)(ppbuffer->payLoad+ 6); 
	
	if((afn != AFN_CTLCMD)||(fn != FN_CON_CONFIRM))
	{
		FCSW_printf("Recieve frame fn error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;
	return(FCSW_OK);	 	

#endif
#ifdef WORK_IN_BOARD
    u8 serial;
    FCSW_PORT_CFG_FCSW_STATUS_STRU* rte_list;    
	FCSW_RTE_MANAGER_STRU * rte_list_main;
	
	rte_list_main = (FCSW_RTE_MANAGER_STRU *)(((sw_dev->sw_port_cfg)+inPort)->rte_list);
	rte_list = rte_list_main->rte_list_single;
	
    if(sw_dev==NULL)
    {
        FCSW_printf("sw_dev error\n");
        return FCSW_ERROR;    
    }
    if((inPort>sw_dev->sw_info->port_total)
		||(sw_dev==NULL))
		return FCSW_ERROR; 

	
	serial = FCSW_RTE_Manager_Find_Serial(inPort, did,rte_list);


	if(FCSW_OK!=FCSW_RTE_Delete_FPort_Num(inPort,serial,sw_dev))
	{
		FCSW_printf("delete error\n");
		return FCSW_ERROR;	
	}

	FCSW_RTE_Manager_Delete(inPort,serial,rte_list);

	return FCSW_OK;

	
#endif

}
FCSW_STATUS FCSW_RTE_Delete_Multi_FPort_Did(u8 inPort, u32 did, FCSW_STRU *sw_dev)
{
#ifdef WORK_IN_PC
	AppBuf		appBuf;
	PBuf *		ppbuffer;
	ChanType	chanType;
	u32 		evalue;
	u8			afn,fn;
	u32 		raddr;

	chanType = sw_dev->sw_prtl_path;

	PBUF_ALLOC(ppbuffer);

	APPBUF_POAOUT_INIT((&appBuf), chanType, ppbuffer,
			AFN_CTLCMD, PRM_REQU, FRAME_SINGLE, 0, NULL);

	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = FN_CTL_RTE_DEL_MULTI;
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = inPort;
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)(did & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((did >> 8) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((did >> 16) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((did >> 24) & 0x000000FF); 

	FCSW_RS_PRTL_DO(&appBuf);

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;

	PBUF_ALLOC(ppbuffer);

	if(FCSW_OK != FCSW_RS_Recv(ppbuffer->payLoad))
	{
		FCSW_printf("Recieve frame error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}			

	afn = *(u8 *)(ppbuffer->payLoad+ 4);
	fn = *(u8 *)(ppbuffer->payLoad+ 6); 
	
	if((afn != AFN_CTLCMD)||(fn != FN_CON_CONFIRM))
	{
		FCSW_printf("Recieve frame fn error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;
	return(FCSW_OK);	 	

#endif
#ifdef WORK_IN_BOARD
	u8 serial;
	FCSW_PORT_CFG_FCSW_STATUS_STRU* rte_list;	
	FCSW_RTE_MANAGER_STRU * rte_list_main;
	
	rte_list_main = (FCSW_RTE_MANAGER_STRU *)(((sw_dev->sw_port_cfg)+inPort)->rte_list);
	rte_list = rte_list_main->rte_list_multi;
	
    if(sw_dev==NULL)
    {
        FCSW_printf("sw_dev error\n");
        return FCSW_ERROR;    
    }
	if((inPort>sw_dev->sw_info->port_total)
		||(sw_dev==NULL))
		return FCSW_ERROR; 

	
	serial = FCSW_RTE_Manager_Find_Serial(inPort,did,rte_list);

	if(FCSW_OK!=FCSW_RTE_Delete_FPort_Num(inPort,serial,sw_dev))
	{
		FCSW_printf("delete error\n");
		return FCSW_ERROR;	
	}
	
	FCSW_RTE_Manager_Delete(inPort,serial,rte_list);
	
	return FCSW_OK;
	
		
#endif

}
FCSW_STATUS FCSW_RTE_Delete_BroadCast_FPort_Did(u8 inPort, u32 did, FCSW_STRU *sw_dev)
{
#ifdef WORK_IN_PC
	AppBuf		appBuf;
	PBuf *		ppbuffer;
	ChanType	chanType;
	u32 		evalue;
	u8			afn,fn;
	u32 		raddr;

	chanType = sw_dev->sw_prtl_path;

	PBUF_ALLOC(ppbuffer);

	APPBUF_POAOUT_INIT((&appBuf), chanType, ppbuffer,
			AFN_CTLCMD, PRM_REQU, FRAME_SINGLE, 0, NULL);


	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = FN_CTL_RTE_DEL_BC;
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = inPort;
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)(did & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((did >> 8) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((did >> 16) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((did >> 24) & 0x000000FF); 

	FCSW_RS_PRTL_DO(&appBuf);

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;

	PBUF_ALLOC(ppbuffer);

	if(FCSW_OK != FCSW_RS_Recv(ppbuffer->payLoad))
	{
		FCSW_printf("Recieve frame error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}			


	afn = *(u8 *)(ppbuffer->payLoad+ 4);
	fn = *(u8 *)(ppbuffer->payLoad+ 6); 
	
	if((afn != AFN_CTLCMD)||(fn != FN_CON_CONFIRM))
	{
		FCSW_printf("Recieve frame fn error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;
	return(FCSW_OK);	 	

#endif
#ifdef WORK_IN_BOARD
	u8 serial;
	FCSW_PORT_CFG_FCSW_STATUS_STRU* rte_list;	
	FCSW_RTE_MANAGER_STRU * rte_list_main;
	
	rte_list_main = (FCSW_RTE_MANAGER_STRU *)(((sw_dev->sw_port_cfg)+inPort)->rte_list);
	rte_list = rte_list_main->rte_list_broadcast;
	
    if(sw_dev==NULL)
    {
        FCSW_printf("sw_dev error\n");
        return FCSW_ERROR;    
    }
	if((inPort>sw_dev->sw_info->port_total)
		||(sw_dev==NULL))
		return FCSW_ERROR; 

	serial = FCSW_RTE_Manager_Find_Serial(inPort,did,rte_list);

	if(FCSW_OK!=FCSW_RTE_Delete_FPort_Num(inPort,serial,sw_dev))
	{
		FCSW_printf("delete error\n");
		return FCSW_ERROR;	
	}

	FCSW_RTE_Manager_Delete(inPort,serial,rte_list);

	return FCSW_OK;
	
		
#endif

}

FCSW_STATUS FCSW_RTE_Delete_FPort(u8 inPort, FCSW_STRU *sw_dev)
{
#ifdef WORK_IN_PC
	AppBuf		appBuf;
	PBuf *		ppbuffer;
	ChanType	chanType;
	u32 		evalue;
	u8			afn,fn;
	u32 		raddr;

	chanType = sw_dev->sw_prtl_path;

	PBUF_ALLOC(ppbuffer);

	APPBUF_POAOUT_INIT((&appBuf), chanType, ppbuffer,
			AFN_CTLCMD, PRM_REQU, FRAME_SINGLE, 0, NULL);


	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = FN_CTL_RTE_DEL_PORT;
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = inPort;

	FCSW_RS_PRTL_DO(&appBuf);

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;

	PBUF_ALLOC(ppbuffer);

	if(FCSW_OK != FCSW_RS_Recv(ppbuffer->payLoad))
	{
		FCSW_printf("Recieve frame error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}			

	afn = *(u8 *)(ppbuffer->payLoad+ 4);
	fn = *(u8 *)(ppbuffer->payLoad+ 6); 
	
	if((afn != AFN_CTLCMD)||(fn != FN_CON_CONFIRM))
	{
		FCSW_printf("Recieve frame fn error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;
	return(FCSW_OK);	 	

#endif
#ifdef WORK_IN_BOARD
    u8 max;
    u8 tmp;
	FCSW_PORT_CFG_FCSW_STATUS_STRU* rte_list;

	
    if(sw_dev==NULL)
    {
        FCSW_printf("sw_dev error\n");
        return FCSW_ERROR;    
    }
	if((inPort>sw_dev->sw_info->port_total)
		||(sw_dev==NULL))
		return FCSW_ERROR; 

	rte_list = (FCSW_PORT_CFG_FCSW_STATUS_STRU*)((sw_dev->sw_port_cfg)+inPort)->rte_list;
	max = sw_dev->sw_info->mc_lut_size;

	for(tmp=0;tmp<max;tmp++)
	{
	    if(FCSW_OK!=FCSW_RTE_Delete_FPort_Num(inPort,tmp,sw_dev))
			return FCSW_ERROR;
		FCSW_RTE_Manager_Delete_All(inPort,rte_list);
	}
	return FCSW_OK;
#endif

}

FCSW_STATUS FCSW_RTE_Delete_AllF(FCSW_STRU *sw_dev)
{
#ifdef WORK_IN_PC
	AppBuf		appBuf;
	PBuf *		ppbuffer;
	ChanType	chanType;
	u32 		evalue;
	u8			afn,fn;
	u32 		raddr;

	chanType = sw_dev->sw_prtl_path;

	PBUF_ALLOC(ppbuffer);

	APPBUF_POAOUT_INIT((&appBuf), chanType, ppbuffer,
			AFN_CTLCMD, PRM_REQU, FRAME_SINGLE, 0, NULL);


	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = FN_CTL_RTE_DEL_ALL;

	FCSW_RS_PRTL_DO(&appBuf);

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;

	PBUF_ALLOC(ppbuffer);

	if(FCSW_OK != FCSW_RS_Recv(ppbuffer->payLoad))
	{
		FCSW_printf("Recieve frame error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}			

	afn = *(u8 *)(ppbuffer->payLoad+ 4);
	fn = *(u8 *)(ppbuffer->payLoad+ 6); 
	
	if((afn != AFN_CTLCMD)||(fn != FN_CON_CONFIRM))
	{
		FCSW_printf("Recieve frame fn error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;
	return(FCSW_OK);	 	

#endif
#ifdef WORK_IN_BOARD
    u8 port;
    u8 tmp;
	
	
    if(sw_dev==NULL)
    {
        FCSW_printf("sw_dev error\n");
        return FCSW_ERROR;    
    }

	port = sw_dev->sw_info->port_total;

	for(tmp=0;tmp<port;tmp++)
	{
	    if(FCSW_OK!=FCSW_RTE_Delete_FPort(tmp,sw_dev))
			return FCSW_ERROR;	    
	}

	return FCSW_OK;
    
#endif

}

FCSW_STATUS FCSW_Config_MON(u8 mode, u8 port, u8 enable, FCSW_STRU *sw_dev)
{
#ifdef WORK_IN_PC
	AppBuf		appBuf;
	PBuf *		ppbuffer;
	ChanType	chanType;
	u32 		evalue;
	u8			afn,fn;
	u32 		raddr;

	chanType = sw_dev->sw_prtl_path;

	
	PBUF_ALLOC(ppbuffer);

	APPBUF_POAOUT_INIT((&appBuf), chanType, ppbuffer,
			AFN_CTLCMD, PRM_REQU, FRAME_SINGLE, 0, NULL);


	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = FN_CTL_MON;
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = mode;
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = port;
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = enable;

	FCSW_RS_PRTL_DO(&appBuf);

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;

	
	
	PBUF_ALLOC(ppbuffer);

	if(FCSW_OK != FCSW_RS_Recv(ppbuffer->payLoad))
	{
		FCSW_printf("Recieve frame error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}			

	

	afn = *(u8 *)(ppbuffer->payLoad+ 4);
	fn = *(u8 *)(ppbuffer->payLoad+ 6); 
	
	
	if((afn != AFN_CTLCMD)||(fn != FN_CON_CONFIRM))
	{
		FCSW_printf("Recieve frame fn error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;
	return(FCSW_OK);	 	

#endif
#ifdef WORK_IN_BOARD
    u32 value=0;
    u32 tmp;

	
	if((mode!=FCSW_PORT_MON_TYPE_INPUT)||(mode!=0x10)||(mode!=0x100))
	{
		FCSW_printf("mon mode error\n");
		return FCSW_ERROR;		
	}
	if(sw_dev==NULL)
	{
		FCSW_printf("sw_dev error\n");
		return FCSW_ERROR;	
	}
	if(port>sw_dev->sw_info->port_total)
	{
	    FCSW_printf("port error\n");
		return FCSW_ERROR;
	}

    tmp = mode;
	value = tmp<<12;

	switch (mode)
	{
		case FCSW_PORT_MON_TYPE_INPUT:
			value += port;
			break;
		case FCSW_PORT_MON_TYPE_OUTPUT:
			tmp = port;
			value += (tmp<<6);
		    break;
		default:
			break;
	}

	if(FCSW_OK!=FCSW_Reg_Write_Virtual(FCSW_REG_DEV_MON_PORT_FCR(port), value, sw_dev))
		return FCSW_ERROR;
	return FCSW_OK;
	
#endif

}

FCSW_STATUS FCSW_Config_MON_msg(u8 inPort, u32 msgid, u8 msgidnum, FCSW_STRU *sw_dev)
{
#ifdef WORK_IN_PC
	AppBuf		appBuf;
	PBuf *		ppbuffer;
	ChanType	chanType;
	u32 		evalue;
	u8			afn,fn;
	u32 		raddr;


	chanType = sw_dev->sw_prtl_path;

	PBUF_ALLOC(ppbuffer);

	APPBUF_POAOUT_INIT((&appBuf), chanType, ppbuffer,
			AFN_CTLCMD, PRM_REQU, FRAME_SINGLE, 0, NULL);


	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = FN_CTL_MON_MSG;
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = inPort;
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)(msgid & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((msgid >> 8) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((msgid >> 16) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((msgid >> 24) & 0x000000FF); 
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = msgidnum;

	FCSW_RS_PRTL_DO(&appBuf);

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;

	
	
	PBUF_ALLOC(ppbuffer);

	if(FCSW_OK != FCSW_RS_Recv(ppbuffer->payLoad))
	{
		FCSW_printf("Recieve frame error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}			

	

	afn = *(u8 *)(ppbuffer->payLoad+ 4);
	fn = *(u8 *)(ppbuffer->payLoad+ 6); 
	
	
	if((afn != AFN_CTLCMD)||(fn != FN_CON_CONFIRM))
	{
		FCSW_printf("Recieve frame fn error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;
	return(FCSW_OK);	 	

#endif
#ifdef WORK_IN_BOARD
    u8 serial;
	FCSW_MON_CFG_STRU* mon_cfg;

	mon_cfg = sw_dev->mon_cfg;


    

	
	

	
	if(FCSW_OK!=FCSW_Reg_Write_Virtual(FCSW_REG_DEV_MON_PORT_FCR(inPort),msgid,sw_dev))
	{
		FCSW_printf("reg write error\n");
		return FCSW_ERROR;	
	}	
	if(FCSW_OK!=FCSW_Reg_Write_Virtual(FCSW_REG_DEV_MC_ADDR_COR(inPort),serial,sw_dev))
	{
		FCSW_printf("reg write error\n");
		return FCSW_ERROR;	
	}

    
#endif

}

FCSW_STATUS FCSW_Config_SYNC_sever(u16 gap, u32 set0, u32 set1, FCSW_STRU *sw_dev)
{
#ifdef WORK_IN_PC
	AppBuf		appBuf;
	PBuf *		ppbuffer;
	ChanType	chanType;
	u32 		evalue;
	u8			afn,fn;
	u32 		raddr;

	
	
	chanType = sw_dev->sw_prtl_path;

	
	PBUF_ALLOC(ppbuffer);

	APPBUF_POAOUT_INIT((&appBuf), chanType, ppbuffer,
			AFN_CTLCMD, PRM_REQU, FRAME_SINGLE, 0, NULL);


	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = FN_CTL_SYNC_SERVER;
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)(gap & 0x00FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((gap >> 8) & 0x00FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)(set0 & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((set0 >> 8) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((set0 >> 16) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((set0 >> 24) & 0x000000FF); 
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)(set1 & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((set1 >> 8) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((set1 >> 16) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((set1 >> 24) & 0x000000FF); 

	FCSW_RS_PRTL_DO(&appBuf);

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;

	
	
	PBUF_ALLOC(ppbuffer);

	if(FCSW_OK != FCSW_RS_Recv(ppbuffer->payLoad))
	{
		FCSW_printf("Recieve frame error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}			

	

	afn = *(u8 *)(ppbuffer->payLoad+ 4);
	fn = *(u8 *)(ppbuffer->payLoad+ 6); 
	
	
	if((afn != AFN_CTLCMD)||(fn != FN_CON_CONFIRM))
	{
		FCSW_printf("Recieve frame fn error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;
	return(FCSW_OK);	 	

#endif
#ifdef WORK_IN_BOARD
    u32 value = 0;

    
    if(sw_dev==NULL)
    {
        FCSW_printf("sw_dev error\n");
        return FCSW_ERROR;    
    }

	
	if(FCSW_OK!=FCSW_Reg_Write(FCSW_REG_DEV_MODE_CFG_FCR,value,sw_dev))
	{
		FCSW_printf("reg write error\n");
		return FCSW_ERROR;	
	}
	if(FCSW_OK!=FCSW_Reg_Write(FCSW_REG_DEV_SYNC_SET0_FCR,set0,sw_dev))
	{
		FCSW_printf("reg write error\n");
		return FCSW_ERROR;	
	}
	if(FCSW_OK!=FCSW_Reg_Write(FCSW_REG_DEV_SYNC_SET1_FCR,set1,sw_dev))
	{
		FCSW_printf("reg write error\n");
		return FCSW_ERROR;	
	}

	return FCSW_OK;

#endif

}

FCSW_STATUS FCSW_Config_SYNC_client(u8 port, FCSW_STRU *sw_dev)
{
#ifdef WORK_IN_PC
	AppBuf		appBuf;
	PBuf *		ppbuffer;
	ChanType	chanType;
	u32 		evalue;
	u8			afn,fn;
	u32 		raddr;

	
	
	chanType = sw_dev->sw_prtl_path;

	
	PBUF_ALLOC(ppbuffer);

	APPBUF_POAOUT_INIT((&appBuf), chanType, ppbuffer,
			AFN_CTLCMD, PRM_REQU, FRAME_SINGLE, 0, NULL);


	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = FN_CTL_SYNC_CLIENT;
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = port;

	FCSW_RS_PRTL_DO(&appBuf);

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;

	
	
	PBUF_ALLOC(ppbuffer);

	if(FCSW_OK != FCSW_RS_Recv(ppbuffer->payLoad))
	{
		FCSW_printf("Recieve frame error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}			

	

	afn = *(u8 *)(ppbuffer->payLoad+ 4);
	fn = *(u8 *)(ppbuffer->payLoad+ 6); 
	
	
	if((afn != AFN_CTLCMD)||(fn != FN_CON_CONFIRM))
	{
		FCSW_printf("Recieve frame fn error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;
	return(FCSW_OK);	 	

#endif
#ifdef WORK_IN_BOARD
    u32 value = 0;

    
    if(sw_dev==NULL)
    {
        FCSW_printf("sw_dev error\n");
        return FCSW_ERROR;    
    }

	
	if(FCSW_OK!=FCSW_Reg_Write(FCSW_REG_DEV_MODE_CFG_FCR,value,sw_dev))
	{
		FCSW_printf("reg write error\n");
		return FCSW_ERROR;	
	}	
#endif

}


FCSW_STATUS FCSW_Set_ConfigMode(FCSW_CTRL mode, FCSW_STRU *sw_dev)
{
#ifdef WORK_IN_PC
	AppBuf		appBuf;
	PBuf *		ppbuffer;
	ChanType	chanType;
	u32 		evalue;
	u8			afn,fn;
	u32 		raddr;

	
	
	chanType = sw_dev->sw_prtl_path;

	
	PBUF_ALLOC(ppbuffer);

	APPBUF_POAOUT_INIT((&appBuf), chanType, ppbuffer,
			AFN_CTLCMD, PRM_REQU, FRAME_SINGLE, 0, NULL);

	
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = FN_CTL_MODE;
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)mode;

	FCSW_RS_PRTL_DO(&appBuf);

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;

	
	
	PBUF_ALLOC(ppbuffer);

	if(FCSW_OK != FCSW_RS_Recv(ppbuffer->payLoad))
	{
		FCSW_printf("Recieve frame error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}			

	

	afn = *(u8 *)(ppbuffer->payLoad+ 4);
	fn = *(u8 *)(ppbuffer->payLoad+ 6); 
	
	
	if((afn != AFN_CTLCMD)||(fn != FN_CON_CONFIRM))
	{
		FCSW_printf("Recieve frame fn error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;
	return(FCSW_OK);	 	

#endif
#ifdef WORK_IN_BOARD
#endif

}




FCSW_STATUS FCSW_Query_SW_Info(u32 addr, u32 *value,  FCSW_STRU *sw_dev)
{
#ifdef WORK_IN_PC
	AppBuf appBuf;
	PBuf * ppbuffer;
	u8  chanType;
	u32 evalue;
	u8  afn,fn;

	
	
	chanType = sw_dev->sw_prtl_path;

	
	PBUF_ALLOC(ppbuffer);

	APPBUF_POAOUT_INIT((&appBuf), chanType, ppbuffer,
			AFN_QUERYPARAM, PRM_REQU, FRAME_SINGLE, 0, NULL);


	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = FN_QUERY_SWINFO;
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)(addr & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 8) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 16) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 24) & 0x000000FF); 

	FCSW_RS_PRTL_DO(&appBuf);

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;

	
	
	PBUF_ALLOC(ppbuffer);

	if(FCSW_OK != BOARD_PC_Recv(ppbuffer->payLoad, sw_dev, FCSW_RECVTIMEOUT))
	{
		FCSW_printf("Recieve frame error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return(FCSW_ERROR);
	}

	

	afn = *(u8 *)(ppbuffer->payLoad+ 4);
	fn = *(u8 *)(ppbuffer->payLoad+ 6); 
	
	
	if((afn != AFN_QUERYPARAM)||(fn != FN_QUERY_SWINFO))
	{
		FCSW_printf("Recieve frame fn error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return(FCSW_ERROR);
	}

	evalue = *(u8 *)(ppbuffer->payLoad+7);
	evalue += 0x100*(*(u8 *)(ppbuffer->payLoad+8));
	evalue += 0x10000*(*(u8 *)(ppbuffer->payLoad+9));
	evalue += 0x1000000*(*(u8 *)(ppbuffer->payLoad+10));
	*value = evalue;
	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;

	return FCSW_OK;
		
#endif
#ifdef WORK_IN_BOARD
	u32 evalue;
	
	
	if((addr < FCSW_REG_DEV_ID_CDR)||(addr > FCSW_REG_DEV_TRUNK_GROUP_NUM_CDR)||(0!=addr%4))
	{
	 FCSW_printf("parament addr error\n");
	 return(FCSW_ERROR);
	}
	if(sw_dev==NULL)
	{
	 FCSW_printf("parament dev error\n");
	 return(FCSW_ERROR); 	 
	}

	
	if(FCSW_OK!=FCSW_Reg_Read(addr,&evalue,sw_dev))
	{
	 FCSW_printf("reg read error\n");
	 return(FCSW_ERROR);  
	}

	*value = evalue;
	return(FCSW_OK);
		 
		 
#endif


}

FCSW_STATUS FCSW_Query_SW_Config(u32 addr, u32 *value,  FCSW_STRU *sw_dev)
{
#ifdef WORK_IN_PC
	AppBuf appBuf;
	PBuf * ppbuffer;
	u8  chanType;
	u32 evalue;
	u8  afn,fn;

	
	
	chanType = sw_dev->sw_prtl_path;

	
	PBUF_ALLOC(ppbuffer);

	APPBUF_POAOUT_INIT((&appBuf), chanType, ppbuffer,
			AFN_QUERYPARAM, PRM_REQU, FRAME_SINGLE, 0, NULL);


	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = FN_QUERY_SWCFG;
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)(addr & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 8) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 16) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 24) & 0x000000FF); 

	FCSW_RS_PRTL_DO(&appBuf);

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;

	
	
	PBUF_ALLOC(ppbuffer);

	if(FCSW_OK != BOARD_PC_Recv(ppbuffer->payLoad, sw_dev, FCSW_RECVTIMEOUT))
	{
		FCSW_printf("Recieve frame error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return(FCSW_ERROR);
	}

	

	afn = *(u8 *)(ppbuffer->payLoad+ 4);
	fn = *(u8 *)(ppbuffer->payLoad+ 6); 
	
	
	if((afn != AFN_QUERYPARAM)||(fn != FN_QUERY_SWCFG))
	{
		FCSW_printf("Recieve frame fn error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return(FCSW_ERROR);
	}

	evalue = *(u8 *)(ppbuffer->payLoad+7);
	evalue += 0x100*(*(u8 *)(ppbuffer->payLoad+8));
	evalue += 0x10000*(*(u8 *)(ppbuffer->payLoad+9));
	evalue += 0x1000000 * (*(u8 *)(ppbuffer->payLoad + 10));
	*value = evalue;
	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;

	return FCSW_OK;
			
#endif
#ifdef WORK_IN_BOARD
	u32 evalue;
	
	
	if((addr < FCSW_REG_DEV_MODE_CFG_FCR)||(addr > FCSW_REG_DEV_MON_MSGID_FCR(sw_dev->sw_info->port_total))||(0!=addr%4))
	{
		FCSW_printf("parament addr error\n");
		return(FCSW_ERROR);
	}
	if(sw_dev==NULL)
	{
		FCSW_printf("parament dev error\n");
		return(FCSW_ERROR); 	 
	}

	
	if(FCSW_OK!=FCSW_Reg_Read(addr,&evalue,sw_dev))
	{
		FCSW_printf("reg read error\n");
		return(FCSW_ERROR);  
	}

	*value = evalue;
	return(FCSW_OK);
			 
			 
#endif


}

FCSW_STATUS FCSW_Query_Port_Config(u32 addr, u32 *value,  FCSW_STRU *sw_dev)
{
#ifdef WORK_IN_PC
	AppBuf appBuf;
	PBuf * ppbuffer;
	u8  chanType;
	u32 evalue;
	u8  afn,fn;

	
	
	chanType = sw_dev->sw_prtl_path;

	
	PBUF_ALLOC(ppbuffer);

	APPBUF_POAOUT_INIT((&appBuf), chanType, ppbuffer,
			AFN_QUERYPARAM, PRM_REQU, FRAME_SINGLE, 0, NULL);


	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = FN_QUERY_PORTCFG;
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)(addr & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 8) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 16) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 24) & 0x000000FF); 

	FCSW_RS_PRTL_DO(&appBuf);

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;
	
	PBUF_ALLOC(ppbuffer);

	if(FCSW_OK != BOARD_PC_Recv(ppbuffer->payLoad, sw_dev, FCSW_RECVTIMEOUT))
	{
		FCSW_printf("Recieve frame error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return(FCSW_ERROR);
	}

	

	afn = *(u8 *)(ppbuffer->payLoad+ 4);
	fn = *(u8 *)(ppbuffer->payLoad+ 6); 
	
	
	if((afn != AFN_QUERYPARAM)||(fn != FN_QUERY_PORTCFG))
	{
		FCSW_printf("Recieve frame fn error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return(FCSW_ERROR);
	}

	evalue = *(u8 *)(ppbuffer->payLoad+7);
	evalue += 0x100*(*(u8 *)(ppbuffer->payLoad+8));
	evalue += 0x10000*(*(u8 *)(ppbuffer->payLoad+9));
	evalue += 0x1000000*(*(u8 *)(ppbuffer->payLoad+10));
	*value = evalue;
	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;

	return FCSW_OK;
				
#endif
#ifdef WORK_IN_BOARD
	u32 evalue;
	
	if((addr < DEV_PORT_E_D_TOV_COR(0))||(addr > DEV_CREDIE_COR(sw_dev->sw_info->port_total))||(0!=addr%4))
	{
		FCSW_printf("parament addr error\n");
		return(FCSW_ERROR);
	}
	if(sw_dev==NULL)
	{
		FCSW_printf("parament dev error\n");
		return(FCSW_ERROR);	 
	}

	
	if(FCSW_OK!=FCSW_Reg_Read(addr,&evalue,sw_dev))
	{
		FCSW_printf("reg read error\n");
		return(FCSW_ERROR);	
	}

	*value = evalue;
	return(FCSW_OK);	
				 
				 
#endif

}

FCSW_STATUS FCSW_Query_Port_Error(u32 addr, u32 *value, FCSW_STRU *sw_dev)
{
#ifdef WORK_IN_PC
	AppBuf appBuf;
	PBuf * ppbuffer;
	u8	chanType;
	u32 evalue;
	u8	afn,fn;
	
	
	
	chanType = sw_dev->sw_prtl_path;
	
	
	PBUF_ALLOC(ppbuffer);
	
	APPBUF_POAOUT_INIT((&appBuf), chanType, ppbuffer,
			AFN_QUERYPARAM, PRM_REQU, FRAME_SINGLE, 0, NULL);
	
	
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = FN_QUERY_PORTSTAT;
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)(addr & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 8) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 16) & 0x000000FF);
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((addr >> 24) & 0x000000FF); 
	
	FCSW_RS_PRTL_DO(&appBuf);
	
	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;
	
	
	
	PBUF_ALLOC(ppbuffer);
	
	if(FCSW_OK != BOARD_PC_Recv(ppbuffer->payLoad, sw_dev, FCSW_RECVTIMEOUT))
	{
		FCSW_printf("Recieve frame error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return(FCSW_ERROR);
	}
	
	
	
	afn = *(u8 *)(ppbuffer->payLoad+ 4);
	fn = *(u8 *)(ppbuffer->payLoad+ 6); 
	
	
	if((afn != AFN_QUERYPARAM)||(fn != FN_QUERY_PORTSTAT))
	{
		FCSW_printf("Recieve frame fn error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return(FCSW_ERROR);
	}
	
	evalue = *(u8 *)(ppbuffer->payLoad+7);
	evalue += 0x100*(*(u8 *)(ppbuffer->payLoad+8));
	evalue += 0x10000*(*(u8 *)(ppbuffer->payLoad+9));
	evalue += 0x1000000*(*(u8 *)(ppbuffer->payLoad+10));
	*value = evalue;
	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;
	
	return FCSW_OK;
	
#endif
#ifdef WORK_IN_BOARD
	 u32 evalue;
     
	 if((addr < FCSW_REG_DEV_PORT_STR(0))||(addr > FCSW_REG_DEV_PORT_POLARITY_ERR(sw_dev->sw_info->port_total))||(0!=addr%4))
	 {
	     FCSW_printf("parament addr error\n");
		 return(FCSW_ERROR);
	 }
	 if(sw_dev==NULL)
	 {
	     FCSW_printf("parament dev error\n");
		 return(FCSW_ERROR);	     
	 }
	 
	 
	 if(FCSW_OK!=FCSW_Reg_Read(addr,&evalue,sw_dev))
	 {
	     FCSW_printf("reg read error\n");
		 return(FCSW_ERROR);	 
	 }
	 
     *value = evalue;
	 return(FCSW_OK);
	 
	 
#endif


}

FCSW_STATUS FCSW_Query_ConfigMode(FCSW_CTRL *mode, FCSW_STRU *sw_dev)
{
#ifdef WORK_IN_PC
	AppBuf		appBuf;
	PBuf *		ppbuffer;
	ChanType	chanType;
	u32 		evalue;
	u8			afn,fn;
	u32 		raddr;

	
	
	chanType = sw_dev->sw_prtl_path;

	
	PBUF_ALLOC(ppbuffer);

	APPBUF_POAOUT_INIT((&appBuf), chanType, ppbuffer,
			AFN_QUERYPARAM, PRM_REQU, FRAME_SINGLE, 0, NULL);

	
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = FN_QUERY_CFGMODE;

	FCSW_RS_PRTL_DO(&appBuf);

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;

	
	
	PBUF_ALLOC(ppbuffer);

	if(FCSW_OK != FCSW_RS_Recv(ppbuffer->payLoad))
	{
		FCSW_printf("Recieve frame error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}			

	

	afn = *(u8 *)(ppbuffer->payLoad+ 4);
	fn = *(u8 *)(ppbuffer->payLoad+ 6); 
	
	
	if((afn != AFN_QUERYPARAM)||(fn != FN_QUERY_CFGMODE))
	{
		FCSW_printf("Recieve frame fn error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}
	*mode = *(u8 *)(ppbuffer->payLoad+ 7);

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;
	return(FCSW_OK);	 	

#endif
#ifdef WORK_IN_BOARD

    *mode = sw_dev->sw_cfg_switch;
	return FCSW_OK;
#endif

}

FCSW_STATUS FCSW_Query_SYNC_Count(u32 *get0, u32 *get1, FCSW_STRU *sw_dev)
{
#ifdef WORK_IN_PC
	AppBuf		appBuf;
	PBuf *		ppbuffer;
	ChanType	chanType;
	u32 		rget0,rget1;
	u8			afn,fn;

	
	
	chanType = sw_dev->sw_prtl_path;

	
	PBUF_ALLOC(ppbuffer);

	APPBUF_POAOUT_INIT((&appBuf), chanType, ppbuffer,
			AFN_QUERYPARAM, PRM_REQU, FRAME_SINGLE, 0, NULL);

	
	appBuf.pbuf->payLoad[appBuf.pbuf->len++] = FN_QUERY_SYNC_COUNT;

	FCSW_RS_PRTL_DO(&appBuf);

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;

	
	
	PBUF_ALLOC(ppbuffer);

	if(FCSW_OK != FCSW_RS_Recv(ppbuffer->payLoad))
	{
		FCSW_printf("Recieve frame error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}			

	

	afn = *(u8 *)(ppbuffer->payLoad+ 4);
	fn = *(u8 *)(ppbuffer->payLoad+ 6); 
	
	
	if((afn != AFN_QUERYPARAM)||(fn != FN_QUERY_SYNC_COUNT))
	{
		FCSW_printf("Recieve frame fn error\n");
		PBUF_FREE(ppbuffer);
		appBuf.pbuf = NULL;
		return FCSW_ERROR;
	}
	rget0 = *(u8 *)(ppbuffer->payLoad+7);
	rget0 += 0x100*(*(u8 *)(ppbuffer->payLoad+8));
	rget0 += 0x10000*(*(u8 *)(ppbuffer->payLoad+9));
	rget0 += 0x1000000*(*(u8 *)(ppbuffer->payLoad+10));
	*get0 = rget0;
	rget1 = *(u8 *)(ppbuffer->payLoad+11);
	rget1 += 0x100*(*(u8 *)(ppbuffer->payLoad+12));
	rget1 += 0x10000*(*(u8 *)(ppbuffer->payLoad+13));
	rget1 += 0x1000000*(*(u8 *)(ppbuffer->payLoad+14));
	*get1 = rget1;

	PBUF_FREE(ppbuffer);
	appBuf.pbuf = NULL;
	return(FCSW_OK); 	
	
#endif
#ifdef WORK_IN_BOARD
	if(FCSW_OK!=FCSW_Query_SW_Config(FCSW_REG_DEV_SYNC_GET0_FCR, get0, sw_dev))
	{
		return FCSW_ERROR;
	}
	if(FCSW_OK!=FCSW_Query_SW_Config(FCSW_REG_DEV_SYNC_GET1_FCR, get1, sw_dev))
	{
		return FCSW_ERROR;
	}		
	return FCSW_OK;
#endif

}

FCSW_STATUS FCSW_File_Trans(char *filename, u8 dir, FCSW_STRU *sw_dev)
{
#ifdef WORK_IN_PC
#endif
#ifdef WORK_IN_BOARD
#endif

}


FCSW_STATUS FCSW_Recv_Active_Report(AppBuf *appBuf, u8 *port, u32 *regaddr, u32 *value)
{
#ifdef WORK_IN_PC
#endif
#ifdef WORK_IN_BOARD
#endif

}






