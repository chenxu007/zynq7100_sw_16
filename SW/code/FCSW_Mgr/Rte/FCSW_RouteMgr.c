/* 閸掓繂顬婇崠鏍у礋閹绢叀鐭鹃悽杈�缁狅紕鎮婂Ο鈥虫健閿涘苯鍨庨柊宥堢熅閻㈣鲸娼弫锟�/ */
#include "FCSW_Type.h"
#include "FCSW_Mgr\Cfg\FCSW.h"
#include "FCSW_RouteMgr.h"



FCSW_STATUS FCSW_RTE_Manager_Init(FCSW_STRU *sw_dev)
{
    FCSW_RTE_MANAGER_STRU *rte_list;
	u8 rte_max_single,rte_max_multi,rte_max_broadcast;
	u8 tmp,tmp2;
	u8 port_total;
	FCSW_PORT_CFG_FCSW_STATUS_STRU* port_cfg;

    port_total = sw_dev->sw_info->port_total;
	rte_list = malloc(port_total*sizeof(FCSW_RTE_MANAGER_STRU));

	tmp = sw_dev->sw_info->mc_lut_size;
	if(rte_max_single > tmp)
	{
	    return FCSW_ERROR;
	}
	rte_max_single = sw_dev->sw_info->port_total;
    tmp -= rte_max_single;
	if(tmp<1)
	{
	    return FCSW_ERROR;
	}
	rte_max_broadcast = 1;
	tmp -= rte_max_broadcast;
	rte_max_multi = tmp;


	
	for(tmp=0;tmp<sw_dev->sw_info->port_total;tmp++)
	{
	    port_cfg->rte_list = (void* )rte_list;
		rte_list->rte_list_single = malloc(sizeof(FCSW_RTE_LIST_STRU));
		rte_list->rte_list_multi = malloc(sizeof(FCSW_RTE_LIST_STRU));
		rte_list->rte_list_broadcast = malloc(sizeof(FCSW_RTE_LIST_STRU));
		
		rte_list->rte_list_single->length = rte_max_single;
		rte_list->rte_list_single->free = rte_max_single;
		rte_list->rte_list_single->used = 0;
		rte_list->rte_list_single->deleted = 0;
		rte_list->rte_list_single->rte_cfg_list = malloc(rte_max_single*sizeof(FCSW_RTE_CFG_STRU));
        for(tmp2=0;tmp2<rte_max_single;tmp2++)
    	{
    	    rte_list->rte_list_single->rte_cfg_list->inuse = 0;
			rte_list->rte_list_single->rte_cfg_list++;
    	}
        rte_list->rte_list_single->deleted_serial_list = malloc(rte_max_single);
		rte_list->rte_list_multi->length = rte_max_multi;
		rte_list->rte_list_multi->free = rte_max_multi;
		rte_list->rte_list_multi->used = 0;
		rte_list->rte_list_multi->deleted = 0;
		rte_list->rte_list_multi->rte_cfg_list = malloc(rte_max_multi*sizeof(FCSW_RTE_CFG_STRU));
        for(tmp2=0;tmp2<rte_max_multi;tmp2++)
    	{
    	    rte_list->rte_list_single->rte_cfg_list->inuse = 0;
			rte_list->rte_list_single->rte_cfg_list++;
    	}		
        rte_list->rte_list_multi->deleted_serial_list = malloc(rte_max_multi);
		rte_list->rte_list_broadcast->length = rte_max_broadcast;
		rte_list->rte_list_broadcast->free = rte_max_broadcast;
		rte_list->rte_list_broadcast->used = 0;
		rte_list->rte_list_broadcast->deleted = 0;
		rte_list->rte_list_broadcast->rte_cfg_list = malloc(rte_max_broadcast*sizeof(FCSW_RTE_CFG_STRU));
        for(tmp2=0;tmp2<rte_max_broadcast;tmp2++)
    	{
    	    rte_list->rte_list_single->rte_cfg_list->inuse = 0;
			rte_list->rte_list_single->rte_cfg_list++;
    	}
        rte_list->rte_list_broadcast->deleted_serial_list = malloc(rte_max_broadcast);
		port_cfg++;
		rte_list++;
	}
}


FCSW_RTE_LIST_STRU* FCSW_RTE_Manager_Get_List(u8 inPort, FCSW_STRU *sw_dev)
{
    FCSW_RTE_MANAGER_STRU* rte_list;
	FCSW_RTE_LIST_STRU* rte_list_single;
	FCSW_PORT_CFG_FCSW_STATUS_STRU* port_cfg;

	port_cfg = sw_dev->sw_port_cfg +inPort;
	rte_list = port_cfg->rte_list;
	rte_list_single = rte_list->rte_list_single;

	return rte_list_single;
}

