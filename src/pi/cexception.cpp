#include "cexception.h"

string CExceptionHTML::GetReason()
{
	string	result = "";

	if(db)
	{
		if(db->Query("SELECT `message` FROM `exception` WHERE `id`=\"" + messageID + "\" AND `lng`=\"" + lng + "\";"))
			result = db->Get(0, "message");
		else
			MESSAGE_ERROR("CExceptionHTML", "", "exception.id(" + messageID + ") not found");
	}
	else
	{
		MESSAGE_ERROR("CExceptionHTML", "", "DB connection didn't initialized");
	}

	return result;
}

string CExceptionHTML::GetTemplate()
{
	string	result = "";

	if(db)
	{
		if(db->Query("SELECT `template` FROM `exception` WHERE `id`=\"" + messageID + "\" AND `lng`=\"" + lng + "\";"))
			result = db->Get(0, "template");
		else
			MESSAGE_ERROR("CExceptionHTML", "", "exception.id(" + messageID + ") not found");
	}
	else
	{
		MESSAGE_ERROR("CExceptionHTML", "", "DB connection didn't initialized");
	}

	return result;
}

