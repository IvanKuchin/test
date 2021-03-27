#ifndef __UTILITIES_COMMON__H__
#define __UTILITIES_COMMON__H__

#include "ccgi.h"
#include "cmysql.h"
#include "cuser.h"

using namespace std;

auto		LogEnvVariables() -> void;
auto 		RegisterInitialVariables(CCgi *, CMysql *, CUser *) -> bool;
auto 		SetLocale(string locale) -> bool;
auto 		GenerateSession(string action, CCgi *, CMysql *, CUser *) -> string;
auto 		LogoutIfGuest(string action, CCgi *, CMysql *, CUser *) -> string;
auto		AJAX_ResponseTemplate(CCgi *indexPage, const string &success_message, const string &error_message) -> string;
auto		AJAX_ResponseTemplate(CCgi *indexPage, const string &success_message, const vector<pair<string, string>> &error_messages) -> string;

#endif
