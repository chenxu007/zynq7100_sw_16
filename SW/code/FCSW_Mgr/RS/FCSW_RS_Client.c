#include "FCSW_Mgr\Cfg\FCSW.h"
#include "FCSW_Mgr\Inc\FCSW_Debug.h"
#ifdef WORK_IN_PC
#include "FCSW_Type.h"
#include "FCSW_Mgr\RS\Buf\AppBuf.h"
#include "FCSW_Mgr\RS\FCSW_CheckFrame.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
//#include <unistd.h>
//#include <arpa/inet.h>
//#include <sys/socket.h>

u8 *buf;
static int fd_client = -1;
#define BUFSIZE 200


void FCSW_RS_Client_Task(void);
FCSW_STATUS FCSW_RS_Client_Recv(AppBuf * appBuf);


int FCSW_RS_Client_Init(struct in_addr server_addr, int port_num)
{
	struct sockaddr_in serv_addr;

	memset(&serv_addr, 0, sizeof(serv_addr));

    FCSW_RS_Init();

	fd_client = socket(PF_INET, SOCK_STREAM, 0);
	if (fd_client == -1) {
		FCSW_printf("socket() error");
        return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr = server_addr;
	serv_addr.sin_port = htons(port_num);

	if( connect(fd_client, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
	    FCSW_printf("connect() error");
        return -1;
	}

	return 0;
}

FCSW_STATUS FCSW_RS_Client_Send(AppBuf *appBuf)
{
	int buf_len = appBuf->pbuf->len;
    int len;

    while(buf_len)
	{
	    len = send(fd_client, appBuf->pbuf->payLoad, appBuf->pbuf->len, MSG_WAITALL);
        FCSW_ASSERT(len >= 0);
		if(len > 0)
			buf_len -= len;
	}

    return FCSW_OK;   
}

FCSW_STATUS FCSW_RS_Client_Recv(AppBuf *appBuf)
{
	int str_len;

	str_len = recv(fd_client, appBuf->pbuf->payLoad, BUFSIZE, 0);
	if (str_len == -1) {
	    FCSW_printf("recv() error");
	} 
	FCSW_RS_CheckFrame(CHAN_TYPE_Eth0, appBuf->pbuf->payLoad, str_len);

    return FCSW_OK;
}
#endif

