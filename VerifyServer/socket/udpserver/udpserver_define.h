#ifndef _UDPSERVER_DEFINR_H__
#define _UDPSERVER_DEFINR_H__

//#if defined (_DYNAMIC_DLL) && defined (_WINSTATIC)
//#error  "Both _DYNAMIC_DLL and _WINSTATIC are defined."
//#elif defined (_DYNAMIC_DLL)
//#define _UDPSERVER_IMPORT_EXPORT_DLL_ __declspec(dllexport)
//#elif !defined (_WINSTATIC)
//#define _UDPSERVER_IMPORT_EXPORT_DLL_ __declspec(dllimport)
//#elif defined (_WINSTATIC)
//#define _UDPSERVER_IMPORT_EXPORT_DLL_
//#endif
////_UDPSERVER_IMPORT_EXPORT_DLL_

#if defined (_DYNAMIC_DLL) && defined (_WINSTATIC)
#error "Both _DYNAMIC_DLL and _WINSTATIC are defined."
#elif defined (_DYNAMIC_DLL)
#define _UDPSERVER_IMPORT_EXPORT_DLL_		__declspec(dllexport)
#elif !defined (_WINSTATIC)
#define _UDPSERVER_IMPORT_EXPORT_DLL_		__declspec(dllimport)
#elif defined (_WINSTATIC)
#define _UDPSERVER_IMPORT_EXPORT_DLL_
#endif 
//_UDPSERVER_IMPORT_EXPORT_DLL_

#endif