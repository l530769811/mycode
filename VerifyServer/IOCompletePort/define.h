#ifndef __IO_DEFINE_H__
#define __IO_DEFINE_H__

#if defined (_DYNAMIC_DLL) && defined(_WINSTATIC)
#error "Both _DYNAMIC_DLL and _WINSTATIC are defined."
#elif defined(_DYNAMIC_DLL)
#define _IO_IMPORT_EXPORT_DLL_ __declspec(dllexport)
#elif !defined(_WINSTATIC)
#define _IO_IMPORT_EXPORT_DLL_ __declspec(dllimport)
#elif defined(_WINSTATIC)
#define _IO_IMPORT_EXPORT_DLL_
#endif
// _DYNAMIC_DLL && _WINSTATIC

#endif //__IO_DEFINE_H__