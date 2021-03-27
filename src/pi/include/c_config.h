#ifndef __C_CONFIG__H__
#define __C_CONFIG__H__

#include <iostream>
#include <fstream>
#include <regex>
#include <tuple>
#include <vector>
#include <map>

#include "clog.h"

using namespace std;

class c_config
{
	protected:
		string					config_file		=	CONFIG_FNAME;
		tuple<string, string>	ExtractSingleValue(string line, const vector<string> &params);

	public:
		void					SetFile(string param) { config_file = param; };
		map<string, string>		Read(const vector<string> &params);
};

ostream&	operator<<(ostream& os, const c_config &);

#endif
