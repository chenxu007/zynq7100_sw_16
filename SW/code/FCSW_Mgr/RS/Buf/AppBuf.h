
/**
  ******************************************************************************
  * @par Copyright (c): ChengDu BinHong Science & Technology Co.,ltd
  * @file    APPBUF.H
  * @author  chenxu
  * @version V2.0.0
  * @date    2017/07/05
  * @defgroup UTM
  * @ingroup  UTM  
  * @brief   Templet for header files. You can change this templet depend on your
  *          requirments.
  * @par History
  * Date          Owner         BugID/CRID        Contents
  * 2017/07/05    chenxu     	None              Draft
  ****************************************************************************** 
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APPBUF_H__
#define __APPBUF_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "FCSW_Mgr\RS\Buf\PBuf.h"


/** @addtogroup UTM
  * @{
  */
  
/* Exported macro ------------------------------------------------------------*/

#define APPBUF_RAW             0x01 /*Ϊ��Լ��ԭʼ���뱨��*/
#define APPBUF_POAIN           0x02 /*Ϊ��ԼӦ�ò����뱨��*/
#define APPBUF_POAOUT          0x03 /*Ϊ��ԼӦ�ò��������*/

#ifndef FCSW_RS_PRTL_BSN_DATA_OFF
#define FCSW_RS_PRTL_BSN_DATA_OFF     6u
#endif

#define APPBUF_RAW_INIT(appBuf, type, ppbuf)\
    do\
    {\
        if ((appBuf) != NULL)\
        {\
            (appBuf)->bufTyp = APPBUF_RAW;\
            (appBuf)->chTyp = (type);\
            (appBuf)->pbuf = ppbuf; \
            break;\
        }\
    }while(1)

#define APPBUF_POAIN_INIT(appBuf, type, ppbuf, Afn, Prm) \
    do \
    { \
        if ((appBuf) != NULL) \
        { \
            (appBuf)->bufTyp = APPBUF_POAIN; \
            (appBuf)->chTyp = (type); \
            (appBuf)->pbuf = ppbuf; \
            (appBuf)->poa.in.afn = (Afn); \
            (appBuf)->poa.in.prm = (Prm); \
            break; \
        } \
    } \
    while (1)

#define APPBUF_POAOUT_INIT(appBuf, type, ppbuf, Afn, Prm, Fts, Con, CallBack) \
    do \
    { \
        if ((appBuf) != NULL) \
        { \
            (appBuf)->bufTyp = APPBUF_POAOUT; \
            (appBuf)->chTyp = (type); \
            (appBuf)->pbuf = ppbuf; \
            (appBuf)->pbuf->len = FCSW_RS_PRTL_BSN_DATA_OFF; \
            (appBuf)->poa.out.afn = (Afn); \
            (appBuf)->poa.out.prm = (Prm); \
            (appBuf)->poa.out.fts = (Fts); \
            (appBuf)->poa.out.con = (Con); \
            (appBuf)->poa.out.callback = (CallBack); \
            break; \
        } \
    } \
    while (1)

/* Exported types ------------------------------------------------------------*/

typedef struct
{
    /**
     * ԭʼ���Ĵӵ�1���ֽڿ�ʼ,Ӧ�ò����ݱ�����ӵ�15���ֽڿ�ʼ,ΪDADT��
     * 1.����β���ĸ�����Ϣ��Ԫ��ɾ��
     * 2.frame->tot_len = 14 + �����򳤶ȣ�������aux����
     */
    PBuf* pbuf;
    /**
     * buf����
     * 1: ԭʼ����;
     * 2: ϵͳ�ڲ�ƴװ�ĵ������ԼӦ�ò㱨��
     * 3: ϵͳӦ����������͵������ԼӦ�ò㱨��
     */
    u8 bufTyp;
    u8 chTyp;                     /**< �ŵ����� */
    union
    {
        /**
         * ������Ϣ
         */
        struct
        {
            u8 afn;                 /**< �����ͱ��ĵ�Ӧ�ù����� */
            u8 prm;                 /**< ���������ͻ���Ӧ���� */
        } in;

        /**
         * �����Ϣ
         */
        struct
        {
            u8 afn;                 /**< �����ͱ��ĵ�Ӧ�ù����� */
            u8 prm;                 /**< ���������ͻ���Ӧ���� */
            u8 fts;                 /**< ֡���ͱ�ʶ:�������Ƕ�� */
            u8 con;                 /**< ȷ�ϱ�ʶ:0 - ����ȷ�ϣ� 1 - Ҫȷ�� */
            void (*callback)(void);    /**< ���ķ��ͺ���õĹ��Ӻ���*/
        } out;
    } poa;
} AppBuf;

/* Exported constants --------------------------------------------------------*/


/* Exported Global Variables -------------------------------------------------*/
 /**  Description of global variable  */

/* Exported functions ------------------------------------------------------- */
/**  Description of this function  */

#ifdef __cplusplus
}
#endif 

#endif /* APPBUF.H */

/**
  * @}
  */

/*********************************End Of File *********************************/

