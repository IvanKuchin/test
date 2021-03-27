#include "c_smsc.h"

//==============================================================================
struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *) userp;

    MESSAGE_DEBUG("", "", "curl received " + to_string(realsize) + " bytes of content");

    mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
//    printf("%d", mem->memory);
    if (mem->memory == NULL)
    {
		/* out of memory! */ 
		MESSAGE_ERROR("", "", "not enough memory (realloc returned NULL)")
		return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}
//==============================================================================

string c_smsc::send_sms (string phones, string message, int translit, string time, int id, int format, string sender, string query, string files) 
{
    auto    res=""s, arg=""s;
    auto    error_message = ""s;
    
    char formats[][10] = {"&flash=1", "&push=1", "&hlr=1", "&bin=1", "&bin=2", "&ping=1", "&mms=1", "&mail=1", "&call=1", "&viber=1"};
    
	MESSAGE_DEBUG("", "", "start");

    arg =	"cost=3"
    		"&phones="s + urlencode(phones) + 
    		(message.length() ? "&mes=" + urlencode(message + "\n" + gettext("Sent by") + " " + DOMAIN_NAME) : "") + 
    		"&translit=" + to_string(translit) + 
    		"&id=" + to_string(id) + 
    		(format > 0 ? formats[format - 1] : "") + 
			(sender.length() ? "&sender=" + urlencode(sender.substr(0, 11)): "") +
			(time.length() ? "&time=" + urlencode(time) : "") + 
			(query.length() ? "&" + query : "");

	MESSAGE_DEBUG("", "", "built arg: " + arg);

    res = _smsc_send_cmd("send", arg, files);

    if(res.length())
    {
    	auto	report = split(res, ',');

    	if(report.size() == 4)
    	{
    		auto error_message_scoped = TrackSMS(phones, report[0], report[2], report[1], message, report[3]);
	    	if(error_message_scoped.empty())
	    	{
	    	}
	    	else
	    	{
	    		MESSAGE_ERROR("", "", "fail to report about SMS sending");
	    	}
    	}
    	else
    	{
            error_message = gettext("sms sending error");
            MESSAGE_ERROR("", "", "report.size() must be 4, (" + res + "). Probably sms has _NOT_ been sent.");
    	}
    }
    else
    {
        error_message = gettext("sms sending error");
    	MESSAGE_ERROR("", "", "sms to pnohe# " + phones + " has not been sent");
    }

	MESSAGE_DEBUG("", "", "finish (" + error_message + ")");

    return error_message;
}

string c_smsc::get_sms_cost(string phones, string mes, int translit, int format, string sender, string query) {
    auto res = ""s, arg = ""s;

    char formats[][10] = {"&flash=1", "&push=1", "&hlr=1", "&bin=1", "&bin=2", "&ping=1", "&mms=1", "&mail=1", "&call=1", "&viber=1"};

	MESSAGE_DEBUG("", "", "start");

    arg =	"cost=1"
    		"&phones="s + urlencode(phones) + 
    		(mes.length() ? "&mes=" + urlencode(mes) : "") + 
    		"&translit=" + to_string(translit) + 
    		(format > 0 ? formats[format - 1] : "") + 
			(sender.length() ? "&sender=" + urlencode(sender): "") +
			(query.length() ? "&query=" + urlencode(query) : "");

	MESSAGE_DEBUG("", "", "built arg: " + arg);

    res = _smsc_send_cmd("send", arg, "");

    if(res.length())
    {
    	auto	report = split(res, ',');

    	if(report.size() == 4)
    	{
    		auto error_message = TrackSMS(phones, report[0], report[2], report[1], "cost query", report[3]);
	    	if(error_message.empty())
	    	{
	    	}
	    	else
	    	{
	    		MESSAGE_ERROR("", "", "fail to report about SMS sending");
	    	}
    	}
    	else
    	{
    		MESSAGE_ERROR("", "", "report.size() must be 4, (" + res + ")");
    	}
    }
    else
    {
    	MESSAGE_ERROR("", "", "sms to pnohe# " + phones + " has not been sent");
    }

	MESSAGE_DEBUG("", "", "finish");

    return res;
}

string c_smsc::TrackSMS(string phones, string sms_id, string sms_cost, string sms_quantity, string sms_text, string current_balance)
{
	auto	error_message = ""s;

	MESSAGE_DEBUG("", "", "start");

	if(db)
	{
		if(db->InsertQuery("INSERT INTO `sms_report` (`number`, `sms_id`, `sms_cost`, `sms_quantity`, `sms_text`, `current_balance`, `eventTimestamp`)"
							" VALUES ("
							"\"" + phones + "\","
							"\"" + sms_id + "\","
							"\"" + sms_cost + "\","
							"\"" + sms_quantity + "\","
							"\"" + sms_text + "\","
							"\"" + current_balance + "\","
							"NOW()"
							")"
							))
		{
		}
		else
		{
			MESSAGE_ERROR("", "", "fail to insert to db");
			error_message = gettext("fail to insert to db");
		}
	}
	else
	{
		MESSAGE_ERROR("", "", "db is not initialized");
		error_message = gettext("db is not initialized");
	}
	
	MESSAGE_DEBUG("", "", "finish");

	return error_message;
}

string c_smsc::get_balance(void)
{
    return _smsc_send_cmd("balance", "", ""); // (balance) или (0, -error)
}

string c_smsc::get_status(string id, string phone, int all)
{
    return _smsc_send_cmd("status", "phone=" + urlencode(phone) + "&id=" + urlencode(id) + "&all=" + to_string(all), NULL);
}


