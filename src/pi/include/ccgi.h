#ifndef __CCGI__H__
#define __CCGI__H__

#include <stdio.h>

using namespace std;

#include "ccookie.h"
#include "crequest.h"
#include "clog.h"
#include "cvars.h"
#include "cfiles.h"
#include "cmysql.h"
#include "csession.h"

#define	EXTERNAL_TEMPLATE	0	//this need to be used for main template i.e. external template CCgi(EXTERNAL_TEMPLATE)
#define	INTERNAL_TEMPLATE	1	//do not use :)

#define	WITHOUT_ENDL		false	//this constant use in OutString. Wether or not to write '\n' at the end of string ?
#define	WITH_ENDL			true

class CCgi
{
    private:
		FILE				*templateFile;
		CMysql				*db;
		CVars				vars;
		CFiles				files;
		CSession			sessionDB;
		int					initHeadersFlag;
		vector<string>		empty_var_list = {};

		string				content = "";

		void				InitHeaders();

		//it's temporarily
		string				cookieName, cookieVal, cookieMaxAge, cookiePath, cookieDomain;
		int					cookieSecure;
		CCookies			cookie;

		string				RecvLine(FILE *s);
		string				GlobalMessageReplace(string where, string src, string dst);

		string				FindLanguageByIP(string ip);
    public:
				CCgi();
				CCgi(int typeTemplate);
				CCgi(const char *fileName, int typeTemplate);
				CCgi(const char *fileName, CVars v);
				CCgi(const char *fileName, CVars v, CFiles f);

		void	SetDB(CMysql *mysql);

		void	SetVars(CVars &param) 				{ vars = param; };

		bool	SetTemplateFile(string fileName);
		bool	SetTemplate(string templ);

		//Render string wich content <<>> tags
		//in: string
		//out: string
		string	RenderLine(string rLine);
		auto	RenderTemplate() -> string;

		//Out templateFile wich content <<>> tags
		//in: nothing
		//out:nothing
		void	OutTemplate();
		auto	GetContent()						{ return content; };

		//Simple out string to stdout
		//in: nothing
		//out:nothing
		void	OutString(const string &str, bool endlFlag);
		void	AddToContent(const string &str, bool endlFlag);

		void	OutStringEncode(const char *str);
		string	StringEncode(string str);

		//Creates new var in "CVars var"
		//in: const char *name, const char *value
		//out: nothing
		void	RegisterVariable(string name, string value);
	    void    RegisterVariableForce(string name, string value);

		//Register all parameters of URL in "CVars var"
		void	ParseURL();

		//Return CVars enum
		//in:
		//out: CVars*
		CVars*	GetVarsHandler();
		
		//Return CFiles enum
		//in:
		//out: Files*
		CFiles*	GetFilesHandler();

		//Redirect to url
		//in: const char
		//out: void
		void	Redirect(string url);
		void	RedirectHTTP(string url);

		//Get value cookie named "name"
		//in: string name
		//out: string value
		string	GetCookie(string name);

		//add cookie to set in HTTP-Response
		void	AddCookie(string cn, string cv, struct tm *ce = nullptr, string cp = "", string cd = "", string cs = "");
		void	AddCookie(string cn, string cv, int shift_in_seconds_from_now, string cp = "", string cd = "", string cs = "");
		// void	AddCookie(string cn, string cv, string ce = "", string cp = "", string cd = "", string cs = "");
		// void	AddCookie(string cn, string cv, string ce = "", int cma = 0, string cp = "", string cd = "", string cs = "");
		bool	CookieUpdateTS(string name, int deltaTimeStamp);
		// void	ModifyCookie(string cn, string cv, string cma = "", string cp = "", string cd = "", string cs = "");
		void	DeleteCookie(string cn, string cd = "", string cp = "/", string cs = "");

		//return environment variable HTTP_HOST
		string	GetHttpHost();

		//return environment variable REQUEST_URI
		string	GetRequestURI();

		//replace "<" or ">" to "&lt;" and "&gt;" from string
		string	ReplaceHTMLTags(string src);

		//get name of language for current web-page
		//in: none
		//out: string value
		string	GetLanguage();

		//get encoding from language
		//in: none
		//out: string value
		string	GetEncoding();

		//check for existence language templates
		//in: string lang
		//out: bool
		bool	isAcceptedLanguage(string lang);

		// --- get detected city/country from session
		string	GetCountry();
		string	GetCity();

		string	SessID_Get_FromHTTP(void);
		string	SessID_Create_HTTP_DB(int max_age = SESSION_LEN * 60);
		// bool	SessID_Exist_InDB(string sessionHTTP);
		bool	SessID_Load_FromDB(string sessid);
		bool	SessID_CheckConsistency(void);
		bool	SessID_Update_HTTP_DB();
		string	SessID_Get_UserFromDB();
		bool	Cookie_Expire();
		bool	Cookie_InitialAction_Expire();
		bool	Cookie_InitialAction_Assign(string inviteHash);

		auto	GetEmptyVarList()		{ return empty_var_list; };

				~CCgi();
};

#endif

