#include "StdAfx.h"

#include <Windows.h>
#include "LotteryFiler.h"
#include "IniFile.h"
#include "QxcLottery.h"
#include "GlobalFunc.h"

#ifdef _DEBUG
#include <iostream>
#endif

std::wstring CLotteryFiler::strFileName = GetModuleFilePath() + _T("\\lottery.ini");

CLotteryFiler::CLotteryFiler(void)
	: m_phead(NULL)
{
	m_phead = new CQxcLottery();
	lottery_list_init(m_phead);
}


CLotteryFiler::~CLotteryFiler(void)
{
	free_lottery(m_phead);
	delete m_phead;
}

void CLotteryFiler::read_lottery()
{
	CIniFile inifile(strFileName.c_str());
	std::wstring strDefault ;//= //_T("0");
	std::wstring strResult;
	TCHAR cI[13] = {0};
	TCHAR key[2561] = {0};	
	//GetPrivateProfileString(_T("LOTTERY"), NULL, strDefault.c_str(),key, 3, strFileName.c_str());

	//GetPrivateProfileSection(_T("LOTTERY"), key, 2560, strFileName.c_str());
#ifdef _DEBUG
	int count = 0;
#endif
	for (int i = 1; (strResult=inifile.ReadString(_T("LOTTERY"), ::_itot(i,  cI,10), strDefault)) != strDefault; i++)
	{ 
#ifdef _DEBUG
		count++;
#endif
		std::wstring::size_type pos = 0;
		std::wstring::size_type pos_tmp = 0;
		
		pos = strResult.find(_T(","));
		if(pos == std::string::npos)//can not find
			continue;

		std::wstring strSesion = strResult.substr(0, pos-pos_tmp);

		pos_tmp = strResult.find(_T(","), pos+1);
		std::wstring strI1 = strResult.substr(pos+1, pos_tmp-pos-1);
		
		pos = pos_tmp;
		pos_tmp = strResult.find(_T(","), pos+1);
		std::wstring strI2 = strResult.substr(pos+1, pos_tmp-pos-1);

		pos = pos_tmp;
		pos_tmp = strResult.find(_T(","), pos+1);
		std::wstring strI3 = strResult.substr(pos+1, pos_tmp-pos-1);

		pos = pos_tmp;
		pos_tmp = strResult.find(_T(","), pos+1);
		std::wstring strI4 = strResult.substr(pos+1, pos_tmp-pos-1);

		pos = pos_tmp;
		pos_tmp = strResult.find(_T(","), pos+1);
		std::wstring strI5 = strResult.substr(pos+1, pos_tmp-pos-1);

		pos = pos_tmp;
		pos_tmp = strResult.find(_T(","), pos+1);
		std::wstring strI6 = strResult.substr(pos+1, pos_tmp-pos-1);

		pos = pos_tmp;
		pos_tmp = strResult.find(_T(","), pos+1);
		std::wstring strI7 = strResult.substr(pos+1, pos_tmp-pos-1);

		create_lottery(m_phead,
			strSesion, 
			::_ttoi(strI1.c_str()),
			::_ttoi(strI2.c_str()),
			::_ttoi(strI3.c_str()),
			::_ttoi(strI4.c_str()),
			::_ttoi(strI5.c_str()),
			::_ttoi(strI6.c_str()),
			::_ttoi(strI7.c_str()),
			false);
	}

#ifdef _DEBUG 
	std::cout << "ddd" << std::string::npos << std::endl;
	std::cout << count << std::endl;
#endif
	
}

void CLotteryFiler::write_lottery()
{

}

const CQxcLottery* CLotteryFiler::get_lottery_head() const
{
	return m_phead;
}

