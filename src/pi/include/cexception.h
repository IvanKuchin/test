#ifndef __CEXCEPTION__H__
#define __CEXCEPTION__H__

#include <string>
#include "cmysql.h"
#include "localy.h"

using namespace std;

class CException
{
	string	message;
public:

			CException() : message("unknown error")	{};
			CException(string m) : message(m)		{};
	string	GetReason() const 						{ return message; };
};

class CExceptionHTML
{
	string	messageID, lng, param1;
	CMysql	*db;
public:

			CExceptionHTML() : messageID("unknown error"), lng(DEFAULT_LANGUAGE), db(NULL)					{};
			CExceptionHTML(string m) : messageID(m), lng(DEFAULT_LANGUAGE), db(NULL)						{};
			CExceptionHTML(string m, string n) : messageID(m), lng(DEFAULT_LANGUAGE), param1(n), db(NULL)	{};
	void	SetLanguage(string lang)				{ lng = lang; }
	void	SetDB(CMysql *mysql)					{ db = mysql; }
	void	SetParam1(const string &p) 				{ param1 = p; };
	void	SetParam1(string &&p) noexcept			{ param1 = move(p); };
	string	GetID() const							{ return messageID; }
	string	GetReason();
	string	GetTemplate();
	string	GetParam1() { return param1; };
};

class CEmptyException
{
	string	message;
public:
    
			CEmptyException() : message("empty error")	{};
			CEmptyException(string m) : message(m) 		{};
	string	GetReason() const							{ return message; };
};

#endif
