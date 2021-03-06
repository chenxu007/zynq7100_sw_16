#ifndef FCSW_H
#define FCSW_H



#include "FCSW_Type.h"
#include "FCSW_Mgr\RS\FCSW_RS_PRTL.h"

/*// Must FCSW_EXPORTS *only* when building the DLL.*/
#ifdef WORK_IN_PC
#ifdef FCSW_EXPORTS
#define FCSWAPI __declspec(dllexport)
#else
#define FCSWAPI __declspec(dllimport)
#endif
#else
#define FCSWAPI
#endif

#ifdef WORK_IN_PC
/*// Define calling convention in one place, for convenience.*/
#define FCSWCALL __cdecl
#else
#define FCSWCALL 
#endif

#ifdef WORK_IN_PC
#ifdef __cplusplus
extern "C" {
#endif
#endif

#define FCSW_SPEED_16G  0x10
#define FCSW_SPEED_8G   0x8
#define FCSW_SPEED_4G   0x4
#define FCSW_SPEED_2G   0x2
#define FCSW_SPEED_1G   0x1


#define FCSW_PORT_TYPE_F  0x1
#define FCSW_PORT_TYPE_E  0x2


#define FCSW_PORT_MON_TYPE_INPUT  0x1
#define FCSW_PORT_MON_TYPE_OUTPUT 0x10
#define FCSW_PORT_MON_TYPE_MSGID  0x100


#define FCSW_PORT_TRUNK_2  0x02
#define FCSW_PORT_TRUNK_4  0x04
#define FCSW_PORT_TRUNK_8  0x08
#define FCSW_PORT_TRUNK_16 0x10


#define FCSW_SYNC_MODE_SERVER  0x1
#define FCSW_SYNC_MODE_CLIENT  0x2


#define FCSW_PORT_LINK_RESET  	0x1
#define FCSW_PORT_LINK_OUTOFSYNC  0x2
#define FCSW_PORT_LINK_FAILURE  	0x4
#define FCSW_PORT_LINK_ACTIVE 	0x8


#define FCSW_PORT_ERR_SOF_EN  		0x1
#define FCSW_PORT_ERR_EOF_EN      	(0x1<<1)
#define FCSW_PORT_ERR_SHORT_EN  		(0x1<<2)
#define FCSW_PORT_ERR_LONG_EN 		(0x1<<3)
#define FCSW_PORT_ERR_CRC_EN      	(0x1<<4)
#define FCSW_PORT_ERR_DID_EN  		(0x1<<5)
#define FCSW_PORT_ERR_CODE_EN 		(0x1<<6)
#define FCSW_PORT_ERR_POLARITY_EN     (0x1<<7)


#define FCSW_MODE_48PORT  0x1 
#define FCSW_MODE_24PORT  0x2 
#define FCSW_MODE_16PORT  0x3 
#define FCSW_MODE_8PORT 	0x4 


#define SW_IPV4 "192.168.1.250"
#define SW_IP_PORT 6000
#define FCSW_printf printf
#define FCSW_Dbg_printf printf

#pragma pack(push, 1)

typedef enum {
	FCSW_OK=0,
	PARAM_ERROR,
	ADDR_ERROR,
	INVALID_ERROR,
	FCSW_ERROR
}FCSW_STATUS;

typedef enum {
	FCSW_PATH_LOCALBUS,
	FCSW_PATH_AXI
}FCSW_CFG_PATH;

typedef enum {
	FCSW_PC_CTRL,
	FCSW_BOARD_CTRL
}FCSW_CTRL;


typedef struct
{
	u32* msgid_list;
	u8   used;
	u8   max;
}FCSW_MON_CFG_STRU;


typedef struct
{
	u8 domain_id;
	u8 e_port;
}FCSW_ISL_CFG_STRU;



typedef struct
{
	char *filehead;
	char *curr_linehead;
}FCSW_CFG_FILE_CFG_STRU;


typedef struct
{
	u8 sw_mode;
	u8 sync_mode;
	u8 client_sync_port;
	u16 server_sync_gap;
	u8 domain_id;
	u8 mon_en;
	u8 mon_type;
	u8 mon_ingress_port;
	u8 mon_egress_port;
	FCSW_MON_CFG_STRU *mon_msgid_list;
	FCSW_ISL_CFG_STRU *isl_cfg_list;
}FCSW_SW_CFG_FCSW_STATUS_STRU;


typedef struct
{
	u16 deviceid;
	u16 vendorid;
	u8  version[3];
	u8  sw_support;
	u8  speed;
	u8  monitor_support;
	u8  bcast_support;
	u8  mcast_support;
	u8  bb_credit_support;
	u8  redundant_support;
	u8  priority_support;
	u8  f_port_support;
	u8  e_port_support;
	u8  synx_support;
	u8  syny_support;
	u8  synz_support;
	u8  err_capture_support;
	u8  clk_server_support;
	u8  clk_client_support;
	u8  isl_trunk_support;
	u8  port_total;
	u8  monitor_port_num;
	u16 lut_size;
	u8  mc_lut_size;
	u8  mon_msgid_num;
	u16 trunk_group_num;
}FCSW_SW_INFO_STRU;


typedef struct
{
	u8 rx_en;
	u8 tx_en;
	u8 speed;
	u8 bb_credit;
	u8 trunk_en;
	u16 trunk_ch_num;
	u8 port_type;
	u8 sync_out_en;
	u16 e_d_tov;
	u16 r_d_tov;
	u8 used_rte_number;
	void *rte_list;
	u32 err_en;
}FCSW_PORT_CFG_FCSW_STATUS_STRU;


typedef struct
{
#ifdef WORK_IN_PC
#endif
#ifdef WORK_IN_BOARD
	FCSW_SW_INFO_STRU *sw_info;
	FCSW_SW_CFG_FCSW_STATUS_STRU *sw_cfg;
	FCSW_PORT_CFG_FCSW_STATUS_STRU *sw_port_cfg;
	FCSW_MON_CFG_STRU *mon_cfg;
	FCSW_CFG_PATH sw_path;
	u32 RegBase;
#endif
    ChanType sw_prtl_path;
	FCSW_CTRL sw_cfg_switch;
	u8 swipv4[50];
    u8 pcipv4[30];
	u8 swport;
	u8 prepareOK_flag; 

}FCSW_STRU;






typedef struct
{
	u8 rx_en;
	u8 tx_en;
	u8 speed;
	u8 bb_credit;
	u8 link_stas;
	u32 rxcnt;
	u32 txcnt;
	u32 err_stat;
	u32 err_sof_cnt;
	u32 err_eof_cnt;
	u32 err_short_cnt;
	u32 err_long_cnt;
	u32 err_crc_cnt;
	u32 err_did_cnt;
	u32 err_code_cnt;
	u32 err_polarity_cnt;
}FCSW_PORT_STAU_STRU;


#pragma pack(pop)





#define FCSW_RECVTIMEOUT 1000

extern FCSW_STRU* sw_dev;
#ifdef WORK_IN_PC
FCSWAPI FCSW_STRU* FCSW_Dev_Init(struct in_addr server_addr, int port_num);
#else
FCSWAPI FCSW_STRU* FCSW_Dev_Init(void);
#endif
FCSWAPI FCSW_STATUS FCSW_Reg_Read(u32 addr, u32 *value, FCSW_STRU *sw_dev);
FCSWAPI FCSW_STATUS FCSW_Reg_Write(u32 addr, u32 value, FCSW_STRU *sw_dev);
FCSWAPI FCSW_STATUS FCSW_WriteReg_bit(u32 addr, u32 value, u32 validmask, FCSW_STRU *sw_dev);
FCSWAPI FCSW_STATUS FCSW_RTE_Config_Single(u8 inPort, u8 outPort, u32 did, FCSW_STRU *sw_dev);
FCSWAPI FCSW_STATUS FCSW_RTE_Config_Multi(u8 inPort, u32 outPortMask0, u32 outPortMask1, u32 did, FCSW_STRU *sw_dev);
FCSWAPI FCSW_STATUS FCSW_RTE_Config_BroadCast(u8 inPort, u32 did, FCSW_STRU *sw_dev);
FCSWAPI FCSW_STATUS FCSW_RTE_Config_ISL(u8 EportNum, u8 onoff, u16 domain_id, FCSW_STRU *sw_dev);
FCSWAPI FCSW_STATUS FCSW_RTE_Delete_FPort_Num(u8 inPort, u8 serial, FCSW_STRU *sw_dev);
FCSWAPI FCSW_STATUS FCSW_RTE_Delete_Single_FPort_Did(u8 inPort, u32 did, FCSW_STRU *sw_dev);
FCSWAPI FCSW_STATUS FCSW_RTE_Delete_Multi_FPort_Did(u8 inPort, u32 did, FCSW_STRU *sw_dev);
FCSWAPI FCSW_STATUS FCSW_RTE_Delete_BroadCast_FPort_Did(u8 inPort, u32 did, FCSW_STRU *sw_dev);
FCSWAPI FCSW_STATUS FCSW_RTE_Delete_FPort(u8 inPort, FCSW_STRU *sw_dev);
FCSWAPI FCSW_STATUS FCSW_RTE_Delete_AllF(FCSW_STRU *sw_dev);

FCSWAPI FCSW_STATUS FCSW_Config_MON(u8 mode, u8 port, u8 enable, FCSW_STRU *sw_dev);
FCSWAPI FCSW_STATUS FCSW_Config_MON_msg(u8 port, u32 msgid, u8 msgidnum, FCSW_STRU *sw_dev);
FCSWAPI FCSW_STATUS FCSW_Config_SYNC_sever(u16 gap, u32 set0, u32 set1, FCSW_STRU *sw_dev);
FCSWAPI FCSW_STATUS FCSW_Config_SYNC_client(u8 port, FCSW_STRU *sw_dev);


FCSWAPI FCSW_STATUS FCSW_Set_ConfigMode(FCSW_CTRL mode, FCSW_STRU *sw_dev);

FCSWAPI FCSW_STATUS FCSW_Query_SW_Info(u32 addr, u32 *value,  FCSW_STRU *sw_dev);
FCSWAPI FCSW_STATUS FCSW_Query_SW_Config(u32 addr, u32 *value,  FCSW_STRU *sw_dev);
FCSWAPI FCSW_STATUS FCSW_Query_Port_Config(u32 addr, u32 *value,  FCSW_STRU *sw_dev);
FCSWAPI FCSW_STATUS FCSW_Query_Port_Error(u32 addr, u32 *value, FCSW_STRU *sw_dev);
FCSWAPI FCSW_STATUS FCSW_Query_ConfigMode(FCSW_CTRL *mode, FCSW_STRU *sw_dev);
FCSWAPI FCSW_STATUS FCSW_Query_SYNC_Count(u32 *get0, u32 *get1, FCSW_STRU *sw_dev);

FCSWAPI FCSW_STATUS FCSW_File_Trans(char *filename, u8 dir, FCSW_STRU *sw_dev);






typedef struct 
{
    u8 isUsed;
    
    u8 payLoad[PBUF_BYTE_MAX];

    u16 len;
}FCSW_Query_Answer_STRU; 

#endif

