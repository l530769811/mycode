#ifndef __ESOP_SOCKETPROTOCOLDEF_H__
#define __ESOP_SOCKETPROTOCOLDEF_H__

#ifndef cJSON__h
#include "cJSON_Unicode.h"
#endif //cJSON__h
#include "stdafx.h"

cJSON* CreateCommonJsonHead();
//cJSON* CreateCommonEncryptRequestJson(const MyString& strCombinaSrc);
void DeleteCommonJsonHead(cJSON* proot);

cJSON* CreateJsonContent(cJSON *pjson);
cJSON* WriteJsonType(cJSON *pjson, MyString strType);
cJSON* CrateJsonValue(cJSON *pjson);

#define JSON_COMMON_REQUEST_TRANSITIONID	_T("transitionid")
#define JSON_COMMON_REQUEST_MAGICID			_T("magicid")
#define JSON_COMMON_CONTENT_KEY				_T("content")
#define JSON_COMMON_CTYPE_KEY						_T("ctype")
#define JSON_COMMON_CVALUE_KEY						_T("cvalue")
#define JSON_SERVERNAME_KEY					_T("server_name")
#define JSON_IP_KEY							_T("ip")
#define JSON_PORT_KEY						_T("port")
#define JSON_NET_KIND_KEY					_T("net_kind")
#define JSON_USER_NAME_KEY					_T("user_name")
#define JSON_USER_PASSWORD_KEY				_T("user_password")
#define JSON_RESULT_KEY						_T("result")
#define JSON_STRRESULT_KEY					_T("strresult")
#define JSON_IDENTIFY_CODE_KEY				_T("identify_code")
#define JSON_PHONE_NUMBER_KEY				_T("phone_number")
#define JSON_RESULT_INFO_KEY				_T("result_info")
#define JSON_CARD_NUMBER					_T("card_number")
#define JSON_CARD_PASSWORD_KEY				_T("card_password")
#define JSON_CARD_USER_NAME_KEY				_T("card_user_name")
#define JSON_SEX_KEY						_T("sex")

#endif //__ESOP_SOCKETPROTOCOLDEF_H__