#include "csession.h"

CSession::CSession() : db(NULL), cookies(NULL)
{
	struct	timeval	tv;

	MESSAGE_DEBUG("", "", "start");

	gettimeofday(&tv, NULL);
	srand(tv.tv_sec * tv.tv_usec * 100000);

	sessID = GetRandom(SESSION_LEN);

	MMDB_usage = false;

	MESSAGE_DEBUG("", "", "end");
}

void CSession::InitMaxMind()
{
	MESSAGE_DEBUG("", "", "start");

	MMDB_usage = false;

#ifndef MAXMIND_DISABLE
	if(getenv("REMOTE_ADDR"))
	{
		int		status = MMDB_open(MMDB_fname, MMDB_MODE_MMAP, &mmdb);

	    if (status == MMDB_SUCCESS)
	    {
	    	int gai_error, mmdb_error;

			MESSAGE_DEBUG("", "", "MMDB_open(" + MMDB_fname + ") opened successfully. ");

		    MMDB_result = MMDB_lookup_string(&mmdb, getenv("REMOTE_ADDR"), &gai_error, &mmdb_error);

		    if (0 == gai_error) 
		    {
				if (mmdb_error == MMDB_SUCCESS) 
				{
					MMDB_entry_data_list = NULL;

					if (MMDB_result.found_entry) 
					{
					    int status = MMDB_get_entry_data_list(&MMDB_result.entry, &MMDB_entry_data_list);

					    if (status == MMDB_SUCCESS) 
					    {
							MMDB_usage = true;
					    }
					    else
					    {
							MESSAGE_ERROR("", "", "ERROR: MMDB_open(" + MMDB_fname + ") : Got an error looking up the entry data -" + MMDB_strerror(status));

							MMDB_close(&mmdb);
					    }
					}
					else
					{
						MMDB_usage = true;
					}
				}
				else
				{
					MESSAGE_ERROR("", "", "ERROR: MMDB_open(" + MMDB_fname + ") Got an error from libmaxminddb: " + MMDB_strerror(mmdb_error));

					MMDB_close(&mmdb);
				}
		    }
		    else
		    {
				MESSAGE_ERROR("", "", "ERROR: MMDB_open(" + MMDB_fname + ") Error from getaddrinfo for " + getenv("REMOTE_ADDR") + ", " + gai_strerror(gai_error) + ". ");

				MMDB_close(&mmdb);
		    }
	    }
	    else
	    {
			MESSAGE_ERROR("", "", "ERROR: in reading GeoInfo DB MMDB_open(" + MMDB_fname + ") error is: " + MMDB_strerror(status) + ". ");

	        if (MMDB_IO_ERROR == status)
	        {
				MESSAGE_ERROR("", "", "ERROR: MMDB_open(" + MMDB_fname + ") returned MMDB_IO_ERROR: " + strerror(status) + ". ");
	        }
	    }

	}
	else
	{
		MESSAGE_DEBUG("", "", "REMOTE_ADDR is empty, no way to determine remote IP");
	}
#endif

	MESSAGE_DEBUG("", "", "finish (MMDB_usage = " + to_string(MMDB_usage) + ")");
}

string CSession::DetectItem(string MMDB_itemName) 
{
	string		item = "";

#ifndef MAXMIND_DISABLE
    MESSAGE_DEBUG("", "", "start (detectItem " + MMDB_itemName + " by IP)");

	if(MMDB_usage)
	{
        MMDB_entry_data_s       MMDB_entryDataS;
        auto					error_code = MMDB_get_value(&MMDB_result.entry, &MMDB_entryDataS, MMDB_itemName.c_str(), "names", "en", NULL);

		if(error_code == MMDB_SUCCESS)
		{
			if(MMDB_entryDataS.has_data && (MMDB_entryDataS.type == MMDB_DATA_TYPE_UTF8_STRING))
			{
				char	buffer[1024];

				if(MMDB_entryDataS.data_size < sizeof(buffer))
				{
				    memcpy(buffer, MMDB_entryDataS.utf8_string, MMDB_entryDataS.data_size);
				    buffer[MMDB_entryDataS.data_size] = 0;
				    item = buffer;
				}
				else
				{
				    MESSAGE_ERROR("", "", "mmdb result is too long");
				}
			}
			else
			{
				MESSAGE_DEBUG("", "", "MMDB_entry has no data");
			}
		}
		else
		{
			if((error_code == 9) && (MMDB_itemName == "city"))
			{
				auto remote_addr = getenv("REMOTE_ADDR") ? getenv("REMOTE_ADDR") : ""s;

				MESSAGE_DEBUG("", "", "MMDB_get_value(" + MMDB_itemName + ") returned error code: " + to_string(error_code) + " city name not found in MMDB (use mmdblookup --ip " + remote_addr + " --file " + MMDB_fname + " to confirm that city name missed). Here is the MMDB library error message (" + MMDB_strerror(error_code) + ")");
			}
			else if((error_code == 9) && (MMDB_itemName == "country"))
			{
				auto remote_addr = getenv("REMOTE_ADDR") ? getenv("REMOTE_ADDR") : ""s;

				MESSAGE_DEBUG("", "", "MMDB_get_value(" + MMDB_itemName + ") returned error code: " + to_string(error_code) + " country name not found in MMDB (use mmdblookup --ip " + remote_addr + " --file " + MMDB_fname + " to confirm that country name missed). Here is the MMDB library error message (" + MMDB_strerror(error_code) + ")");
			}
			else
			{
				MESSAGE_ERROR("", "", "MMDB_get_value(" + MMDB_itemName + ") returned error code: " + to_string(error_code) + " (" + MMDB_strerror(error_code) + ")");
			}
		}
	}

    MESSAGE_DEBUG("", "", "finish (" + item + ")");
#endif
    
	return	item;
}

