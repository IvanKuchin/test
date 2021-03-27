#ifndef __CVARS__H__
#define __CVARS__H__

#include <map>
#include <algorithm>
#include <sstream>
#include <string>

#include <regex>
#include "clog.h"

using namespace std;

class CNocaseCmp
{
    public:
	bool operator()(const string &x, const string &y) const
	{
	    return(strcasecmp(x.c_str(), y.c_str()) < 0);
	}
};

class CVars : public map<string, string, CNocaseCmp>
{
			string		__index = "";
			auto		GetUUID() -> string;

    public:
					    CVars();

		    bool 		Add(string name, string value);
            bool 		Redefine(string name, string value);
			string		GetNameByRegex(regex r);
		    string 		Get(string name);
		    bool 		Set(string name, string val);
		    bool 		Delete(string name);
		    int			Count();

		    auto		SetIndex(const	string&  param	)	{ __index = param; };
		    auto		SetIndex(		string&& param	)	{ __index = move(param); };

		    auto		GetIndex()	const					{ return __index; };
};

#endif
