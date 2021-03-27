#include "cvars.h"

CVars::CVars()
{
}

auto CVars::GetUUID() -> string
{
    auto    base = "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx"s;
    auto    result = ""s;

    for_each(base.begin(), base.end(), [&result](char param) 
                                        {
                                            if(param == 'x')
                                            {
                                                stringstream ss;

                                                ss << hex << rand() % 16;                                                
                                                result += ss.str();
                                            }
                                            else if(param == 'y')
                                            {
                                                stringstream    ss;
                                                int             my_num = ((rand() % 16) & 3) | 8;

                                                ss << hex << my_num;
                                                result += ss.str();
                                            }
                                            else
                                                result += param;
                                            
                                        });

    return result;
}

bool CVars::Redefine(string name, string value)
{
    bool        bResult = false;

    if(name.empty())
    {
        MESSAGE_ERROR("", "", "can't set variable with empty name");
        return bResult;
    }

    if(find(name) != end())
    {
        MESSAGE_DEBUG("", "", "variable(" + name + ") exists with value(" + Get(name) + "), redefine to (" + value + ").");

    	Delete(name);
    }

    CVars::iterator     lb = lower_bound(name);
    insert(lb, value_type(name, value));
    bResult = true;

    return bResult;
}

bool CVars::Add(string name, string value)
{
    bool	bResult = false;

    if(name.length() == 0)
    {
        MESSAGE_ERROR("", "", "can't set variable with empty name");
    	return bResult;
    }
    
    if(find(name) != end())
    {
    	CLog	log;
    	log.Write(DEBUG, "CVars::" + string(__func__) + "(" + name + ")[" + to_string(__LINE__) + "]: variable [" + name + "(" + value + ")] already exist with other value");
    	return bResult;
    }
    
    CVars::iterator	lb = lower_bound(name);
    insert(lb, value_type(name, value));
    bResult = true;
    
    return bResult;
}

bool CVars::Delete(string name)
{
    bool	bResult = false;
    
    try
    {
    	CVars::iterator	itr = find(name);
    	
    	if(itr != end())
    	    erase(itr);
    	bResult = true;
    }
    catch(...)
    {
    	bResult = false;
    }

    return bResult;
}

string CVars::Get(string name)
{
    auto result = ""s;

    if(name.find("new:UUID") == 0) { result = GetUUID(); }
    else
    {
        if(name.find("index:") == 0)
        {
            name = name.substr("index:"s.length()) + GetIndex();
        }

        try
        {
            // --- do NOT replace below with self[name]
            // --- this check helps to identify if variable already exists
            CVars::iterator itr = find(name);
            if(itr == end())
                result = "";
            else
                result = itr->second;
        }
        catch(...)
        {
            result = "";
            MESSAGE_ERROR("", "", "exception thrown in map.find()");
        }
    }
    
    return result;
}

string  CVars::GetNameByRegex(regex r)
{
    string  result = "";

    for (auto it=begin(); it!=end(); ++it)
    {
        smatch  m;

        if(regex_search(it->first, m, r))
        {
            result = it->first;
            break;
        }
    }

    return result;
}

//returned val
// true: error
// false: success
bool CVars::Set(string name, string val)
{
    auto	result = false;
    
    if(name.empty())
    {
        result = true;

        MESSAGE_ERROR("", "", "can't set variable with empty name");
    }
    else
    {
        erase(name);
        CVars::iterator	lb = lower_bound(name);
        insert(lb, value_type(name, val));
    }

    return result;    
}

int CVars::Count()
{
    return size();
}
