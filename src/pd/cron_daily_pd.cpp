#include "cron_daily_pd.h"
#include "cactivator.h"

bool BirthdayNotificationToFriends(CMysql *db)
{
	bool 				result = false;
	long int			affected;
	vector<string>		birthdayUserList;

	{ MESSAGE_DEBUG("", "", "start"); }

	affected = db->Query("SELECT `id` FROM `users` WHERE `birthday` LIKE DATE_FORMAT(NOW(), '%d/%m/%') AND `birthdayAccess`=\"public\" AND `isblocked`=\"N\";");
	for(long int i = 0; i < affected; ++i)
	{
		birthdayUserList.push_back(db->Get(i, "id"));
	}

	for(auto &birthdayUserID: birthdayUserList)
	{
		vector<string>		userListToNotify;
		string				sqlQuery = "";

		userListToNotify.clear();
		affected = db->Query("SELECT `id` FROM `users` WHERE `id` IN (SELECT `friendID` FROM `users_friends` WHERE `userid`=\"" + birthdayUserID + "\" and `state`=\"confirmed\") AND `isblocked`=\"N\";");
		for(long int i = 0; i < affected; ++i)
		{
			if(sqlQuery.length()) sqlQuery += ",";
			sqlQuery += "(\"\", \"" + string(db->Get(i, "id")) + "\", \"58\", \"" + birthdayUserID + "\", UNIX_TIMESTAMP())";
		}

		if(sqlQuery.length())
		{
			if(db->InsertQuery("INSERT INTO `users_notification` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) VALUES " + sqlQuery + ";"))
			{
			}
			else
			{
				MESSAGE_ERROR("", "", "inserting into `users_notifications` table"); 
			}
		}
		else
		{
			MESSAGE_DEBUG("", "", "birthday user have no friends"); 
		}
	}

	{ MESSAGE_DEBUG("", "", "finish"); }

	return result;
}

pair<int, int> GetMinMaxCommentsOfMessagesOverYear(string userID, CMysql *db)
{
	int		commentsMinValue = 0, commentsMaxValue = 0;

	{ MESSAGE_DEBUG("", "", "start"); }

	// --- get MAX number of comments over last year
	if(db->Query(
		"SELECT MAX(`counter`) AS `commentsMaxValue` FROM ("
			" SELECT COUNT(`messageID`) AS `counter` "
			" FROM `feed_message_comment` "
			" WHERE "
				" `type`=\"message\""
				" AND"
				" `messageID` IN ("
					"SELECT `actionId` FROM `feed` WHERE"
						" `actionTypeId`=\"11\""
						" AND "
						"`srcType`=\"user\""
						" AND"
						" `userId`=\"" + userID + "\""
						" AND"
						" `eventTimestamp`>=CAST(DATE_SUB(CURRENT_DATE, INTERVAL 1 YEAR) AS DATETIME)"
				" )"
				" GROUP BY `messageID`"
		") AS t WHERE `counter`>\"0\";"
		)) commentsMaxValue = stoi(db->Get(0, "commentsMaxValue"));

	// --- get MIN number of comments over last year
	if(db->Query(
		"SELECT MIN(`counter`) AS `commentsMinValue` FROM ("
			" SELECT COUNT(`messageID`) AS `counter` "
			" FROM `feed_message_comment` "
			" WHERE "
				" `type`=\"message\""
				" AND"
				" `messageID` IN ("
					"SELECT `actionId` FROM `feed` WHERE"
						" `actionTypeId`=\"11\""
						" AND "
						"`srcType`=\"user\""
						" AND"
						" `userId`=\"" + userID + "\""
						" AND"
						" `eventTimestamp`>=CAST(DATE_SUB(CURRENT_DATE, INTERVAL 1 YEAR) AS DATETIME)"
				" )"
				" GROUP BY `messageID`"
		") AS t WHERE `counter`>\"0\";"
		)) commentsMinValue = stoi(db->Get(0, "commentsMinValue"));

	{ MESSAGE_DEBUG("", "", "finish (min / max = " + to_string(commentsMinValue) + " / " + to_string(commentsMaxValue) + ")"); }

	return make_pair(commentsMinValue, commentsMaxValue);
}

