#include "c_date_spelling.h"


auto C_Date_Spelling::GetMonthInEnglish(int month_num) -> string
{
	auto	result = "";

	if(month_num == 1) result = "January";
	if(month_num == 2) result = "February";
	if(month_num == 3) result = "March";
	if(month_num == 4) result = "April";
	if(month_num == 5) result = "May";
	if(month_num == 6) result = "June";
	if(month_num == 7) result = "July";
	if(month_num == 8) result = "August";
	if(month_num == 9) result = "September";
	if(month_num == 10) result = "October";
	if(month_num == 11) result = "November";
	if(month_num == 12) result = "December";

	return result;
}

auto C_Date_Spelling::GetMonthDeclensionInEnglish(int month_num) -> string
{
	auto	result = "";

	if(month_num == 1) result = "January declension";
	if(month_num == 2) result = "February declension";
	if(month_num == 3) result = "March declension";
	if(month_num == 4) result = "April declension";
	if(month_num == 5) result = "May declension";
	if(month_num == 6) result = "June declension";
	if(month_num == 7) result = "July declension";
	if(month_num == 8) result = "August declension";
	if(month_num == 9) result = "September declension";
	if(month_num == 10) result = "October declension";
	if(month_num == 11) result = "November declension";
	if(month_num == 12) result = "December declension";

	return result;
}

auto C_Date_Spelling::Spell() -> string
{
	auto	result = ""s;

	MESSAGE_DEBUG("", "", "start");

	mktime(&date_struct);

	result = to_string(date_struct.tm_mday) + " " + gettext(GetMonthDeclensionInEnglish(date_struct.tm_mon + 1).c_str()) + " " + to_string(1900 + date_struct.tm_year);

	MESSAGE_DEBUG("", "", "end (" + result + ")");

	return result;
}

auto C_Date_Spelling::SpellMonth(int month_num) -> string
{
	return gettext(GetMonthInEnglish(month_num).c_str());
}

auto C_Date_Spelling::GetFormatted(string format) -> string
{
	char	buffer[256];

	MESSAGE_DEBUG("", "", "start");

	strftime(buffer, sizeof(buffer) - 1, format.c_str(), &date_struct);

	MESSAGE_DEBUG("", "", "end (result = " + buffer + ")");

	return string(buffer);
}


ostream& operator<<(ostream& os, const C_Date_Spelling &var)
{
	os << "object C_Date_Spelling [empty for now]";

	return os;
}

