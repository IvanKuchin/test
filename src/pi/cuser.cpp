#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iomanip>
#include <iostream>

#include <sstream>

#include "cactivator.h"
#include "cmail.h"
#include "cuser.h"
#include "cexception.h"
//#include "GeoIP.h"

bool CUser::isBlock()
{
	if(!db)
	{
		MESSAGE_ERROR("", "", "connecting db in CUser::Block");
		throw CExceptionHTML("error db");
	}

	if(db->Query("SELECT * FROM `users_block` WHERE `userid`=\"" + GetLogin() + "\";") > 0)
		return true;
	else
		return false;
}

bool CUser::isActive()
{

	if(!db)
	{
		MESSAGE_ERROR("", "", "connecting db in CUser::Block");
		throw CExceptionHTML("error db");
	}

	if(db->Query("SELECT * FROM `users` WHERE `login`=\"" + GetLogin() + "\";") > 0)
	{
		if(db->Get(0, "isactivated") == "Y") return true;
	}
	return false;
}

void CUser::Block(string reason)
{
	ostringstream	ost;

	if(GetLogin().empty())
	{
		MESSAGE_ERROR("", "", "it is require to set user_login before block in CUser::Block");
		throw CExceptionHTML("no user");
	}
	ost << "INSERT INTO users_block(`userid`,`date`,`notes`) VALUE('" << GetLogin() << "',now(),'" << reason << "')";
	if(!db)
	{
		MESSAGE_ERROR("", "", "connecting db in CUser::Block");
		throw CExceptionHTML("error db");
	}
	db->Query(ost.str());
}

bool CUser::isLoginExist()
{
	ostringstream	ost;

	ost << "select type from users where login='" << login << "'";
	if(!db)
	{
		MESSAGE_ERROR("", "", "connecting db");
		throw CExceptionHTML("error db");
	}
	if(db->Query(ost.str()) <= 0)
		return false;

	return true;
}

bool CUser::isPasswdError()
{
	if(passwd.empty()) return true;
	if(passwd != passwdConfirm)
		return true;
	else
		return false;
}

bool CUser::isEmailCorrect()
{
	string::size_type	atPos, dotPos;
	ostringstream	ost;

	if(email.empty()) return false;

	atPos = email.find("@");
	if((atPos == 0) || (atPos == string::npos)) return false;

	dotPos = email.rfind(".");
	if((dotPos == 0) || (dotPos == string::npos)) return false;
	if(dotPos < email.rfind("@")) return false;
	return true;
}

bool CUser::isEmailDuplicate()
{
	ostringstream	ost;

	ost << "select type from users where email='" << email << "'";
	if(!db) throw CExceptionHTML("error db");
	if(db->Query(ost.str()) > 0) return true;

	return false;
}

bool CUser::isPhoneCorrect()
{
	string		ph;
	ostringstream	ost;

	ph = GetPhone();
	if(ph.find_first_not_of("01234567890-()") != string::npos) return false;
	while(ph.find_first_not_of("0123456789") != string::npos)
	{
		ph.replace(ph.find_first_not_of("0123456789"), 1, "");
	}
	if(ph.length() != 10) return false;
	return true;
}

bool CUser::isPhoneDuplicate()
{
	string		ph;
	ostringstream	ost;

	ph = GetPhone();
	ost.str("");
	ost << "select * from `users` where `phone`='" << ph << "'";
	if(!db)
	{
		MESSAGE_ERROR("", "", "connecting db in CUser::isPhoneCorrect");
		throw CExceptionHTML("error db");
	}
	if(db->Query(ost.str()) > 0) return true;

	return false;
}

bool CUser::isTypeCorrect()
{
	if(GetType().empty()) return false;

	if(GetType() == "user") return true;
	if(GetType() == "partner") return true;
	if(GetType() == "investor") return true;
	if(GetType() == "no role") return true;
	if(GetType() == "approver") return true;
	if(GetType() == "agency") return true;
	if(GetType() == "subcontractor") return true;

	return false;
}

bool CUser::isAgree()
{
	if((agreement[0] == 'n') || (agreement[0] == 'N')) return false;

	return true;
}

