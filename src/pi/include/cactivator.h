#ifndef __CACTIVATOR__H__
#define __CACTIVATOR__H__

#include <string>
#include <sstream>
#include "localy.h"
#include "cmysql.h"
#include "ccgi.h"

using namespace std;

#define	ACTIVATOR_LEN			20
#define ACTIVATOR_SESSION_LEN	24*60

class CActivator
{
	private:
		CCgi		*cgi;
		CMysql		*db;
		string		actID;
		string		type;
		string		time;
		string		user;

	public:
					CActivator();

		void		SetUser(const string &u)		{ user = u; }
		void		SetUser(string &&u) noexcept	{ user = move(u); }
		string		GetUser() const 				{ return user; }

		void		GenerateID();
		void		SetID(const string &id)			{ actID = id; }
		void		SetID(string &&id) noexcept		{ actID = move(id); }
		string		GetID() const					{ return actID; }

		void		SetType(const string &t)		{ type = t; }
		void		SetType(string &&t) noexcept	{ type = move(t); }
		string		GetType() const					{ return type; }

		void		SetTime(const string &t)		{ time = t; }
		void		SetTime(string &&t) noexcept	{ time = t; }
		string		GetTime() const					{ return time; }

		void		SetDB(CMysql *param)			{ db = param; }
		void		SetCgi(CCgi *param)				{ cgi = param; }

		void		Save();
		bool		Load(string id);
		void		Delete(string id);
		void		DeleteByUser(string userToDelete);
		void		Delete();

		void		Activate();
};

#endif
