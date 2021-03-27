#ifndef __C_DATE_SPELLING__H__
#define __C_DATE_SPELLING__H__

#include <string>
#include <locale>

#include "clog.h"

using namespace std;

class C_Date_Spelling
{
	private:
		struct	tm			date_struct;
		
		auto				GetMonthInEnglish(int month_num) -> string;
		auto				GetMonthDeclensionInEnglish(int month_num) -> string;
	public:
							C_Date_Spelling() {};
							C_Date_Spelling(struct tm param) : date_struct(param) {};

		auto				SetTMObj(struct tm param)	{ date_struct = param; };
		auto				GetTMObj() 					{ return date_struct; };

		auto				Spell() -> string;
		auto				SpellMonth(int) -> string;
		auto 				GetFormatted(string format) -> string;
};

ostream&	operator<<(ostream& os, const C_Date_Spelling &);

#endif