string CUser::GetSiteTheme()
{
	string	result = DEFAULT_SITE_THEME;

	if(site_theme.length()) result = site_theme;
	else
	{
		if(db)
		{
			if(db->Query("SELECT `path` FROM `site_themes` WHERE `id`=\"" + GetSiteThemeID() + "\";"))
			{
				site_theme = db->Get(0, "path");
				result = site_theme;
			}
		}
		else
		{
			MESSAGE_ERROR("", "", "db not initialized");
		}
	}
	return result;
}

bool CUser::isAllowedLng(string p)
{
	struct	stat	buf;
	string			directory;
	bool			result = false;

	directory = TEMPLATE_PATH + p + "/";

	if(stat(directory.c_str(), &buf) == 0)
		result = true;
	else
	{
		MESSAGE_ERROR("", "", "directory [" + directory + "] doesn't exists");

		result = false;
	}

	return result;
}

void CUser::SetLng(const string &p)
{
	if(isAllowedLng(p))
		lng = p;
	else
	{
		MESSAGE_ERROR("", "", "language for user " + GetLogin() + " changed to default, because [" + p + "] is not supported yet.");
		lng = DEFAULT_LANGUAGE;
	}
}

void CUser::Create()
{
	if(db == NULL)
	{
		MESSAGE_ERROR("", "", "db is null");
		throw CExceptionHTML("error db in User Create");
	}
	else
	{
		SetID(
			to_string(
				db->InsertQuery(
					"INSERT INTO `users` "
						"(`login`, `email`, `type`, `isactivated`, `lng`, `regdate`, `partnerid`, `country`, `ip`, `country_code`, `phone`, `activator_sent`, `cv`) "
					"VALUES "
						"(" + 
							quoted(GetLogin()) + "," + 
							quoted(GetEmail()) + "," + 
							quoted(GetType()) + "," +
							quoted(GetIsActivated()) + "," +
							quoted(GetLng()) + "," + 
							"NOW()," + 
							quoted(GetPartnerID().length() ? GetPartnerID() : "0") + "," + 
							"\"0\"," + 
							quoted(GetIP()) + "," + 
							quoted(GetCountryCode()) + "," +
							quoted(GetPhone()) + "," + 
							"NOW()," + 
							quoted(GetCV()) + 
						");"
					)
				)
			);


		if(GetID() != "0")
		{
			// --- safety check
			// --- `users_passwd` should not contain any passwords for a new user before the user actually created
			auto	passwdCount = db->Query("SELECT `passwd` FROM `users_passwd` WHERE `userID`=\"" + GetID() + "\";");

			if(passwdCount)
			{
				MESSAGE_ERROR("", "", "new user(" + GetID() + ") having " + to_string(passwdCount) + " password(s) set in `users_passwd` table, before password actually created. Here is one of passwords(" + db->Get(0, "passwd") + ")")

				db->Query("DELETE FROM `users_passwd` WHERE `userID`=\"" + GetID() + "\";");
			}

			if(db->InsertQuery(
					"INSERT INTO `users_passwd` (`userID`, `passwd`, `isActive`, `eventTimestamp`)"
					"VALUES (\"" + GetID() + "\", \"" + GetPasswd() + "\", \"true\", NOW());"
					)
			)
			{
				// --- update user data with city/country, if detected
				auto	country = GetCountry();
				auto	city = GetCity();
				auto	countryID = ""s, cityID = ""s;

				if(country.length())
				{
					if(db->Query("SELECT `id` FROM `geo_country` WHERE `title`=\"" + country + "\";"))
					{
						countryID = db->Get(0, "id");
					}
					else
					{
						auto	temp = db->InsertQuery("INSERT INTO `geo_country` SET `title`=\"" + country + "\";");

						if(temp)
							countryID = to_string(temp);
						else
						{
							MESSAGE_ERROR("", "", "fail to insert to table `geo_country`")
						}
					}
				}

				if(city.length())
				{
					if(db->Query("SELECT `id` FROM `geo_locality` WHERE `title`=\"" + city + "\";"))
					{
						cityID = db->Get(0, "id");
					}
					else
					{
						auto	temp = db->InsertQuery("INSERT INTO `geo_locality` SET `title`=\"" + city + "\";");

						if(temp)
							cityID = to_string(temp);
						else
						{
							MESSAGE_ERROR("", "", "fail to insert to table `geo_city`")
						}
					}

					if(cityID.length())
					{
						db->Query("UPDATE `users` SET `geo_locality_id`=\"" + cityID + "\" WHERE `id`=\"" + GetID() + "\";");
						if(db->isError())
						{
							MESSAGE_ERROR("", "", "fail to update table `users` (" + db->GetErrorMessage() + ")")
						}
					}
					else
					{
						MESSAGE_ERROR("", "", "cityID is empty, although cityName defined (" + city + ").")
					}
				}
			}
			else
			{
				MESSAGE_ERROR("", "", "fail to insert to table users_passwd");
			}
		}
		else
		{
			MESSAGE_ERROR("", "", "fail to insert to table `users`");
			throw CExceptionHTML("error db");
		}
	}
}

