#include "FCSW_Type.h"

#pragma pack(push, 1)

/* 瀹氫箟鍗曟潯璺敱琛ㄩ厤缃粨鏋勪綋 */
typedef struct
{
    u32 did;		
	u32 outPortMask0;		
	u32 outPortMask1;
	u8  inuse;
}FCSW_RTE_CFG_STRU;

typedef struct
{
    u8 length;		/* 璺敱琛ㄩ暱搴�  */
	u8 used;		/* 宸蹭娇鐢ㄨ鏁�/*/
	u8 free;		/* 绌洪棽鐨勮鏁�/*/
	u8 deleted;		/* 鍒犻櫎鐨勮鏁�/ */
	u8* deleted_serial_list; /* 鍒犻櫎鐨勫簭鍙峰垪琛�/*/
	FCSW_RTE_CFG_STRU* rte_cfg_list;
}FCSW_RTE_LIST_STRU;


typedef struct
{
	FCSW_RTE_LIST_STRU *rte_list_single;/* 鍗曟挱璺敱绠＄悊琛�/ */
	FCSW_RTE_LIST_STRU *rte_list_multi;/* 缁勬挱璺敱绠＄悊琛�/*/
	FCSW_RTE_LIST_STRU *rte_list_broadcast;/* 骞挎挱璺敱绠＄悊琛�/*/
}FCSW_RTE_MANAGER_STRU;
	
#pragma pack(pop)

/* 瀹氫箟璺敱琛ㄧ鐞嗙粨鏋勪綋 */


FCSW_STATUS FCSW_RTE_Manager_Init(FCSW_STRU *sw_dev);
FCSW_RTE_LIST_STRU* FCSW_RTE_Manager_Get_List(u8 inPort, FCSW_STRU *sw_dev);
u8   FCSW_RTE_Manager_Get_Used(FCSW_RTE_LIST_STRU* rte_list);
u8   FCSW_RTE_Manager_Get_Max(FCSW_RTE_LIST_STRU* rte_list);
u8   FCSW_RTE_Manager_RTE_List_Is_Full(FCSW_RTE_LIST_STRU* rte_list);
u8   FCSW_RTE_Manager_Get_NewSerial(FCSW_RTE_LIST_STRU* rte_list);
void FCSW_RTE_Manager_Add(u8 inPort, u32 outPortMask0, u32 outPortMask1, u32 did, u8 serial, FCSW_RTE_LIST_STRU* rte_list);
u8 FCSW_RTE_Manager_Find_Serial(u8 inPort, u32 did, FCSW_RTE_LIST_STRU* rte_list);
FCSW_STATUS FCSW_RTE_Manager_Delete(u8 inPort, u8 serial, FCSW_RTE_LIST_STRU* rte_list);
void FCSW_RTE_Manager_Delete_All(u8 inPort, FCSW_RTE_LIST_STRU* rte_list);









