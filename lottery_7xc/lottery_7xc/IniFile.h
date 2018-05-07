#ifndef _CINITFILE_H__
#define _CINITFILE_H__

#include <string>

typedef unsigned int UINT;

class CIniFile
{
public:
	CIniFile(std::wstring csFile);
	virtual ~CIniFile();
public:
	bool IsExistFile();		//判读文件是否存在 
	bool HasWritePermission();	//判断文件是否可写

	// 读取 std::wstring , bool, int, double 四种类型的值
	std::wstring  ReadString(const std::wstring & csSection, const std::wstring & csItem,
		const std::wstring & csDefault);
	bool ReadBool(const std::wstring & csSection, const std::wstring & csItem,
		bool bDefault);
	UINT ReadInt(const std::wstring & csSection, const std::wstring & csItem,
		UINT iDefault);
	double ReadDouble(const std::wstring & csSection, const std::wstring & csItem,
		double dDefault);

	// 写入 std::wstring , bool, int, double 四种类型的值
	void WriteString(const std::wstring & csSection, const std::wstring & csItem,
		const std::wstring & csValue);
	void WriteBool(const std::wstring & csSection, const std::wstring & csItem,
		bool bValue);
	void WriteInt(const std::wstring & csSection, const std::wstring & csItem, UINT iValue);
	void WriteFloat(const std::wstring & csSection, const std::wstring & csItem,
		double fValue);

	

private:
	std::wstring  m_csFile;
};

#endif // _CINITFILE_H__