void CUser::Email(string messageID)
{
	CMailLocal	mail;
	CActivator	activator;

	if(GetVars() == NULL)
	{
		MESSAGE_ERROR("", "", "Vars array must be send to CUser()");
		throw CExceptionHTML("vars array");
	}

	if(GetDB() == NULL)
	{
		MESSAGE_ERROR("", "", "DB-connection must be send to CUser()");
		throw CExceptionHTML("db error parameter");
	}

	if(GetLogin().empty())
	{
		MESSAGE_ERROR("", "", "Login must be set in CUser()");
		throw CExceptionHTML("recipient before template");
	}

	if(messageID.empty())
	{
		MESSAGE_ERROR("", "", "Type of mail message is unknown");
		throw CExceptionHTML("mail template");
	}

	activator.SetDB(GetDB());
	activator.SetUser(GetLogin());
	activator.SetType("reg_user");
	GetVars()->Set("actid", activator.GetID());
	GetVars()->Set("host", getenv("HTTP_HOST"));
	activator.Save();
	mail.Send(GetLogin(), messageID, GetVars(), GetDB());
}

bool CUser::FillObjectFromDB()
{
	SetID(db->Get(0, "id"));
	SetLogin(db->Get(0, "login"));
	SetEmail(db->Get(0, "email"));
	SetSiteThemeID(db->Get(0, "site_theme_id"));
	SetLng(db->Get(0, "lng"));
	SetIP(db->Get(0, "ip"));
	SetType(db->Get(0, "type"));
	SetCV(db->Get(0, "cv"));
	SetName(db->Get(0, "name"));
	SetNameLast(db->Get(0, "nameLast"));
	SetSmartwayEmployeeID(db->Get(0, "smartway_employee_id"));
	if(LoadAvatar()) {}
	else { MESSAGE_DEBUG("", "", "no avatar for user.id(" + GetID() + ")"); }

	return  true;
}

bool CUser::GetFromDBbyLogin(string login)
{
	ostringstream		ost;
	bool				result;

	if(db == NULL) throw CExceptionHTML("error db");

	if(db->Query(
			"SELECT * FROM `users` "
			"INNER JOIN `users_passwd` ON `users`.`id`=`users_passwd`.`userID` "
			"WHERE `users`.`login`=\"" + login + "\" AND `users_passwd`.`isActive`='true';"
		)) 
	{
		auto	passwd = db->Get(0, "passwd"); // --- keep it temporarily

		if(FillObjectFromDB()) // --- will change db object due to LoadAvatar
		{
			SetPasswd(passwd);
			result = true;
		}
		else MESSAGE_ERROR("", "", "fail to fill up user object");
	}
	else
	{
		result = false;
		MESSAGE_DEBUG("", "", "user(" + login + ") not found")
	}

	return result;
}

auto CUser::PatchRussianPhoneNumber(string number) -> auto
{
	MESSAGE_DEBUG("", "", "start (number = " + number + ")");

	if((number.length() == 11) && (number[0] == '8')) number[0] = '7';

	MESSAGE_DEBUG("", "", "finish (number = " + number + ")");

	return number;
}

