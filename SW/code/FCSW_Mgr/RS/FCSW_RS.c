#include "FCSW_Mgr\RS\FCSW_CheckFrame.h"
#include "FCSW_Mgr\Cfg\FCSW.h"


CheckFrameCB *RS_CheckFrame;
void FCSW_RS_Init(void);
void FCSW_RS_Send(AppBuf * appBuf);
void FCSW_RS_Read(AppBuf * appBuf);

void FCSW_RS_CheckFrame(ChanType chanType, u8 *buf, u32 len);


void FCSW_RS_Init(void)
{
    RS_CheckFrame = CheckFrameInit();

}
void FCSW_RS_Send(AppBuf * appBuf)
{
#ifdef WORK_IN_PC
    FCSW_RS_Client_Send(appBuf);
#endif
#ifdef WORK_IN_BOARD
    FCSW_RS_Server_Send(appBuf);
#endif

}

void FCSW_RS_Recv(AppBuf * appBuf)
{
#ifdef WORK_IN_PC
		FCSW_RS_Client_Recv(appBuf);
#endif

}

void FCSW_RS_CheckFrame(ChanType chanType, u8 *buf, u32 len)
{
	RS_CheckFrame->pchkfrm(RS_CheckFrame, chanType, buf, len);
}