pair<float, float> GetMessageGPSCoordinates(string messageID, CMysql *db)
{
	pair<float, float>			result(0, 0);
	vector<pair<float, float>>	imagesCoordinates;
	float						maxDistance = 0;
	float						pivotLatitude = 0, pivotLongitude = 0;

	{ MESSAGE_DEBUG("", "", "start"); }


	if(db->Query("SELECT `imageSetID` FROM `feed_message` WHERE `id`=\"" + messageID + "\" AND `imageSetID`!=\"0\";"))
	{
		string		imgTempSetID = db->Get(0, "imageSetID");

		if(imgTempSetID == "0")
		{
			{ MESSAGE_DEBUG("", "", "imageSetID == 0"); }
		}
		else
		{
			int			affected;

			affected = db->Query("SELECT `exifGPSLatitude`, `exifGPSLongitude` FROM `feed_images` WHERE `setID`=\"" + imgTempSetID + "\";");

			if(affected)
			{
				maxDistance = 0;

				for(int i = 0; i < affected; ++i)
				{
					auto		latitude  = 0.0f;
					auto		longitude = 0.0f;

					try
					{
						latitude  = stof(db->Get(i, "exifGPSLatitude").length()  ? db->Get(i, "exifGPSLatitude")  : "0"s);
					}
					catch(...)
					{
						MESSAGE_ERROR("", "", "can't stof latitude(" + db->Get(i, "exifGPSLatitude") + ")");
					}

					try
					{
						longitude = stof(db->Get(i, "exifGPSLongitude").length() ? db->Get(i, "exifGPSLongitude") : "0"s);
					}
					catch(...)
					{
						MESSAGE_ERROR("", "", "can't stof longitude(" + db->Get(i, "exifGPSLongitude") + ")");
					}

					if(latitude && longitude)
					{
						if(pivotLatitude && pivotLongitude)
						{
							if(maxDistance < (pivotLongitude - longitude)*(pivotLongitude - longitude) + (pivotLatitude - latitude)*(pivotLatitude - latitude))
								maxDistance = (pivotLongitude - longitude)*(pivotLongitude - longitude) + (pivotLatitude - latitude)*(pivotLatitude - latitude);
						}
						else
						{
							// --- init pivot point
							pivotLongitude = longitude;
							pivotLatitude = latitude;
						}
					}

					imagesCoordinates.push_back(make_pair(latitude, longitude));
				}
			}
			else
			{
				{ MESSAGE_DEBUG("", "", "no images in tempSet(" + imgTempSetID + ")"); }
			}
		}
	}
	else
	{
		{ MESSAGE_DEBUG("", "", "no images in messageID(" + messageID + ")"); }
	}

	if(maxDistance > SINGLE_MESSAGE_DISTANCE)
	{
		{ MESSAGE_DEBUG("", "", "GPS distance between photos more than 100 km, made shooting point 'unknown'"); }
		result = make_pair(0, 0);
	}
	else
	{
		result = make_pair(pivotLatitude, pivotLongitude);
	}

	MESSAGE_DEBUG("", "", "finish (GPS lat/long = " + to_string(result.first) + " / "  + to_string(result.second) + ")");

	return result;
}


