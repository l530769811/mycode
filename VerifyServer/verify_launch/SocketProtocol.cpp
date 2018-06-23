#include "StdAfx.h"
#include "cJSON_Unicode.h"
#include "SocketProtocol.h"

cJSON* CreateCommonJsonHead()
{
	cJSON *root = cJSON_CreateObject();
	cJSON_AddNullToObject(root, JSON_COMMON_REQUEST_TRANSITIONID);
	cJSON_AddNullToObject(root, JSON_COMMON_REQUEST_MAGICID);
	return root;
}

//cJSON* CreateCommonEncryptRequestJson(const MyString& strCombinaSrc){
//	cJSON *root = cJSON_CreateObject();
//
//	return root;
//}

void DeleteCommonJsonHead(cJSON* proot){
	cJSON_Delete(proot);
}

cJSON* CreateJsonContent(cJSON *pjson){
	cJSON *ret = NULL;
	if(pjson!=NULL)	{

		cJSON_AddItemToObject(pjson, JSON_COMMON_CONTENT_KEY, ret=cJSON_CreateObject());
	}
	return ret;
}
cJSON* WriteJsonType(cJSON *pjson, MyString strType) {
	cJSON *ret = NULL;
	if (pjson!=NULL) {
		cJSON_AddStringToObject(pjson, JSON_COMMON_CTYPE_KEY, strType.c_str());
		ret = pjson;
	}
	return ret;
}
cJSON* CrateJsonValue(cJSON *pjson){
	cJSON *ret = NULL;
	if(pjson!=NULL)	{

		cJSON_AddItemToObject(pjson, JSON_COMMON_CVALUE_KEY, ret=cJSON_CreateObject());
	}
	return ret;
}