
/**
  ******************************************************************************
  * @par Copyright (c): ChengDu BinHong Science & Technology Co.,ltd
  * @file    PBuf.C
  * @author  chenxu
  * @version V2.0.0
  * @date    2017/07/05
  * @defgroup USER
  * @ingroup  USER
  * @brief   Templet for header files. You can change this templet depend on your
  *          requirments.
  * @par History
  * Date      	Owner       BugID/CRID        Contents
  * 2017/07/05	chenxu		
  ****************************************************************************** 
  */

/* Includes ------------------------------------------------------------------*/
#include "FCSW_Mgr\RS\Buf\PBuf.h"
#include "FCSW_Type.h"



/** @addtogroup USER
  * @{
  */
  
/* Private macro -------------------------------------------------------------*/
/** Description of the macro */
#define PBUF_ITEM_MAX   4

/* Private variables ---------------------------------------------------------*/
static PBuf sysPBuf[PBUF_ITEM_MAX];

/* Private typedef -----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief   This function   
  * @param[in]   inArgName  input argument description. 
  * @param[out]  outArgName output argument description.  
  * @retval  FCSW_OK      Success 
  * @retval  ERROR   Failed
  * @par History
  * Date          Owner         BugID/CRID        Contents
  * 2015/02/05    hezongbin     None              Draft 
  */

PBuf * PBufAlloc (void)
{
    u32 idx;
    
    for(idx = 0; idx < PBUF_ITEM_MAX; idx++)
    {
        if(0 == sysPBuf[idx].isUsed)
        {
            sysPBuf[idx].isUsed = 1;
            return (&sysPBuf[idx]);
        }
            
    }
    
    return NULL;
}

void PBufFree(PBuf * pbuf)
{
    pbuf->isUsed = 0;
}

/**
  * @}
  */
/*********************************End Of File *********************************/