string MostCommentableMessages(string userID, vector<MessageType> myPostsYearAgo, CMysql *db)
{
	MessageType		yearAgoPostWithMostComments;
	int				yearAgoPostsWithCommentsCounter;
	string			messageList = "";
	string			result = "";

	{ MESSAGE_DEBUG("", "", "start"); }

	yearAgoPostWithMostComments.commentsValue = 0;

	for(auto &it: myPostsYearAgo)
	{
		if(messageList.length()) messageList += ",";
		messageList += it.messageID;
	}

	yearAgoPostsWithCommentsCounter = db->Query(
		" SELECT `messageID`, COUNT(`messageID`) AS `counter`"
		" FROM `feed_message_comment`"
		" WHERE"
		" `type`=\"message\""
		" AND"
		" `messageID` IN (" + messageList + ") GROUP BY `messageID`;"
		);

	if(yearAgoPostsWithCommentsCounter)
	{
		int		commentsMinValue = 0, commentsMaxValue = 0;

		for(int j = 0; j < yearAgoPostsWithCommentsCounter; ++j)
		{
			long int 		commentsValue = stol(db->Get(j, "counter"));

			if(commentsValue > yearAgoPostWithMostComments.commentsValue)
			{
				yearAgoPostWithMostComments.commentsValue = commentsValue;
				yearAgoPostWithMostComments.messageID = db->Get(j, "messageID");
			}
		}


		tie(commentsMinValue, commentsMaxValue) = GetMinMaxCommentsOfMessagesOverYear(userID, db);

		// --- if commentsMaxValue == commentsMinValue then popularity will get division to 0
		if(commentsMaxValue > commentsMinValue)
		{
			int	postPopularity = (yearAgoPostWithMostComments.commentsValue - commentsMinValue) / (commentsMaxValue - commentsMinValue) * 100;

			if(postPopularity > YEAR_AGO_REPOST_THRESHOLD)
			{
				// --- best messageID - yearAgoPostWithMostComments.messageID
				result = yearAgoPostWithMostComments.messageID;
				{ MESSAGE_DEBUG("", "", "winner is messageID[" + yearAgoPostWithMostComments.messageID + "] (min / curr / max = " + to_string(commentsMinValue) + " / " + to_string(yearAgoPostWithMostComments.commentsValue) + " / " + to_string(commentsMaxValue) + ")"); }
			}
		}
		else
		{
			MESSAGE_DEBUG("", "", "user[" + userID + "] (max # of comments / " + to_string(commentsMaxValue) + ") == (max # of comments / " + to_string(commentsMinValue) + ") over a year");
		}

	}
	else
	{
		{ MESSAGE_DEBUG("", "", "no userID[" + userID + "] commented messages year ago"); }
	}

	{ MESSAGE_DEBUG("", "", "finish (result = " + result + ")"); }

	return	result;
}

