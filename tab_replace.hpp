
#ifndef _TAB_REPLACE_H_
#define _TAB_REPLACE_H_

#include <vector>
#include <string>



class TabReplace
{

public:
	TabReplace() : m_vSpilt(2) { m_vSpilt[0] = "\n"; m_vSpilt[1] = "\r\n"; };
	TabReplace(const std::vector<const char *> &vSpilt) : m_vSpilt(vSpilt) {}
	~TabReplace() {}
	int replace(const std::string &szInText, std::string &szOutText) const;

private:
	std::vector<const char *> m_vSpilt;

private:
	int _spilt(const std::string &szInText, std::vector<std::string> &vszOutText) const;
	bool _isStringInSpilt(const std::string &szInText) const;

};




#endif