bool CUser::GetFromDBbyPhone(const string &country_code, const string &phone)
{
	auto	result = false;
	auto	phone_digits = SymbolReplace_KeepDigitsOnly(phone);

	if(db == NULL) throw CExceptionHTML("error db");

	if(country_code.length() && phone_digits.length())
	{
		phone_digits = PatchRussianPhoneNumber(phone_digits);

		if(db->Query(
				"SELECT * FROM `users` "
				"INNER JOIN `users_passwd` ON `users`.`id`=`users_passwd`.`userID` "
				"WHERE "
					"`users_passwd`.`isActive`='true' "
					"AND "
					"`users`.`country_code`=\"" + country_code + "\""
					"AND "
					"`users`.`phone`=\"" + phone_digits + "\" "
				";"
			)) 
		{
			auto	passwd = db->Get(0, "passwd"); // --- keep it temporarily

			if(FillObjectFromDB()) // --- will change db object due to LoadAvatar
			{
				SetPasswd(passwd);
				result = true;
			}
			else MESSAGE_ERROR("", "", "fail to fill up user object");
		}
		else
		{
			MESSAGE_DEBUG("", "", "user(" + login + ") not found")
		}
	}
	else
	{
		MESSAGE_DEBUG("", "", "phone number is empty");
	}

	return result;
}

bool CUser::GetFromDBbyID(string id)
{
	ostringstream		ost;
	bool				result;

	if(db == NULL) throw CExceptionHTML("error db");

	ost << "SELECT * FROM `users`  \
			INNER JOIN `users_passwd` ON `users`.`id`=`users_passwd`.`userID` \
			WHERE `users`.`id`=\"" << id << "\" AND `users_passwd`.`isActive`='true';";
	if(db->Query(ost.str()) == 0) {
		result =  false;
	}
	else {
		if(FillObjectFromDB()) result = true;
		else MESSAGE_ERROR("", "", "fail to fill up user object");
	}

	return result;
}

bool CUser::GetFromDBbyEmailNoPassword(string email)
{
	ostringstream		ost;
	bool				result = false;

	MESSAGE_DEBUG("", "", "start");

	if(db == NULL)
	{
		MESSAGE_ERROR("", "", "db must be initialized");

		throw CExceptionHTML("error db");
	}

	if(db->Query("SELECT * FROM `users` WHERE `email`=\"" + email + "\";") == 0)
	{
		MESSAGE_DEBUG("", "", "user.email [" + email + "] not found");
	}
	else
	{
		if(FillObjectFromDB()) result = true;
		else MESSAGE_ERROR("", "", "fail to fill up user object");
	}

	MESSAGE_DEBUG("", "", "finish (return " + (result ? string("true") : string("false")) + ")");
	return result;
}

bool CUser::GetFromDBbyEmail(string email)
{
	ostringstream		ost;
	bool				result = false;

	MESSAGE_DEBUG("", "", "start (" + email + ")");

	if(db == NULL)
	{
		MESSAGE_ERROR("", "", "db must be initialized");

		throw CExceptionHTML("error db");
	}

	if(GetFromDBbyEmailNoPassword(email))
	{
		if(db->Query("SELECT * FROM `users_passwd` WHERE `userID`=\"" + GetID() + "\" AND `users_passwd`.`isActive`='true';"))
		{
			SetPasswd(db->Get(0, "passwd"));
			result = true;
		}
		else
		{
			MESSAGE_ERROR("", "", "no active password found for user.id(" + GetID() + ")");
		}
	}
	else
	{
		MESSAGE_DEBUG("", "", "user not found (" + email + ")")
	}

	MESSAGE_DEBUG("", "", "finish (" + (result ? string("true") : string("false")) + ")");

	return result;
}

bool CUser::LoadAvatar()
{
	ostringstream	ost1;
	bool			result = false;
	string			avatarPath = "empty";

	MESSAGE_DEBUG("", "", "start(user.id = " + GetID() + ")");

	// --- Get user avatars
	if(db->Query("select * from `users_avatars` where `userid`=\"" + GetID() + "\" and `isActive`='1';"))
	{
		avatarPath = "/images/avatars/avatars"s + db->Get(0, "folder") + "/" + db->Get(0, "filename");
		result = true;
	}
	SetAvatar(avatarPath);

	MESSAGE_DEBUG("", "", "finish");

	return result;
}

