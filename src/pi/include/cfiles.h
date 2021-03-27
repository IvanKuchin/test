#ifndef __CFILES__H__
#define __CFILES__H__

#include <vector>
#include <string>

using namespace std;

#include "cexception.h"
#include "clog.h"


class CFile
{
	private:
		char*		content = NULL;
		int			length = 0;
		string		name = "";
	public:
		string		GetName()		const			{ return name; }
		int			GetSize()		const			{ return length; }
		char*		GetContent()	const			{ return content; }

		void		SetName(const string &n)		{ name = n; }
		void		SetName(string &&n) noexcept	{ name = move(n); }
		void		SetSize(int s)					{ length = s; }
		void		SetContent(char *c);
		void		Set(string n, char* c, int s);

		void		Delete();

					~CFile();
};

class CFiles
{
	private:
		vector<CFile *>	files;
	public:
		bool		Add(string name, char *content, int size);
		string		GetName(int i);
		char*		Get(int i);
		char*		Get(string name);
		int			GetSize(int i);
		int			GetSize(string name);
		void		Delete(string name);
		int			Count();

					~CFiles();
};

#endif
