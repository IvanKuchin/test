#ifndef __CMAIL__H__
#define __CMAIL__H__

#include <string>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <cstdlib>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <memory>

#include "ctemplate.h"
#include "utilities.h"

using namespace std;

#define	CONNECT_TIMEOUT	10
#define	RECEIVE_TIMEOUT	1

class CMail
{
	private:
		string			rcptto, mailfrom, subject, message;
		string			smtpServer;
		int				smtpPort;
		bool			smtpFlag;
		string			uniqFilePostfix;
		string			stdoutFname, stdinFname, stderrFname;

		unsigned long 	Addr2Num (const char *addr);
		int				WaitSocket(int s, int sec);
		string			SocketReceive(int s);
		void			SocketSend(int s, string data);

		int				SendMailExec(const char *progr, char **av, int inp);
		int				MakeMailFile(string text);
		int				Input(int fd);

		string			GetTimeStamp();
		void			BuildUniqPostfix();

		bool			SendLocal();
		bool			SendSMTP();

		void			SetUniqPostfix(const string &tmp)		{ uniqFilePostfix = tmp; }
		void			SetStdinFname(const string &tmp) 		{ stdinFname = tmp; }
		void			SetStdoutFname(const string &tmp)		{ stdoutFname = tmp; }
		void			SetStderrFname(const string &tmp)		{ stderrFname = tmp; }

		void			SetUniqPostfix(string &&tmp) 	noexcept{ uniqFilePostfix = move(tmp); }
		void			SetStdinFname(string &&tmp) 	noexcept{ stdinFname = move(tmp); }
		void			SetStdoutFname(string &&tmp)	noexcept{ stdoutFname = move(tmp); }
		void			SetStderrFname(string &&tmp)	noexcept{ stderrFname = move(tmp); }

		string			GetUniqPostfix() 				const	{ return uniqFilePostfix; }
		string			GetStdinFname() 				const	{ return stdinFname; }
		string			GetStdoutFname()				const	{ return stdoutFname; }
		string			GetStderrFname()				const	{ return stderrFname; }

	public:
						CMail();

		void			To(const string &param)					{ rcptto = param; }
		void			From(const string &param)				{ mailfrom = param; }
		void			Subject(const string &param)			{ subject = param; }
		void			Message(const string &param)			{ message = param; }

		void			To(string &&param)				noexcept{ rcptto = move(param); }
		void			From(string &&param)			noexcept{ mailfrom = move(param); }
		void			Subject(string &&param)			noexcept{ subject = move(param); }
		void			Message(string &&param)			noexcept{ message = move(param); }

		// setting SMTP-server like "127.0.0.1" or "65.25.0.4"
		// in: string
		// out: none
		void			SetSMTPServer(const string &param)		{ smtpServer = param; }
		void			SetSMTPServer(string &&param)	noexcept{ smtpServer = move(param); }
		void			SetSMTPPort(const int port)				{ smtpPort = port; }

		bool			Send();
		bool			Send(string to, string subj, string mess);

		void			SetSMTP();
		void			SetLocal();
};

class CMailLocal : public CMail
{
	private:
		CMysql		*db;
		CVars		*vars;
		string		userLogin, userID;

	public:
		void		SetVars(CVars *param)					{ vars = param; }
		void		SetDB(CMysql *param)					{ db = param; }

		void		UserLogin(const string &param)			{ userLogin = param; }
		void		UserID(const string &param)				{ userID = param; }

		void		UserLogin(string &&param)		noexcept{ userLogin = move(param); }
		void		UserID(string &&param)			noexcept{ userID = move(param); }

		string		GetUserLogin()					const	{ return userLogin; }
		string		GetUserID()						const	{ return userID;}
		string		GetUserLng();


		string		SetTemplate(string templID);
		string		SetTemplateFile(string fileName);

		// mainly usage function.
		void		Send(string login, string templID, CVars *, CMysql *);
		void		SendToEmail(string email, string login, string templID, CVars *, CMysql *);
};

#endif
