

/**
  ******************************************************************************
  * @par Copyright (c): ChengDu BinHong Science & Technology Co.,ltd
  * @file    C_File_Templet.C
  * @author  chenxu
  * @version V2.0.0
  * @date    2017/07/05
  * @defgroup UTM
  * @ingroup  DAL
  * @brief   Templet for header files. You can change this templet depend on your
  *          requirments.
  * @par History
  * Date      	Owner       BugID/CRID        Contents
  * 2017/06/01	chenxu
  ****************************************************************************** 
  */

/* Includes ------------------------------------------------------------------*/
#include "FCSW_Type.h"
#include "FCSW_Mgr\RS\Buf\AppBuf.h"
#include "FCSW_Mgr\RS\FCSW_RS_PRTL.h"
#include "FCSW_Mgr\Cfg\FCSW.h"
#if defined WORK_IN_BOARD || defined WORK_IN_PC
/** @addtogroup UTM
  * @{
  */
  
/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
int CMD_FileTrans(AppBuf * appBuf, u8 con);


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
int CMD_FileTrans(AppBuf * appBuf, u8 con)
{
    u8  fn;
    
	fn = *((u8 *)appBuf->pbuf->payLoad+6);


	return 0;
	
}

#endif
