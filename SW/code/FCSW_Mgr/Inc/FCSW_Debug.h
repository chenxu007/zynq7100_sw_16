#ifndef __FCSW_DEBUG_H__
#define __FCSW_DEBUG_H__

#define FCSW_ASSERT(x)\
    if(!(x))\
    {\
        FCSW_printf("%s %s %d ASSERT !!!\n", __FILE__, __FUNCTION__, __LINE__);\
        while(1);\
    }

#endif
