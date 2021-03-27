#ifndef __CCOOKIE__H__
#define __CCOOKIE__H__

#include <string>
#include <vector>
#include "utilities.h"
#include "cvars.h"

using namespace std;

class CCookie
{
	private:
		string		name = "", value = "", domain = "", path = "", secure = "";
		bool		isNew;		// this parameter control sentback cookie to server
		struct tm	expiration_tm;

	public:
				CCookie();

		void	SetName(const string &s)				{ name = s; }
		void	SetName(string &&s) 		noexcept	{ name = move(s); }
		void	SetValue(const string &s)				{ value = s; }
		void	SetValue(string &&s) 		noexcept	{ value = move(s); }
		void	SetDomain(const string &s)				{ domain = s; }
		void	SetDomain(string &&s) 		noexcept	{ domain = move(s); }
		void	SetPath(const string &s)				{ path = s; }
		void	SetPath(string &&s) 		noexcept	{ path = move(s); }
		void	SetExpiration(struct tm *tm)noexcept	{ if(tm) expiration_tm = (*tm); };
		void	SetSecure(const string &s)				{ secure = s; }
		void	SetSecure(string &&s) 		noexcept	{ secure = move(s); }
		void	SetNew(bool s);

		string	SetExpirationShiftedFromNow(int delta_seconds);

		string	GetName()					const		{ return name; }
		string	GetValue()					const		{ return value; }
		string	GetDomain()					const		{ return domain; }
		string	GetPath()					const		{ return path; }
		string	GetExpiration();
		string	GetSecure()					const		{ return secure; }
		bool	GetNew()					const		{ return isNew; }
};

class CCookies
{
	private:
		vector<CCookie>	cookies;
		
		CCookie	PreBuildNewCookie(string name, string value, string domain, string path, string secure, bool newCookie);
		void	Remove_VectorInstance_IfExists(string name);
		
	public:
				CCookies()								{ cookies.reserve(8); };

		void	Add(string name, string value, struct tm *expiration = nullptr, string domain = "", string path = "", string secure = "", bool newCookie = TRUE);
		void	Add(string name, string value, int shift_in_seconds_from_now, string domain = "", string path = "", string secure = "", bool newCookie = TRUE);
		bool	UpdateTS(string name, int deltaTimeStamp);
		void	Delete(string name, string domain = "", string path = "", string secure = "");
		string	Get(string name)			const;
		bool	IsExist(string name)		const;
		string	GetAll();
		void	ParseString(string str);
		void	RegisterCookieVariables(CVars *v);
};

#endif
