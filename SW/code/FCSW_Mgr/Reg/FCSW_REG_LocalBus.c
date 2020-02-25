#include "FCSW_Type.h"

u32 FCSW_Reg_Read_LB(u32 addr)/* 读寄存器*/
{
    u32 val;
	
    val = *(volatile u32 *)addr;
	return val;
}
void FCSW_Reg_Write_LB(u32 addr, u32 value)/* 写寄存器*/
{

    *(volatile u32 *)addr = value;

}

