#ifndef __MPLAYER_MEMCPY
#define __MPLAYER_MEMCPY 1


#include <stddef.h>


#ifdef __cplusplus
extern "C"
{
#endif
extern void * fast_memcpy(void * to, const void * from, size_t len);
#ifdef __cplusplus
}
#endif


#define memcpy(a,b,c) fast_memcpy(a,b,c)

#endif