string c_smsc::_smsc_send_cmd(string cmd, string arg, string files) 
{
    MESSAGE_DEBUG("", "", "start");

    c_config            config;
    auto                auth_map        = config.Read({"SMSC_LOGIN", "SMSC_PASSWORD"});
    auto                is_auth_valid   = (auth_map["SMSC_LOGIN"].length() > 0);
    auto                __SMSC_LOGIN    = (is_auth_valid ? auth_map["SMSC_LOGIN"] : "");
    auto                __SMSC_PASSWORD = (is_auth_valid ? auth_map["SMSC_PASSWORD"] : "");

    auto                result = ""s;

    if(is_auth_valid)
    {
        CURL                    *curl;
        CURLcode                res;
        // struct curl_slist *list = NULL;
        struct curl_httppost*    cpost = NULL;
        struct curl_httppost*    clast = NULL;
        
        auto                    _arg = ""s, url = ""s;
        char                    post, i=0;

        struct MemoryStruct chunk;
        chunk.memory = (char *)malloc(1); // will be grown as needed by the realloc above
        chunk.memory[0] = 0;
        chunk.size = 0; // no data at this point  

        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();
        if (curl) 
        {
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);
            _arg = "login=" + urlencode(__SMSC_LOGIN) + "&psw=" + urlencode(__SMSC_PASSWORD) + "&fmt=1&charset=" + SMSC_CHARSET + "&" + arg;
        
            post = __SMSC_POST || files.length() || (_arg.length() > 2000);
            do {
                url = "smsc.ru/sys/" + cmd + ".php";

                if (i++)
                    url = (__SMSC_HTTPS ? "https"s : "http"s) + "://www" + to_string(i) + "." + url;
                else
                    url = (__SMSC_HTTPS ? "https"s : "http"s) + "://" + url;

                if (post) 
                {
                    MESSAGE_DEBUG("", "", "sending HTTP POST-request (NOT TESTED)");
                    // разбираем строку параметров

                    auto current_pos = _arg.find_first_of("=&"); // --- init only for type deduction
                    current_pos = 0;
                    auto token_pos = _arg.find_first_of("=&", current_pos);

                    while (token_pos != string::npos) 
                    {
                        if(_arg.find_first_of("=&", token_pos + 1) != string::npos)
                        {
                            auto     dstr = _arg.substr(current_pos + 1, token_pos - current_pos);
                            auto     istr = _arg.substr(token_pos + 1, _arg.find_first_of("=&", token_pos + 1) - token_pos);

                            MESSAGE_DEBUG("", "", dstr + "=" + istr);

                            curl_formadd(&cpost, &clast, CURLFORM_COPYNAME, dstr.c_str(), CURLFORM_COPYCONTENTS, urldecode(istr).c_str(), CURLFORM_END);

                        }
                        current_pos = token_pos + 1;
                        token_pos = _arg.find_first_of("=&", current_pos);
                    }

                    if (files.length())
                    {
                        MESSAGE_DEBUG("", "", "adding files to curl");

                        // --- ATTENTION
                        // --- no error check performed, add it in case using POST method
                        curl_formadd(&cpost, &clast, CURLFORM_COPYNAME, "pictures", CURLFORM_FILE, files.c_str(), CURLFORM_END);
                    }

                    curl_easy_setopt(curl, CURLOPT_HTTPPOST, cpost);
                }
                else
                {
                    MESSAGE_DEBUG("", "", "sending HTTP GET-request")
                    url = url + "?" + _arg;
                }
            

                MESSAGE_DEBUG("", "", "URL to be requested by curl (" + url + ")");

                curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
                res = curl_easy_perform(curl);
                if (res == CURLE_OK)
                {
                    result = chunk.memory;

                    MESSAGE_DEBUG("", "", to_string((long)chunk.size) + " bytes retrieved (result is " + result + ")");

                    free(chunk.memory);
                }
                else
                {
                    MESSAGE_DEBUG("", "", "curl_easy_perform() failed: (" + curl_easy_strerror(res) + ")");
                }

            } while ((i < 5) && (res != CURLE_OK));
            curl_easy_cleanup(curl);
        }
        else
        {
            MESSAGE_ERROR("", "", "fail to initialize curl")
        }
    }
    else
    {
        MESSAGE_ERROR("", "", gettext("service credentials not found"));
    }

    MESSAGE_DEBUG("", "", "finish (result is " + result + ")");

    return(result);
}

// кодирование параметра в http-запросе
string c_smsc::urlencode(string str) {
    auto output = ""s;
    CURL *curl;
    
    if (str.length()) 
    {
        curl = curl_easy_init();
        if (curl)
        {
            output = curl_easy_escape(curl, str.c_str(), 0);
            curl_easy_cleanup(curl);
        }
        else
        {
        	MESSAGE_ERROR("", "", "fail to initialize curl");
        }
    }
    else
    {
    	MESSAGE_DEBUG("", "", "nothing to encode")
    }

    return output;
}

// декодирование параметра в http-запросе
string c_smsc::urldecode(string str) {
    auto output = ""s;
    CURL *curl;
    
    if (str.length())
    {
        curl = curl_easy_init();
        if (curl)
        {
            output = curl_easy_unescape(curl, str.c_str(), 0, NULL);
            curl_easy_cleanup(curl);
        }
        else
        {
        	MESSAGE_ERROR("", "", "fail to initialize curl");
        }
    }
    else
    {
    	MESSAGE_DEBUG("", "", "nothing to decode")
    }

    return output;
}
