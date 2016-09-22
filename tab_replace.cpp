#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>
#include <locale>
#include <codecvt>
#include <windows.h>
//#include "tab_replace.hpp"

using namespace std;


class TabReplace
{
public:
	static const int DEFAULT_TAB2SPACE = 4;

private:
	vector<const wchar_t *> m_vSpilt;
	int m_tab2space;

public:
	TabReplace() { this->TabReplace::TabReplace(DEFAULT_TAB2SPACE); }
	explicit TabReplace(int iTab2Space) : m_vSpilt(2), m_tab2space(iTab2Space) 
	{
		m_vSpilt[0] = L"\n"; 
		m_vSpilt[1] = L"\r\n"; 
	}
	TabReplace(const std::vector<const wchar_t *> &vSpilt, int iTab2Space) : m_vSpilt(vSpilt), m_tab2space(iTab2Space) {}
	~TabReplace() {}
	
	wstring replace(const wstring &szInText) const
	{
		vector<wstring> vwsSpilted;
		wstring swOutText;

		(void)_spilt(szInText, vwsSpilted);
		for (auto it = cbegin(vwsSpilted); it != cend(vwsSpilted); ++it)
		{
			swOutText += _line_replace(*it);
		}

		return swOutText;
	}

private:
	int _spilt(const wstring &szInText, vector<wstring> &vszOutText) const
	{
		const wchar_t *p_pos, *pcSegment;
		int iSegmentCount = 0;

		p_pos = szInText.c_str();
		pcSegment = p_pos;
		while (*p_pos != '\0')
		{
			int iSpiltLen = _get_spilt_str_len(p_pos);
			if (0 == iSpiltLen)
			{
				p_pos++;
				continue;
			}

			int iSegmentLen = p_pos - pcSegment + iSpiltLen;
			wstring str_tmp(pcSegment, iSegmentLen);
			vszOutText.push_back(str_tmp);
			iSegmentCount++;


			p_pos += iSpiltLen;
			pcSegment = p_pos;
		}

		if (pcSegment != p_pos)
		{
			vszOutText.push_back(wstring(pcSegment));
			iSegmentCount++;
		}

		return iSegmentCount;
	}
	
	
	int _get_spilt_str_len(const wchar_t *pcStr) const
	{
		// 返回 0 表示该位置无分隔符
		int iSpiltLen = 0;

		for (auto it = begin(m_vSpilt); it != end(m_vSpilt); ++it)
		{
			int l = wcslen(*it);
			if (0 == wcsncmp(pcStr, *it, l))
			{
				iSpiltLen = l;
				break;
			}
		}
		
		return iSpiltLen;
	}


	wstring _line_replace(const wstring &szLine) const
	{
		wstring wsOut;

		int iLen = szLine.length();
		for (int i = 0; i < iLen; i++)
		{
			if (szLine[i] != '\t')
			{
				wsOut += szLine[i];
			}
			else
			{
				int n_space = m_tab2space - wsOut.length() % m_tab2space;
				wsOut += wstring(n_space, ' ');
			}
		}

		return wsOut;
	}

};


class FileSys
{
private:
	//获取某路径下的目录名和文件名(结果添加到vwsDirs、vwsFiles后面)
	static errno_t _ls(const wchar_t *pathName, vector<wstring> &vwsDirs, vector<wstring> &vwsFiles)
	{
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind = INVALID_HANDLE_VALUE;
		TCHAR PathBuffer[_MAX_PATH];

		wcscpy_s(PathBuffer, _MAX_PATH, pathName);
		wcscat_s(PathBuffer, _MAX_PATH, L"\\*");

		hFind = FindFirstFile(PathBuffer, &FindFileData);
		if (INVALID_HANDLE_VALUE == hFind)      // 比如路径不存在或无法访问
		{
			return 1;
		}

		// 然后再接着查找下去
		//files.push_back(wstring(FindFileData.cFileName)); '.'
		while (0 != FindNextFile(hFind, &FindFileData))
		{
			wstring ws_tmp = wstring(FindFileData.cFileName);
			if (ws_tmp == L"." || ws_tmp == L"..")
			{
				continue;
			}

			wstring full_path = wstring(pathName) + L"\\" + ws_tmp;
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				vwsDirs.push_back(full_path);
			}
			else
			{
				vwsFiles.push_back(full_path);
			}
		}

		FindClose(hFind);
		return 0;
	}


	//获取某路径下的所有目录和文件夹(结果添加到vwsDirs、vwsFiles后面(这两个入参可有原有的数据))
	static errno_t _find(const wchar_t *pathName, vector<wstring> &vwsDirs, vector<wstring> &vwsFiles)
	{
		vector<wstring> vwsCurrDirs;
		int iRet = _ls(pathName, vwsCurrDirs, vwsFiles);
		vwsDirs.insert(vwsDirs.end(), vwsCurrDirs.begin(), vwsCurrDirs.end());
		if (0 != iRet)
		{
			return iRet;
		}

		for (auto it = cbegin(vwsCurrDirs); it != cend(vwsCurrDirs); ++it)
		{
			(void)_find((*it).c_str(), vwsDirs, vwsFiles);
		}

		return iRet;
	}


public:
	//获取某路径下的目录名和文件名
	static errno_t ls(const wchar_t *pathName, vector<wstring> &vwsDirs, vector<wstring> &vwsFiles)
	{
		vwsDirs.clear();
		vwsFiles.clear();		
		return _ls(pathName, vwsDirs, vwsFiles);
	}

	//获取某路径下的所有目录和文件夹(结果添加到vwsDirs、vwsFiles后面)
	static errno_t find(const wchar_t *pathName, vector<wstring> &vwsDirs, vector<wstring> &vwsFiles)
	{
		vwsDirs.clear();
		vwsFiles.clear();
		return _find(pathName, vwsDirs, vwsFiles);
	}

};