bool YearAgoReminder(CMysql *db)
{
	bool 				result = false;
	struct	UserClass
	{
		string	userID;
		int		commentsMaxValue;
		int		commentsMinValue;
	};
	vector<UserClass>		usersList;
	long int				itemsCount;

	{ MESSAGE_DEBUG("", "", "start"); }

	if((itemsCount = db->Query("SELECT `id` FROM `users` WHERE `isblocked`=\"N\" AND `isactivated`=\"Y\";")) > 0)
	{
		long int	usersCount = itemsCount;

		usersList.reserve(usersCount);

		for(int i = 0; i < usersCount; ++i)
		{
			UserClass	item;
			item.userID = db->Get(i, "id");
			usersList.push_back(item);
		}

		for(int i = 0; i < usersCount; ++i)
		{
			int					yearAgoPostsCounter;
			set<long int>		bestPostsSet;

			bestPostsSet.erase(bestPostsSet.begin(), bestPostsSet.end());

			// --- get posts from year ago sharp
			yearAgoPostsCounter = db->Query(
					" SELECT `actionId` as `messageID` FROM `feed` WHERE"
					" `actionTypeId`=\"11\""
					" AND"
					" `srcType`=\"user\""
					" AND"
					" `userId`=\"" + usersList[i].userID + "\""
					" AND"
					" `eventTimestamp`>=CAST(DATE_SUB(CURRENT_DATE, INTERVAL 1 YEAR) AS DATETIME)"
					" AND"
					" `eventTimestamp`<=CAST(DATE_ADD(DATE_SUB(CURRENT_DATE, INTERVAL 1 YEAR), INTERVAL 1 DAY) AS DATETIME);"
				);

			if(yearAgoPostsCounter)
			{
				vector<MessageType>		myPostsYearAgo;
				vector<MessageType>		postsOverYear;
				string					popularMessageID = "";


				for(int j = 0; j < yearAgoPostsCounter; ++j)
				{
					MessageType		tempObj;

					tempObj.messageID = db->Get(j, "messageID");
					myPostsYearAgo.push_back(tempObj);
				}

				// --- message popularity from comments perspective
				popularMessageID = MostCommentableMessages(usersList[i].userID, myPostsYearAgo, db);
				if(popularMessageID.length()) bestPostsSet.insert(stol(popularMessageID));

				// --- message importance from distance perspective
				{
					bool	isYearAgoMessageHasGPS = false;

					// --- define mean messages GPS positioning of a yearAgo message
					for(auto it = myPostsYearAgo.begin(); it != myPostsYearAgo.end(); ++it)
					{
						it->gps = GetMessageGPSCoordinates(it->messageID, db);

						if(it->gps.first && it->gps.second)	isYearAgoMessageHasGPS = true;
					}

					if(isYearAgoMessageHasGPS)
					{
						int						overYearMessageCounter;
						vector<LocationBubble>	overYearLocations;

						// --- get all messages over year
						overYearMessageCounter = db->Query(
							" SELECT `actionId` FROM `feed` WHERE"
							" `actionTypeId`=\"11\""
							" AND"
							" `srcType`=\"user\""
							" AND"
							" `userId`=\"" + usersList[i].userID + "\""
							" AND"
							" `eventTimestamp`>=CAST(DATE_SUB(CURRENT_DATE, INTERVAL 1 YEAR) AS DATETIME);"
							);
						if(overYearMessageCounter)
						{
							int			totalPresenceCounter = 0;

							for(int j = 0; j < overYearMessageCounter; ++j)
							{
								MessageType		tempObj;

								tempObj.messageID = db->Get(j, "actionId");
								postsOverYear.push_back(tempObj);
							}

							// --- you can NOT merge these two for-loops
							// --- GetMessageGPSCoordinates will break dialog with DB
							for(int j = 0; j < overYearMessageCounter; ++j)
								postsOverYear[j].gps = GetMessageGPSCoordinates(postsOverYear[j].messageID, db);

							// --- distribute user presence by places
							// --- shortest distance between points
							for(int j = 0; j < overYearMessageCounter; ++j)
							{
								bool	inBubble = false;

								for(unsigned int k = 0; k < overYearLocations.size() && !inBubble; ++k)
								{
									float	distance = (overYearLocations[k].center.first - postsOverYear[j].gps.first) * (overYearLocations[k].center.first - postsOverYear[j].gps.first) + (overYearLocations[k].center.second - postsOverYear[j].gps.second) * (overYearLocations[k].center.second - postsOverYear[j].gps.second);

									if(distance < overYearLocations[k].radius)
									{
										overYearLocations[k].counter++;
										inBubble = true;

										{ MESSAGE_DEBUG("", "RemoteLocationPopularity", "curr position (lat/long = " + to_string(postsOverYear[j].gps.first) + " / " + to_string(postsOverYear[j].gps.second) + ") belongs to presence bubble (lat/long = " + to_string(overYearLocations[k].center.first) + " / " + to_string(overYearLocations[k].center.second) + "). Total positions in this bubble is/are " + to_string(overYearLocations[k].counter)); }
									}
								}

								if(inBubble)
								{
								}
								else
								{
									if(postsOverYear[j].gps.first && postsOverYear[j].gps.second)
									{
										LocationBubble	newBubble;

										{ MESSAGE_DEBUG("", "RemoteLocationPopularity", "new presence bubble (lat/long = " + to_string(postsOverYear[j].gps.first) + " / " + to_string(postsOverYear[j].gps.second) + ")"); }

										newBubble.center = postsOverYear[j].gps;
										newBubble.radius = PRESENCE_DIAMETER;
										newBubble.counter = 1;

										overYearLocations.push_back(newBubble);
									}
									else
									{
										{ MESSAGE_DEBUG("", "RemoteLocationPopularity", "location with lat/long=0/0 out of scope"); }
									}
								}
							}

							// sort(overYearLocations.begin(), overYearLocations(end), [](auto &loc1, auto &loc2) { return loc1.counter < loc2.counter});
							for(auto it = overYearLocations.begin(); it != overYearLocations.end(); ++it)
							{
								totalPresenceCounter += it->counter;
							}

							// --- approximate yearAgo presence to all places
							for(auto it = myPostsYearAgo.begin(); it != myPostsYearAgo.end(); ++it)
							{
								bool	isFound = false;

								for(unsigned int k = 0; k < overYearLocations.size() && !isFound; ++k)
								{
									float	distance = (overYearLocations[k].center.first - it->gps.first) * (overYearLocations[k].center.first - it->gps.first) + (overYearLocations[k].center.second - it->gps.second) * (overYearLocations[k].center.second - it->gps.second);

									if(distance < overYearLocations[k].radius)
									{
										float	timeSpentInTHisPlace = overYearLocations[k].counter * 100 / totalPresenceCounter;
										
										if(timeSpentInTHisPlace < (100 - YEAR_AGO_REPOST_THRESHOLD))
										{
											{ MESSAGE_DEBUG("", "RemoteLocationPopularity", "re-posting messageID(" + it->messageID + ") because this is unusual place for user (" + to_string(int(timeSpentInTHisPlace)) + "% < threshold " + to_string(100 - YEAR_AGO_REPOST_THRESHOLD) + "%)"); }
											bestPostsSet.insert(stol(it->messageID));
										}
										else
										{
											{ MESSAGE_DEBUG("", "RemoteLocationPopularity", "no re-posting messageID(" + it->messageID + ") because user staying in this place too much time over year (" + to_string(int(timeSpentInTHisPlace)) + "% >= threshold " + to_string(100 - YEAR_AGO_REPOST_THRESHOLD) + "%)"); }
										}
									}
								}
							}
						}
						else
						{
							MESSAGE_DEBUG("", "RemoteLocationPopularity", "no messages over year posted by user[" + usersList[i].userID + "]");
						}
					}
					else
					{
						{
							MESSAGE_DEBUG("", "RemoteLocationPopularity", "yearAgo message of user[" + usersList[i].userID + "] have no GPS coordinates");
						}
					}
				} // --- scoping

				// --- repost messages to user feed
				for(auto it = bestPostsSet.begin(); it != bestPostsSet.end(); ++it)
				{
					if(db->InsertQuery("INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) VALUES(\"\",\"" + usersList[i].userID + "\", \"12\", \"" + to_string(*it) + "\", NOW());") )
					{

					}
					else
					{
						MESSAGE_ERROR("", "", " error inserting into `feed` table");
					}
				}
			}
			else
			{
				{ MESSAGE_DEBUG("", "", "user[" + usersList[i].userID + "] didn't post anything this day year ago"); }
			}
		}
	}
	else
	{ MESSAGE_DEBUG("", "", "no active users"); }

	{ MESSAGE_DEBUG("", "", "finish"); }

	return result;
}

