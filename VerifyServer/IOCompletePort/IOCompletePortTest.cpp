// IOCompletePort.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "IOCompletePortOperate.h"
#include "StringIOCompletePortOverlapped.h"
#include "IOCompletePort.h"

int _tmain(int argc, _TCHAR* argv[])
{
	
	CIOCompletePort *io_operate = new CIOCompletePort();

	for (int i=0; i<=20; i++)
	{
		::Sleep(500);
		CStringIOCompletePortOverlapped *string_overlapped = new CStringIOCompletePortOverlapped;
		io_operate->IOCompletePortPost(0, NULL, string_overlapped);
	}
	CStringIOCompletePortOverlapped *string_overlapped1 = new CStringIOCompletePortOverlapped;
	delete io_operate;
	
	delete 	string_overlapped1;
	return 0;
}

