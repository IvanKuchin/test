#include "chat-server-protocol-status.h"

extern	struct per_session_data__message	*connectionsList;
extern	CPresenceCache						presenceCache;

string			statusResult;
char			server_info[1024];

string ClearConnectionID(long int cnxID)
{
	struct per_session_data__message *connectionIterator = connectionsList;
	string result = "\"status\":\"fail\",\"description\":\"connection not found\"";

	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << "ClearConnectionID(" << hex << cnxID << "): start";
		log.Write(DEBUG, ost.str());
	}

	while(connectionIterator)
	{
		if(connectionIterator->wsi == (struct lws *)cnxID)
		{

			connectionIterator->closeFlag = true;
			result = "\"status\":\"ok\"";

			{
				CLog	log(CHAT_LOG_FILE_NAME);
				ostringstream	ost;
	
				ost.str("");
				ost << "ClearConnectionID(" << hex << cnxID << "): signal to connection to close";
				log.Write(DEBUG, ost.str());
			}

			lws_callback_on_writable(connectionIterator->wsi);
			break;
		}
		connectionIterator = connectionIterator->nextConnection;
	}


	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << "ClearConnectionID(" << hex << cnxID << "): end";
		log.Write(DEBUG, ost.str());
	}
	
	return result;
}

string GetConnectionStatus()
{
	struct per_session_data__message *connectionIterator = connectionsList;
	int				i = 0;
	ostringstream	ostResult;

	{
		CLog	log(CHAT_LOG_FILE_NAME);
		log.Write(DEBUG, "GetConnectionStatus: start");
	}

	ostResult.str("");
	ostResult << server_info << ", \"connectionList\": [";

	i=0;
	while(connectionIterator)
	{
		char 			date[128];
		time_t			t;
		struct tm 		*ptm;
		struct tm 		tm;

		auto			first_name = ""s;
		auto			last_name = ""s;

		memset(date, 0, sizeof(date));
		t = connectionIterator->tv_established.tv_sec;
		ptm = &tm;
		if (!localtime_r(&t, &tm))
			strcpy(date, "unknown");
		else
			strftime(date, sizeof(date), "%F %H:%M %Z", ptm);

		if(connectionIterator->user)
		{
			first_name = connectionIterator->user->GetName();
			last_name = connectionIterator->user->GetNameLast();
		}
		else
		{
			CLog	log(CHAT_LOG_FILE_NAME);
			log.Write(ERROR, "GetConnectionStatus: ERROR: stuck connection without user assigned, drop it ...");
		}

		ostResult << (i ? "," : "") << "{\"peer\":\"" << connectionIterator->ip << "\",\"time\":\"" << date << "\"," <<
				"\"wsi\":\"" << connectionIterator->wsi << "\", " << 
				"\"ua\":\"" << connectionIterator->user_agent << "\", " << 
				"\"userID\": \"" << (connectionIterator->user ? connectionIterator->user->GetID() : "") << "\"," << 
				"\"userName\": \"" << first_name << "\"," <<
				"\"userNameLast\": \"" << last_name << "\"" << "}";

		connectionIterator = connectionIterator->nextConnection;
		i++;
	}
	ostResult << "]";

	{
		CLog	log(CHAT_LOG_FILE_NAME);
		log.Write(DEBUG, "GetConnectionStatus: status [", ostResult.str(), "]");
	}
	{
		CLog	log(CHAT_LOG_FILE_NAME);
		log.Write(DEBUG, "GetConnectionStatus: end");
	}

	return	ostResult.str();
}

string GetPresenceCache()
{
	string result;
	{
		CLog	log(CHAT_LOG_FILE_NAME);
		log.Write(DEBUG, "GetPresenceCache: begin");
	}

	result = server_info + (string)(", \"presenceCache\": ") + presenceCache.DumpCache();

	{
		CLog			log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << "GetPresenceCache: end (result.length = " << result.length() << ")";
		log.Write(DEBUG, ost.str());
	}

	return result;
}