auto CSession::DetectCountry() -> string
{
	return DetectItem("country");
}

auto CSession::DetectCity() -> string
{
	return DetectItem("city");
}

bool CSession::Save(string u, string i, string l)
{
	if(!db)
	{
		MESSAGE_ERROR("", "", "ERROR: connect to database in CSession module");

		throw CExceptionHTML("error db");
	}

	if(db->Query("SELECT `id` FROM `users` WHERE `login`=" + quoted(u) + ";"))
	{
		SetUserID(db->Get(0, 0));
	}
	else
	{
		MESSAGE_ERROR("", "", " user ID must be set");

		throw CExceptionHTML("session error");
	}

	SetUser(u);
	SetIP(i);
	SetLng(l);
	// --- try to detect IP address on new session
	InitMaxMind();
	return Save();
}

bool CSession::Save()
{
	auto	result = false;

	if(!db)
	{
		MESSAGE_ERROR("", "", "ERROR: connect to database in CSession module");

		throw CExceptionHTML("error db");
	}
	if(GetUserID().empty())
	{
		MESSAGE_ERROR("", "", " user ID must be set");

		throw CExceptionHTML("session error");
	}
	if(GetID().empty())
	{
		MESSAGE_ERROR("", "", " id must be set");

		throw CExceptionHTML("session error");
	}
	if(GetIP().empty())
	{
		MESSAGE_ERROR("", "", " ip must be set");

		throw CExceptionHTML("session error");
	}
	if(GetLng().empty())
	{
		MESSAGE_ERROR("", "", " lng must be set");

		throw CExceptionHTML("session error");
	}

	if(db->Query("INSERT INTO `sessions` (`id`, `user_id`, `country_auto`, `city_auto`, `lng`, `ip`, `time`,`expire` ) VALUES "
				"(" + quoted(GetID()) + ", " + quoted(GetUserID()) + ", " + quoted(DetectCountry()) + ", " + quoted(DetectCity()) + ", " + quoted(GetLng()) + ", " + quoted(GetIP()) + ", NOW(), " + quoted(to_string(SESSION_LEN * 60)) + ");") != 0)
	{
		MESSAGE_ERROR("", "", "ERROR: in insert SQL-query");
	}
	else
	{
		result = true;
	}

	return result;
}



bool CSession::Load(string id)
{
	ostringstream	ost;
	string			currIP;
	bool			result = true;

	MESSAGE_DEBUG("", "", "start");

	if(id.find(",") != string::npos)
	{
		string		ip = (getenv("REMOTE_ADDR") ? getenv("REMOTE_ADDR") : "");

		if(isBotIP(ip))
		{
			MESSAGE_DEBUG("", "", "wrong cookie behavior from search engine bot [" + ip + "]");
		}
		else
		{
			MESSAGE_ERROR("", "", "ERROR: \"sessid\" got the wrong value WITH COMMA (" + id + "). 1-st reason - bot, check IP in whois DB, 2-nd reason - bad browser, such a behavior has seen on SAMSUNG J3 [" + (getenv("HTTP_USER_AGENT") ? getenv("HTTP_USER_AGENT") : "") + "]");
		}
	}

	if(isExist(id))
	{
		// --- DB should not return NULL value
		// --- all fields must have "not null"-attribute
		SetID(id);
		SetIP(db->Get(0, "ip"));
		SetLng(db->Get(0, "lng"));
		SetExpire(stoi(db->Get(0, "expire")));
		SetUserID(db->Get(0, "user_id"));

		if(db->Query("SELECT `login` FROM `users` WHERE `id`=" + quoted(GetUserID()) + ";"))
			SetUser(db->Get(0, 0));
		else
		{
			MESSAGE_ERROR("", "", "user id not found");
		}

		MESSAGE_DEBUG("", "", "session loaded for user [" + GetUser() + "]");
	}
	else
	{
		MESSAGE_DEBUG("", "", "there is no session in DB, session hasn't been loaded from DB");

		result = false;
	}


	MESSAGE_DEBUG("", "", "result (" + to_string(result) + ")");

	return result;
}