auto RemoveYearAgoReminder(CMysql *db)
{
	auto	error_message = ""s;

	MESSAGE_DEBUG("", "", "start");

	db->Query("DELETE FROM `feed` WHERE `actionTypeId` = \"12\" AND eventTimestamp < DATE_SUB(NOW(), INTERVAL 1 MONTH);");

	MESSAGE_DEBUG("", "", "finish");

	return error_message;
}

int main()
{
	CStatistics		appStat;  // --- CStatistics must be a first statement to measure end2end param's
	c_config		config(CONFIG_DIR);
	CMysql			db;
	struct timeval	tv;

	MESSAGE_DEBUG("", "", "" + __FILE__);

	signal(SIGSEGV, crash_handler);

	gettimeofday(&tv, NULL);
	srand(tv.tv_sec * tv.tv_usec * 100000);    /* Flawfinder: ignore */

	try
	{
		auto	error_message = ""s;

		if(db.Connect(&config) < 0)
		{
			{ MESSAGE_ERROR("", "", "Can not connect to mysql database"); }
			throw CExceptionHTML("MySql connection");
		}

		//--- start of daily cron main functionality

		//--- notify friends about public birthday
		BirthdayNotificationToFriends(&db);

		//--- year ago activity remainder
		YearAgoReminder(&db);

		// --- Remove old messages "that happened 1 year ago"
		if((error_message = RemoveYearAgoReminder(&db)).length()) MESSAGE_ERROR("", "", error_message);

		//--- end of daily cron main functionality
	}
	catch(CExceptionHTML &c)
	{
		c.SetLanguage("ru");
		c.SetDB(&db);

		MESSAGE_DEBUG("", "", "catch CExceptionHTML: DEBUG exception reason: [" + c.GetReason() + "]");

		return(0);
	}
	catch(CException &c)
	{
		MESSAGE_ERROR("", "", "catch CException: exception: ERROR  " + c.GetReason());

		return(-1);
	}
	catch(exception& e)
	{
		MESSAGE_ERROR("", "", "catch(exception& e): catch standard exception: ERROR  " + e.what());

		return(-1);
	}

	return(0);
}

