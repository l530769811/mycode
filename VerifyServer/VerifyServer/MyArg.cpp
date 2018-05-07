#include "MyArg.h"


CMyArg::CMyArg(void)
{
}


CMyArg::~CMyArg(void)
{
}

int CMyArg::GetIntArg(){
	return 0;
}

int* CMyArg::GetIntArgs(int *args){
	return args;
}

void* CMyArg::GetObjectArgs(void * args){
	return args;
}