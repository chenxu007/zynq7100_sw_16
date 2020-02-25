
/**
  ******************************************************************************
  * @par Copyright (c): ChengDu BinHong Science & Technology Co.,ltd
  * @file    PBuf.H
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
#ifndef __PBUF_H__
#define __PBUF_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <assert.h>
#include "FCSW_Type.h"

/** @addtogroup 
  * @{
  */
  
/* Exported macro ------------------------------------------------------------*/

#define PBUF_ALLOC(pbuf) \
    pbuf = PBufAlloc(); \
    assert(NULL != pbuf)
    
#define PBUF_FREE(pbuf) \
    PBufFree(pbuf); \
    pbuf = NULL

#define PBUF_BYTE_MAX   1024

/* Exported types ------------------------------------------------------------*/

typedef struct 
{
    u8 isUsed;
    
    /** pointer to the actual data in the buffer */
    u8 payLoad[PBUF_BYTE_MAX];

    u16 len;
}PBuf; 
  
/* Exported constants --------------------------------------------------------*/


/* Exported Global Variables -------------------------------------------------*/

extern PBuf * PBufAlloc (void);
extern void PBufFree(PBuf * pbuf);

/* Exported functions ------------------------------------------------------- */
/**  Description of this function  */

#ifdef __cplusplus
 }
#endif 
 
#endif /* PBuf.H */

/**
  * @}
  */

/*********************************End Of File *********************************/

