#include "c_cache_obj.h"

// --- static variable definition makes cache allocation permanent to the process
map<string, string>	c_cache_obj::cache;

auto c_cache_obj::GetFromCache(string _key) -> string
{
	auto	result = cache[_key];

	if(result.length())
	{
		cache_hits++;
	}
	else
	{
		cache_misses++;
	}

	return result;
}
auto c_cache_obj::InCache(string _key) -> bool
{
	auto	result = cache[_key];

	return result.length() > 0;
}

auto c_cache_obj::AddToCache(string _key, string _value) -> string
{
	auto error_message = ""s;

	if(_key.length())
	{
		cache[_key] = _value;
	}
	else
	{
		error_message = "cache key is empty";
		MESSAGE_ERROR("", "", error_message);
	}

	return error_message;
}

auto c_cache_obj::Get(string db_query, CMysql *db, CUser *user, string (*func)(string, CMysql *, CUser *)) -> string
{
	MESSAGE_DEBUG("", "", "start (" + db_query + ")");

	auto	result = GetFromCache(db_query);

	if(result.length())
	{
	}
	else
	{
		if(db)
		{
			if(user)
			{
				result = func(db_query, db, user);
				AddToCache(db_query, result);
			}
			else
			{
				MESSAGE_ERROR("", "", "user is not initialized");
			}
		}
		else
		{
			MESSAGE_ERROR("", "", "db not initialized");
		}
	}

	MESSAGE_DEBUG("", "", "finish");

	return result;
}

c_cache_obj::~c_cache_obj()
{
	MESSAGE_DEBUG("", "", "cache hits = " + to_string(cache_hits) + ", cache misses = " + to_string(cache_misses) + (cache_hits + cache_misses ? ", cache performance = " + to_string(cache_hits * 100 / (cache_hits + cache_misses)) + "%" : ""));
}
