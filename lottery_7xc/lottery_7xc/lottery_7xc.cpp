// lottery_7xc.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "lotteryfiler.h"
#include "RateOparetor.h"
#include "IntervalRateOparetor.h"
#include <iostream>
#include <sstream>
#include <windows.h>

#include <stdio.h>
#include <conio.h>
#include <iomanip>


std::wstring notify_string =
	_T("\t\tthis program is to find out the \n\
	   \t\trate of the 7XingCai whitch number on sit \n \
	   \t          you can hit under key to input order \n \
	   \t one: <number,number,number> to count the 7XingCai \n \
	   \t\twhitch number on sit by the number count, \n \t\t\t exmaple: 100,30,20 \n\
	   \t two: [numberX,numberY],[numberX,numberY],[numberX,numberY] to count the.... \
	   \t three: (numberX,numberY),(numberX,numberY),(numberX,numberY) to count the....\
	   \t four: hit ctrl + X to copy the result string to the clipboard \n \
	   \t five: hit the 'c' to clear\n\
	   \t six: hit the q key to exit\n \
	   \t            now hit you option:  ");

int _tmain(int argc, _TCHAR* argv[])
{

	std::wstring copy_to_clipboard_string;
	CLotteryFiler file_loader;
	file_loader.read_lottery();

	while (1)
	{
		std::wcout << notify_string;
		char input[100] = {0};
		char *p = gets(input);
		
		//getch();
		switch(p[0])
		{
		case 'C':
		case 'c':
			::system("cls");
			break;
		case 'q':
		case 'Q':
			std::wcout << _T("are you sure to exit?(y/n)");
			
			if(getch()=='y' || getch()=='Y')
			{
				::CloseClipboard();
				exit(0);
			}
			break;
		case 24:
			if(!copy_to_clipboard_string.empty()){

				::OpenClipboard(NULL);
				HANDLE hData = GetClipboardData(CF_TEXT);
				if (hData == nullptr)
					int j=0; // error

					// Lock the handle to get the actual text pointer
				char * pszText = static_cast<char*>( GlobalLock(hData) );
				::EmptyClipboard();

				HGLOBAL pMem = ::GlobalAlloc(GMEM_MOVEABLE, wcslen(copy_to_clipboard_string.c_str())*2 + 1);
				 void *lptstrCopy = GlobalLock(pMem);
				memcpy(lptstrCopy, copy_to_clipboard_string.c_str(), 
					wcslen(copy_to_clipboard_string.c_str()) * 2); 
				GlobalUnlock(pMem);

				SetClipboardData(CF_UNICODETEXT, pMem);
				::CloseClipboard();
				GlobalFree(pMem);

			}
			else
			{
				int i = 0;
			}
			break;
		default:
			copy_to_clipboard_string.clear();

			std::string input_string = input;
			std::string::size_type pos = std::string::npos;

			if (input_string.find(("[")) != std::string::npos)
			{
				while(1)
				{
					std::string::size_type pos1 = std::string::npos;
					std::string::size_type pos2 = std::string::npos;
					std::string tmp_input;

					pos1 = input_string.find("[");
					pos2 = input_string.find(']');
					
					if (pos1 ==std::string::npos || pos2 == std::string::npos)
					{
						break;
					}
					tmp_input = input_string.substr(pos1+1, pos2-pos1-1);

					std::ostringstream s;
					std::string season;
					std::string strinterval;
					
					std::string::size_type pos_interval = tmp_input.find(",");
					if (pos_interval != std::string::npos)
					{
						season = tmp_input.substr(0, pos_interval);
						strinterval = tmp_input.substr(pos_interval+1, strlen(tmp_input.c_str())-pos_interval);
						CRateOparetor rateOp(::atoi(season.c_str()), ::atoi(strinterval.c_str()), 3, file_loader.get_lottery_head());
						rateOp.DoOparete();
						rateOp.ToPrint();

						copy_to_clipboard_string = copy_to_clipboard_string + rateOp.toString();
					}
					


					std::string::size_type pos3 = input_string.find(",", pos2);
					if (pos3 == std::string::npos )
					{
						break;
					}
					input_string = input_string.substr( pos3+1, strlen(input_string.c_str())-pos3-1 );

									
				}
			}
			else if(input_string.find(("(")) != std::string::npos){
				while(1)
				{
					std::string::size_type pos1 = std::string::npos;
					std::string::size_type pos2 = std::string::npos;
					std::string tmp_input;

					pos1 = input_string.find("(");
					pos2 = input_string.find(')');

					if (pos1 ==std::string::npos || pos2 == std::string::npos)
					{
						break;
					}
					tmp_input = input_string.substr(pos1+1, pos2-pos1-1);

					std::ostringstream s;
					std::string season;
					std::string stringTimes;

					std::string::size_type pos_interval = tmp_input.find(",");
					if (pos_interval != std::string::npos)
					{
						season = tmp_input.substr(0, pos_interval);
						stringTimes = tmp_input.substr(pos_interval+1, strlen(tmp_input.c_str())-pos_interval);
						CIntervalRateOparetor rateOp(::atoi(season.c_str()), ::atoi(season.c_str()), ::atoi(stringTimes.c_str()), file_loader.get_lottery_head());
						rateOp.DoOparete();
						rateOp.ToPrint();

						copy_to_clipboard_string = copy_to_clipboard_string + rateOp.toString();
					}



					std::string::size_type pos3 = input_string.find(",", pos2);
					if (pos3 == std::string::npos )
					{
						break;
					}
					input_string = input_string.substr( pos3+1, strlen(input_string.c_str())-pos3-1 );
				}
			}
			else
			{
				while(1){			
					pos=input_string.find(',');
					std::ostringstream s;
					std::string season;
					if(pos==std::string::npos)
					{
						season = input_string;
					}
					else
					{
						season = input_string.substr(0, pos);
						input_string = input_string.substr(pos+1, strlen(input_string.c_str())-pos+1);
					}

					CRateOparetor rateOp(::atoi(season.c_str()), ::atoi(season.c_str()), 3, file_loader.get_lottery_head());
					rateOp.DoOparete();
					rateOp.ToPrint(); 

					copy_to_clipboard_string = copy_to_clipboard_string + rateOp.toString();

					if(pos==std::string::npos)
						break;
					pos = 	input_string.find(",");
				}
			}

			
			break;
		}
	}
#ifdef _DEBUG
	std::wostringstream sstring;
	sstring << 0.0111;
	std::wcout << sstring.str() << std::endl;
#endif


#ifdef _DEBUG
	wprintf(_T("%s"), _T("sit_string[i我"));
#endif



	getchar();
	
	return 0;
}