/*static void
update_status(struct lws *wsi, struct per_session_data__lws_status *pss)
{
	struct per_session_data__lws_status **pp = &list;
	int subsequent = 0;
	char *p = cache + LWS_PRE, *start = p;
	char date[128];
	time_t t;
	struct tm *ptm;
	struct tm tm;

	p += snprintf(p, 512, " { %s, \"wsi\":\"%d\", \"conns\":[",
		     server_info, live_wsi);

	while (*pp) {

		t = (*pp)->tv_established.tv_sec;
		ptm = &tm;
		if (!localtime_r(&t, &tm))
			strcpy(date, "unknown");
		else
			strftime(date, sizeof(date), "%F %H:%M %Z", ptm);

		if ((p - start) > (signed int)(sizeof(cache) - 512))
			break;
		if (subsequent)
			*p++ = ',';
		subsequent = 1;
		p += snprintf(p, sizeof(cache) - (p - start) - 1,
				"{\"peer\":\"%s\",\"time\":\"%s\","
				"\"ua\":\"%s\"}",
			     (*pp)->ip, date, (*pp)->user_agent);
		pp = &((*pp)->list);
	}

	p += sprintf(p, "]}");
	cache_len = p - start;
	lwsl_err("cache_len %d\n", cache_len);
	*p = '\0';

	current++; // current version
	lws_callback_on_writable_all_protocol(lws_get_context(wsi),
					      lws_get_protocol(wsi));
}
*/

/* lws-status protocol */

