#include <iostream>
#include <vector>
#include <string>
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

//private:
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


class FilesProc
{

public:
	static errno_t ls(const wchar_t *pathName, vector<wstring> &files)
	{
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind = INVALID_HANDLE_VALUE;
		TCHAR PathBuffer[_MAX_PATH];

		wcscpy_s(PathBuffer, _MAX_PATH, pathName);
		wcscat_s(PathBuffer, _MAX_PATH, L"\\*");

		hFind = FindFirstFile(PathBuffer, &FindFileData);
		if (INVALID_HANDLE_VALUE == hFind)      // 如果出现了某种异常就直接抛出便可
		{
			return 1;
		}
		
		// 然后再接着查找下去
		//files.push_back(wstring(FindFileData.cFileName)); '.'
		while (0 != FindNextFile(hFind, &FindFileData))
		{
			wstring ws_tmp = wstring(FindFileData.cFileName);
			if (ws_tmp != L"." && ws_tmp != L"..")
			{
				files.push_back(ws_tmp);
			}					
		}

		FindClose(hFind);
		return 0;
	}


};



int test_file_list()
{
	vector<wstring> vwsFiles;

	FilesProc::ls(L"E:\\X 发行资料", vwsFiles);

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
	test_file_list();
	return 0;
}