bool CUser::UpdatePresence()
{
	bool 			result = true;
	ostringstream	ost;

	MESSAGE_DEBUG("", "", "start(user.id = " + GetID() + ")");

	if(db->Query("update `users` set `last_online`=now(), `last_onlineSecondsSinceY2k`=\"" + to_string(GetSecondsSinceY2k()) + "\" where `id`=\"" + GetID() + "\";"))
	{
		MESSAGE_ERROR("", "", "update presence SQL-query must return 0. It is not zero, means userID [" + GetID() + "] having more than 1 users.");
		result = false;
	}

	MESSAGE_DEBUG("", "", "finish");

	return result;
}

auto CUser::ChangePasswordTo(string password_hash) -> string
{
	auto	error_message = ""s;

	MESSAGE_DEBUG("", "", "start(user.id = " + GetID() + ")");

	if(GetID().length())
	{
		if(db->Query("SELECT `eventTimestamp` FROM `users_passwd` WHERE `userID`=\"" + GetID() + "\" and `passwd`=\"" + password_hash + "\";"))
		{
			string timestamp = db->Get(0, "eventTimestamp");

			error_message = "этот пароль использовался " + GetHumanReadableTimeDifferenceFromNow(timestamp) + " Выберите другой.";
			MESSAGE_DEBUG("", "", "user.id(" + GetID() + ") not allowed to re-use old password(" + timestamp + ").");
		}
		else
		{
			db->Query("UPDATE `users_passwd` SET `isActive`=\"false\" WHERE `userID`=\"" + GetID() + "\";");
			if(db->isError())
			{
				error_message = "Ошибка обновления пароля";
				MESSAGE_ERROR("", "", "fail to update users_passwd table user.id(" + GetID() + ")");
			}
			else
			{
				long	passwd_id = db->InsertQuery("INSERT INTO `users_passwd` SET `userID`=\"" + GetID() + "\", `passwd`=\"" + password_hash + "\", `isActive`=\"true\", `eventTimestamp`=NOW();");

				if(passwd_id)
				{
				}
				else
				{
					error_message = "Ошибка обновления пароля";
					MESSAGE_ERROR("", "", "fail to insert new passwd to users_passwd table");
				}
			}
		}
	}
	else
	{
		error_message = gettext("user not found");
		MESSAGE_ERROR("", "", error_message);
	}

	MESSAGE_DEBUG("", "", "finish(error_message length is " + to_string(error_message.length()) + ")");

	return error_message;
}

auto CUser::DeleteFromDB() -> string
{
	auto	error_message = ""s;

	MESSAGE_DEBUG("", "", "start(user.id = " + GetID() + ")");

	if(GetID().length())
	{
		db->Query("DELETE FROM `users_passwd` WHERE `userID`=\"" + GetID() + "\";");
		if(db->isError())
		{
			error_message = gettext("SQL syntax error");
			MESSAGE_ERROR("", "", "fail to update table `users` (" + db->GetErrorMessage() + ")")
		}
		else
		{
			db->Query("DELETE FROM `sessions` WHERE `user_id`=\"" + GetID() + "\";");
			if(db->isError())
			{
				error_message = gettext("SQL syntax error");
				MESSAGE_ERROR("", "", "fail to update table `users` (" + db->GetErrorMessage() + ")")
			}
			else
			{
				db->Query("DELETE FROM `users` WHERE `id`=\"" + GetID() + "\";");
				if(db->isError())
				{
					error_message = gettext("SQL syntax error");
					MESSAGE_ERROR("", "", "fail to update table `users` (" + db->GetErrorMessage() + ")")
				}
				else
				{
					
				}
			}
		}
	}
	else
	{
		error_message = gettext("user not found");
		MESSAGE_ERROR("", "", error_message);
	}

	MESSAGE_DEBUG("", "", "finish(error_message length is " + to_string(error_message.length()) + ")");

	return error_message;
}