class Chinese
{
public:
	enum ENCODING
	{
		INVALID,
		UTF_8,
		GB18030,				
		ENCODE_MAX
	};

	static ENCODING decode(const char *pcSrc, wchar_t *pwWcs, int maxCount, ENCODING enEncode = INVALID);

private:
	static const char *apcStrEncoding[];

};


const char *Chinese::apcStrEncoding[ENCODE_MAX] =
{
	"",
	"zh_CN.utf8",	
	"zh_CN.gb18030",	
};


Chinese::ENCODING Chinese::decode(const char *pcSrc, wchar_t *pwWcs, int maxCount, Chinese::ENCODING enEncode)
{		
	if (INVALID < enEncode && enEncode < ENCODE_MAX)
	{
		setlocale(LC_ALL, apcStrEncoding[enEncode]);
		if (-1 != mbstowcs(pwWcs, pcSrc, maxCount))
		{
			return enEncode;
		}
		pwWcs[0] = 0;
		return INVALID;
	}

	int enI;
	for (enI = INVALID + 1; enI < ENCODE_MAX; enI++)
	{
		setlocale(LC_ALL, apcStrEncoding[enI]);
		if (-1 != mbstowcs(pwWcs, pcSrc, maxCount))
		{
			break;
		}
	}

	if (ENCODE_MAX == enI)
	{
		pwWcs[0] = 0;
	}
	
	return (ENCODING)enI;
}






class FileRW
{
private:
	string m_FileName;

public:
	FileRW(const char *pcFileName) :m_FileName(pcFileName) {}

	int readall(string &szOut) const
	{
		ifstream ifs;
		ifs.open(m_FileName.c_str(), ios_base::in);
		if (!ifs)
		{
			return -1;
		}

		ifs.seekg(0, ios_base::end);
		int txt_len = int(ifs.tellg());
		ifs.seekg(0, ios_base::beg);

		char *buf = new char[txt_len + 1];
		ifs.read(buf, txt_len);
		int count = int(ifs.gcount());
		buf[count] = '\0';
		ifs.close();

		szOut = buf;
		delete[] buf;
		return 0;
	}


	int readall(wstring &wsOut) const
	{
		string szTxt;
		int iRet;

		iRet = readall(szTxt);
		if (0 != iRet)
		{
			return iRet;
		}


		return iRet;
	}


	int writeall(const string &szData) const
	{
		ofstream ofs;
		ofs.open(m_FileName, ios_base::out | ios_base::trunc);
		if (!ofs)
		{
			return -1;
		}

		ofs << szData;
		int iRet = 0;
		if (!ofs)
		{
			iRet = -1;
		}

		ofs.close();
		return iRet;
	}
};


int s2ws(const std::string& src, wstring &dst)
{
	std::locale sys_locale("");

	const char* data_from = src.c_str();
	const char* data_from_end = src.c_str() + src.size();
	const char* data_from_next = 0;

	wchar_t* data_to = new wchar_t[src.size() + 1];
	wchar_t* data_to_end = data_to + src.size() + 1;
	wchar_t* data_to_next = 0;

	wmemset(data_to, 0, src.size() + 1);

	typedef std::codecvt<wchar_t, char, mbstate_t> convert_facet;
	mbstate_t in_state = 0;
	auto result = std::use_facet<convert_facet>(sys_locale).in(
		in_state, data_from, data_from_end, data_from_next,
		data_to, data_to_end, data_to_next);
	if (result != convert_facet::ok)
	{
		dst = L"";
		delete[] data_to;
		return 1;
	}

	dst = data_to;
	delete[] data_to;
	return 0;
}


wstring s2ws(const std::string& src)
{
	wstring wsRet;
	s2ws(src, wsRet);
	return wsRet;
}


int test_decode()
{
	//FileRW f("E:\\X 发行资料\\读取测试.txt");
	FileRW f("E:\\X 发行资料\\读取测试_UTF8.txt");
	string txt;

	f.readall(txt);
	cout << txt << endl;
	cout << "--------------------------------------" << endl;

	wcout.imbue(locale("chs"));
	//wstring wsTmp;
	wchar_t wsTmp[1024];
	int encoding = Chinese::decode(txt.c_str(), wsTmp, sizeof(wsTmp) / sizeof(wchar_t));
	if (Chinese::INVALID != encoding)
	{
		wcout << wsTmp << endl;
	}
	else
	{
		cout << "Decode error!" << endl;
		return 1;
	}	

	return 0;

}


int test_read_wfile()
{
	FileRW f("E:\\X 发行资料\\读取测试.txt");
	string txt;

	f.readall(txt);
	cout << txt << endl;
	cout << "--------------------------------------" << endl;
	f.writeall(txt + "####\n");

	wcout.imbue(locale("chs"));
	wstring wsTmp;
	s2ws(txt, wsTmp);
	wcout << wsTmp << endl;

	return 0;
}


int test_file_list()
{
	vector<wstring> vwsFiles, vwsDirs;
	wchar_t *path;
	int ret;

	path = L"E:\\X 发行资料";
	ret = FileSys::ls(path, vwsDirs, vwsFiles);

	path = path;
	ret = FileSys::find(path, vwsDirs, vwsFiles);
	
	return 0;
}


int test_text_replace()
{
	TabReplace tab(8);
	wstring out;
	wstring txt = L"a\t\n我\t中华\t人民共和国\t\r\n\t789\tad flow\tkoko\n  ada\r\n";
	
	wcout.imbue(locale("chs"));
	wcout << txt << endl;
	out = tab.replace(txt);
	wcout << out << endl;

	return 0;
}


int main()
{
	test_decode();
	return 0;
}


