#include "utilities_common.h"

auto LogEnvVariables() -> void
{
	MESSAGE_DEBUG("", "", "start");

	if(getenv("HTTP_REFERER")) MESSAGE_DEBUG("", "", "HTTP_REFERER: "s + getenv("HTTP_REFERER"));
	if(getenv("REMOTE_ADDR")) MESSAGE_DEBUG("", "", "REMOTE_ADDR: "s + getenv("REMOTE_ADDR"));
	if(getenv("SERVER_NAME")) MESSAGE_DEBUG("", "", "SERVER_NAME: "s + getenv("SERVER_NAME"));

	MESSAGE_DEBUG("", "", "finish");

	return;
}

static auto LogGitCommitID(CCgi *indexPage)
{
	// MESSAGE_DEBUG("", "", "start");

	ifstream	ifs(GIT_COMMIT_ID_FILE_NAME, ifstream::in);

	if(ifs.is_open())
	{
		auto		result = ""s;
		auto		temp = ""s;

		while(getline(ifs, temp))
		{
			result += temp;
		}

		ifs.close();

		indexPage->RegisterVariableForce("git_commit_id", result);
		MESSAGE_WARNING("", "", "git commit id: " + result);
	}
	else
	{
		MESSAGE_ERROR("", "", "git commit file (" + GIT_COMMIT_ID_FILE_NAME + ") doesn't exists");
	}

	// MESSAGE_DEBUG("", "", "finish");

	return;
}

auto RegisterInitialVariables(CCgi *indexPage, CMysql *db, CUser *user) -> bool
{
	auto	result = true;

	MESSAGE_DEBUG("", "", "start");

	LogEnvVariables();
	LogGitCommitID(indexPage);

	indexPage->RegisterVariableForce("rand", GetRandom(10));
	indexPage->RegisterVariableForce("random", GetRandom(10));
	indexPage->RegisterVariableForce("DOMAIN_NAME", DOMAIN_NAME);
	indexPage->RegisterVariableForce("EMAIL_FROM_DOMAIN", GetDomain());
	indexPage->RegisterVariableForce("site_theme", DEFAULT_SITE_THEME);

	if(getenv("REMOTE_ADDR")) indexPage->RegisterVariableForce("REMOTE_ADDR", getenv("REMOTE_ADDR"));
	else
	{
		MESSAGE_ERROR("", "", "env variable REMOTE_ADDR doesn't defined");
		result = false;
	}
	if(getenv("SERVER_NAME")) indexPage->RegisterVariableForce("SERVER_NAME", getenv("SERVER_NAME"));
	else
	{
		MESSAGE_ERROR("", "", "env variable SERVER_NAME doesn't defined");
		result = false;
	}

	if(db->Query("SELECT `value` FROM `settings_default` WHERE `setting`=\"keywords_head\""))
		indexPage->RegisterVariable("keywords_head", db->Get(0, "value"));
	else
	{
		MESSAGE_ERROR("", "", "getting keywords_head FROM settings_default");
	}

	if(db->Query("SELECT `value` FROM `settings_default` WHERE `setting`=\"title_head\""))
		indexPage->RegisterVariable("title_head", db->Get(0, "value"));
	else
	{
		MESSAGE_ERROR("", "", "getting title_head FROM settings_default");
	}

	if(db->Query("SELECT `value` FROM `settings_default` WHERE `setting`=\"description_head\""))
		indexPage->RegisterVariable("description_head", db->Get(0, "value"));
	else
	{
		MESSAGE_ERROR("", "", "getting description_head FROM settings_default");
	}

	if(db->Query("SELECT `value` FROM `settings_default` WHERE `setting`=\"NewsOnSinglePage\""))
		indexPage->RegisterVariable("NewsOnSinglePage", db->Get(0, "value"));
	else
	{
		MESSAGE_ERROR("", "", "getting NewsOnSinglePage FROM settings_default");

		indexPage->RegisterVariable("NewsOnSinglePage", to_string(NEWS_ON_SINGLE_PAGE));
	}

	if(db->Query("SELECT `value` FROM `settings_default` WHERE `setting`=\"FriendsOnSinglePage\""))
		indexPage->RegisterVariable("FriendsOnSinglePage", db->Get(0, "value"));
	else
	{
		MESSAGE_ERROR("", "", "getting FriendsOnSinglePage FROM settings_default");

		indexPage->RegisterVariable("FriendsOnSinglePage", to_string(FRIENDS_ON_SINGLE_PAGE));
	}
			
	MESSAGE_DEBUG("", "", "finish");

	return result;
}

