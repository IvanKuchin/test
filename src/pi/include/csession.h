#ifndef __CSESSION__H__
#define __CSESSION__H__

using namespace std;

#ifndef MAXMIND_DISABLE
#include <maxminddb.h>
#endif

#include <sys/time.h>
#include <string>
#include <sstream>
#include "cexception.h"
#include "cmysql.h"
#include "ccookie.h"
#include "utilities.h"
#include "localy.h"

#define	SESSION_LEN	60	// --- session duration in minutes

class CSession
{
	private:
		CMysql		*db;
		CCookie		*cookies;
		string		sessID;
		string		user = "";
		string		user_id = "";
		string		lng;
		string		ip;
		int			expire;

		// --- Possible to convert to "static" to reduce memory usage (MMDB is using about 9K per instance)
		bool					MMDB_usage;
#ifndef MAXMIND_DISABLE
		MMDB_s					mmdb;
		MMDB_entry_data_list_s *MMDB_entry_data_list;
		MMDB_lookup_result_s	MMDB_result;
#endif

		bool		Save();
		string		DetectItem(string MMDB_itemName);
		void		InitMaxMind();

	public:
					CSession();

		void		SetID	(const string &param)		{ sessID = param; }
		void		SetUser	(const string &param)		{ user = param; }
		void		SetUserID(const string &param)		{ user_id = param; }
		void		SetIP	(const string &param)		{ ip = param; }
		void		SetLng	(const string &param)		{ lng = param; }
		void		SetDB	(CMysql *param)				{ db = param; }
		void		SetExpire(int param)				{ expire = param; }

		void		SetID	(string &&param)	noexcept{ sessID = move(param); }
		void		SetUser	(string &&param)	noexcept{ user = move(param); }
		void		SetUserID(string &&param)	noexcept{ user_id = move(param); }
		void		SetIP	(string &&param)	noexcept{ ip = move(param); }
		void		SetLng	(string &&param)	noexcept{ lng = move(param); }

		string		GetIP()						const	{ return ip; }
		string		GetID()						const	{ return sessID; }
		string		GetUser()					const	{ return user; }
		string		GetUserID()					const	{ return user_id; }
		string		GetLng()					const	{ return lng; }
		int			GetExpire()					const	{ return expire; }

		void		GenerateID();

		string		DetectCountry();
		string		DetectCity();


		bool		Save(string u, string i, string l);
		bool		Load(string id);
		bool		Update();
		bool		UpdateIP(string newIP);


		bool		isExist(string id);
		bool		CheckConsistency();
		bool		SessID_ExpireSession();

					~CSession();
};

#endif
