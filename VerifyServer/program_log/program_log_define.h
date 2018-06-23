#ifndef __PROGRAMLOGDEFINE_H_
#define __PROGRAMLOGDEFINE_H_

#if defined (_DYNAMIC_DLL) && defined (_WINSTATIC)
#error "Both _DYNAMIC_DLL and _WINSTATIC are defined."
#elif defined (_DYNAMIC_DLL)
#define _PROGRAMLOG_IMPORT_EXPORT_DLL_		__declspec(dllexport)
#elif !defined (_WINSTATIC)
#define _PROGRAMLOG_IMPORT_EXPORT_DLL_		__declspec(dllimport)
#elif defined (_WINSTATIC)
#define _PROGRAMLOG_IMPORT_EXPORT_DLL_
#endif 
//_PROGRAMLOG_IMPORT_EXPORT_DLL_

#endif __PROGRAMLOGDEFINE_H_