int
callback_lws_status(struct lws *wsi, enum lws_callback_reasons reason,
		    void *user, void *in, size_t len)
{
	struct per_session_data__lws_status *pss = (struct per_session_data__lws_status *)user;

	switch (reason) {

	case LWS_CALLBACK_PROTOCOL_INIT:
		/*
		 * Prepare the static server info
		 */
		snprintf((char *)server_info, sizeof(server_info),
					  "\"version\":\"%s\","
					  " \"hostname\":\"%s\"",
					  lws_get_library_version(),
					  lws_canonical_hostname(
							lws_get_context(wsi)));
		break;

	case LWS_CALLBACK_ESTABLISHED:
		
		break;

	case LWS_CALLBACK_SERVER_WRITEABLE:
		{
			CLog	log(CHAT_LOG_FILE_NAME);
			log.Write(DEBUG, "callback_lws_status: LWS_CALLBACK_SERVER_WRITEABLE: start");
		}

		if(statusResult.length())
		{
			char			*cache;
			unsigned int	m;
			unique_ptr<char[]>	tempSmartPointer(new char[statusResult.length() + 2 * LWS_PRE + 1]);

			{
				CLog	log(CHAT_LOG_FILE_NAME);
				ostringstream	ost;

				ost.str("");
				ost << "callback_lws_status: LWS_CALLBACK_SERVER_WRITEABLE: strResult.length() = " << statusResult.length();
				log.Write(DEBUG, ost.str());
			}
			// cache = (unsigned char *)malloc(statusResult.length() + 2 * LWS_PRE);
			cache = tempSmartPointer.get();
			memset(cache, 0, statusResult.length() + 2 * LWS_PRE + 1);
			memcpy(cache + LWS_PRE, statusResult.c_str(), statusResult.length());

			m = lws_write(wsi, (unsigned char *)cache + LWS_PRE, statusResult.length(),
				      LWS_WRITE_TEXT);
			// free(cache);
			tempSmartPointer.reset(nullptr);
			statusResult = "";

			if (m < statusResult.length()) {
				{
					CLog	log(CHAT_LOG_FILE_NAME);
					log.Write(ERROR, "callback_lws_status: LWS_CALLBACK_SERVER_WRITEABLE: ERROR: writing to di socket");
				}
				lwsl_err("callback_lws_status: ERROR %d writing to di socket\n", m);
				return -1;
			}

			{
				CLog	log(CHAT_LOG_FILE_NAME);
				ostringstream	ost;

				ost.str("");
				ost << "callback_lws_status: LWS_CALLBACK_SERVER_WRITEABLE: written " << m << " bytes.";
				log.Write(DEBUG, ost.str());
			}
		}

		{
			CLog	log(CHAT_LOG_FILE_NAME);
			log.Write(DEBUG, "callback_lws_status: LWS_CALLBACK_SERVER_WRITEABLE: end");
		}
		break;

	case LWS_CALLBACK_RECEIVE:
		{
			CLog	log(CHAT_LOG_FILE_NAME);
			log.Write(DEBUG, "callback_lws_status: LWS_CALLBACK_RECEIVE: begin");
		}
		{
			CLog	log(CHAT_LOG_FILE_NAME);
			ostringstream	ost;
			ost.str("");
			ost << "callback_lws_status: LWS_CALLBACK_RECEIVE: begin (" << (char *)in << ")";
			log.Write(DEBUG, ost.str());
		}

		{
			string	requestFromSocket = (const char *)in;

			if (requestFromSocket.compare(0, 10, "clear_cnx_") == 0)
			{
				{
					CLog	log(CHAT_LOG_FILE_NAME);
					log.Write(DEBUG, "callback_lws_status: LWS_CALLBACK_RECEIVE: clear_cnx (", requestFromSocket.substr(10, requestFromSocket.length() - 10), "): begin");
				}

				statusResult = ClearConnectionID(stoll(requestFromSocket.substr(10, requestFromSocket.length() - 10), 0, 16));
				if(statusResult.length())
				{
					statusResult = "{\"type\": \"statusResponse\", " + statusResult + "}";
					lws_callback_on_writable_all_protocol(lws_get_context(wsi),
							      lws_get_protocol(wsi));
				}

				{
					CLog	log(CHAT_LOG_FILE_NAME);
					log.Write(DEBUG, "callback_lws_status: LWS_CALLBACK_RECEIVE: clear_cnx (", requestFromSocket.substr(10, requestFromSocket.length() - 10), "): end");
				}
			} // --- if "clear_cnx_"
			if (strcmp((const char *)in, "get_cnx_list") == 0)
			{
				{
					CLog	log(CHAT_LOG_FILE_NAME);
					log.Write(DEBUG, "callback_lws_status: LWS_CALLBACK_RECEIVE: get_cnx_list: begin");
				}

				statusResult = GetConnectionStatus();
				if(statusResult.length())
				{
					statusResult = "{\"type\": \"statusResponse\", " + statusResult + "}";
					lws_callback_on_writable_all_protocol(lws_get_context(wsi),
							      lws_get_protocol(wsi));
				}

				{
					CLog	log(CHAT_LOG_FILE_NAME);
					log.Write(DEBUG, "callback_lws_status: LWS_CALLBACK_RECEIVE: get_cnx_list: end");
				}
			} // --- if "get_cnx_list"
			if (strcmp((const char *)in, "dump_presence_cache") == 0)
			{
				{
					CLog	log(CHAT_LOG_FILE_NAME);
					log.Write(DEBUG, "callback_lws_status: LWS_CALLBACK_RECEIVE: dump_presence_cache: begin");
				}

				statusResult = GetPresenceCache();
				if(statusResult.length())
				{
					statusResult = "{\"type\": \"presenceCacheDumpResponse\", " + statusResult + "}";
					lws_callback_on_writable(wsi);
				}

				{
					CLog	log(CHAT_LOG_FILE_NAME);
					log.Write(DEBUG, "callback_lws_status: LWS_CALLBACK_RECEIVE: dump_presence_cache: end");
				}
			} // --- if "dump_presence_cache"
		} // ---  scoping on receiving from socket

		{
			CLog	log(CHAT_LOG_FILE_NAME);
			log.Write(DEBUG, "callback_lws_status: LWS_CALLBACK_RECEIVE: end");
		}
		break;

	case LWS_CALLBACK_CLOSED:
		/*
		 * remove ourselves from live pss list
		 */
		lwsl_err("status: CLOSING pss %p ********\n", pss);

		// update_status(wsi, pss);
		break;

	case LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION:
		dump_handshake_info(wsi);
		/* you could return non-zero here and kill the connection */
		break;

	default:
		break;
	}

	return 0;
}