u8   FCSW_RTE_Manager_Get_Used(FCSW_RTE_LIST_STRU* rte_list)
{
    u8 used;

	used = rte_list->used;

	return used;

}
u8   FCSW_RTE_Manager_Get_Max(FCSW_RTE_LIST_STRU* rte_list)
{
    u8 max;

	max = rte_list->length;

	return max;

}
u8   FCSW_RTE_Manager_RTE_List_Is_Full(FCSW_RTE_LIST_STRU* rte_list)
{
    u8 free;

	free = rte_list->free;
	if(free==0)
	{
	    return 0;
	}

	return 1;

}
u8   FCSW_RTE_Manager_Get_NewSerial(FCSW_RTE_LIST_STRU* rte_list)
{
    u8 newserial;

	if(rte_list->deleted>0)
	{
	    newserial = (u8*)(rte_list->deleted_serial_list + (rte_list->deleted-1));
		free(rte_list->deleted_serial_list + (rte_list->deleted-1));
	}
	else
	{
	    if(rte_list->free>0)
	    {
	        newserial = rte_list->used;
	    }
		else/*//婵″倹鐏夋稉宥嗩棏閺屻儴銆冨鈥虫皑閻╁瓨甯撮崙鍡楊樃婵夘偉銆冮柇锝呮皑鐟曞棛娲婄粭顑跨閺夛拷*/
		{
		    newserial = 0;
		}
	}

	return newserial;

}
void FCSW_RTE_Manager_Add(u8 inPort, u32 outPortMask0, u32 outPortMask1, u32 did, u8 serial, FCSW_RTE_LIST_STRU* rte_list)
{
    FCSW_RTE_CFG_STRU* cfg_list;
    
	if(rte_list->deleted>0)
	{
	    rte_list->deleted--;
		rte_list->free--;
		rte_list->used++;
		free(rte_list->deleted_serial_list + (rte_list->deleted-1));
	}
	else
	{
	    if(rte_list->free>0)
	    {
	        rte_list->free--;
			rte_list->used++;
	    }
	}

	cfg_list = rte_list->rte_cfg_list + serial;
	cfg_list->did = did;
	cfg_list->outPortMask0 = outPortMask0;
	cfg_list->outPortMask1 = outPortMask1;
	cfg_list->inuse = 1;

}
u8 FCSW_RTE_Manager_Find_Serial(u8 inPort, u32 did, FCSW_RTE_LIST_STRU* rte_list)
{
    FCSW_RTE_CFG_STRU* cfg_list; 
	u8 tmp;

    cfg_list = rte_list->rte_cfg_list;
	while(cfg_list->did == did)
	{
	   cfg_list++;
	   tmp++;
	   if(tmp>rte_list->length)
	   	{
	   	    tmp = 0xff;
		    break;
	   	}
	}
	return tmp;

}
FCSW_STATUS FCSW_RTE_Manager_Delete(u8 inPort, u8 serial, FCSW_RTE_LIST_STRU* rte_list)
{
    FCSW_RTE_CFG_STRU* cfg_list;    

    cfg_list = rte_list->rte_cfg_list + inPort;
	if(cfg_list->inuse == 0)
		return FCSW_ERROR;
		
	cfg_list->did = 0;
	cfg_list->outPortMask0 = 0;
	cfg_list->outPortMask1 = 0;
	cfg_list->inuse = 0;
	rte_list->deleted++;
	*(rte_list->deleted_serial_list + rte_list->deleted) = serial;
	rte_list->free++;
	rte_list->used--;

	return FCSW_OK;
}
void FCSW_RTE_Manager_Delete_All(u8 inPort, FCSW_RTE_LIST_STRU* rte_list)
{
	FCSW_RTE_CFG_STRU* cfg_list; 
	u8 tmp;

	cfg_list = rte_list->rte_cfg_list + inPort;

	tmp = rte_list->length;

	for(;tmp>0;tmp--)
	{
        cfg_list->inuse = 0;
		cfg_list++;	
	}
    tmp = rte_list->deleted;
	for(;tmp>0;tmp--)
	{
	    free(rte_list->deleted_serial_list + tmp);
	}
	rte_list->deleted = 0;
	rte_list->free = rte_list->length;
	rte_list->used = 0;

}