auto SetLocale(string locale) -> bool
{
	auto	result = true;
	
	MESSAGE_DEBUG("", "", "start");

	if(locale.length())
	{
		setlocale(LC_ALL, locale.c_str());
		bindtextdomain(DOMAIN_NAME.c_str(), LOCALE_PATH.c_str());
		textdomain(DOMAIN_NAME.c_str());
	}
	else
	{
		MESSAGE_ERROR("", "", "locale is empty");
	}

	MESSAGE_DEBUG("", "", "finish: locale is "s + locale);

	return result;
}

static auto __LoadUserAndSetVariables(string login, CCgi *indexPage, CMysql *db, CUser *user)
{
	auto error_message = ""s;

	MESSAGE_DEBUG("", "", "start");

	user->SetDB(db);
	if(user->GetFromDBbyLogin(login))
	{
		indexPage->RegisterVariableForce("loginUser", indexPage->SessID_Get_UserFromDB());
		indexPage->RegisterVariableForce("loginUserID", user->GetID());
		indexPage->RegisterVariableForce("myLogin", user->GetLogin());
		indexPage->RegisterVariableForce("myFirstName", user->GetName());
		indexPage->RegisterVariableForce("myLastName", user->GetNameLast());
		indexPage->RegisterVariableForce("myUserAvatar", user->GetAvatar());
		indexPage->RegisterVariableForce("user_type", user->GetType());
		indexPage->RegisterVariableForce("smartway_enrolled", user->GetSmartwayEnrolled());
	}
	else
	{
		error_message = "user login " + login + " not found in DB";
		MESSAGE_ERROR("", "", error_message);
	}

	MESSAGE_DEBUG("", "", "finish");

	return error_message;
}

