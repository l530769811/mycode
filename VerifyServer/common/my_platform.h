#ifndef __COMMON_MYPLATFORM_H__
#define __COMMON_MYPLATFORM_H__

#ifdef _WIN32 //wi32 platform

	#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN //for #include <windows.h> 
	#endif

	#include <windows.h>

	#define atomic_t long
	#define atomic_inc(atomic_pointer) ::InterlockedIncrement(atomic_pointer)
	#define atomic_dec(atomic_pointer) ::InterlockedDecrement(atomic_pointer)
	#define atomic_set(atomic_pointer, set_val) ::InterlockedExchange(atomic_pointer, set_val)
	#define ATOMIC_INIT(value)

	#define container_of ::CONTAINING_RECORD

#else //linux platform

	#include <linux/types.h>
	#include <asm/atomic.h>



#endif




#endif //__COMMON_MYPLATFORM_H__