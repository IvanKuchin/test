#include "book.h"

int main()
{
	CStatistics		appStat;  // --- CStatistics must be a first statement to measure end2end param's
	CCgi			indexPage(EXTERNAL_TEMPLATE);
	CUser			user;
	c_config		config(CONFIG_DIR);
	string			action, partnerID;
	CMysql			db;
	struct timeval	tv;

	MESSAGE_DEBUG("", "", __FILE__);

	signal(SIGSEGV, crash_handler); 

	gettimeofday(&tv, NULL);
	srand(tv.tv_sec * tv.tv_usec * 100000);    /* Flawfinder: ignore */

	try
	{

	indexPage.ParseURL();

	if(!indexPage.SetTemplate("index.htmlt"))
	{
		CLog	log;

		log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:ERROR: template file was missing");
		throw CException("Template file was missing");
	}

	if(db.Connect(&config) < 0)
	{
		CLog	log;

		log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:ERROR: Can not connect to mysql database");
		throw CExceptionHTML("MySql connection");
	}

	indexPage.SetDB(&db);

	action = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("action"));

	MESSAGE_DEBUG("", "", "action taken from HTTP is " + action);

	// ------------ generate common parts
	{
		if(RegisterInitialVariables(&indexPage, &db, &user))
		{
		}
		else
		{
			MESSAGE_ERROR("", "", "RegisterInitialVariables failed, throwing exception");
			throw CExceptionHTML("environment variable error");
		}

		action = GenerateSession(action, &config, &indexPage, &db, &user);
	}
	// ------------ end generate common parts

	MESSAGE_DEBUG("", "", "pre-condition if(action == \"" + action + "\")");

	if((action == "JSON_getBookByISBN10") || (action == "JSON_getBookByISBN13"))
	{
		ostringstream   ostResult;
		string		  id;

		{
			CLog	log;
			log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: action == JSON_getBookByISBN10: start");
		}

		ostResult.str("");
		if(user.GetLogin() == "Guest")
		{
			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]:action == JSON_getBookByISBN10: re-login required");
			}

			ostResult << "{\"result\":\"error\",\"description\":\"сессия закончилась, необходимо вновь зайти на сайт\"}";
		}
		else
		{
			id = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("id"));
			id = SymbolReplace_KeepDigitsOnly(id);

			ostResult << "{" 
					  << "\"result\":\"success\","
					  << "\"books\":[" << GetBookListInJSONFormat("select * from `book` where `isbn10`=\"" + id + "\" or `isbn13`=\"" + id + "\" or `isbn13`=\"978" + id + "\";", &db) << "]"
					  << "}";
		} // --- if(user.GetLogin() == "Guest")


		indexPage.RegisterVariableForce("result", ostResult.str());

		if(!indexPage.SetTemplate("json_response.htmlt"))
		{
			CLog	log;

			log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: template file json_response.htmlt was missing");
			throw CException("Template file json_response.htmlt was missing");
		}  // if(!indexPage.SetTemplate("JSON_getBookByISBN10.htmlt"))

		{
			CLog	log;
			log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == JSON_getBookByISBN10: end");
		}
	}   // if(action == "JSON_getBookByISBN10")

	if(action == "AJAX_setBookRating")
	{
		ostringstream   ostResult;
		string		  id, bookID, rating;

		{
			CLog	log;
			log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: action == AJAX_setBookRating: start");
		}

		ostResult.str("");
		if(user.GetLogin() == "Guest")
		{
			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_setBookRating: re-login required");
			}

			ostResult << "{\"result\":\"error\",\"description\":\"сессия закончилась, необходимо вновь зайти на сайт\"}";
		}
		else
		{
			bool	isSuccess = false;

			try
			{
				id = to_string(stol(indexPage.GetVarsHandler()->Get("id")));
			}
			catch(...)
			{
				id = "";
				{
					CLog log;
					log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_setBookRating: can't convert id to number");
				}
			}

			try
			{
				bookID = to_string(stol(indexPage.GetVarsHandler()->Get("bookID")));
			}
			catch(...)
			{
				bookID = "";
				{
					CLog log;
					log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_setBookRating: can't convert bookID to number");
				}
			}

			try
			{
				int 	temp = stoi(indexPage.GetVarsHandler()->Get("rating"));
				if(temp <= 5) rating = to_string(temp); else rating = "0";
			}
			catch(...)
			{
				rating = "";
				{
					CLog log;
					log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_setBookRating:ERROR: fail to convert rating to number");
				}
			}

			if(id.length() && db.Query("select `id`, `bookID`, `rating` from `users_books` WHERE `userID`=\"" + user.GetID() + "\" and `id`=\"" + id + "\";"))
			{
				bookID = db.Get(0, "bookID");
				db.Query("UPDATE `users_books` SET `rating`=\"" + rating + "\" WHERE `id`=\"" + id + "\";");
				isSuccess = true;
			}
			else if(bookID.length() && db.Query("select `id`, `rating` from `users_books` WHERE `userID`=\"" + user.GetID() + "\" and `bookID`=\"" + bookID + "\";"))
			{
				id = db.Get(0, "id");

				db.Query("UPDATE `users_books` SET `rating`=\"" + rating + "\" WHERE `id`=\"" + id + "\";");
				isSuccess = true;
			}
			else
			{
				if(bookID.length())
				{
					id = to_string(db.InsertQuery("INSERT INTO `users_books` SET `userID`=\"" + user.GetID() + "\", `bookID`=\"" + bookID + "\", `rating`=\"" + rating + "\", `bookReadTimestamp`=UNIX_TIMESTAMP();"));
					if(id.length())
					{
						isSuccess = true;
					}
					else
					{
						CLog log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_setBookRating:ERROR: inserting  into `users_books` fail");

						ostResult << "{\"result\":\"error\",\"description\":\"ошибка: не указан bookID\"}";
					}

				}
				else
				{
					CLog log;
					log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_setBookRating:ERROR: bookID is not defined");

					ostResult << "{\"result\":\"error\",\"description\":\"ошибка: не указан bookID\"}";
				} // --- if(bookID.length())
			} // --- insert or update liked book

			if(isSuccess)
			{
				if(!db.Query("SELECT * FROM `feed` WHERE `userId`=\"" + user.GetID() + "\" and (`actionTypeId`=\"53\" or `actionTypeId`=\"54\") and `actionId`=\"" + id + "\""))
					db.InsertQuery("INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" + user.GetID() + "\", \"53\", \"" + id + "\", NOW());");

				ostResult << "{\"result\":\"success\",\"id\":\"" << id << "\",\"bookReadersRatingList\":["<< GetBookRatingList(bookID, &db) << "]}";
			}
		} // --- if(user != Guest)

		indexPage.RegisterVariableForce("result", ostResult.str());

		if(!indexPage.SetTemplate("json_response.htmlt"))
		{
			CLog	log;

			log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: template file json_response.htmlt was missing");
			throw CException("Template file json_response.htmlt was missing");
		}  // if(!indexPage.SetTemplate("AJAX_setBookRating.htmlt"))

		{
			CLog	log;
			log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_setBookRating: end");
		}
	}   // if(action == "AJAX_setBookRating")

	if(action == "updateBookReadTimestamp")
	{
		ostringstream   ostResult;
		string		  id, value;

		{
			CLog	log;
			log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: action == updateBookReadTimestamp: start");
		}

		ostResult.str("");
		if(user.GetLogin() == "Guest")
		{
			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == updateBookReadTimestamp: re-login required");
			}

			ostResult << "{\"result\":\"error\",\"description\":\"сессия закончилась, необходимо вновь зайти на сайт\"}";
		}
		else
		{
			id = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));
			value = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("value"));

			db.Query("UPDATE `users_books` SET `bookReadTimestamp`=\"" + value + "\" WHERE `id`=\"" + id + "\" and `userID`=\"" + user.GetID() + "\";");
			ostResult << "{\"result\":\"success\"}";