auto GenerateSession(string action, CCgi *indexPage, CMysql *db, CUser *user) -> string
{
	auto			locale = LOCALE_DEFAULT;
	auto			sessidHTTP = ""s;

	MESSAGE_DEBUG("", "", "start");

	// --- internalization settings
	if(indexPage->GetLanguage() == "ru") locale = LOCALE_RUSSIAN;
	if(SetLocale(locale)) {}
	else
	{
		MESSAGE_ERROR("", "", "fail to setup locale");
	}


	// --- session generation
	sessidHTTP = indexPage->SessID_Get_FromHTTP();
	if(sessidHTTP.length() < 5)
	{
		if((action.find("AJAX_") != string::npos) || (action.find("JSON_") != string::npos))
		{
			// --- this line prevents double cookie assignment in abnormal workflow
			// --- for example:
			// ---              browser cached _main_Doc_ w/o cookies
			// ---              it will request many AJAX-s: EchoRequest, GetNavChatStatus, GetNewsFeed
			// ---              some of those will assign _new_sessid_ cookies and these cookies will be different
			// ---              prohibiting AJAX to assign sessid cookies, help to avoid assignment multiple sessid cookies 
			MESSAGE_DEBUG("", action, "no sessid cookie assignment in AJAX/JSON handler. This line may appear in the log only if AJAX_ been requested prior to _main_doc_ or _main_doc_ with clean cookies has been cached in a browser.");
		}
		else
		{
			MESSAGE_DEBUG("", action, "session cookie doesn't exists, generating new session");

			sessidHTTP = indexPage->SessID_Create_HTTP_DB();
			if(sessidHTTP.length() < 5)
			{
				MESSAGE_ERROR("", action, "in generating session ID");
				throw CExceptionHTML("session can't be created");
			}
		}

/*
		if(isAllowed_NoSession_Action(action))
		{
			// --- guest user access,
			// --- 1) user wall, if exact link known
		}
		else
		{
			action = GUEST_USER_DEFAULT_ACTION;
		}
*/
		if(__LoadUserAndSetVariables("Guest", indexPage, db, user).empty())
		{
			if(isAllowed_NoSession_Action(action))
			{
				// --- guest user access,
				// --- 1) user wall, if exact link known
			}
			else
			{
				action = GUEST_USER_DEFAULT_ACTION;
			}
		}
		else
		{
			MESSAGE_ERROR("", action, "can't load Guest user profile");

			action = GUEST_USER_DEFAULT_ACTION;
		}
	}
	else
	{
		if(indexPage->SessID_Load_FromDB(sessidHTTP))
		{
			if(indexPage->SessID_CheckConsistency())
			{
				if(indexPage->SessID_Update_HTTP_DB())
				{
					indexPage->RegisterVariableForce("loginUser", "");
					indexPage->RegisterVariableForce("loginUserID", "");
/*
					user->SetDB(db);
					if(user->GetFromDBbyLogin(indexPage->SessID_Get_UserFromDB()))
					{
						indexPage->RegisterVariableForce("loginUser", indexPage->SessID_Get_UserFromDB());
						indexPage->RegisterVariableForce("loginUserID", user->GetID());
						indexPage->RegisterVariableForce("myLogin", user->GetLogin());
						indexPage->RegisterVariableForce("myFirstName", user->GetName());
						indexPage->RegisterVariableForce("myLastName", user->GetNameLast());
						indexPage->RegisterVariableForce("myUserAvatar", user->GetAvatar());
						indexPage->RegisterVariableForce("user_type", user->GetType());
						indexPage->RegisterVariableForce("smartway_enrolled", user->GetSmartwayEnrolled());
*/
					if(__LoadUserAndSetVariables(indexPage->SessID_Get_UserFromDB(), indexPage, db, user).empty())
					{
						if(user->GetLogin() != "Guest")
						{
							// --- actions specific to registered user
							indexPage->RegisterVariableForce("site_theme", user->GetSiteTheme());
						}

						MESSAGE_DEBUG("", action, "user (" + user->GetLogin() + ") logged in");
					}
					else
					{
						// --- enforce to close session
						action = "logout";

						MESSAGE_ERROR("", action, "user [" + indexPage->SessID_Get_UserFromDB() + "] not found. Change the [action = logout].");
					}
				}
				else
				{
					MESSAGE_ERROR("", action, "update session in HTTP or DB failed");
				}
			}
			else
			{
				MESSAGE_ERROR("", action, "session consistency check failed");
			}
		}
		else
		{
			MESSAGE_DEBUG("", "", "cookie session and DB session is not equal. Need to recreate session")

			if(!indexPage->Cookie_Expire()) MESSAGE_ERROR("", action, "fail to expire session");

			indexPage->Redirect("/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10));
		} // --- if(indexPage->SessID_Load_FromDB(sessidHTTP))
	} // --- if(sessidHTTP.length() < 5)

	// --- check default action if needed
	if(action.empty())
	{
		if(user)
		{
			action = GetDefaultActionFromUserType(user, db);

			MESSAGE_DEBUG("", "", "META-registration: action has been overridden user.type(" + user->GetType() + ") default action [action = " + action + "]");

			if(user->GetLogin().empty()) MESSAGE_ERROR("", "", "user login is empty (user login must not be empty, either \"Guest\", or actual user login)");
		}
		else
		{
			MESSAGE_ERROR("", "", "user is NULL");
		}
	}

	MESSAGE_DEBUG("", "", "finish (action = " + action + ")");

	return action;
}

auto 		LogoutIfGuest(string action, CCgi *indexPage, CMysql *db, CUser *user) -> string
{
	MESSAGE_DEBUG("", "", "start");

	if(user->GetLogin() == "Guest")
	{
		if(action.compare(0, 5, "AJAX_") == 0)
		{
			auto	template_name = "json_response.htmlt"s;

			indexPage->RegisterVariableForce("result", "{\"result\":\"error\",\"description\":\""s + gettext("re-login required") + "\"}");

			if(!indexPage->SetTemplate(template_name))
			{
				MESSAGE_ERROR("", "", "can't find template " + template_name);
			}
		}
		else
		{
			// --- eventually it will throw exception (may need to be reworked)
			indexPage->Redirect("/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10));
		}

		action = GUEST_USER_DEFAULT_ACTION;
		MESSAGE_DEBUG("", "", "re-login required, reset action");
	}

	MESSAGE_DEBUG("", "", "finish");

	return action;

}

auto AJAX_ResponseTemplate(CCgi *indexPage, const string &success_message, const string &error_message) -> string
{
	vector<pair<string, string>> error_messages;

	if(error_message.length()) error_messages.push_back(make_pair("description", error_message));

	return AJAX_ResponseTemplate(indexPage, success_message, error_messages);
}

auto AJAX_ResponseTemplate(CCgi *indexPage, const string &success_message, const vector<pair<string, string>> &error_messages) -> string
{
	MESSAGE_DEBUG("", "", "start");

	auto	template_name = "json_response.htmlt"s;
	auto	result = ""s;

	if(error_messages.size())
	{
		auto	error_message = ""s;

		for(const auto &temp: error_messages)
		{
			error_message += ",\"" + RemoveQuotas(temp.first) + "\":\"" + RemoveQuotas(temp.second) + "\"";
		}

		MESSAGE_DEBUG("", "", error_message);
		result = "{\"result\":\"error\"" + error_message + "}";
	}
	else
	{
		result = "{\"result\":\"success\"" + (success_message.length() ? "," + success_message : "") + "}";
	}

	indexPage->RegisterVariableForce("result", result);

	if(!indexPage->SetTemplate(template_name))
	{
		MESSAGE_DEBUG("", "", "can't find template " + template_name);
	}

	MESSAGE_DEBUG("", "", "finish");

	return ""s;
}
