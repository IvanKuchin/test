#include "chat-server-jsonparser.h"

std::string::size_type JSONParser::GetNextPairNameValue(const std::string str, std::string::size_type it)
{
    std::string             JSONObjectNameSeparator = "\"";
    std::string             JSONObjectValueSeparator = "\"";
    std::string             JSONObjectParamValueSeparator = ":";
    std::string             name, value;
    std::string::size_type  JSONNameStartIterator, JSONNameEndIterator;
    std::string::size_type  JSONValueStartIterator, JSONValueEndIterator;
    std::string::size_type  JSONParamValueIterator;
    std::string::size_type  result = true;
    
    JSONNameStartIterator = str.find(JSONObjectNameSeparator, it + 1);
    JSONNameEndIterator = str.find(JSONObjectNameSeparator, JSONNameStartIterator + 1);
    JSONParamValueIterator = str.find(JSONObjectParamValueSeparator, JSONNameEndIterator + 1);
    JSONValueStartIterator = str.find(JSONObjectValueSeparator, JSONParamValueIterator + 1);
    JSONValueEndIterator = str.find(JSONObjectValueSeparator, JSONValueStartIterator + 1);

    result = JSONValueEndIterator;

    if(JSONValueEndIterator != std::string::npos)
    {
        name = "";
        value = "";

        name = str.substr(JSONNameStartIterator + 1, JSONNameEndIterator - JSONNameStartIterator - 1);
        value = str.substr(JSONValueStartIterator + 1, JSONValueEndIterator - JSONValueStartIterator - 1);
        paramHash.insert(std::pair<std::string, std::string>(name, value));
    }

    return result;
}

bool JSONParser::BuildNameValuePairs(const std::string jsonLine)
{
    std::string             JSONObjectStartSeparator = "{";
    std::string             JSONObjectEndSeparator = "}";
    std::string             JSONObjectParamSeparator = ",";
    std::string::size_type  JSONObjectStartIterator, JSONObjectEndIterator;
    std::string::size_type  JSONNextLV;
    bool                    result = true;
    
    paramHash.clear();
    JSONObjectStartIterator = jsonLine.find(JSONObjectStartSeparator);
    JSONObjectEndIterator = std::string::npos;
    JSONNextLV = JSONObjectStartIterator;

    while(JSONNextLV != std::string::npos)
    {
        JSONNextLV = GetNextPairNameValue(jsonLine, JSONNextLV);
        if(JSONNextLV != std::string::npos)
        {
            JSONNextLV = jsonLine.find_first_not_of("\" \t", JSONNextLV);
            if(JSONNextLV != std::string::npos)
            {
                if(jsonLine.at(JSONNextLV) == '}')
                {
                    JSONObjectEndIterator = JSONNextLV;
                    JSONNextLV = std::string::npos;
                }
                else if(jsonLine.at(JSONNextLV) == ',')
                {
                }
                else
                {
                    result = false;
                    {
                        CLog                log(CHAT_LOG_FILE_NAME);
                        std::ostringstream  ost;
                        ost.str("");
                        ost << "ERROR: parameter separator wrong at position " << JSONNextLV << " [" << jsonLine << "]";
                        log.Write(ERROR, ost.str());
                    }
                    break;
                }
            }
            else
            {
                result = false;
                {
                    CLog                log(CHAT_LOG_FILE_NAME);
                    std::ostringstream  ost;
                    ost.str("");
                    ost << "ERROR: parameter separator not found [" << jsonLine << "]";
                    log.Write(ERROR, ost.str());
                }
                break;
            }
        }
        else
        {
            result = false;
            {
                CLog                log(CHAT_LOG_FILE_NAME);
                std::ostringstream  ost;
                ost.str("");
                ost << "ERROR: error parsing name:value pair [" << jsonLine << "]";
                log.Write(ERROR, ost.str());
            }
            break;
        }
    }
    if(JSONObjectEndIterator == std::string::npos)
    {
        result = false;
        {
            CLog                log(CHAT_LOG_FILE_NAME);
            std::ostringstream  ost;
            ost.str("");
            ost << "ERROR: problem detected at position " << JSONObjectEndIterator;
            log.Write(ERROR, ost.str());
        }
    }
   

    return result;
}


bool JSONParser::ParseJSONObject (const std::string jsonLine)
{
    bool result = false;

    {
        CLog                log(CHAT_LOG_FILE_NAME);
        std::ostringstream  ost;
        ost.str("");
        ost << "ParseJSONObject: start";
        log.Write(DEBUG, ost.str());
    }
    if(BuildNameValuePairs(jsonLine))
    {
        result = true;
    }
    else
    {
        {
            CLog                log(CHAT_LOG_FILE_NAME);
            std::ostringstream  ost;
            ost.str("");
            ost << "ERROR: parsing Name:Value pairs";
            log.Write(ERROR, ost.str());
        }
    }
    
    {
        CLog                log(CHAT_LOG_FILE_NAME);
        std::ostringstream  ost;
        ost.str("");
        ost << "ParseJSONObject: end (return " << result << ")";
        log.Write(DEBUG, ost.str());
    }
    return result;
}

bool JSONParser::isNameExists(std::string name)
{
    std::map<std::string, std::string>::iterator it;

    it = paramHash.find(name);
    if(it != paramHash.end())
        return  true;
    return false;
}

std::string JSONParser::GetValue(std::string name)
{
    std::map<std::string, std::string>::iterator it;

    it = paramHash.find(name);
    if(it != paramHash.end())
        return  it->second;

    return "";
}

void  JSONParser::DumpParamHash()
{
    std::map<std::string, std::string>::iterator    it;
    ostringstream                                   ost;

    ost << endl << "DumpParamHash: --- " << endl;
    for(it = paramHash.begin(); it != paramHash.end(); it++)
    {
        ost << it->first << "->" << (it->second.length() > 512 ? it->second.substr(0, 511) : it->second) << endl;
    }
    ost << "DumpParamHash: --- " << endl;
    {
        CLog                log(CHAT_LOG_FILE_NAME);
        log.Write(DEBUG, ost.str());
    }
}

