#ifndef __C_CACHE_OBJ__H__
#define __C_CACHE_OBJ__H__

#include <stdio.h>
#include <map>

using namespace std;

#include "clog.h"
#include "cmysql.h"
#include "cvars.h"
#include "cuser.h"
// #include "utilities.h"

class c_cache_obj
{
	private:
		// --- static variable definition makes cache allocation permanent to the process
		// --- actual variable declaration in .cpp file
		static map<string, string>			cache;
		unsigned int						cache_misses = 0;
		unsigned int						cache_hits = 0;

	public:
		auto		AddToCache(string _key, string _value) -> string;
		auto		GetFromCache(string _key) -> string; // --- do not call it randomly, it will increase cache_hits/cache_misses
		auto		InCache(string _key) -> bool;

		auto		Get(string db_query, CMysql *db, CUser *user, string (*func)(string, CMysql *, CUser *)) -> string;

					~c_cache_obj();
};

#endif