/*			if(db.InsertQuery())
			{
			}
			else
			{
				CLog log;
				log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == updateBookReadTimestamp: can't update live feed");
			}
*/
		}

		indexPage.RegisterVariableForce("result", ostResult.str());

		if(!indexPage.SetTemplate("json_response.htmlt"))
		{
			CLog	log;

			log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: template file json_response.htmlt was missing");
			throw CException("Template file json_response.htmlt was missing");
		}  // if(!indexPage.SetTemplate("updateBookReadTimestamp.htmlt"))

		{
			CLog	log;
			log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == updateBookReadTimestamp: end");
		}
	}   // if(action == "updateBookReadTimestamp")

	if(action == "AJAX_complainBook")
	{
		ostringstream   ostResult;
		string		  complainBookAuthor, complainBookTitle, complainBookISBN10, complainBookISBN13, complainBookCover;
		string			complainBookComment;

		{
			CLog	log;
			log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: action == AJAX_complainBook: start");
		}

		ostResult.str("");
		if(user.GetLogin() == "Guest")
		{
			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_complainBook: re-login required");
			}

			ostResult << "{\"result\":\"error\",\"description\":\"сессия закончилась, необходимо вновь зайти на сайт\"}";
		}
		else
		{

			complainBookAuthor	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("complainBookAuthor"));
			complainBookTitle	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("complainBookTitle"));
			complainBookISBN10	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("complainBookISBN10"));
			complainBookISBN13	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("complainBookISBN13"));
			complainBookCover	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("complainBookCover"));
			complainBookComment	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("complainBookComment"));

			if(db.Query("select * from `book_complains` WHERE `authorName`=\"" + complainBookAuthor + "\" AND `title`=\"" + complainBookTitle + "\" AND `isbn10`=\"" + complainBookISBN10 + "\" AND `isbn13`=\"" + complainBookISBN13 + "\" AND `cover`=\"" + complainBookCover + "\""))
			{
				CLog	log;

				log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: inserting complain");

				ostResult << "{\"result\":\"error\", \"description\":\"Подобная жалоба уже есть.\"}";
			}
			else
			{
				long int id;

				id = db.InsertQuery("INSERT INTO `book_complains` SET `authorName`=\"" + complainBookAuthor + "\",`title`=\"" + complainBookTitle + "\",`isbn10`=\"" + complainBookISBN10 + "\",`isbn13`=\"" + complainBookISBN13 + "\",`cover`=\"" + complainBookCover + "\",`complain`=\"" + complainBookComment + "\", `userID`=\"" + user.GetID() + "\", `eventTimestamp`=UNIX_TIMESTAMP(), `state`=\"new\";");
				if(id)
					ostResult << "{\"result\":\"success\", \"id\":\"" << id << "\"}";
				else
				{
					CLog	log;
					log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: inserting complain");

					ostResult << "{\"result\":\"error\", \"description\":\"Ошибка, попробуйте еще раз.\"}";
				}
			}
		}

		indexPage.RegisterVariableForce("result", ostResult.str());

		if(!indexPage.SetTemplate("json_response.htmlt"))
		{
			CLog	log;

			log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: template file json_response.htmlt was missing");
			throw CException("Template file json_response.htmlt was missing");
		}  // if(!indexPage.SetTemplate("AJAX_complainBook.htmlt"))

		{
			CLog	log;
			log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_complainBook: end");
		}
	}   // if(action == "AJAX_complainBook")

	if((action == "JSON_getBookAuthorListAutocomplete") || (action == "JSON_getBookTitleListAutocomplete"))
	{
		ostringstream   ostResult;
		string		  lookForKey;

		{
			CLog	log;
			log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: action == JSON_getBookAuthorListAutocomplete: start");
		}

		lookForKey = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("lookForKey"));

		ostResult.str("");
		if(user.GetLogin() == "Guest")
		{
			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == JSON_getBookAuthorListAutocomplete: re-login required");
			}

			ostResult << "{\"result\":\"error\",\"description\":\"сессия закончилась, необходимо вновь зайти на сайт\"}";
		}
		else
		{
			string  		itemList("");

			if(lookForKey.length() >= 3)
			{
				int	 		affected;

				if(action == "JSON_getBookTitleListAutocomplete")
				{
					affected = db.Query("select * from `book` WHERE `title` LIKE \"%" + lookForKey + "%\";");

					for(int i = 0; i < affected; ++i)
					{
						if(i) itemList += ",";
						itemList += "{";
						itemList += quoted(string("id")) + ":" + quoted(string(db.Get(i, "id"))) + ",";
						itemList += quoted(string("name")) + ":" + quoted(string(db.Get(i, "title")));
						itemList += "}";
					}
				}

				if(action == "JSON_getBookAuthorListAutocomplete")
				{
					affected = db.Query("select * from `book_author` WHERE `name` LIKE \"%" + lookForKey + "%\";");

					for(int i = 0; i < affected; ++i)
					{
						if(i) itemList += ",";
						itemList += "{";
						itemList += quoted(string("id")) + ":" + quoted(string(db.Get(i, "id"))) + ",";
						itemList += quoted(string("name")) + ":" + quoted(string(db.Get(i, "name")));
						itemList += "}";
					}
				}

			}
			else
			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == JSON_getBookAuthorListAutocomplete: lookFor key [" + lookForKey + "] length less than 3");
			}
			ostResult << "{\"result\":\"success\", \"items\":[" << itemList << "]}";
		}

		indexPage.RegisterVariableForce("result", ostResult.str());

		if(!indexPage.SetTemplate("json_response.htmlt"))
		{
			CLog	log;

			log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: template file json_response.htmlt was missing");
			throw CException("Template file json_response.htmlt was missing");
		}  // if(!indexPage.SetTemplate("JSON_getBookAuthorListAutocomplete.htmlt"))

		{
			CLog	log;
			log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == JSON_getBookAuthorListAutocomplete: end");
		}
	}   // if(action == "JSON_getBookAuthorListAutocomplete")

	// --- AJAX remove book from read list
	if(action == "AJAX_removeBookEntry")
	{
		ostringstream   ost, ostFinal;
		CMysql		  db1;
		string		  usersBooksID;

		{
			CLog	log;
			log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + string("]:") + ":action == AJAX_removeBookEntry: start");
		}

		usersBooksID = indexPage.GetVarsHandler()->Get("id");

		if(usersBooksID.empty())
		{
			CLog	log;
			log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + string("]:") + ": action == AJAX_removeBookEntry: usersBooksID is not defined");

			ostFinal.str("");
			ostFinal << "{" << std::endl;
			ostFinal << "\"result\" : \"error\"," << std::endl;
			ostFinal << "\"description\" : \"usersBooksID is empty\"" << std::endl;
			ostFinal << "}" << std::endl;
		}
		else
		{
			if(user.GetLogin() == "Guest")
			{
				ostringstream   ost;

				{
					CLog	log;
					log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + string("]:") + ": action == AJAX_removeBookEntry: re-login required");
				}

				ost.str("");
				ost << "/?rand=" << GetRandom(10);
				indexPage.Redirect(ost.str());
			}


			if(db.Query("select * from `users_books` where `id`='" + usersBooksID + "' AND `userID`='" + user.GetID() + "';"))
			{   
				string		bookID = db.Get(0, "bookID");

				// --- delete notifications on likeBook
				db.Query("delete from `users_notification` where `actionTypeId`=\"49\" and `actionId` in (select `id` from `feed_message_params` where `messageID`=\"" + usersBooksID + "\" and `parameter`=\"likeBook\");");
				// --- delete notifications on comment with type "book"
				db.Query("delete from `users_notification` where `actionTypeId`=\"19\" and `actionId` in (select `id` from `feed_message_comment` where `messageID`=\"" + bookID + "\" and `type`=\"book\") AND `userId`=\"" + user.GetID() + "\";");

				// --- delete book comments
				db.Query("delete from `feed_message_params` where `parameter`=\"likeBook\" AND `messageID`=\"" + usersBooksID + "\";");
				// --- remove book from read list
				db.Query("delete from `users_books` where `id`='" + usersBooksID + "' AND `userID`=\"" + user.GetID() + "\";");
				// --- remove comment written event
				db.Query("delete from `feed` where `actionTypeId`=\"55\" AND `actionId`=\"" + usersBooksID + "\";");
				// --- remove book read
				db.Query("delete from `feed` where `actionTypeId`=\"54\" AND `actionId`=\"" + usersBooksID + "\";");
				// --- remove book like
				db.Query("delete from `feed` where `actionTypeId`=\"53\" AND `actionId`=\"" + usersBooksID + "\";");

				ostFinal.str("");
				ostFinal << "{" << std::endl;
				ostFinal << "\"result\" : \"success\"," << std::endl;
				ostFinal << "\"description\" : \"\"" << std::endl;
				ostFinal << "}" << std::endl;
			}
			else
			{
				CLog	log;
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + string("]:") + ": action == AJAX_removeBookEntry: usersBooksID doesn't belongs to you or empty");

				ostFinal.str("");
				ostFinal << "{" << std::endl;
				ostFinal << "\"result\" : \"error\"," << std::endl;
				ostFinal << "\"description\" : \"usersBooksID doesn't belongs to you or empty\"" << std::endl;
				ostFinal << "}" << std::endl;
			}
		}

		indexPage.RegisterVariableForce("result", ostFinal.str());

		if(!indexPage.SetTemplate("json_response.htmlt"))
		{
			CLog	log;

			log.Write(ERROR, __func__ + string("[") + to_string(__LINE__) + string("]:template file json_response.htmlt was missing"));
			throw CException("Template file was missing");
		}

		{
			CLog	log;
			log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + string("]:") + ":action == AJAX_removeBookEntry: end");
		}
	} // --- if(action == "AJAX_removeBookEntry")

	if(action == "JSON_getBookISBNsByAuthorAndTitle")
	{
		ostringstream   ostResult;
		string		  bookTitle(""), bookAuthor(""), bookAuthorID("");

		{
			CLog	log;
			log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: action == JSON_getBookISBNsByAuthorAndTitle: start");
		}

		bookTitle	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("bookTitle"));
		bookAuthor	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("bookAuthor"));

		ostResult.str("");
		if(user.GetLogin() == "Guest")
		{
			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == JSON_getBookISBNsByAuthorAndTitle: re-login required");
			}

			ostResult << "{\"result\":\"error\",\"description\":\"сессия закончилась, необходимо вновь зайти на сайт\"}";
		}
		else
		{
			string	  bookList = "";

			if(!bookTitle.empty() && !bookAuthor.empty())
			{
				if(db.Query("SELECT * FROM `book_author` WHERE `name`=\"" + bookAuthor + "\";"))
				{
					bookAuthorID = db.Get(0, "id");
					bookList = GetBookListInJSONFormat("select * from `book` where `title`=\"" + bookTitle + "\" and `authorID`=\"" + bookAuthorID + "\";", &db);
				}
				else
				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == JSON_getBookISBNsByAuthorAndTitle: author doesn't exists");
				}
			}
			else
			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == JSON_getBookISBNsByAuthorAndTitle: title or author is empty");
			}
			ostResult << "{\"result\":\"success\", \"books\":[" << bookList << "]}";
		}

		indexPage.RegisterVariableForce("result", ostResult.str());

		if(!indexPage.SetTemplate("json_response.htmlt"))
		{
			CLog	log;

			log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: template file json_response.htmlt was missing");
			throw CException("Template file json_response.htmlt was missing");
		}  // if(!indexPage.SetTemplate("JSON_getBookISBNsByAuthorAndTitle.htmlt"))

		{
			CLog	log;
			log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == JSON_getBookISBNsByAuthorAndTitle: end");
		}
	}   // if(action == "JSON_getBookISBNsByAuthorAndTitle")

	// --- AJAX get author by name
	if(action == "JSON_getBookDetailsByTitle")
	{
		ostringstream   ostResult;
		string		  bookTitle(""), bookAuthor(""), bookAuthorID("");

		{
			CLog	log;
			log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: action == JSON_getBookDetailsByTitle: start");
		}

		bookTitle = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("bookTitle"));

		ostResult.str("");
		if(user.GetLogin() == "Guest")
		{
			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == JSON_getBookDetailsByTitle: re-login required");
			}

			ostResult << "{\"result\":\"error\",\"description\":\"сессия закончилась, необходимо вновь зайти на сайт\"}";
		}
		else
		{
			string	  bookList = "";

			if(!bookTitle.empty())
			{
				if(db.Query("SELECT * FROM `book` WHERE `title`=\"" + bookTitle + "\";"))
				{
					string bookID = db.Get(0, "id");
					bookList = GetBookListInJSONFormat("select * from `book` where `id`=\"" + bookID + "\";", &db);
				}
				else
				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == JSON_getBookDetailsByTitle: book doesn't exists");
				}
			}
			else
			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == JSON_getBookDetailsByTitle: title is empty");
			}
			ostResult << "{\"result\":\"success\", \"books\":[" << bookList << "]}";
		}

		indexPage.RegisterVariableForce("result", ostResult.str());

		if(!indexPage.SetTemplate("json_response.htmlt"))
		{
			CLog	log;

			log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: template file json_response.htmlt was missing");
			throw CException("Template file json_response.htmlt was missing");
		}  // if(!indexPage.SetTemplate("JSON_getBookDetailsByTitle.htmlt"))

		{
			CLog	log;
			log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == JSON_getBookDetailsByTitle: end");
		}
	} // --- if(action == "AJAX_getAuthorByName")

	if(action == "AJAX_addEditProfileAddBook")
	{
		ostringstream   ostResult;
		string		  newBookTitle, newBookAuthor, newBookISBN10, newBookISBN13;
		string		  newBookID = "", newAuthorID = "";
		string			isbn10FromDB = "", isbn13FromDB = "";

		{
			CLog	log;
			log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: action == AJAX_addEditProfileAddBook: start");
		}

		newBookTitle	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("title"));
		newBookAuthor	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("author"));
		newBookISBN10	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("isbn10"));
		newBookISBN13	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("isbn13"));

		newBookISBN10 = SymbolReplace_KeepDigitsOnly(newBookISBN10);
		newBookISBN13 = SymbolReplace_KeepDigitsOnly(newBookISBN13);

		{
			string		tmp10DigLen = "";
			string		tmp13DigLen = "";

			if(newBookISBN13.length() == 10) tmp10DigLen = newBookISBN13;
			if(newBookISBN10.length() == 10) tmp10DigLen = newBookISBN10;
			if(newBookISBN10.length() == 13) tmp13DigLen = newBookISBN10;
			if(newBookISBN13.length() == 13) tmp13DigLen = newBookISBN13;

			newBookISBN10 = tmp10DigLen;
			newBookISBN13 = tmp13DigLen;
		}

		ostResult.str("");
		if(user.GetLogin() == "Guest")
		{
			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_addEditProfileAddBook: re-login required");
			}

			ostResult << "{\"result\":\"error\",\"description\":\"сессия закончилась, необходимо вновь зайти на сайт\"}";
		}
		else
		{
			string			nextStep = "";
			string  		newUsersBooksID = "";

			if(!newBookTitle.empty() && !newBookAuthor.empty())
			{
				if(db.Query("select * from `book_author` where `name`=\"" + newBookAuthor + "\";"))
					newAuthorID = db.Get(0, "id");
				if(db.Query("select * from `book` where `title`=\"" + newBookTitle + "\" and `authorID`=\"" + newAuthorID + "\";"))
				{
					newBookID = db.Get(0, "id");
					isbn10FromDB = db.Get(0, "isbn10");
					isbn13FromDB = db.Get(0, "isbn13");
				}
/*
				// --- isbn10 provided by user
				if(!newBookISBN10.empty() && !newBookISBN13.empty())
				{
					{
						CLog	log;
						log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_addEditProfileAddBook: check by ISBN10 and ISBN13");
					}

					if(db.Query("select * from `book` where `isbn10`=\"" + newBookISBN10 + "\" and `isbn13`=\"" + newBookISBN13 + "\";"))
					{
						string	  existingBookID = db.Get(0, "id");
						string	  existingAuthorID = db.Get(0, "authorID");

						if((existingBookID == newBookID) && (existingAuthorID == newAuthorID))
						{
							nextStep = "readExistingBook";
						}
						else
						{
							CLog log;
							log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_addEditProfileAddBook:ERROR: book with that pair (ISBN10, ISBN13)[" + newBookISBN10 + ", " + newBookISBN13 + "] exists with different title or author");

							nextStep = "error";
							ostResult << "{\"result\":\"error\", \"description\":\"книга с такими ISBN уже есть, но у нее другое название или автор\"}";
						}
					}
					else
					{
						if(db.Query("select * from `book` where `isbn10`=\"" + newBookISBN10 + "\" and `isbn13`=\"\";"))
						{
							string	  existingBookID = db.Get(0, "id");
							string	  existingAuthorID = db.Get(0, "authorID");

							if((existingBookID == newBookID) && (existingAuthorID == newAuthorID))
							{
								db.Query("UPDATE `book` SET `isbn13`=\"" + newBookISBN13 + "\" where `isbn10`=\"" + newBookISBN10 + "\" and `isbn13`=\"\";");
								nextStep = "readExistingBook";
							}
							else
							{
								CLog log;
								log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_addEditProfileAddBook:ERROR: book with that pair (ISBN10, ISBN13)[" + newBookISBN10 + ", ] exists with different title or author");

								nextStep = "error";
								ostResult << "{\"result\":\"error\", \"description\":\"книга с такими ISBN уже есть, но у нее другое название или автор\"}";
							}
						}
						else if(db.Query("select * from `book` where `isbn13`=\"" + newBookISBN13 + "\" and `isbn10`=\"\";"))
						{
							string	  existingBookID = db.Get(0, "id");
							string	  existingAuthorID = db.Get(0, "authorID");

							if((existingBookID == newBookID) && (existingAuthorID == newAuthorID))
							{
								db.Query("UPDATE `book` SET `isbn10`=\"" + newBookISBN10 + "\" where `isbn13`=\"" + newBookISBN13 + "\" and `isbn10`=\"\";");
								nextStep = "readExistingBook";
							}
							else
							{
								CLog log;
								log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_addEditProfileAddBook:ERROR: book with that pair (ISBN10, ISBN13)[" + newBookISBN10 + ", ] exists with different title or author");

								nextStep = "error";
								ostResult << "{\"result\":\"error\", \"description\":\"книга с такими ISBN уже есть, но у нее другое название или автор\"}";
							}
						}
						else
							nextStep = "readNewBook";
					}
				}
				// --- isbn10 provided by user
				else if(!newBookISBN10.empty())
				{
					{
						CLog	log;
						log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_addEditProfileAddBook: check by ISBN10");
					}

					if(db.Query("select * from `book` where `isbn10`=\"" + newBookISBN10 + "\";"))
					{
						string	  existingBookID = db.Get(0, "id");
						string	  existingAuthorID = db.Get(0, "authorID");

						if((existingBookID == newBookID) && (existingAuthorID == newAuthorID))
						{
							nextStep = "readExistingBook";
						}
						else
						{
							CLog log;
							log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_addEditProfileAddBook:ERROR: wrong ISBN10[" + newBookISBN10 + "]");

							nextStep = "error";
							ostResult << "{\"result\":\"error\", \"description\":\"книга с ISBN10 уже есть, но у нее другое название или автор\"}";
						}
					}
					else
						nextStep = "readNewBook";
				}
				// --- isbn13 provided by user
				else if(!newBookISBN13.empty())
				{
					{
						CLog	log;
						log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_addEditProfileAddBook: check by ISBN13");
					}

					if(db.Query("select * from `book` where `isbn13`=\"" + newBookISBN13 + "\";"))
					{
						string	  existingBookID = db.Get(0, "id");
						string	  existingAuthorID = db.Get(0, "authorID");

						if((existingBookID == newBookID) && (existingAuthorID == newAuthorID))
						{
							nextStep = "readExistingBook";
						}
						else
						{
							CLog log;
							log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_addEditProfileAddBook:ERROR: wrong ISBN13[" + newBookISBN13 + "]");

							nextStep = "error";
							ostResult << "{\"result\":\"error\", \"description\":\"книга с ISBN13 уже есть, но у нее другое название или автор\"}";
						}
					}
					else
						nextStep = "readNewBook";
				}
				// --- existing book read (no isbn's provided)
				else
*/
				if(newBookID.empty() || newAuthorID.empty())
				{
					// --- requested book doesn't exists 
					{
						CLog	log;
						log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_addEditProfileAddBook: new book read (no isbn's provided)");
					}
					nextStep = "readNewBook";
				}
				else
				{
					// --- requested book exists 
					{
						CLog	log;
						log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_addEditProfileAddBook: existing book read (no isbn's provided)");
					}
					nextStep = "readExistingBook";
				}

				// --- check uniqueness of isbn10
				if(!newBookISBN10.empty())
				{
					{
						CLog	log;
						log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_addEditProfileAddBook: check with ISBN10");
					}

					if(db.Query("select * from `book` where `isbn10`=\"" + newBookISBN10 + "\";"))
					{
						string		bookIDbyISBN10 = db.Get(0, "id");

						if(bookIDbyISBN10 != newBookID)
						{
							{
								CLog	log;
								// --- must be converted to DEBUG later
								log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_addEditProfileAddBook:ERROR: book with ISBN10[" + newBookISBN10 + "] already exists and differ");
							}

							nextStep = "error";
							ostResult << "{\"result\":\"error\", \"description\":\"книга с ISBN10 уже есть, но у нее другое название или автор\"}";
						}
					}
				}

				// --- check uniqueness of isbn13
				if(!newBookISBN13.empty())
				{
					{
						CLog	log;
						log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_addEditProfileAddBook: check with ISBN13");
					}

					if(db.Query("select * from `book` where `isbn13`=\"" + newBookISBN13 + "\";"))
					{
						string		bookIDbyISBN13 = db.Get(0, "id");

						if(bookIDbyISBN13 != newBookID)
						{
							{
								CLog	log;
								// --- must be converted to DEBUG later
								log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_addEditProfileAddBook:ERROR: book with ISBN13[" + newBookISBN13 + "] already exists and differ");
							}

							nextStep = "error";
							ostResult << "{\"result\":\"error\", \"description\":\"книга с ISBN13 уже есть, но у нее другое название или автор\"}";
						}
					}
				}

				if(nextStep == "readNewBook")
				{
					{
						CLog	log;
						log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_addEditProfileAddBook:nextStep == readNewBook: start");
					}

					nextStep = "readExistingBook";

					if(newAuthorID.empty())
					{
						newAuthorID = to_string(db.InsertQuery("INSERT INTO `book_author` SET `name`=\"" + newBookAuthor + "\";"));
						if(newAuthorID.empty() || newAuthorID == "0")
						{
							{
								CLog log;
								log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_addEditProfileAddBook:ERROR: can't insert new author into `book_author`");
							}

							nextStep = "error";
							ostResult << "{\"result\":\"error\", \"description\":\"внутренняя ошибка сайта\"}";
						}
					}

					if(newBookID.empty())
					{
						isbn10FromDB = newBookISBN10;
						isbn13FromDB = newBookISBN13;
						newBookID = to_string(db.InsertQuery("INSERT INTO `book` SET `authorID`=\"" + newAuthorID + "\", `title`=\"" + newBookTitle + "\", `isbn10`=\"" + newBookISBN10 + "\", `isbn13`=\"" + newBookISBN13 + "\", `owner_userID`=\"" + user.GetID() + "\";"));

						if(newBookID.empty() || newBookID == "0")
						{
							{
								CLog log;
								log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_addEditProfileAddBook:ERROR: can't insert new book into `book`");
							}

							nextStep = "error";
							ostResult << "{\"result\":\"error\", \"description\":\"внутренняя ошибка сайта\"}";
						}
					}
				}

				if(nextStep == "readExistingBook")
				{
					{
						CLog	log;
						log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_addEditProfileAddBook:nextStep == readExistingBook:start");
					}
					nextStep = "success";

					// --- update ISBN10, if required
					if(
						(isbn10FromDB.empty() || isbn10FromDB == "0") 
						&& 
						(!newBookISBN10.empty() && newBookISBN10 != "0")
					  )
					{
						db.Query("UPDATE `book` SET `isbn10`=\"" + newBookISBN10 + "\" WHERE `id`=\"" + newBookID + "\";");
					}
					else
					{
						CLog	log;
						log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_addEditProfileAddBook:nextStep == readExistingBook: no need to update ISBN10");
					}

					// --- update ISBN13, if required
					if(
						(isbn13FromDB.empty() || isbn13FromDB == "0") 
						&& 
						(!newBookISBN13.empty() && newBookISBN13 != "0")
					  )
					{
						db.Query("UPDATE `book` SET `isbn13`=\"" + newBookISBN13 + "\" WHERE `id`=\"" + newBookID + "\";");
					}
					else
					{
						CLog	log;
						log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_addEditProfileAddBook:nextStep == readExistingBook: no need to update ISBN13");
					}

					if(db.Query("SELECT * FROM `users_books` WHERE `userID`=\"" + user.GetID() + "\" AND `bookID`=\"" + newBookID + "\";"))
					{
						// --- user already read this book earlier
						// --- news_feed won't be updated
						newUsersBooksID = db.Get(0, "id");
						db.Query("UPDATE `users_books` SET `bookReadTimestamp`=UNIX_TIMESTAMP() WHERE `id`=\"" + newUsersBooksID + "\";");
					}
					else
					{
						// --- user haven't read this book earlier
						newUsersBooksID = to_string(db.InsertQuery("INSERT INTO `users_books` SET `userID`=\"" + user.GetID() + "\", `bookID`=\"" + newBookID + "\", `bookReadTimestamp`=UNIX_TIMESTAMP();"));
						
						if(!newUsersBooksID.empty())
						{

							if(db.InsertQuery("INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" + user.GetID() + "\", \"54\", \"" + newUsersBooksID + "\", NOW())"))
							{
							}
							else
							{
								{
									CLog	log;
									log.Write(ERROR, string(__func__) + ": action == AJAX_addEditProfileAddBook: ERROR: inserting into news_feed");
								}
							}
						}
						else
						{
							{
								CLog log;
								log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_addEditProfileAddBook:ERROR: can't insert new book into `book`");
							}

							nextStep = "error";
							ostResult << "{\"result\":\"error\", \"description\":\"внутренняя ошибка сайта\"}";
						}
					}
				} // --- if(nextStep == "readExistingBook")

				if(nextStep == "success")
				{
					ostringstream	ost;
					int				affected;

					ostResult << "{\"result\":\"success\", \"users_books_id\":\"" + newUsersBooksID + "\", \"books\":[";

					ost.str("");
					ost << "\
		SELECT `users_books`.`id` as 'users_books_id', `book`.`id` as 'book_id', `book`.`title` as 'book_title', `book`.`isbn10` as 'book_isbn10', `book`.`isbn13` as 'book_isbn13', `book`.`coverPhotoFolder` as 'book_coverPhotoFolder', `book`.`coverPhotoFilename` as 'book_coverPhotoFilename',  \
		`book_author`.`name` as `book_author_name`, `users_books`.`rating` as `users_books_rating` , `users_books`.`bookReadTimestamp` as `users_books_bookReadTimestamp` \
		FROM `users_books` \
		RIGHT JOIN `book`		ON `users_books`.`bookID`=`book`.`id` \
		RIGHT JOIN `book_author` ON `book`.`authorID`=`book_author`.`id` \
		where `users_books`.`userID`=\"" << user.GetID() << "\";";
					affected = db.Query(ost.str());
					if(affected)
					{
							for(int i = 0; i < affected; i++) 
							{
								ostResult << (i ? "," : "")
								<< "{"
								<< "\"id\":\"" << db.Get(i, "users_books_id") << "\","
								<< "\"bookID\":\"" << db.Get(i, "book_id") << "\","
								<< "\"bookTitle\":\"" << db.Get(i, "book_title") << "\","
								<< "\"bookAuthorName\":\"" << db.Get(i, "book_author_name") << "\","
								<< "\"bookRating\":\"" << db.Get(i, "users_books_rating") << "\","
								<< "\"bookReadTimestamp\":\"" << db.Get(i, "users_books_bookReadTimestamp") << "\","
								<< "\"bookISBN10\":\"" << db.Get(i, "book_isbn10") << "\","
								<< "\"bookISBN13\":\"" << db.Get(i, "book_isbn13") << "\","
								<< "\"bookCoverPhotoFolder\":\"" << db.Get(i, "book_coverPhotoFolder") << "\","
								<< "\"bookCoverPhotoFilename\":\"" << db.Get(i, "book_coverPhotoFilename") << "\""
								<< "}";
							}
					}
					else 
					{
						CLog	log;
						log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: action == AJAX_addEditProfileAddBook: book path is empty");
					}

					ostResult << "]}";
				}

			}
			else
			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_addEditProfileAddBook: book title or book author empty");
				
				ostResult << "{\"result\":\"error\", \"description\":\"название или автор книги пустые\"}";
			}
		}

		indexPage.RegisterVariableForce("result", ostResult.str());

		if(!indexPage.SetTemplate("json_response.htmlt"))
		{
			CLog	log;

			log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: template file json_response.htmlt was missing");
			throw CException("Template file json_response.htmlt was missing");
		}  // if(!indexPage.SetTemplate("AJAX_addEditProfileAddBook.htmlt"))

		{
			CLog	log;
			log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_addEditProfileAddBook: end");
		}
	}   // if(action == "AJAX_addEditProfileAddBook")











	{
		CLog	log;
		ostringstream	ost;

		ost.str("");
		ost << __func__ << "[" << __LINE__ << "]: end (action's == \"" << action << "\") condition";
		log.Write(DEBUG, ost.str());
	}

	indexPage.OutTemplate();

	}
