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


class FileRW
{
private:
	string m_FileName;

public:
	FileRW(const char *pcFileName) :m_FileName(pcFileName) {}

	/* 返回-1表示失败，成功返回字符串长度 */
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
		return count;
	}

	/* 返回-1表示失败，成功返回wchar_t字符串长度 */
	int readall(wstring &wsOut) const
	{
		string szTxt;
		int iRet;

		iRet = readall(szTxt);
		if (-1 == iRet)
		{
			return iRet;
		}

		int max_size = szTxt.length() + 1;
		wchar_t *w_tmp = new wchar_t[max_size];
		
		setlocale(LC_ALL, "");
		iRet = mbstowcs(w_tmp, szTxt.c_str(), max_size);
		if (iRet != -1)
		{
			wsOut = w_tmp;
		}

		delete [] w_tmp;
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


	int writeall(const wstring &wszData) const
	{
		setlocale(LC_ALL, "");
		
		int max_size = wszData.length() * 7;
		char *tmp = new char[max_size];

		int count = wcstombs(tmp, wszData.c_str(), max_size);
		if (-1 == count)
		{
			return -1;
		}

		string szData = tmp;
		delete[] tmp;

		return writeall(szData);
	}

};


int test_rw()
{
	FileRW f("E:\\X 发行资料\\读取测试.txt");
	//FileRW f("E:\\X 发行资料\\读取测试_UTF8.txt");
	string txt;

	f.readall(txt);
	cout << txt << endl;
	cout << "--------------------------------------" << endl;

	wcout.imbue(locale("chs"));
	wstring wtxt;
	if (-1 != f.readall(wtxt))
	{
		wcout << wtxt << endl;
	}
	else
	{
		wcout << L"解码失败！" << endl;
	}
	
	//--------------------------------写测试-----------------------------------
	wtxt += L"今天天气很好\n";
	f.writeall(wtxt);

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


string ws2s(const wstring &wstr)
{
	string strRet;
	
	setlocale(LC_ALL, "");
	int max_size = wstr.length() * 7;
	char *tmp = new char[max_size];
	if (-1 != wcstombs(tmp, wstr.c_str(), max_size))
	{
		strRet = tmp;
	}
	else
	{
		strRet = "";
	}
	
	delete[] tmp;
	return strRet;
}


wstring user_input()
{
	wcout << L"一个把文件夹里面所有*.c/*.cpp/*.h/*.hpp文件的Tab转为空格的工具" << endl;
	wcout << L"请输入文件夹路径：" << endl;

	string path;
	wstring ws;
	getline(cin, path);
	if (!cin)
	{
		ws = L"";
		wcout << L"输入有误！" << endl;
		return ws;
	}

	int max_count = path.length() + 1;
	wchar_t *w_tmp = new wchar_t[max_count];
	setlocale(LC_ALL, "");
	if (-1 == mbstowcs(w_tmp, path.c_str(), max_count))
	{		
		ws = L"";
		wcout << L"文件路径解码失败！" << endl;
		delete[] w_tmp;
		return ws;
	}
	ws = w_tmp;
	delete[] w_tmp;

	return ws;
}


bool wstr_endwith(const wstring &wstr, const wchar_t *pcEnd)
{
	int len = wstr.length();
	int end_len = wcslen(pcEnd);

	if (len < end_len)
	{
		return false;
	}

	if (0 != wcscmp(wstr.c_str() + len - end_len, pcEnd))
	{
		return false;
	}

	return true;
}


bool match_cfile_extension(const wstring &filename)
{
	wchar_t *pcExt[] = {L".c", L".h", L".cpp", L".hpp"};
	bool bRet = false;

	for (int i = 0; i < sizeof(pcExt) / sizeof(char *); i++)
	{
		if (wstr_endwith(filename, pcExt[i]))
		{
			bRet = true;
			break;
		}
	}

	return bRet;
}


int replace_one_file(const wstring &filename)
{
	TabReplace tabrep;
	FileRW fs(ws2s(filename).c_str());
	
	wstring wsOld, wsNew;
	if (-1 == fs.readall(wsOld))
	{
		return -1;
	}

	wsNew = tabrep.replace(wsOld);
	if (-1 == fs.writeall(wsNew))
	{
		return -1;
	}

	return 0;
}

int run()
{
	wcout.imbue(locale("chs"));

	wstring wpath = user_input();
	if (wpath == L"")
	{
		return 1;
	}

	vector<wstring> vwsDirs;
	vector<wstring> vwsFiles;
	if (0 != FileSys::find(wpath.c_str(), vwsDirs, vwsFiles))
	{
		wcout << L"输入的路径不存在！" << endl;
		return 1;
	}

	for (auto it = cbegin(vwsFiles); it != cend(vwsFiles); ++it)
	{
		wstring filename = (*it);
		if (match_cfile_extension(filename))
		{
			wcout << L"Replace: " << filename << endl;
			if (-1 == replace_one_file(filename))
			{
				wcout << L"FAILED REPLACE!!! : " << filename << endl;
			}
		}
	}

	return 0;
}


int main()
{
	int iRet;
	iRet = run();
	cin.get();
	return iRet;
}