bool CSession::CheckConsistency() {
	bool		result = true;

	{
		MESSAGE_DEBUG("", "", "start");
	}
	// --- Check IP address changing
	if(getenv("REMOTE_ADDR"))
	{
		string	currIP = getenv("REMOTE_ADDR");
		string	oldIP = GetIP();

		if(currIP != oldIP)
		{
			if(UpdateIP(currIP))
			{
				MESSAGE_DEBUG("", "", "user (" + GetUser() + ") IP was changed (" + oldIP + " -> " + currIP + ") during the session.");
			}
			else
			{
				MESSAGE_ERROR("", "", "ERROR: fail to update IP (" + oldIP + " -> " + currIP + ")");
			}
		}
	}
	else
	{
		MESSAGE_DEBUG("", "", "environment variable 'REMOTE_ADDR' doesn't defined");
	}

	{
		MESSAGE_DEBUG("", "", "end (result: " + (result ? "true" : "false") + ")");
	}
	return result;
}

bool CSession::Update() {

	if(GetUser() == "")
	{
		MESSAGE_ERROR("", "", "ERROR: session [" + GetID() + "], user is empty");

		return false;
	}

	if(GetID() == "")
	{
		MESSAGE_ERROR("", "", "ERROR: sessionID is empty");

		return false;
	}

	if(db->Query("UPDATE `sessions` SET `time`=NOW(),`http_user_agent`=\""s + (getenv("HTTP_USER_AGENT") ? RemoveQuotas(getenv("HTTP_USER_AGENT")) : "") + "\" WHERE `id`=\"" + GetID() + "\";")) 
	{
		MESSAGE_ERROR("", "", "ERROR: session [" + GetID() + "] there are more than one session with the same session ID.");

		return false;
	}

	return true;
}

bool CSession::UpdateIP(string newIP) {
	ostringstream	ost;

	if(GetUser() == "")
	{
		MESSAGE_ERROR("", "", "ERROR: session [" + GetID() + "], user is empty");

		return false;
	}

	if(GetID() == "")
	{
		MESSAGE_ERROR("", "", "ERROR: session ID is empty");

		return false;
	}

	// --- try to detect IP address on session with changed IP-address
	// --- call stack here
	// --- CCgi::SessID_CheckConsistency
	// --- CSession::CheckConsistency
	// --- CSession::UpdateDB (here)
	InitMaxMind();

	if(db->Query("UPDATE `sessions` SET `ip`=\"" + newIP + "\", `country_auto`=\"" + DetectCountry() + "\", `city_auto`=\"" + DetectCity() + "\" WHERE `id`=\"" + GetID() + "\";"))
	{
		MESSAGE_ERROR("", "", "ERROR: session [" + GetID() + "] there are more than one session with the same session ID.");

		return false;
	}
	SetIP(newIP);

	return true;
}

bool CSession::isExist(string id)
{
	ostringstream	ost;
	string		currIP;

	MESSAGE_DEBUG("", "", "start");

	if(!db)
	{
		MESSAGE_ERROR("", "", "db not initialized");

		throw CExceptionHTML("error db");
	}
	if(id.empty())
	{
		MESSAGE_ERROR("", "", "id is empty");

		throw CExceptionHTML("activator error");
	}

	db->Query("DELETE FROM `sessions` WHERE `time`<=(NOW()-INTERVAL `expire` SECOND) AND `expire`>'0'");
	if(db->Query("select * from `sessions` where `id`=\"" + id + "\";"))
	{
	}
	else
	{
		MESSAGE_DEBUG("", "", "finish (false)");

		return false;
	}

	MESSAGE_DEBUG("", "", "finish (true)");

	return true;
}

CSession::~CSession()
{
#ifndef MAXMIND_DISABLE
	MESSAGE_DEBUG("", "", "start");

	if(MMDB_usage) {
		MMDB_close(&mmdb);
	}

	MESSAGE_DEBUG("", "", "finish");
#endif
}




 