/*
	catch(CExceptionRedirect &c) {
		CLog	log;
		ostringstream	ost;

		ost.str("");
		ost << string(__func__) + ":: catch CRedirectHTML: exception used for redirection";
		log.Write(DEBUG, ost.str());

		c.SetDB(&db);

		if(!indexPage.SetTemplate(c.GetTemplate())) {

			ost.str("");
			ost << string(__func__) + ":: catch CRedirectHTML: ERROR, template redirect.htmlt not found";
			log.Write(ERROR, ost.str());

			throw CException("Template file was missing");
		}

		indexPage.RegisterVariableForce("content", "redirect page");
		indexPage.OutTemplate();

	}
*/
	catch(CExceptionHTML &c)
	{
		CLog	log;

		c.SetLanguage(indexPage.GetLanguage());
		c.SetDB(&db);

		log.Write(DEBUG, string(__func__) + ": catch CExceptionHTML: DEBUG exception reason: [", c.GetReason(), "]");

		if(!indexPage.SetTemplate(c.GetTemplate()))
		{
			return(-1);
		}
		indexPage.RegisterVariable("content", c.GetReason());
		indexPage.OutTemplate();
		return(0);
	}
	catch(CException &c)
	{
		CLog 	log;

		if(!indexPage.SetTemplateFile("templates/error.htmlt"))
		{
			return(-1);
		}

		log.Write(ERROR, string(__func__) + ": catch CException: exception: ERROR  ", c.GetReason());

		indexPage.RegisterVariable("content", c.GetReason());
		indexPage.OutTemplate();
		return(-1);
	}
	catch(exception& e)
	{
		CLog 	log;
		log.Write(PANIC, string(__func__) + ": catch(exception& e): catch standard exception: ERROR  ", e.what());

		if(!indexPage.SetTemplateFile("templates/error.htmlt"))
		{
			return(-1);
		}
		indexPage.RegisterVariable("content", e.what());
		indexPage.OutTemplate();
		return(-1);
	}

	return(0);
}

