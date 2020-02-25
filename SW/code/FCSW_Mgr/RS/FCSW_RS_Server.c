#include "FCSW_Mgr\Cfg\FCSW.h"
#ifdef WORK_IN_BOARD
#include <socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <vxWorks.h>
#include "FCSW_Type.h"
#include "FCSW_Mgr\RS\Buf\AppBuf.h"
#include "FCSW_Mgr\RS\FCSW_CheckFrame.h"

u8 *buf;
staitc int fd_server;
#define BUFSIZE 200

void FCSW_RS_Server_Task(void);


void FCSW_RS_Server_Init(void)
{
	struct sockaddr_in server_addr;


    buf =malloc(BUFSIZE);
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SW_IP_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_len = sizeof(server_addr);

	FCSW_RS_Init();
	
	/* 第一步，socket*/
	fd_server = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_server < 0)
    {
            /* show the error information */
        printf ("create socket error: %s",strerror(errno));
        return;
    }
    
    /* 第二步， bind */
    if (bind (fd_server, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        close(fd_server);
        printf ("bind error: %s",strerror(errno));
        return;
    }
	
	printf("Bind Success!\n");
	
	/* 3. listen*/
    if (listen(fd_server, 1) < 0)
    {
        close(fd_server);
        printf ("listen error: %s",strerror(errno));
        return;
    }
	printf("Listening....\n");
	
	/* 4.创建一个进程来处理连接之后收到的数据  */
	if(NULL == taskSpawn("runTask", 100, 0, 100 * 1024, FCSW_RS_Server_Task, 0,0,0,0,0,0,0,0,0,0))
	{
		printf("create runTask failed!!!\n");
		return;
	}

}
void FCSW_RS_Server_Send(AppBuf * appBuf)
{
    int numbytes;
    size_t len;
    
    len = appBuf->pbuf->len;
    
    numbytes = send(fd_client,appBuf->pbuf->payLoad,len,0);   
    
    if(numbytes==-1)
    {
    	printf ("send error: %s\n", strerror(errno));	
    }
}

void FCSW_RS_Server_Task(void)
{
	struct sockaddr_in client_addr;
	int retbytes;
	u8  tmp;
	socklen_t client_len;
	
	client_len = sizeof(client_addr);
	
	while(1)
	{
		if ((fd_client = accept(fd_server, (struct sockaddr*)&client_addr, &client_len)) == -1)
		{
			printf ("accept error: %s",strerror(errno));
		}
		else
		{
			printf ("peer %s connected\n",inet_ntoa(client_addr.sin_addr));
			while(1)
			{
				retbytes = recv(fd_client, buf, BUFSIZE, 0);
				if(retbytes < 0)
				{
					printf ("recv error: %s\n", strerror(errno));
					break;
				}
				printf("recv buf = :\n");
				for(tmp=0;tmp<retbytes;tmp++)
				{
					printf("%02x ", buf[tmp]);
				}
				FCSW_RS_CheckFrame(CHAN_TYPE_Eth0, buf, retbytes);
				memset(buf,0,BUFSIZE);
			}
            close(fd_client);
		}
		
	}
    
}

#endif
