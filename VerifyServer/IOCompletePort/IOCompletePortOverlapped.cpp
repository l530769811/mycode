#include "StdAfx.h"
#include "IOCompletePortOverlapped.h"


CIOCompletePortOverlapped::CIOCompletePortOverlapped(void)
{
	::memset(&ol, 0, sizeof(OVERLAPPED));
}


CIOCompletePortOverlapped::~CIOCompletePortOverlapped(void)
{
}
