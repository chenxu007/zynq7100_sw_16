#include "FCSW_Type.h"
#include "FCSW_Mgr\Cfg\FCSW.h"

u32 FCSW_Reg_Read_AXI(u32 addr)
{
    u32 val;
	
    val = *(volatile u32 *)addr;
    FCSW_Dbg_printf("FCSW_Reg_Read_AXI 0x%08x, val=0x%08x\n",addr,val);
	return val;
}
void FCSW_Reg_Write_AXI(u32 addr, u32 value)
{
    *(volatile u32 *)addr = value;
    FCSW_Dbg_printf("FCSW_Reg_Write_AXI addr 0x%08x= 0x%08x\n",addr,value);
}

