#include "c_config.h"

tuple<string, string> c_config::ExtractSingleValue(string line, const vector<string> &params)
{
	MESSAGE_DEBUG("", "", "start");

	tuple<string, string>	result;

	for(auto &param: params)
	{
		regex	e("^\\s*" + param + "\\s*=\\s*\"(.*)\"");
		smatch	m;

		if(regex_search(line, m, e))
		{
			result = make_tuple(param, m[1]);
			MESSAGE_DEBUG("", "", "match found: " + get<0>(result));
			break;
		}
	}

	MESSAGE_DEBUG("", "", "finish");

	return result;
}

map<string, string>	c_config::Read(const vector<string> &params)
{
	MESSAGE_DEBUG("", "", "start");

	map<string, string>		result;
	ifstream				f(config_file);

	if (f.is_open())
	{
		auto	line = ""s;

		while ( getline (f,line) )
		{
			auto	value = ExtractSingleValue(line, params);

			if(get<0>(value).length())
				result[get<0>(value)] = get<1>(value);
		}
		f.close();
	}
	else
	{
		MESSAGE_ERROR("", "", "can't open " + config_file)
	}

	MESSAGE_DEBUG("", "", "finish (result size: " + to_string(result.size()) + ")");

	return result;
}

ostream& operator<<(ostream& os, const c_config &var)
{
	os << string("test line");
	return os;
}

