#ifndef _COMMON_DEFINE_H__
#define _COMMON_DEFINE_H__


#if defined (_DYNAMIC_DLL) && defined(_WINSTATIC)
#error "Both _DYNAMIC_DLL and _WINSTATIC are defined."
#elif defined(_DYNAMIC_DLL)
#define _COMMON_IMPORT_EXPORT_DLL_ __declspec(dllexport)
#elif !defined(_WINSTATIC)
#define _COMMON_IMPORT_EXPORT_DLL_ __declspec(dllimport)
#elif defined(_WINSTATIC)
#define _COMMON_IMPORT_EXPORT_DLL_
#endif
// _DYNAMIC_DLL && _WINSTATIC

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#endif //_COMMON_DEFINE_H__