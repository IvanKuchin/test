#ifndef __CHAT__SERVER__JSONPARSER__H__
#define __CHAT__SERVER__JSONPARSER__H__
 
#include <iostream>
#include <map>
#include <string>
#include <algorithm>
#include <sstream>
#include "clog.h"

class JSONParser {
private:
	std::map<std::string, std::string> paramHash;

    std::string::size_type GetNextPairNameValue(const std::string str, std::string::size_type it);
    bool BuildNameValuePairs(const std::string jsonLine);

public:
    bool ParseJSONObject (const std::string jsonLine);
    bool isNameExists(std::string name);
    std::string GetValue(std::string name);
    void  DumpParamHash();
};

#endif