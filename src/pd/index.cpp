#include "index.h"

// --- check that src image is actually image, resize it and save to dst
bool ImageSaveAsJpgToFeedFolder (const string src, const string dst, struct ExifInfo &exifInfo, c_config *config)
{
	MESSAGE_DEBUG("", "", "start (src = " + src + ", dst = " + dst + ")");

#ifndef IMAGEMAGICK_DISABLE
	// Construct the image object. Separating image construction FROM the
	// the read operation ensures that a failure to read the image file
	// doesn't render the image object useless.
	try {
		Magick::Image		   image;
		Magick::OrientationType imageOrientation;
		Magick::Geometry		imageGeometry;

		auto					image_max_width	= stod_noexcept(config->GetFromFile("image_max_width", "feed"));
		auto					image_max_height = stod_noexcept(config->GetFromFile("image_max_height", "feed"));		

		// Read a file into image object
		image.read( src );    /* Flawfinder: ignore */

		imageGeometry = image.size();
		imageOrientation = image.orientation();

		MESSAGE_DEBUG("", "", "imageOrientation = " + to_string(imageOrientation) + ", xRes = " + to_string(imageGeometry.width()) + ", yRes = " + to_string(imageGeometry.height()));

		if(imageOrientation == Magick::TopRightOrientation) image.flop();
		if(imageOrientation == Magick::BottomRightOrientation) image.rotate(180);
		if(imageOrientation == Magick::BottomLeftOrientation) { image.flop(); image.rotate(180); }
		if(imageOrientation == Magick::LeftTopOrientation) { image.flop(); image.rotate(-90); }
		if(imageOrientation == Magick::RightTopOrientation) image.rotate(90);
		if(imageOrientation == Magick::RightBottomOrientation) { image.flop(); image.rotate(90); }
		if(imageOrientation == Magick::LeftBottomOrientation) image.rotate(-90);

		if((imageGeometry.width() > image_max_width) || (imageGeometry.height() > image_max_height))
		{
			int   newHeight, newWidth;
			if(imageGeometry.width() >= imageGeometry.height())
			{
				newWidth = image_max_width;
				newHeight = newWidth * imageGeometry.height() / imageGeometry.width();
			}
			else
			{
				newHeight = image_max_height;
				newWidth = newHeight * imageGeometry.width() / imageGeometry.height();
			}

			image.resize(Magick::Geometry(newWidth, newHeight, 0, 0));
		}

		{
			// --- Exif information save
			exifInfo.DateTime = "";
			exifInfo.DateTime = image.attribute("exif:DateTime");
			exifInfo.GPSAltitude = "";
			exifInfo.GPSAltitude = image.attribute("exif:GPSAltitude");
			exifInfo.GPSLatitude = "";
			exifInfo.GPSLatitude = image.attribute("exif:GPSLatitude");
			exifInfo.GPSLongitude = "";
			exifInfo.GPSLongitude = image.attribute("exif:GPSLongitude");
			exifInfo.GPSSpeed = "";
			exifInfo.GPSSpeed = image.attribute("exif:GPSSpeed");
			exifInfo.Model = "";
			exifInfo.Model = image.attribute("exif:Model");
			exifInfo.Authors = "";
			exifInfo.Authors = image.attribute("exif:Authors");
			exifInfo.ApertureValue = "";
			exifInfo.ApertureValue = image.attribute("exif:ApertureValue");
			exifInfo.BrightnessValue = "";
			exifInfo.BrightnessValue = image.attribute("exif:BrightnessValue");
			exifInfo.ColorSpace = "";
			exifInfo.ColorSpace = image.attribute("exif:ColorSpace");
			exifInfo.ComponentsConfiguration = "";
			exifInfo.ComponentsConfiguration = image.attribute("exif:ComponentsConfiguration");
			exifInfo.Compression = "";
			exifInfo.Compression = image.attribute("exif:Compression");
			exifInfo.DateTimeDigitized = "";
			exifInfo.DateTimeDigitized = image.attribute("exif:DateTimeDigitized");
			exifInfo.DateTimeOriginal = "";
			exifInfo.DateTimeOriginal = image.attribute("exif:DateTimeOriginal");
			exifInfo.ExifImageLength = "";
			exifInfo.ExifImageLength = image.attribute("exif:ExifImageLength");
			exifInfo.ExifImageWidth = "";
			exifInfo.ExifImageWidth = image.attribute("exif:ExifImageWidth");
			exifInfo.ExifOffset = "";
			exifInfo.ExifOffset = image.attribute("exif:ExifOffset");
			exifInfo.ExifVersion = "";
			exifInfo.ExifVersion = image.attribute("exif:ExifVersion");
			exifInfo.ExposureBiasValue = "";
			exifInfo.ExposureBiasValue = image.attribute("exif:ExposureBiasValue");
			exifInfo.ExposureMode = "";
			exifInfo.ExposureMode = image.attribute("exif:ExposureMode");
			exifInfo.ExposureProgram = "";
			exifInfo.ExposureProgram = image.attribute("exif:ExposureProgram");
			exifInfo.ExposureTime = "";
			exifInfo.ExposureTime = image.attribute("exif:ExposureTime");
			exifInfo.Flash = "";
			exifInfo.Flash = image.attribute("exif:Flash");
			exifInfo.FlashPixVersion = "";
			exifInfo.FlashPixVersion = image.attribute("exif:FlashPixVersion");
			exifInfo.FNumber = "";
			exifInfo.FNumber = image.attribute("exif:FNumber");
			exifInfo.FocalLength = "";
			exifInfo.FocalLength = image.attribute("exif:FocalLength");
			exifInfo.FocalLengthIn35mmFilm = "";
			exifInfo.FocalLengthIn35mmFilm = image.attribute("exif:FocalLengthIn35mmFilm");
			exifInfo.GPSDateStamp = "";
			exifInfo.GPSDateStamp = image.attribute("exif:GPSDateStamp");
			exifInfo.GPSDestBearing = "";
			exifInfo.GPSDestBearing = image.attribute("exif:GPSDestBearing");
			exifInfo.GPSDestBearingRef = "";
			exifInfo.GPSDestBearingRef = image.attribute("exif:GPSDestBearingRef");
			exifInfo.GPSImgDirection = "";
			exifInfo.GPSImgDirection = image.attribute("exif:GPSImgDirection");
			exifInfo.GPSImgDirectionRef = "";
			exifInfo.GPSImgDirectionRef = image.attribute("exif:GPSImgDirectionRef");
			exifInfo.GPSInfo = "";
			exifInfo.GPSInfo = image.attribute("exif:GPSInfo");
			exifInfo.GPSTimeStamp = "";
			exifInfo.GPSTimeStamp = image.attribute("exif:GPSTimeStamp");
			exifInfo.ISOSpeedRatings = "";
			exifInfo.ISOSpeedRatings = image.attribute("exif:ISOSpeedRatings");
			exifInfo.JPEGInterchangeFormat = "";
			exifInfo.JPEGInterchangeFormat = image.attribute("exif:JPEGInterchangeFormat");
			exifInfo.JPEGInterchangeFormatLength = "";
			exifInfo.JPEGInterchangeFormatLength = image.attribute("exif:JPEGInterchangeFormatLength");
			exifInfo.Make = "";
			exifInfo.Make = image.attribute("exif:Make");
			exifInfo.MeteringMode = "";
			exifInfo.MeteringMode = image.attribute("exif:MeteringMode");
			exifInfo.Orientation = "";
			exifInfo.Orientation = image.attribute("exif:Orientation");
			exifInfo.ResolutionUnit = "";
			exifInfo.ResolutionUnit = image.attribute("exif:ResolutionUnit");
			exifInfo.SceneCaptureType = "";
			exifInfo.SceneCaptureType = image.attribute("exif:SceneCaptureType");
			exifInfo.SceneType = "";
			exifInfo.SceneType = image.attribute("exif:SceneType");
			exifInfo.SensingMethod = "";
			exifInfo.SensingMethod = image.attribute("exif:SensingMethod");
			exifInfo.ShutterSpeedValue = "";
			exifInfo.ShutterSpeedValue = image.attribute("exif:ShutterSpeedValue");
			exifInfo.Software = "";
			exifInfo.Software = image.attribute("exif:Software");
			exifInfo.SubjectArea = "";
			exifInfo.SubjectArea = image.attribute("exif:SubjectArea");
			exifInfo.SubSecTimeDigitized = "";
			exifInfo.SubSecTimeDigitized = image.attribute("exif:SubSecTimeDigitized");
			exifInfo.SubSecTimeOriginal = "";
			exifInfo.SubSecTimeOriginal = image.attribute("exif:SubSecTimeOriginal");
			exifInfo.WhiteBalance = "";
			exifInfo.WhiteBalance = image.attribute("exif:WhiteBalance");
			exifInfo.XResolution = "";
			exifInfo.XResolution = image.attribute("exif:XResolution");
			exifInfo.YCbCrPositioning = "";
			exifInfo.YCbCrPositioning = image.attribute("exif:YCbCrPositioning");
			exifInfo.YResolution = "";
			exifInfo.YResolution = image.attribute("exif:YResolution");

			exifInfo.GPSAltitude = image.attribute("exif:GPSAltitudeRef") + ": " + exifInfo.GPSAltitude;
			exifInfo.GPSLatitude = image.attribute("exif:GPSLatitudeRef") + ": " + exifInfo.GPSLatitude;
			exifInfo.GPSLongitude = image.attribute("exif:GPSLongitudeRef") + ": " + exifInfo.GPSLongitude;
			exifInfo.GPSSpeed = image.attribute("exif:GPSSpeedRef") + ": " + exifInfo.GPSSpeed;

			image.strip();
		}


		// Write the image to a file
		image.write( dst );
	}
	catch( Magick::Exception &error_ )
	{
		MESSAGE_ERROR("", "", "exception in read/write operation [" + error_.what() + "]");

		return false;
	}

	MESSAGE_DEBUG("", "", "finish (image has been successfully converted to .jpg format)");

	return true;
#else

	CopyFile(src, dst);

	MESSAGE_DEBUG("", "", "finish (simple file coping cause ImageMagick++ is not activated)");
	
	return  true;
#endif
}



// --- Generating image with text "randStr"
// --- Input: randStr - text needs to be written on the image
// --- Output: path to the file
string GenerateImage(string randStr)
{
	string	fileName = "dessin.gif", fileFont, fileResultFull, fileResult;
	string	annotateFlag = "yes";

	fileName = IMAGE_DIRECTORY;
	fileName += "pages/login/dessin.gif";

	if(!fileName.empty())
	{
		if(annotateFlag == "yes")
		{
			string				fileNameWater;
			Magick::Image		imageMaster, imageDest;
			// ImageInfo	*image1_info, *anotherInfo;
			// char			geometry[128];
			ostringstream 	ost;

			Magick::InitializeMagick(NULL);

			try 
			{
				bool 		fileFlagExist;

				imageMaster.read(fileName);    /* Flawfinder: ignore */
				imageDest = imageMaster;
				imageDest.fontPointsize(14);
				imageDest.addNoise(Magick::GaussianNoise);
				imageDest.addNoise(Magick::GaussianNoise);
				ost.str("");
				ost << "+" << 1 + (int)(rand()/(RAND_MAX + 1.0) * 45) << "+" << 13 + (int)(rand()/(RAND_MAX + 1.0) * 10);
				imageDest.annotate(randStr, Magick::Geometry(ost.str()));

			
				fileFlagExist = true;	
				do {
					MESSAGE_DEBUG("", "", "checking captcha file existence")

					fileResult = "_";
					fileResult += GetRandom(10);
					fileResult += ".gif";
					fileResultFull = IMAGE_CAPTCHA_DIRECTORY;
					fileResultFull += fileResult;
					auto fh = open(fileResultFull.c_str(), O_RDONLY);    /* Flawfinder: ignore */
					if(fh < 0) 
					{
						fileFlagExist = false;

						MESSAGE_DEBUG("", "", "GenerateImage: trying file " + fileResultFull + " -> can be used for writing");
					}
					else 
					{ 
						close(fh); 

						MESSAGE_DEBUG("", "", "GenerateImage: trying file " + fileResultFull + " -> can't be used, needs another one");
					}
				} while(fileFlagExist == true);


				MESSAGE_DEBUG("", "", "GenerateImage: write captcha-image to " + fileResultFull);

				imageDest.write(fileResultFull);
			}
			catch(Magick::Exception &error_)
			{
				MESSAGE_ERROR("", "", "Caught exception: " + error_.what())
				fileResult = "";		
			}
		}
	}
	return fileResult;
}

int main()
{
	CStatistics		appStat;  // --- CStatistics must be a first statement to measure end2end param's
	CCgi			indexPage(EXTERNAL_TEMPLATE);
	CUser			user;
	c_config		config(CONFIG_DIR);
	string			action = "";
	CMysql			db;
	struct timespec	tv;

	MESSAGE_DEBUG("", action, " " + __FILE__);

	signal(SIGSEGV, crash_handler); 

	timespec_get(&tv, TIME_UTC);
	srand(tv.tv_nsec ^ tv.tv_sec);    /* Flawfinder: ignore */

	try
	{

		indexPage.ParseURL();
		indexPage.AddCookie("lng", "ru", nullptr, "", "/");

		if(!indexPage.SetTemplate("index.htmlt"))
		{
			MESSAGE_ERROR("", action, "template file was missing");
			throw CException("Template file was missing");
		}

		if(db.Connect(&config) < 0)
		{
			MESSAGE_ERROR("", action, "Can not connect to mysql database");
			throw CExceptionHTML("MySql connection");
		}

		indexPage.SetDB(&db);

		action = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("action"));
		action = action.substr(0, 128);
		
		MESSAGE_DEBUG("", action, "action taken from HTTP is " + action);

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

			//------- Cleanup data
			db.Query("DELETE FROM `captcha` WHERE `timestamp`<=(NOW()-INTERVAL " + to_string(SESSION_LEN) + " MINUTE);");
		}
		// ------------ end generate common parts

		MESSAGE_DEBUG("", action, "pre-condition if(action == \"" + action + "\")");

		if((action.length() > 10) && (action.compare(action.length() - 9, 9, "_template") == 0))
		{
			ostringstream	ost;
			string			strPageToGet, strFriendsOnSinglePage;

			MESSAGE_DEBUG("", action, "start");

			string		template_name = action.substr(0, action.length() - 9) + ".htmlt";

			if(!indexPage.SetTemplate(template_name))
			{
				MESSAGE_DEBUG("", action, "can't find template " + template_name);
			} // if(!indexPage.SetTemplate("my_network.htmlt"))

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "setlang")
		{
			string		lng;

			lng = indexPage.GetVarsHandler()->Get("lng");
			indexPage.AddCookie("lng", lng, nullptr, "", "/");

			if(!indexPage.SetTemplate("index.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file index.htmlt was missing");
				throw CException("Template file was missing");
			}
		}

		if(action == "autologin")
		{
			MESSAGE_DEBUG("", action, "start");

			if(!indexPage.SetTemplate("autologin.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file index.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}



		// --- add to "home screen" on iPhone.
		// --- this functionality has not been tested
		if(action == "add_to_home_screen")
		{
			string		lng;

			if(!indexPage.SetTemplate("add_to_home_screen.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file index.htmlt was missing");
				throw CException("Template file was missing");
			}
		}


		// --- JSON part has started

		if(action == "AJAX_getUserWall")
		{
			MESSAGE_DEBUG("", action, "start");

			ostringstream   ost;
			auto		 	currPage = 0, newsOnSinglePage = 0;
			vector<int>		vectorFriendList;
			auto			success_message = ""s;
			auto			error_message = ""s;
			auto			result = ""s;

			auto			userID				= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("id"));
			auto			userLogin			= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("login"));
			auto			strNewsOnSinglePage = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("NewsOnSinglePage"));
			auto			strPageToGet		= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("page"));

			if(strPageToGet.empty()) strPageToGet = "0";

			MESSAGE_DEBUG("", action, "page " + strPageToGet + " requested");

			try{
				currPage = stoi(strPageToGet);
			} catch(...) {
				currPage = 0;
			}

			try{
				newsOnSinglePage = stoi(strNewsOnSinglePage);
			} catch(...) {
				newsOnSinglePage = 30;
			}

/*
			if(user.GetLogin() == "Guest")
			{
				ostringstream   ost;

				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}
*/

			// --- define userID from userLogin
			if(userID.length())
			{

			}
			else if(userLogin.length())
			{
				if(db.Query("SELECT `id` FROM `users` WHERE `login`=\"" + userLogin + "\" AND `isblocked`=\"N\";"))
					userID = db.Get(0, "id");
				else
				{
					MESSAGE_DEBUG("", action, "attempt to get a wall of unknown/blocked user (" + userLogin + ")");
				}
			}
			else
			{
				userID = user.GetID();
			}

			if(userID.length() && db.Query("SELECT `id` FROM `users` WHERE `id`=\"" + userID + "\" AND `isblocked`=\"N\";"))
			{
				auto		whereQuery = "((`feed`.`userId` in (" + userID + ")) AND (`feed`.`srcType` = \"user\") AND (`feed`.`dstType` = \"\"))";

				success_message = "\"feed\":[" + GetNewsFeedInJSONFormat(whereQuery, currPage, newsOnSinglePage, &user, &db) + "]";
			}
			else
			{
				error_message = gettext("user blocked or not found");
				MESSAGE_DEBUG("", action, "attempt to get a wall of unknown/blocked user (" + userLogin + ")");
			}

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);

			MESSAGE_DEBUG("", action, "finish");
		}



		// --- JSON news feed
		if(action == "AJAX_getNewsFeed")
		{
			MESSAGE_DEBUG("", action, "start");
			
			auto		strPageToGet = ""s;
			auto		strNewsOnSinglePage = ""s;
			auto		error_message = ""s;
			auto		success_message = ""s;
			auto		currPage = 0, newsOnSinglePage = 0;

			strNewsOnSinglePage = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("NewsOnSinglePage"));
			strPageToGet		= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("page"));

			if(strPageToGet.empty()) strPageToGet = "0";

			try{
				currPage = stoi(strPageToGet);
			} catch(...) {
				currPage = 0;
			}

			try{
				newsOnSinglePage = stoi(strNewsOnSinglePage);
			} catch(...) {
				newsOnSinglePage = NEWS_ON_SINGLE_PAGE;
			}

			MESSAGE_DEBUG("", action, "page " + strPageToGet + " requested");

			if(user.GetLogin() == "Guest")
			{
				error_message = gettext("re-login required");
				MESSAGE_DEBUG("", action, error_message);
			}
			else
			{
				auto			sql_query =
								"SELECT `users_friends`.`friendID` "s
									"FROM `users_friends` "
									"LEFT JOIN `users` ON `users`.`id`=`users_friends`.`friendID` "
									"WHERE `users_friends`.`userID`='" + user.GetID() + "' and `users_friends`.`state`=\"confirmed\" and `users`.`isactivated`=\"Y\" and `users`.`isblocked`=\"N\";";
				auto			vectorFriendList		= GetValuesFromDB(sql_query, &db);
								vectorFriendList.push_back(user.GetID());
								
				auto			companies_vector		= GetValuesFromDB("SELECT `id` FROM `company` WHERE `isBlocked`=\"N\" AND `id` IN (SELECT `entity_id` FROM `users_subscriptions` WHERE `user_id`=\"" + user.GetID() + "\" AND `entity_type`=\"company\");", &db);
				auto			groups_vector			= GetValuesFromDB("SELECT `id` FROM `groups` WHERE `isBlocked`=\"N\" AND `id` IN (SELECT `entity_id` FROM `users_subscriptions` WHERE `user_id`=\"" + user.GetID() + "\" AND `entity_type`=\"group\");", &db);
				auto			companies_i_own_vector	= GetValuesFromDB("SELECT `id` FROM `company` WHERE `isBlocked`=\"N\" AND `admin_userID`=\"" + user.GetID() + "\";", &db);
				auto			where_query				= 
															"((`feed`.`userId` in (" + join(vectorFriendList, ",") + ")) AND (`feed`.`srcType` = \"user\") AND (`feed`.`dstType` = \"\"))"
															+ (companies_vector.size() ? " OR ((`feed`.`userId` in (" + join(companies_vector, ",") + ")) AND (`feed`.`srcType` = \"company\"))" : "")
															+ (groups_vector.size() ? " OR ((`feed`.`dstID` in (" + join(groups_vector, ",") + ")) AND (`feed`.`dstType` = \"group\"))" : "")
															;
				success_message	=
									"\"my_companies\":[" + join(companies_i_own_vector, ",") + "],"
									"\"feed\":[" + GetNewsFeedInJSONFormat(where_query, currPage, newsOnSinglePage, &user, &db) + "]"
									;
			}
			AJAX_ResponseTemplate(&indexPage, success_message, error_message);
		}

		// --- AJAX delete message FROM news feed
		if(action == "AJAX_deleteNewsFeedMessage")
		{

			ostringstream	ostFinal;
			CMysql			db1;
			string			messageID;

			messageID	= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("messageID"));

			MESSAGE_DEBUG("", action, "start [" + messageID + "]");

			if(messageID.empty())
			{
				MESSAGE_DEBUG("", action, "messageID is not defined");

				ostFinal.str("");
				ostFinal << "{";
				ostFinal << "\"result\" : \"error\",";
				ostFinal << "\"description\" : \"messageID is empty\"";
				ostFinal << "}";
			}
			else
			{
				if(user.GetLogin() == "Guest")
				{
					ostringstream	ost;

					MESSAGE_DEBUG("", action, "re-login required");

					indexPage.Redirect("/autologin?rand=" + GetRandom(10));
				}


				if(AmIMessageOwner(messageID, &user, &db))
				{
					pair<string, string> 	messageOwner = GetMessageOwner(messageID, &user, &db);
					string					messageOwnerType = messageOwner.first;
					string					messageOwnerID = messageOwner.second;

					if(messageOwnerType.length() && messageOwnerID.length())
					{
						// --- delete original message
						db.Query("DELETE FROM `feed` WHERE `srcType`=\"" + messageOwnerType + "\" AND `userId`=\"" + messageOwnerID + "\" AND `actionTypeId`=\"11\" and `actionId`=\"" + messageID + "\";");
						// --- delete reposted messages over a year
						db.Query("DELETE FROM `feed` WHERE `srcType`=\"" + messageOwnerType + "\" AND `userId`=\"" + messageOwnerID + "\" AND `actionTypeId`=\"12\" and `actionId`=\"" + messageID + "\";");

						if(db.Query("SELECT `imageSetID` FROM `feed_message` WHERE `id`='" + messageID + "';"))
						{
							ostringstream	ost;
							string			imageSetID = db.Get(0, "imageSetID");

							ost.str("");
							ost << " `setID`='" << imageSetID << "' AND `srcType`=\"" + messageOwnerType + "\" AND `userID`=\"" << messageOwnerID << "\" ";
							RemoveMessageImages(ost.str(), &db);
						} // --- if ("SELECT * FROM `feed_message` WHERE `id`='" << messageID << "';";)

						db.Query("DELETE FROM `feed_message` WHERE `id`=\"" + messageID + "\";");

						// --- removing likes / dislikes and notifications
						db.Query("DELETE FROM `users_notification` WHERE `actionTypeId`=\"50\" and `actionId`='" + messageID + "';");
						db.Query("DELETE FROM `users_notification` WHERE `actionTypeId`=\"49\" and `actionId` in (SELECT `id` FROM `feed_message_params` WHERE `messageID`='" + messageID + "' and `parameter`=\"like\");");
						db.Query("DELETE FROM `feed_message_params` WHERE `messageID`='" + messageID + "';");

						// --- removing comments and notifications
						db.Query("DELETE FROM `users_notification` WHERE `actionTypeId`=\"19\" and `actionId` in (SELECT `id` FROM `feed_message_comment` WHERE `messageID`='" + messageID + "' and `type`=\"message\");");
						db.Query("DELETE FROM `feed_message_comment` WHERE `messageID`='" + messageID + "' and `type`=\"message\";");

						ostFinal.str("");
						ostFinal << "{";
						ostFinal << "\"result\" : \"success\",";
						ostFinal << "\"description\" : \"\"";
						ostFinal << "}";
					}
					else
					{
						MESSAGE_ERROR("", action, "message owner error (type:" + messageOwnerType + ", id:" + messageOwnerID + ")");

						ostFinal.str("");
						ostFinal << "{";
						ostFinal << "\"result\" : \"error\",";
						ostFinal << "\"description\" : \"Ошибка: Не получилось определить владельца сообщения\"";
						ostFinal << "}";
					}
				}
				else
				{
					MESSAGE_ERROR("", action, "message doesn't belongs to you");

					ostFinal.str("");
					ostFinal << "{";
					ostFinal << "\"result\" : \"error\",";
					ostFinal << "\"description\" : \"Ошибка: Вы не можете удалить чужое сообщение.\"";
					ostFinal << "}";
				}
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			{
				MESSAGE_DEBUG("", action, "end (messageID " + messageID + ")");
			}

		}

		// --- AJAX delete comment FROM message
		if(action == "AJAX_deleteNewsFeedComment")
		{
			ostringstream	ost, ostFinal;
			CMysql			db1;
			string			commentID;

			commentID	= indexPage.GetVarsHandler()->Get("commentID");

			{
				MESSAGE_DEBUG("", action, "start [" + commentID + "]");
			}


			if(commentID.empty())
			{
				MESSAGE_DEBUG("", action, "commentID is not defined");

				ostFinal.str("");
				ostFinal << "{" << std::endl;
				ostFinal << "\"result\" : \"error\"," << std::endl;
				ostFinal << "\"description\" : \"commentID is empty\"" << std::endl;
				ostFinal << "}" << std::endl;
			}
			else
			{
				if(user.GetLogin() == "Guest")
				{
					ostringstream	ost;

					MESSAGE_DEBUG("", action, "re-login required");

					indexPage.Redirect("/autologin?rand=" + GetRandom(10));
				}

				ost.str("");
				ost << "DELETE FROM `feed_message_comment` WHERE `id`='" << commentID << "' and `userID`='" << user.GetID() << "';";
				db.Query(ost.str());

				ost.str("");
				ost << "DELETE FROM `users_notification` WHERE `actionId`='" << commentID << "' and `actionTypeId`=\"19\";";
				db.Query(ost.str());

				ostFinal.str("");
				ostFinal << "{" << std::endl;
				ostFinal << "\"result\" : \"success\"," << std::endl;
				ostFinal << "\"description\" : \"\"" << std::endl;
				ostFinal << "}" << std::endl;
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}
		}

		// --- AJAX delete comment FROM message
		if(action == "AJAX_removeCompanyExperience")
		{
			ostringstream	ost, ostFinal;
			CMysql			db1;
			string			expID;

			expID	= indexPage.GetVarsHandler()->Get("id");

			MESSAGE_DEBUG("", action, "start");


			if(expID.empty())
			{
				MESSAGE_DEBUG("", action, "expID is not defined");

				ostFinal.str("");
				ostFinal << "{" << std::endl;
				ostFinal << "\"result\" : \"error\"," << std::endl;
				ostFinal << "\"description\" : \"expID is empty\"" << std::endl;
				ostFinal << "}" << std::endl;
			}
			else
			{
				string	  companyID = "";

				if(user.GetLogin() == "Guest")
				{
					ostringstream	ost;

					MESSAGE_DEBUG("", action, "re-login required");

					indexPage.Redirect("/autologin?rand=" + GetRandom(10));
				}

				if(db.Query("select * from `users_company` where `id`=\"" + expID + "\";"))
					companyID = db.Get(0, "company_id");

				// --- delete notifications on likesCourse
				db.Query("DELETE FROM `users_notification` WHERE `actionTypeId`=\"49\" and `actionId` in (SELECT `id` FROM `feed_message_params` WHERE `messageID`=\"" + expID + "\" and `parameter`=\"likeCompany\");");
				// --- delete notifications on comment with type "certifications"
				if(companyID.length() && (companyID != "0"))
					db.Query("DELETE FROM `users_notification` WHERE `actionTypeId`=\"19\" and `actionId` in (SELECT `id` FROM `feed_message_comment` WHERE `messageID`=\"" + companyID + "\" and `type`=\"company\") AND `userId`=\"" + user.GetID() + "\";");

				db.Query("DELETE FROM `feed_message_params`  WHERE `messageID`=\"" + expID + "\" and `parameter`=\"likeCompany\";");
				// db.Query("DELETE FROM `feed_message_comment` WHERE `messageID`=\"" + expID + "\" and `type`=\"company\";");
				db.Query("DELETE FROM `users_company` WHERE `id`='" + expID + "' AND `user_id`='" + user.GetID() + "';");
				db.Query("DELETE FROM `feed` WHERE `actionTypeId`=\"1\"  AND `actionId`=\"" + expID + "\" AND `userId`=\"" + user.GetID() + "\";");
				db.Query("DELETE FROM `feed` WHERE `actionTypeId`=\"2\"  AND `actionId`=\"" + expID + "\" AND `userId`=\"" + user.GetID() + "\";");
				db.Query("DELETE FROM `feed` WHERE `actionTypeId`=\"3\"  AND `actionId`=\"" + expID + "\" AND `userId`=\"" + user.GetID() + "\";");
				db.Query("DELETE FROM `feed` WHERE `actionTypeId`=\"20\" AND `actionId`=\"" + expID + "\" AND `userId`=\"" + user.GetID() + "\";");
				db.Query("DELETE FROM `feed` WHERE `actionTypeId`=\"51\" AND `actionId`=\"" + expID + "\" AND `userId`=\"" + user.GetID() + "\";");
				db.Query("DELETE FROM `feed` WHERE `actionTypeId`=\"52\" AND `actionId`=\"" + expID + "\" AND `userId`=\"" + user.GetID() + "\";");

				ostFinal.str("");
				ostFinal << "{" << std::endl;
				ostFinal << "\"result\" : \"success\"," << std::endl;
				ostFinal << "\"description\" : \"\"" << std::endl;
				ostFinal << "}" << std::endl;
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		// --- AJAX remove certification path
		if(action == "AJAX_removeCertificationEntry")
		{
			ostringstream	ost, ostFinal;
			CMysql			db1;
			string		  usersCertificationID;

			usersCertificationID	= indexPage.GetVarsHandler()->Get("id");

			MESSAGE_DEBUG("", action, "start");


			if(usersCertificationID.empty())
			{
				MESSAGE_DEBUG("", action, "usersCertificationID is not defined");

				ostFinal.str("");
				ostFinal << "{" << std::endl;
				ostFinal << "\"result\" : \"error\"," << std::endl;
				ostFinal << "\"description\" : \"usersCertificationID is empty\"" << std::endl;
				ostFinal << "}" << std::endl;
			}
			else
			{
				string			certificationID = "";

				if(user.GetLogin() == "Guest")
				{
					ostringstream	ost;

					MESSAGE_DEBUG("", action, "re-login required");

					indexPage.Redirect("/autologin?rand=" + GetRandom(10));
				}

				if(db.Query("select * from `users_certifications` where `id`=\"" + usersCertificationID + "\";"))
					certificationID = db.Get(0, "track_id");

				// --- delete notifications on likesCertifications
				db.Query("DELETE FROM `users_notification` WHERE `actionTypeId`=\"49\" and `actionId` in (SELECT `id` FROM `feed_message_params` WHERE `messageID`=\"" + usersCertificationID + "\" and `parameter`=\"likeCertification\");");

				// --- delete notifications on comment with type "certifications"
				if(certificationID.length() && (certificationID != "0"))
					db.Query("DELETE FROM `users_notification` WHERE `actionTypeId`=\"19\" AND `actionId` in (SELECT `id` FROM `feed_message_comment` WHERE `messageID`=\"" + certificationID + "\" AND `type`=\"certification\") AND `userId`=\"" + user.GetID() + "\";");

				db.Query("DELETE FROM `feed_message_params` WHERE `messageID`=\"" + usersCertificationID + "\" AND `parameter`=\"likeCertification\";");
				// --- centralized comments should not be removed
				// db.Query("DELETE FROM `feed_message_comment` WHERE `messageID`=\"" + usersCertificationID + "\" AND `type`=\"certification\";");
				db.Query("DELETE FROM `users_certifications` WHERE `id`=\"" + usersCertificationID + "\" AND `user_id`=\"" + user.GetID() + "\";");
				db.Query("DELETE FROM `feed` WHERE `actionTypeId`=\"22\" AND `actionId`=\"" + usersCertificationID + "\" AND `userId`=\"" + user.GetID() + "\";");
				db.Query("DELETE FROM `feed` WHERE `actionTypeId`=\"24\" AND `actionId`=\"" + usersCertificationID + "\" AND `userId`=\"" + user.GetID() + "\";");
				db.Query("DELETE FROM `feed` WHERE `actionTypeId`=\"26\" AND `actionId`=\"" + usersCertificationID + "\" AND `userId`=\"" + user.GetID() + "\";");

				ostFinal.str("");
				ostFinal << "{" << std::endl;
				ostFinal << "\"result\" : \"success\"," << std::endl;
				ostFinal << "\"description\" : \"\"" << std::endl;
				ostFinal << "}" << std::endl;
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		// --- remove applied vacancies entry
		if(action == "AJAX_removeAppliedVacancyEntry")
		{
			ostringstream   ostResult;

			MESSAGE_DEBUG("", action, "start");

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				ostResult << "{\"result\":\"error\",\"description\":\"сессия закончилась, необходимо вновь зайти на сайт\"}";
			}
			else
			{
				string		id = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

				if(db.Query("SELECT * FROM `company_candidates` WHERE `id`=\"" + id + "\" AND `user_id`=\"" + user.GetID() + "\";"))
				{
					// --- IMPORTANT !!! Don't change order.
					// --- delete user notifications
					db.Query("DELETE FROM `users_notification` WHERE (`actionTypeId`=\"59\") AND (`actionID`=\"" + id + "\");");
					// --- delete applied/rejected candidates
					db.Query("DELETE FROM `company_candidates` WHERE `id`=\"" + id + "\";");

					ostResult << "{\"result\":\"success\"}";
				}
				else
				{
					MESSAGE_ERROR("", action, "openVacancyID[" + id + "] not found in DB");

					ostResult << "{\"result\":\"error\",\"description\":\"Вакансия не найдена\"}";
				}
			}

			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file json_response.htmlt was missing");
			}  // if(!indexPage.SetTemplate("AJAX_precreateNewOpenVacancy.htmlt"))

			MESSAGE_DEBUG("", action, "start");
		}

		// --- AJAX remove course path
		if(action == "AJAX_removeCourseEntry")
		{
			ostringstream	ost, ostFinal;
			CMysql			db1;
			string			usersCourseID;

			usersCourseID	= indexPage.GetVarsHandler()->Get("id");

			MESSAGE_DEBUG("", action, "start");


			if(usersCourseID.empty())
			{
				MESSAGE_DEBUG("", action, "usersCourseID is not defined");

				ostFinal.str("");
				ostFinal << "{" << std::endl;
				ostFinal << "\"result\" : \"error\"," << std::endl;
				ostFinal << "\"description\" : \"usersCourseID is empty\"" << std::endl;
				ostFinal << "}" << std::endl;
			}
			else
			{
				string	  courseID = "";

				if(user.GetLogin() == "Guest")
				{
					ostringstream	ost;

					MESSAGE_DEBUG("", action, "re-login required");

					indexPage.Redirect("/autologin?rand=" + GetRandom(10));
				}

				if(db.Query("select * from `users_courses` where `id`=\"" + usersCourseID + "\";"))
					courseID = db.Get(0, "track_id");

				// --- delete notifications on likesCourse
				db.Query("DELETE FROM `users_notification` WHERE `actionTypeId`=\"49\" and `actionId` in (SELECT `id` FROM `feed_message_params` WHERE `messageID`=\"" + usersCourseID + "\" and `parameter`=\"likeCourse\");");

				// --- delete notifications on comment with type "certifications"
				if(courseID.length() && (courseID != "0"))
					db.Query("DELETE FROM `users_notification` WHERE `actionTypeId`=\"19\" and `actionId` in (SELECT `id` FROM `feed_message_comment` WHERE `messageID`=\"" + courseID + "\" and `type`=\"course\") AND `userId`=\"" + user.GetID() + "\";");

				db.Query("DELETE FROM `feed_message_params` WHERE `messageID`=\"" + usersCourseID + "\" and `parameter`=\"likeCourse\";");
				db.Query("DELETE FROM `feed_message_comment` WHERE `messageID`=\"" + usersCourseID + "\" and `type`=\"course\";");
				db.Query("DELETE FROM `users_courses`		WHERE `id`=\"" + usersCourseID +		"\" and `user_id`=\"" + user.GetID() + "\";");
				db.Query("DELETE FROM `feed` WHERE `actionTypeId`=\"23\" AND `actionId`=\"" + usersCourseID + "\" AND `userId`=\"" + user.GetID() + "\";");
				db.Query("DELETE FROM `feed` WHERE `actionTypeId`=\"25\" AND `actionId`=\"" + usersCourseID + "\" AND `userId`=\"" + user.GetID() + "\";");
				db.Query("DELETE FROM `feed` WHERE `actionTypeId`=\"57\" AND `actionId`=\"" + usersCourseID + "\" AND `userId`=\"" + user.GetID() + "\";");

				ostFinal.str("");
				ostFinal << "{" << std::endl;
				ostFinal << "\"result\" : \"success\"," << std::endl;
				ostFinal << "\"description\" : \"\"" << std::endl;
				ostFinal << "}" << std::endl;
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		// --- AJAX remove school path
		if(action == "AJAX_removeSchoolEntry")
		{
			ostringstream	ost, ostFinal;
			CMysql			db1;
			string			usersSchoolID;

			usersSchoolID	= indexPage.GetVarsHandler()->Get("id");

			MESSAGE_DEBUG("", action, "start");


			if(usersSchoolID.empty())
			{
				MESSAGE_DEBUG("", action, "usersSchoolID is not defined");

				ostFinal.str("");
				ostFinal << "{" << std::endl;
				ostFinal << "\"result\" : \"error\"," << std::endl;
				ostFinal << "\"description\" : \"usersSchoolID is empty\"" << std::endl;
				ostFinal << "}" << std::endl;
			}
			else
			{
				if(user.GetLogin() == "Guest")
				{
					ostringstream	ost;

					MESSAGE_DEBUG("", action, "re-login required");

					indexPage.Redirect("/autologin?rand=" + GetRandom(10));
				}

				// --- delete notifications on likesCourse
				db.Query("DELETE FROM `users_notification` WHERE `actionTypeId`=\"49\" and `actionId` in (SELECT `id` FROM `feed_message_params` WHERE `messageID`=\"" + usersSchoolID + "\" and `parameter`=\"likeSchool\");");
				// --- delete notifications on comment with type "certifications"
				db.Query("DELETE FROM `users_notification` WHERE `actionTypeId`=\"19\" and `actionId` in (SELECT `id` FROM `feed_message_comment` WHERE `messageID`=\"" + usersSchoolID + "\" and `type`=\"school\");");

				db.Query("DELETE FROM `feed_message_params`  WHERE `messageID`=\"" + usersSchoolID + "\" and `parameter`=\"likeSchool\";");
				db.Query("DELETE FROM `feed_message_comment` WHERE `messageID`=\"" + usersSchoolID + "\" and `type`=\"school\";");
				db.Query("DELETE FROM `users_school` WHERE `id`='" + usersSchoolID + "' and `user_id`='" + user.GetID() + "';");
				db.Query("DELETE FROM `feed` WHERE `actionTypeId`=\"27\" AND `actionId`=\"" + usersSchoolID + "\" AND `userId`=\"" + user.GetID() + "\";");
				db.Query("DELETE FROM `feed` WHERE `actionTypeId`=\"30\" AND `actionId`=\"" + usersSchoolID + "\" AND `userId`=\"" + user.GetID() + "\";");
				db.Query("DELETE FROM `feed` WHERE `actionTypeId`=\"32\" AND `actionId`=\"" + usersSchoolID + "\" AND `userId`=\"" + user.GetID() + "\";");
				db.Query("DELETE FROM `feed` WHERE `actionTypeId`=\"35\" AND `actionId`=\"" + usersSchoolID + "\" AND `userId`=\"" + user.GetID() + "\";");
				db.Query("DELETE FROM `feed` WHERE `actionTypeId`=\"36\" AND `actionId`=\"" + usersSchoolID + "\" AND `userId`=\"" + user.GetID() + "\";");


				ostFinal.str("");
				ostFinal << "{" << std::endl;
				ostFinal << "\"result\" : \"success\"," << std::endl;
				ostFinal << "\"description\" : \"\"" << std::endl;
				ostFinal << "}" << std::endl;
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		// --- AJAX remove University path
		if(action == "AJAX_removeUniversityEntry")
		{
			ostringstream	ost, ostFinal;
			CMysql			db1;
			string			usersUniversityID;

			usersUniversityID	= indexPage.GetVarsHandler()->Get("id");

			MESSAGE_DEBUG("", action, "start");


			if(usersUniversityID.empty())
			{
				MESSAGE_DEBUG("", action, "usersUniversityID is not defined");

				ostFinal.str("");
				ostFinal << "{" << std::endl;
				ostFinal << "\"result\" : \"error\"," << std::endl;
				ostFinal << "\"description\" : \"usersUniversityID is empty\"" << std::endl;
				ostFinal << "}" << std::endl;
			}
			else
			{
				string	  universityID = "";

				if(user.GetLogin() == "Guest")
				{
					ostringstream	ost;

					MESSAGE_DEBUG("", action, "re-login required");

					indexPage.Redirect("/autologin?rand=" + GetRandom(10));
				}

				if(db.Query("select * from `users_university` where `id`=\"" + usersUniversityID + "\";"))
					universityID = db.Get(0, "university_id");

				// --- delete notifications on likesUniversityDegree
				db.Query("DELETE FROM `users_notification` WHERE `actionTypeId`=\"49\" and `actionId` in (SELECT `id` FROM `feed_message_params` WHERE `messageID`=\"" + usersUniversityID + "\" and `parameter`=\"likeUniversityDegree\");");

				// --- delete notifications on comment with type "certifications"
				if(universityID.length() && (universityID != "0"))
					db.Query("DELETE FROM `users_notification` WHERE `actionTypeId`=\"19\" and `actionId` in (SELECT `id` FROM `feed_message_comment` WHERE `messageID`=\"" + universityID + "\" and `type`=\"university\") AND `userId`=\"" + user.GetID() + "\";");

				db.Query("DELETE FROM `feed_message_params` WHERE `messageID`=\"" + usersUniversityID + "\" and `parameter`=\"likeUniversityDegree\";");
				// --- centralized comments should not be deleted
				// db.Query("DELETE FROM `feed_message_comment` WHERE `messageID`=\"" + usersUniversityID + "\" and `type`=\"university\";");
				db.Query("DELETE FROM `users_university` WHERE `id`=\"" + usersUniversityID + "\" and `user_id`=\"" + user.GetID() + "\";");
				db.Query("DELETE FROM `feed` WHERE `actionTypeId`=\"28\" AND `actionId`=\"" + usersUniversityID + "\" AND `userId`=\"" + user.GetID() + "\";");
				db.Query("DELETE FROM `feed` WHERE `actionTypeId`=\"31\" AND `actionId`=\"" + usersUniversityID + "\" AND `userId`=\"" + user.GetID() + "\";");
				db.Query("DELETE FROM `feed` WHERE `actionTypeId`=\"33\" AND `actionId`=\"" + usersUniversityID + "\" AND `userId`=\"" + user.GetID() + "\";");
				db.Query("DELETE FROM `feed` WHERE `actionTypeId`=\"37\" AND `actionId`=\"" + usersUniversityID + "\" AND `userId`=\"" + user.GetID() + "\";");
				db.Query("DELETE FROM `feed` WHERE `actionTypeId`=\"38\" AND `actionId`=\"" + usersUniversityID + "\" AND `userId`=\"" + user.GetID() + "\";");
				db.Query("DELETE FROM `feed` WHERE `actionTypeId`=\"39\" AND `actionId`=\"" + usersUniversityID + "\" AND `userId`=\"" + user.GetID() + "\";");

				ostFinal.str("");
				ostFinal << "{" << std::endl;
				ostFinal << "\"result\" : \"success\"," << std::endl;
				ostFinal << "\"description\" : \"\"" << std::endl;
				ostFinal << "}" << std::endl;
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		// --- AJAX remove Language path
		if(action == "AJAX_removeLanguageEntry")
		{
			ostringstream	ost, ostFinal;
			CMysql			db1;
			string			usersLanguageID;

			usersLanguageID	= indexPage.GetVarsHandler()->Get("id");

			MESSAGE_DEBUG("", action, "start");


			if(usersLanguageID.empty())
			{
				MESSAGE_DEBUG("", action, "usersLanguageID is not defined");

				ostFinal.str("");
				ostFinal << "{" << std::endl;
				ostFinal << "\"result\" : \"error\"," << std::endl;
				ostFinal << "\"description\" : \"usersLanguageID is empty\"" << std::endl;
				ostFinal << "}" << std::endl;
			}
			else
			{
				string	  languageID = "";

				if(user.GetLogin() == "Guest")
				{
					ostringstream	ost;

					MESSAGE_DEBUG("", action, "re-login required");

					indexPage.Redirect("/autologin?rand=" + GetRandom(10));
				}
				if(db.Query("select * from `users_language` where `id`=\"" + usersLanguageID + "\";"))
					languageID = db.Get(0, "language_id");

				// --- delete notifications on likesCourse
				db.Query("DELETE FROM `users_notification` WHERE `actionTypeId`=\"49\" and `actionId` in (SELECT `id` FROM `feed_message_params` WHERE `messageID`=\"" + usersLanguageID + "\" and `parameter`=\"likeLanguage\");");
				// --- delete notifications on comment with type "certifications"
				if(languageID.length() && (languageID != "0"))
					db.Query("DELETE FROM `users_notification` WHERE `actionTypeId`=\"19\" and `actionId` in (SELECT `id` FROM `feed_message_comment` WHERE `messageID`=\"" + languageID + "\" and `type`=\"language\") AND `userId`=\"" + user.GetID() + "\";");

				db.Query("DELETE FROM `feed_message_params`  WHERE `messageID`=\"" + usersLanguageID + "\" and `parameter`=\"likeLanguage\";");
				// db.Query("DELETE FROM `feed_message_comment` WHERE `messageID`=\"" + usersLanguageID + "\" and `type`=\"language\";");
				db.Query("DELETE FROM `users_language`	   WHERE `id`=\""		+ usersLanguageID + "\" and `user_id`=\"" + user.GetID() + "\";");
				db.Query("DELETE FROM `feed` WHERE `actionTypeId`=\"29\" AND `actionId`=\"" + usersLanguageID + "\" AND `userId`=\"" + user.GetID() + "\";");
				db.Query("DELETE FROM `feed` WHERE `actionTypeId`=\"34\" AND `actionId`=\"" + usersLanguageID + "\" AND `userId`=\"" + user.GetID() + "\";");
				db.Query("DELETE FROM `feed` WHERE `actionTypeId`=\"40\" AND `actionId`=\"" + usersLanguageID + "\" AND `userId`=\"" + user.GetID() + "\";");

				ostFinal.str("");
				ostFinal << "{" << std::endl;
				ostFinal << "\"result\" : \"success\"," << std::endl;
				ostFinal << "\"description\" : \"\"" << std::endl;
				ostFinal << "}" << std::endl;
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		// --- AJAX remove company founder
		if(action == "AJAX_removeCompanyFounder")
		{
			ostringstream	ost, ostFinal;
			CMysql			db1;
			string			companyFounderID;

			companyFounderID	= indexPage.GetVarsHandler()->Get("id");

			MESSAGE_DEBUG("", action, "start");


			if(companyFounderID.empty())
			{
				MESSAGE_DEBUG("", action, "companyFounderID is not defined");

				ostFinal.str("");
				ostFinal << "{" << std::endl;
				ostFinal << "\"result\" : \"error\"," << std::endl;
				ostFinal << "\"description\" : \"companyFounderID is empty\"" << std::endl;
				ostFinal << "}" << std::endl;
			}
			else
			{
				if(user.GetLogin() == "Guest")
				{
					ostringstream	ost;

					MESSAGE_DEBUG("", action, "re-login required");

					indexPage.Redirect("/autologin?rand=" + GetRandom(10));
				}


				ost.str("");
				ost << "SELECT * FROM `company_founder` WHERE `id`='" << companyFounderID << "';";
				if(db.Query(ost.str()))
				{			
					string		companyID = db.Get(0, "companyID");

					ost.str("");
					ost << "SELECT * FROM `company` WHERE `id`='" << companyID << "' and admin_userID=\"" << user.GetID() << "\";";
					if(db.Query(ost.str()))
					{			

						ost.str("");
						ost << "DELETE FROM `company_founder` WHERE `id`='" << companyFounderID << "';";
						db.Query(ost.str());
						ost.str("");
						ost << "DELETE FROM `feed` WHERE `actionTypeId`=\"1001\" and `actionId`='" << companyFounderID << "';";
						db.Query(ost.str());

						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"success\"," << std::endl;
						ostFinal << "\"description\" : \"\"" << std::endl;
						ostFinal << "}" << std::endl;
					}
					else
					{
						MESSAGE_DEBUG("", action, "company does not belongs to you or empty");

						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"error\"," << std::endl;
						ostFinal << "\"description\" : \"company does not belongs to you or empty\"" << std::endl;
						ostFinal << "}" << std::endl;
					}
				}
				else
				{
					MESSAGE_DEBUG("", action, "can't found companyFounderID");

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"error\"," << std::endl;
					ostFinal << "\"description\" : \"companyFounderID not found\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		// --- AJAX remove company owner
		if(action == "AJAX_removeCompanyOwner")
		{
			ostringstream   ost, ostFinal;
			CMysql		  db1;
			string		  companyOwnerID;

			companyOwnerID  = indexPage.GetVarsHandler()->Get("id");

			MESSAGE_DEBUG("", action, "start");


			if(companyOwnerID.empty())
			{
				MESSAGE_DEBUG("", action, "companyOwnerID is not defined");

				ostFinal.str("");
				ostFinal << "{" << std::endl;
				ostFinal << "\"result\" : \"error\"," << std::endl;
				ostFinal << "\"description\" : \"companyOwnerID is empty\"" << std::endl;
				ostFinal << "}" << std::endl;
			}
			else
			{
				if(user.GetLogin() == "Guest")
				{
					ostringstream   ost;

					MESSAGE_DEBUG("", action, "re-login required");

					indexPage.Redirect("/autologin?rand=" + GetRandom(10));
				}


				ost.str("");
				ost << "SELECT * FROM `company_owner` WHERE `id`='" << companyOwnerID << "';";
				if(db.Query(ost.str()))
				{		   
					string	  companyID = db.Get(0, "companyID");

					ost.str("");
					ost << "SELECT * FROM `company` WHERE `id`='" << companyID << "' and admin_userID=\"" << user.GetID() << "\";";
					if(db.Query(ost.str()))
					{		   

						ost.str("");
						ost << "DELETE FROM `company_owner` WHERE `id`='" << companyOwnerID << "';";
						db.Query(ost.str());
						ost.str("");
						ost << "DELETE FROM `feed` WHERE `actionTypeId`=\"1002\" and `actionId`='" << companyOwnerID << "';";
						db.Query(ost.str());

						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"success\"," << std::endl;
						ostFinal << "\"description\" : \"\"" << std::endl;
						ostFinal << "}" << std::endl;
					}
					else
					{
						MESSAGE_DEBUG("", action, "company does not belongs to you or empty");

						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"error\"," << std::endl;
						ostFinal << "\"description\" : \"company does not belongs to you or empty\"" << std::endl;
						ostFinal << "}" << std::endl;
					}
				}
				else
				{
					MESSAGE_DEBUG("", action, "can't found companyOwnerID");

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"error\"," << std::endl;
					ostFinal << "\"description\" : \"companyOwnerID not found\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		// --- AJAX remove company owner
		if(action == "AJAX_removeCompanyIndustry")
		{
			ostringstream   ost, ostFinal;
			CMysql		  db1;
			string		  companyIndustryID;

			companyIndustryID  = indexPage.GetVarsHandler()->Get("id");

			MESSAGE_DEBUG("", action, "start");


			if(companyIndustryID.empty())
			{
				MESSAGE_DEBUG("", action, "companyIndustryID is not defined");

				ostFinal.str("");
				ostFinal << "{" << std::endl;
				ostFinal << "\"result\" : \"error\"," << std::endl;
				ostFinal << "\"description\" : \"companyIndustryID is empty\"" << std::endl;
				ostFinal << "}" << std::endl;
			}
			else
			{
				if(user.GetLogin() == "Guest")
				{
					ostringstream   ost;

					MESSAGE_DEBUG("", action, "re-login required");

					indexPage.Redirect("/autologin?rand=" + GetRandom(10));
				}


				ost.str("");
				ost << "SELECT * FROM `company_industry_ref` WHERE `id`='" << companyIndustryID << "';";
				if(db.Query(ost.str()))
				{		   
					string	  companyID = db.Get(0, "company_id");

					ost.str("");
					ost << "SELECT * FROM `company` WHERE `id`='" << companyID << "' and admin_userID=\"" << user.GetID() << "\";";
					if(db.Query(ost.str()))
					{

						db.Query("DELETE FROM `company_industry_ref` WHERE `id`='" + companyIndustryID + "';");
						db.Query("DELETE FROM `feed` WHERE `actionTypeId`=\"1003\" and `actionId`='" + companyIndustryID + "';");

						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"success\"," << std::endl;
						ostFinal << "\"description\" : \"\"" << std::endl;
						ostFinal << "}" << std::endl;
					}
					else
					{
						MESSAGE_DEBUG("", action, "company does not belongs to you or empty");

						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"error\"," << std::endl;
						ostFinal << "\"description\" : \"company does not belongs to you or empty\"" << std::endl;
						ostFinal << "}" << std::endl;
					}
				}
				else
				{
					MESSAGE_DEBUG("", action, "can't found companyIndustryID");

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"error\"," << std::endl;
					ostFinal << "\"description\" : \"companyIndustryID not found\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}



		// --- AJAX remove skill path
		if(action == "AJAX_removeSkillEntry")
		{
			ostringstream	ost, ostFinal;
			CMysql			db1;
			string			skillID;

			skillID	= indexPage.GetVarsHandler()->Get("id");

			MESSAGE_DEBUG("", action, "start");


			if(skillID.empty())
			{
				MESSAGE_DEBUG("", action, "skillID is not defined");

				ostFinal.str("");
				ostFinal << "{" << std::endl;
				ostFinal << "\"result\" : \"error\"," << std::endl;
				ostFinal << "\"description\" : \"skillID is empty\"" << std::endl;
				ostFinal << "}" << std::endl;
			}
			else
			{
				if(user.GetLogin() == "Guest")
				{
					ostringstream	ost;

					MESSAGE_DEBUG("", action, "re-login required");

					indexPage.Redirect("/autologin?rand=" + GetRandom(10));
				}


				if(db.Query("SELECT * FROM `users_skill` WHERE `id`=\"" + skillID + "\" and `user_id`=\"" + user.GetID() + "\";"))
				{

					db.Query("DELETE FROM `users_notification` WHERE `actionTypeId`=\"44\" and `actionId`='" + skillID + "';");
					db.Query("DELETE FROM `users_notification` WHERE `actionTypeId`=\"43\" and `actionId` in (SELECT `id` FROM `skill_confirmed` WHERE `users_skill_id`='" + skillID + "') AND `userId`=\"" + user.GetID() + "\";");

					db.Query("DELETE FROM `feed` WHERE `actionTypeId`=\"41\" AND `actionId`=\"" + skillID + "\" AND `userId`=\"" + user.GetID() + "\";");
					db.Query("DELETE FROM `feed` WHERE `actionTypeId`=\"42\" AND `actionId`=\"" + skillID + "\" AND `userId`=\"" + user.GetID() + "\";");
					db.Query("DELETE FROM `feed` WHERE `actionTypeId`=\"44\" AND `actionId`=\"" + skillID + "\" AND `userId`=\"" + user.GetID() + "\";");

					db.Query("DELETE FROM `skill_confirmed` WHERE `users_skill_id`='" + skillID + "';");
					db.Query("DELETE FROM `users_skill` WHERE `id`='" + skillID + "' and `user_id`='" + user.GetID() + "';");

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"success\"," << std::endl;
					ostFinal << "\"description\" : \"\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
				else
				{
					MESSAGE_DEBUG("", action, "skillID is not belongs to you or empty");

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"error\"," << std::endl;
					ostFinal << "\"description\" : \"skillID is empty\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}


		// --- AJAX remove recommendation entry
		if(action == "AJAX_removeRecommendationEntry")
		{
			ostringstream	ost, ostFinal;
			CMysql			db1;
			string			recommendationID;

			recommendationID	= indexPage.GetVarsHandler()->Get("id");

			MESSAGE_DEBUG("", action, "start");


			if(recommendationID.empty())
			{
				MESSAGE_DEBUG("", action, "recommendationID is not defined");

				ostFinal.str("");
				ostFinal << "{" << std::endl;
				ostFinal << "\"result\" : \"error\"," << std::endl;
				ostFinal << "\"description\" : \"recommendationID is empty\"" << std::endl;
				ostFinal << "}" << std::endl;
			}
			else
			{
				if(user.GetLogin() == "Guest")
				{
					ostringstream	ost;

					MESSAGE_DEBUG("", action, "re-login required");

					indexPage.Redirect("/autologin?rand=" + GetRandom(10));
				}


				ost.str("");
				ost << "SELECT * FROM `users_recommendation` WHERE `id`='" << recommendationID << "' and (`recommending_userID`='" << user.GetID() << "' or `recommended_userID`='" << user.GetID() << "');";
				if(db.Query(ost.str()))
				{			
					string	recommended_userID = db.Get(0, "recommended_userID");
					string	recommending_userID = db.Get(0, "recommending_userID");

					ost.str("");
					ost << "DELETE FROM `users_recommendation` WHERE `id`='" << recommendationID << "' and (`recommending_userID`='" << user.GetID() << "' or `recommended_userID`='" << user.GetID() << "');";
					db.Query(ost.str());

					// --- DELETE FROM users_notification
					// --- this keeps user notification consistent
					// --- otherwise notification related to insert recommendation  will looks empty, without friend name
					// --- because of "friend name" keeps in users_recommendation table 
					ost.str("");
					ost << "DELETE FROM `users_notification` WHERE (`actionTypeId`='45' or `actionTypeId`='48') and `actionId`=\"" << recommendationID << "\";";
					db.Query(ost.str());

					// --- Update live feed
					ost.str("");
					if(user.GetID() == recommended_userID)
					{
						ost << "INSERT INTO `users_notification` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << recommending_userID << "\", \"46\", \"" << recommended_userID << "\", UNIX_TIMESTAMP())";
					}
					else
					{
						ost << "INSERT INTO `users_notification` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << recommended_userID << "\", \"47\", \"" << recommending_userID << "\", UNIX_TIMESTAMP())";
					}
					if(db.InsertQuery(ost.str()))
					{
						ostFinal.str("");
						ostFinal << "{ \"result\":\"success\", \"description\":\"\" }";
					}
					else
					{
						ostFinal.str("");
						ostFinal << "{ \"result\":\"error\", \"description\":\"ERROR inserting into DB `feed`\" }";

						MESSAGE_ERROR("", action, "inserting into `feed`");
					}
				}
				else
				{
					MESSAGE_DEBUG("", action, "recommendationID is not belongs to you or empty");

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"error\"," << std::endl;
					ostFinal << "\"description\" : \"recommendationID is empty or not belongs to you\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		// --- AJAX job titles
		if(action == "AJAX_getJobTitles") 
		{
			ostringstream	ost;
			string		sessid;
			int			affected;

			MESSAGE_DEBUG("", action, "start");

			ost.str("");
			ost << "SELECT * FROM `company_position`;";
			if((affected = db.Query(ost.str())) > 0)
			{
				ost.str("");
				for(auto i = 0; i < affected; i++) 
				{
	//				ost << "{ \"label\": \"" << db.Get(i, "title") << "\", \"category\": \"" << db.Get(i, "area") << "\" }";
					ost << "\"" << db.Get(i, "title") << "\"";
					if(i < (affected-1)) ost << ", ";
				}
			}
			else
			{
				MESSAGE_ERROR("", action, "table company_position is empty");
				ost.str("");
			}

			indexPage.RegisterVariableForce("result", ost.str());

			if(!indexPage.SetTemplate("ajax_getJobTitles.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file ajax_response.htmlt was missing");
				throw CException("Template file was missing");
			}
		}

		// --- JSON company names
		if(action == "AJAX_getCompanyName") 
		{
			ostringstream	ost;
			string		sessid;
			int			affected;

			MESSAGE_DEBUG("", action, "start");

			ost.str("");
			ost << "SELECT * FROM `company`;";
			if((affected = db.Query(ost.str())) > 0)
			{
				ost.str("");
				for(auto i = 0; i < affected; i++) 
				{
	//				ost << "{ \"label\": \"" << db.Get(i, "title") << "\", \"category\": \"" << db.Get(i, "area") << "\" }";
					ost << "\"" << db.Get(i, "name") << "\"";
					if(i < (affected-1)) ost << ", ";
				}
			}
			else
			{
				MESSAGE_ERROR("", action, "table company is empty");
				ost.str("");
			}

			indexPage.RegisterVariableForce("result", ost.str());

			if(!indexPage.SetTemplate("ajax_getJobTitles.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file ajax_response.htmlt was missing");
				throw CException("Template file was missing");
			}
		}

		// --- AJAX get certification vendors
		if(action == "AJAX_getCertificationTracks") 
		{
			ostringstream	ost;
			string		sessid;
			int			affected;

			MESSAGE_DEBUG("", action, "start");

			ost.str("");
			ost << "SELECT * FROM `certification_tracks`;";
			if((affected = db.Query(ost.str())) > 0)
			{
				ost.str("");
				for(auto i = 0; i < affected; i++) 
				{
	//				ost << "{ \"label\": \"" << db.Get(i, "title") << "\", \"category\": \"" << db.Get(i, "area") << "\" }";
					ost << "\"" << db.Get(i, "title") << "\"";
					if(i < (affected-1)) ost << ", ";
				}
			}
			else
			{
				MESSAGE_ERROR("", action, "table company_position is empty");
				ost.str("");
			}

			indexPage.RegisterVariableForce("result", ost.str());

			if(!indexPage.SetTemplate("ajax_getJobTitles.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file ajax_response.htmlt was missing");
				throw CException("Template file was missing");
			}
		}

		// --- AJAX get data for profile
		if(action == "AJAX_getDataForProfile") 
		{
			ostringstream	ost;
			string		sessid;
			int			affected;

			MESSAGE_DEBUG("", action, "start");

			ost.str("");
			ost << "SELECT * FROM `company_position`;";
			if((affected = db.Query(ost.str())) > 0)
			{
				ost.str("");
				for(auto i = 0; i < affected; i++) 
				{
	//				ost << "{ \"label\": \"" << db.Get(i, "title") << "\", \"category\": \"" << db.Get(i, "area") << "\" }";
					ost << "{\"id\":\"" << db.Get(i, "id") << "\",\"title\":\"" << db.Get(i, "title") << "\"}";
					if(i < (affected-1)) ost << ", ";
				}
			}
			else
			{
				MESSAGE_ERROR("", action, "table company_position is empty");
				ost.str("");
			}

			indexPage.RegisterVariableForce("company_position", ost.str());

			ost.str("");
			ost << "SELECT * FROM `geo_country`;";
			if((affected = db.Query(ost.str())) > 0)
			{
				ost.str("");
				for(auto i = 0; i < affected; i++) 
				{
	//				ost << "{ \"label\": \"" << db.Get(i, "title") << "\", \"category\": \"" << db.Get(i, "area") << "\" }";
					ost << "{\"id\":\"" << db.Get(i, "id") << "\",\"title\":\"" << db.Get(i, "title") << "\"}";
					if(i < (affected-1)) ost << ", ";
				}
			}
			else
			{
				MESSAGE_ERROR("", action, "table geo_country is empty");
				ost.str("");
			}

			indexPage.RegisterVariableForce("geo_country", ost.str());

			ost.str("");
			ost << "SELECT * FROM `geo_region`;";
			if((affected = db.Query(ost.str())) > 0)
			{
				ost.str("");
				for(auto i = 0; i < affected; i++) 
				{
	//				ost << "{ \"label\": \"" << db.Get(i, "title") << "\", \"category\": \"" << db.Get(i, "area") << "\" }";
					ost << "{\"id\":\"" << db.Get(i, "id") << "\",\"geo_country_id\":\"" << db.Get(i, "geo_country_id") << "\",\"title\":\"" << db.Get(i, "title") << "\"}";
					if(i < (affected-1)) ost << ", ";
				}
			}
			else
			{
				MESSAGE_ERROR("", action, "table geo_region is empty");
				ost.str("");
			}

			indexPage.RegisterVariableForce("geo_region", ost.str());

			ost.str("");
			ost << "SELECT * FROM `geo_locality`;";
			if((affected = db.Query(ost.str())) > 0)
			{
				ost.str("");
				for(auto i = 0; i < affected; i++) 
				{
	//				ost << "{ \"label\": \"" << db.Get(i, "title") << "\", \"category\": \"" << db.Get(i, "area") << "\" }";
					ost << "{\"id\":\"" << db.Get(i, "id") << "\",\"geo_region_id\":\"" << db.Get(i, "geo_region_id") << "\",\"title\":\"" << db.Get(i, "title") << "\"}";
					if(i < (affected-1)) ost << ", ";
				}
			}
			else
			{
				MESSAGE_ERROR("", action, "table geo_locality is empty");
				ost.str("");
			}

			indexPage.RegisterVariableForce("geo_locality", ost.str());

			ost.str("");
			ost << "SELECT * FROM `university`;";
			if((affected = db.Query(ost.str())) > 0)
			{
				ost.str("");
				for(auto i = 0; i < affected; i++) 
				{
	//				ost << "{ \"label\": \"" << db.Get(i, "title") << "\", \"category\": \"" << db.Get(i, "area") << "\" }";
					ost << "{\"id\":\"" << db.Get(i, "id") << "\",\"geo_region_id\":\"" << db.Get(i, "geo_region_id") << "\",\"title\":\"" << db.Get(i, "title") << "\"}";
					if(i < (affected-1)) ost << ", ";
				}
			}
			else
			{
				MESSAGE_ERROR("", action, "table university is empty");
				ost.str("");
			}

			indexPage.RegisterVariableForce("university", ost.str());

			ost.str("");
			ost << "SELECT * FROM `school`;";
			if((affected = db.Query(ost.str())) > 0)
			{
				ost.str("");
				for(auto i = 0; i < affected; i++) 
				{
					ost << "{\"id\":\"" << db.Get(i, "id") << "\",\"geo_locality_id\":\"" << db.Get(i, "geo_locality_id") << "\",\"title\":\"" << db.Get(i, "title") << "\"}";
					if(i < (affected-1)) ost << ", ";
				}
			}
			else
			{
				MESSAGE_ERROR("", action, "table school is empty");
				ost.str("");
			}

			indexPage.RegisterVariableForce("school", ost.str());

			ost.str("");
			ost << "SELECT * FROM `language`;";
			if((affected = db.Query(ost.str())) > 0)
			{
				ost.str("");
				for(auto i = 0; i < affected; i++) 
				{
					ost << "{\"id\":\"" << db.Get(i, "id") << "\",\"title\":\"" << db.Get(i, "title") << "\"}";
					if(i < (affected-1)) ost << ", ";
				}
			}
			else
			{
				MESSAGE_ERROR("", action, "table language is empty");
				ost.str("");
			}

			indexPage.RegisterVariableForce("language", ost.str());

			ost.str("");
			ost << "SELECT * FROM `skill`;";
			if((affected = db.Query(ost.str())) > 0)
			{
				ost.str("");
				for(auto i = 0; i < affected; i++) 
				{
					ost << "{\"id\":\"" << db.Get(i, "id") << "\",\"title\":\"" << db.Get(i, "title") << "\"}";
					if(i < (affected-1)) ost << ", ";
				}
			}
			else
			{
				MESSAGE_ERROR("", action, "table skill is empty");
				ost.str("");
			}

			indexPage.RegisterVariableForce("skill", ost.str());

			if(!indexPage.SetTemplate("ajax_getDataForProfile.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file ajax_getDataForProfile.htmlt was missing");
				throw CException("Template file was missing");
			}
		}


		// --- AJAX get URL met data
		if(action == "AJAX_getURLMetaData")
		{
			ostringstream	ost, ostFinal;
			CMysql			db1;
			string			url, imageTempSet;

			MESSAGE_DEBUG("", action, "start");

			url = indexPage.GetVarsHandler()->Get("url");
			imageTempSet = indexPage.GetVarsHandler()->Get("imageTempSet");
			try
			{
				imageTempSet = to_string(stol(imageTempSet));
			} 
			catch(...) 
			{
				{
					MESSAGE_ERROR("", action, "imageTempSet [" + string(indexPage.GetVarsHandler()->Get("imageTempSet")) + "] can't be converted to number");
				}
				imageTempSet = "";
			}

			url = CleanUPText(url);
			if(imageTempSet.empty() || url.empty() || url.length() > HTML_MAX_POLL_URL_LENGTH)
			{
				if(imageTempSet.empty())
				{
					MESSAGE_ERROR("", action, "imageTempSet is empty");

					ostFinal.str("");
					ostFinal << "{"
								"\"result\" : \"error\","
								"\"description\" : \"проблема с выбором imageTempSet\""
								"}";
				}
				else
				{
					MESSAGE_DEBUG("", action, "url is empty OR too long OR imageTempSet is empty");

					ostFinal.str("");
					ostFinal << "{"
								"\"result\" : \"error\","
								"\"description\" : \"ссылка пустая или слишком длинная\""
								"}";
				}
			}
			else
			{
				if(user.GetLogin() == "Guest")
				{
					MESSAGE_DEBUG("", action, "re-login required");

					ostFinal.str("");
					ostFinal << "{"
								"\"result\" : \"error\","
								"\"description\" : \"Ваша сессия истекла. Необходимо выйти и повторно зайти.\""
								"}";
				}


				{
					// important to create and destroy CHTML just once in program.
					CHTML		html;

					if(html.PerformRequest(url))
					{
						unsigned long		feed_imageID = 0;
						string			  feed_imageURL = "";
						string				feed_mediaType = "";

						if(html.isEmbedVideoHostedOnYoutube())
						{
							ostringstream   ost;

							MESSAGE_DEBUG("", action, "youtube video found");

							ost.str("");
							ost << "INSERT INTO `feed_images` set "
								<< "`tempSet`='" << imageTempSet << "', "
								<< "`userID`='" << user.GetID() << "',  "
								<< "`folder`='', "
								<< "`mediaType`=\"youtube_video\",  "
								<< "`filename`=\"" << html.GetEmbedVideoURL() << "\";";

							 feed_imageID = db.InsertQuery(ost.str());
							 feed_mediaType = "youtube_video";
							 feed_imageURL = html.GetEmbedVideoURL();
						}
						else if((!html.GetPreviewImageFolder().empty()) && (!html.GetPreviewImagePrefix().empty()) && (!html.GetPreviewImageExtension().empty()))
						{
							// --- 1) check that image actually image
							// --- 2) move it to final folder
							// --- 3) submit to imageTempSet in DB
							struct ExifInfo exifInfo;
							string			finalFile, tmpFile2Check, tmpImageJPG;
							ostringstream	ost;

							MESSAGE_DEBUG("", action, "preview image found");

#ifndef IMAGEMAGICK_DISABLE
							Magick::InitializeMagick(NULL);
#endif
							// --- init variables
							ost.str("");
							ost << IMAGE_FEED_DIRECTORY << "/" << html.GetPreviewImageFolder() << "/" << html.GetPreviewImagePrefix() << ".jpg";
							finalFile = ost.str();

							ost.str("");
							ost << "/tmp/tmp_" << html.GetPreviewImagePrefix() << html.GetPreviewImageExtension();
							tmpFile2Check = ost.str();

							ost.str("");
							ost << "/tmp/" << html.GetPreviewImagePrefix() << ".jpg";
							tmpImageJPG = ost.str();

							if(ImageSaveAsJpgToFeedFolder(tmpFile2Check, tmpImageJPG, exifInfo, &config))
							{

								MESSAGE_DEBUG("", action, "chosen filename for feed image [" + finalFile + "]");

								CopyFile(tmpImageJPG, finalFile);

								ost.str("");
								ost << "INSERT INTO `feed_images` set "
									<< "`tempSet`='" << imageTempSet << "', "
									<< "`userID`='" << user.GetID() << "',  "
									<< "`folder`='" << html.GetPreviewImageFolder() << "', "
									<< "`filename`='" << html.GetPreviewImagePrefix() << ".jpg', "
									<< "`mediaType`='image',  "
									<< ((exifInfo.DateTime.length() && exifInfo.DateTime != "unknown") ? (string)("`exifDateTime`='") + exifInfo.DateTime + "', " : "")
									<< "`exifGPSAltitude`='" << ParseGPSAltitude(exifInfo.GPSAltitude) << "', "
									<< "`exifGPSLatitude`='" << ParseGPSLatitude(exifInfo.GPSLatitude) << "', "
									<< "`exifGPSLongitude`='" << ParseGPSLongitude(exifInfo.GPSLongitude) << "', "
									<< "`exifGPSSpeed`='" << ParseGPSSpeed(exifInfo.GPSSpeed) << "', "
									<< "`exifModel`='" << exifInfo.Model << "', "
									<< "`exifAuthors`='" << exifInfo.Authors << "', "
									<< "`exifApertureValue`='" << exifInfo.ApertureValue << "', "
									<< "`exifBrightnessValue`='" << exifInfo.BrightnessValue << "', "
									<< "`exifColorSpace`='" << exifInfo.ColorSpace << "', "
									<< "`exifComponentsConfiguration`='" << exifInfo.ComponentsConfiguration << "', "
									<< "`exifCompression`='" << exifInfo.Compression << "', "
									<< "`exifDateTimeDigitized`='" << exifInfo.DateTimeDigitized << "', "
									<< "`exifDateTimeOriginal`='" << exifInfo.DateTimeOriginal << "', "
									<< "`exifExifImageLength`='" << exifInfo.ExifImageLength << "', "
									<< "`exifExifImageWidth`='" << exifInfo.ExifImageWidth << "', "
									<< "`exifExifOffset`='" << exifInfo.ExifOffset << "', "
									<< "`exifExifVersion`='" << exifInfo.ExifVersion << "', "
									<< "`exifExposureBiasValue`='" << exifInfo.ExposureBiasValue << "', "
									<< "`exifExposureMode`='" << exifInfo.ExposureMode << "', "
									<< "`exifExposureProgram`='" << exifInfo.ExposureProgram << "', "
									<< "`exifExposureTime`='" << exifInfo.ExposureTime << "', "
									<< "`exifFlash`='" << exifInfo.Flash << "', "
									<< "`exifFlashPixVersion`='" << exifInfo.FlashPixVersion << "', "
									<< "`exifFNumber`='" << exifInfo.FNumber << "', "
									<< "`exifFocalLength`='" << exifInfo.FocalLength << "', "
									<< "`exifFocalLengthIn35mmFilm`='" << exifInfo.FocalLengthIn35mmFilm << "', "
									<< "`exifGPSDateStamp`='" << exifInfo.GPSDateStamp << "', "
									<< "`exifGPSDestBearing`='" << exifInfo.GPSDestBearing << "', "
									<< "`exifGPSDestBearingRef`='" << exifInfo.GPSDestBearingRef << "', "
									<< "`exifGPSImgDirection`='" << exifInfo.GPSImgDirection << "', "
									<< "`exifGPSImgDirectionRef`='" << exifInfo.GPSImgDirectionRef << "', "
									<< "`exifGPSInfo`='" << exifInfo.GPSInfo << "', "
									<< "`exifGPSTimeStamp`='" << exifInfo.GPSTimeStamp << "', "
									<< "`exifISOSpeedRatings`='" << exifInfo.ISOSpeedRatings << "', "
									<< "`exifJPEGInterchangeFormat`='" << exifInfo.JPEGInterchangeFormat << "', "
									<< "`exifJPEGInterchangeFormatLength`='" << exifInfo.JPEGInterchangeFormatLength << "', "
									<< "`exifMake`='" << exifInfo.Make << "', "
									<< "`exifMeteringMode`='" << exifInfo.MeteringMode << "', "
									<< "`exifOrientation`='" << exifInfo.Orientation << "', "
									<< "`exifResolutionUnit`='" << exifInfo.ResolutionUnit << "', "
									<< "`exifSceneCaptureType`='" << exifInfo.SceneCaptureType << "', "
									<< "`exifSceneType`='" << exifInfo.SceneType << "', "
									<< "`exifSensingMethod`='" << exifInfo.SensingMethod << "', "
									<< "`exifShutterSpeedValue`='" << exifInfo.ShutterSpeedValue << "', "
									<< "`exifSoftware`='" << exifInfo.Software << "', "
									<< "`exifSubjectArea`='" << exifInfo.SubjectArea << "', "
									<< "`exifSubSecTimeDigitized`='" << exifInfo.SubSecTimeDigitized << "', "
									<< "`exifSubSecTimeOriginal`='" << exifInfo.SubSecTimeOriginal << "', "
									<< "`exifWhiteBalance`='" << exifInfo.WhiteBalance << "', "
									<< "`exifXResolution`='" << exifInfo.XResolution << "', "
									<< "`exifYCbCrPositioning`='" << exifInfo.YCbCrPositioning << "', "
									<< "`exifYResolution`='" << exifInfo.YResolution << "';";
								feed_imageID = db.InsertQuery(ost.str());

								if(feed_imageID)
								{
									feed_imageURL = html.GetPreviewImageFolder() + "/" + html.GetPreviewImagePrefix() + ".jpg";
									feed_mediaType = "image";
								}
								else
								{
									MESSAGE_ERROR("", action, "inserting image info into feed_images table");
								}

								// --- Delete temporarily files
								unlink(tmpFile2Check.c_str());
								unlink(tmpImageJPG.c_str());
							}
							else
							{
								MESSAGE_ERROR("", action, "image [" + tmpFile2Check + "] is not valid image format (looks like attack)");
							}
						}
						else
						{
							MESSAGE_DEBUG("", action, "there are no (youtube_video, preview image)");
						}
						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"success\",";
						ostFinal << "\"title\" : \"" << CleanUPText(html.GetTitle()) << "\",";
						ostFinal << "\"description\" : \"" << CleanUPText(html.GetDescription()) << "\",";
						ostFinal << "\"imageURL\" : \"" << feed_imageURL << "\",";
						ostFinal << "\"imageID\" : \"" << feed_imageID << "\",";
						ostFinal << "\"mediaType\" : \"" << feed_mediaType << "\"";
						ostFinal << "}" << std::endl;
					}
					else
					{
						MESSAGE_DEBUG("", action, "can't perform request" + string(url));

						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"error\"," << std::endl;
						ostFinal << "\"description\" : \"Не удалось получить данные с сайта, проверьте корректность ссылки\"" << std::endl;
						ostFinal << "}" << std::endl;
					}
				}
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}




		// --- AJAX delete preview avatar
		if(action == "AJAX_deleteAvatar") 
		{
			ostringstream	result;
			ostringstream	ost;
			string		sessid, avatarID;
			int			affected;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			avatarID = indexPage.GetVarsHandler()->Get("id");

			ost << "SELECT * FROM `users_avatars` WHERE `id`=\"" << avatarID << "\";";
			affected = db.Query(ost.str());
			if((affected = db.Query(ost.str())) > 0)
			{
				if(db.Get(0, "userid") == user.GetID())
				{
					auto	filename = config.GetFromFile("image_folders", "avatar") + "/avatars"+ db.Get(0, "folder") + "/" + db.Get(0, "filename");

					MESSAGE_DEBUG("", action, "removing avatar [id=" + avatarID + "] belongs to user " + user.GetLogin() + " [filename=" + filename + "]");

					ost.str("");
					ost << "DELETE FROM `users_avatars` WHERE `id`=\"" << avatarID << "\";";
					db.Query(ost.str());

					if(isFileExists(filename))
					{
						unlink(filename.c_str());

						// --- Update live feed
						ost.str("");
						ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"9\", \"0\", NOW())";
						if(db.InsertQuery(ost.str()))
						{
							result.str("");
							result << "{ \"result\":\"success\", \"description\":\"\" }";
						}
						else
						{
							result.str("");
							result << "{ \"result\":\"error\", \"description\":\"ERROR inserting into DB `feed`\" }";

							MESSAGE_ERROR("", action, "inserting into `feed` (" + ost.str() + ")");
						}
					}
					else
					{
						result.str("");
						result << "{ \"result\":\"error\", \"description\":\"ERROR file is not exists\" }";

						MESSAGE_ERROR("", action, "file is not exists  [filename=" + filename + "] belongs to user " + user.GetLogin());
					}
				}
				else
				{
					result.str("");
					result << "{ \"result\":\"error\", \"description\":\"avatar do not belongs to you\" }";
						
					MESSAGE_ERROR("", action, "avatar [id=" + avatarID + "] do not belongs to user " + user.GetLogin());
				}
			}
			else
			{
				result.str("");
				result << "{ \"result\":\"error\", \"description\":\"there is no avatar\" }";

				MESSAGE_DEBUG("", action, "there is no avatar [id=" + avatarID + "]");
			}

			indexPage.RegisterVariableForce("result", result.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file ajax_response.htmlt was missing");
				throw CException("Template file was missing");
			}
		}

		// --- AJAX change in friendship status
		if(action == "AJAX_setFindFriend_FriendshipStatus") 
		{
			MESSAGE_DEBUG("", action, "start");

			auto			sessid						= ""s;
			auto			friendID					= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("friendID"));
			auto			requestedFriendshipStatus	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("status"));
			auto			currentFriendshipStatus		= ""s;
			auto			success_message				= ""s;
			auto			error_message				= ""s;

			if(user.GetLogin() == "Guest")
			{
				error_message = gettext("re-login required");
				MESSAGE_DEBUG("", action, error_message);
			}
			else
			{
				if((friendID.length() == 0) || (requestedFriendshipStatus.length() == 0))
				{
					error_message = gettext("mandatory parameter missed");
					MESSAGE_ERROR("", action, error_message);
				}
				else
				{
					if(db.Query("SELECT * FROM `users_friends` WHERE `userID`='" + user.GetID() + "' and `friendID`='" + friendID + "';")) 
						currentFriendshipStatus = db.Get(0, "state");
					else 
						currentFriendshipStatus = "empty";

					if((requestedFriendshipStatus == "requested") || (requestedFriendshipStatus == "requesting")) 
					{
						if((currentFriendshipStatus == "empty") || (currentFriendshipStatus == "ignored"))
						{
							db.Query(
									"START TRANSACTION;"
									"INSERT INTO `users_friends` (`userID`, `friendID`, `state`, `date`) "
									"VALUES (" + quoted(user.GetID()) + "," + quoted(friendID) + ", \"requesting\", NOW());"
									"INSERT INTO `users_friends` (`userID`, `friendID`, `state`, `date`) "
									"VALUES (" + quoted(friendID) + ", " + quoted(user.GetID()) + ", \"requested\", NOW());"
									"INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) "
									"VALUES(\"\", " + quoted(user.GetID()) + ", \"16\", " + quoted(friendID) + ",  NOW());"
									"COMMIT;"
							);
						}
						else
						{
							error_message = gettext("friendship status can't be changed") + " ("s + currentFriendshipStatus + " -> " + requestedFriendshipStatus + ")";
							MESSAGE_ERROR("", action, error_message);
						}
					}
					else if (requestedFriendshipStatus == "disconnect")
					{

						if((currentFriendshipStatus == "requested") || (currentFriendshipStatus == "requesting") || (currentFriendshipStatus == "confirmed") || (currentFriendshipStatus == "blocked"))
						{
							db.Query(
									"DELETE FROM `users_friends` WHERE "
									"(`userID`=\"" + user.GetID() + "\" AND `friendID`=\"" + friendID + "\" ) "
									"OR "
									"(`friendID`=\"" + user.GetID() + "\" AND `userID`=\"" + friendID + "\" );"
									);

							db.Query(
									"DELETE FROM `feed` WHERE "
									"(`userId`=\"" + user.GetID() + "\" AND `actionId`=\"" + friendID + "\" AND `actionTypeId` IN (14, 15, 16)) "
									"OR "
									"(`actionId`=\"" + user.GetID() + "\" AND `userId`=\"" + friendID + "\" AND `actionTypeId` IN (14, 15, 16)) "
									);
						}
						else
						{
							error_message = gettext("friendship status can't be changed") + " ("s + currentFriendshipStatus + " -> " + requestedFriendshipStatus + ")";
							MESSAGE_ERROR("", action, error_message);
						}

					}
					else if(requestedFriendshipStatus == "confirm")
					{
						if((currentFriendshipStatus == "requested") || (currentFriendshipStatus == "requesting") || (currentFriendshipStatus == "confirmed") || (currentFriendshipStatus == "blocked"))
						{
							db.Query(
									"update `users_friends` set `state`='confirmed', `date`=NOW() WHERE "
									"(`userID`=\"" + user.GetID() + "\" AND `friendID`=\"" + friendID + "\") "
									"OR "
									"(`friendID`=\"" + user.GetID() + "\" AND `userID`=\"" + friendID + "\" );"
							);

							if(db.InsertQuery("INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" + user.GetID() + "\", \"14\", \"" + friendID + "\", NOW())")) {}
							else
							{
								MESSAGE_ERROR("", action, "inserting into `feed`");
							}

							if(db.InsertQuery("INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" + friendID + "\", \"14\", \"" + user.GetID() + "\", NOW())")) {}
							else
							{
								MESSAGE_ERROR("", action, "inserting into `feed`");
							}
						}
						else
						{
							error_message = gettext("friendship status can't be changed") + " ("s + currentFriendshipStatus + " -> " + requestedFriendshipStatus + ")";
							MESSAGE_ERROR("", action, error_message);
						}
					}
					else
					{
						error_message = gettext("requested friendship status is unknown") + " ("s + currentFriendshipStatus + " -> " + requestedFriendshipStatus + ")";
						MESSAGE_ERROR("", action, error_message);
					}
				}
			}

	
			AJAX_ResponseTemplate(&indexPage, success_message, error_message);
		}

		// --- AJAX block user account
		if(action == "AJAX_block_user_account") 
		{
			ostringstream	ost, result;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			db.Query("update `users` set `isblocked`='Y' WHERE `id`='" + user.GetID() + "';");
			if(db.Query("SELECT `isblocked` FROM `users` WHERE `id`='" + user.GetID() + "';"))
			{
				auto	blockStatus = db.Get(0, "isblocked");

				if(blockStatus == "Y")
				{

					// --- update news feed
					ost.str("");
					ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"17\", \"0\", NOW());";
					if(db.InsertQuery(ost.str()))
					{

					}
					else
					{
						MESSAGE_ERROR("", action, "inserting into `feed` (" + ost.str() + ")");
					}
		
					result.str("");
					result << "{ \"result\":\"success\", \"description\":\"\" }";
				}
				else
				{
					MESSAGE_ERROR("", "", "userID [" + user.GetID() + "] can't block");


					result.str("");
					result << "{ \"result\":\"error\", \"description\":\"userID [" << user.GetID() << "] can't block\" }";
				}
			}
			else
			{
				MESSAGE_ERROR("", "", "userID [" + user.GetID() + "] has not been found");


				result.str("");
				result << "{ \"result\":\"error\", \"description\":\"userID [" << user.GetID() << "] has not been found\" }";

			}

			indexPage.RegisterVariableForce("result", result.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file ajax_response.htmlt was missing");
				throw CException("Template file was missing");
			}

		}

		// --- AJAX enable user account
		if(action == "AJAX_enable_user_account") 
		{
			ostringstream	ost, result;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			ost.str("");
			ost << "update `users` set `isblocked`='N' WHERE `id`='" << user.GetID() << "';";
			db.Query(ost.str());
			ost.str("");
			ost << "SELECT `isblocked` FROM `users` WHERE `id`='" << user.GetID() << "';";
			if(db.Query(ost.str()))
			{
				string	blockStatus = db.Get(0, "isblocked");

				if(blockStatus == "N")
				{

					// --- update news feed
					ost.str("");
					ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"18\", \"0\", NOW());";
					if(db.InsertQuery(ost.str()))
					{

					}
					else
					{
						MESSAGE_ERROR("", action, "inserting into `feed` (" + ost.str() + ")");
					}
		
					result.str("");
					result << "{ \"result\":\"success\", \"description\":\"\" }";
				}
				else
				{
					MESSAGE_ERROR("", "", "userID [" + user.GetID() + "] can't enable");

					result.str("");
					result << "{ \"result\":\"error\", \"description\":\"userID [" << user.GetID() << "] can't enable\" }";
				}
			}
			else
			{
				MESSAGE_ERROR("", "", "userID [" + user.GetID() + "] has not been found");

				result.str("");
				result << "{ \"result\":\"error\", \"description\":\"userID [" << user.GetID() << "] has not been found\" }";
			}

			indexPage.RegisterVariableForce("result", result.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file ajax_response.htmlt was missing");
				throw CException("Template file was missing");
			}

		}


		// --- JSON like handler
		if(action == "JSON_ClickLikeHandler")
		{
			ostringstream	ost, ostFinal;
			string			sessid, messageId, messageLikeType, userList = "";
			string		  failReason = "";

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				ostringstream   ost;

				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			messageId = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("messageId"));
			messageLikeType = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("messageLikeType"));
			if(messageLikeType.empty()) messageLikeType = "like";

			if(messageId.length())
			{
				int				affected;
				bool			isSuccess = false;
				
				messageId = to_string(stol(messageId));

				affected = db.Query("SELECT * FROM `feed_message_params` WHERE `parameter`=\"" + messageLikeType + "\" AND `messageID`=\"" + messageId + "\" AND `userID`=\"" + user.GetID() + "\";");
				if(affected)
				{
					// --- disliked message
					string  feed_message_params_id_to_remove = db.Get(0, "id");

					isSuccess = true;
					// --- remove "my like"
					db.Query("DELETE FROM `feed_message_params` WHERE `id`=\"" + feed_message_params_id_to_remove + "\";");

					// --- remove it FROM users_notifications
					db.Query("DELETE FROM `users_notification` WHERE `actionTypeId`=\"49\" AND `actionId`='" + feed_message_params_id_to_remove + "';");
				}
				else
				{
					// --- add "my like"
					unsigned long   feed_message_params_id = db.InsertQuery("INSERT INTO  `feed_message_params` (`parameter`, `messageID`, `userID`) VALUES (\"" + messageLikeType + "\", \"" + messageId + "\", \"" + user.GetID() + "\");");

					if(feed_message_params_id)
					{
						string	  userIDtoNotify = "";

						isSuccess = true;

						if((messageLikeType == "like") && db.Query("SELECT * FROM `feed` WHERE `actionTypeId`=\"11\" AND `actionId`=\"" + messageId + "\";"))
						{
							string	srcType = db.Get(0, "srcType");
							string	srcID = db.Get(0, "userId");

							if(srcType == "user") 
							{
								userIDtoNotify = srcID;
							}
							else if(srcType == "company")
							{
								if(db.Query("SELECT `admin_userID` FROM `company` WHERE `id`=\"" + srcID + "\" AND `isBlocked`=\"N\";"))
								{
									userIDtoNotify = db.Get(0, "admin_userID");
								}
								else
								{
									MESSAGE_ERROR("", action, "unknown company.id(" + srcID + ") or blocked");
								}
							}
							else if(srcType == "group")
							{
								if(db.Query("SELECT `owner_id` FROM `groups` WHERE `id`=\"" + srcID + "\" AND `isBlocked`=\"N\";"))
								{
									userIDtoNotify = db.Get(0, "owner_id");
								}
								else
								{
									MESSAGE_ERROR("", action, "unknown group.id(" + srcID + ") or blocked");
								}
							}
							else
							{
								MESSAGE_ERROR("", action, "unknown srcType(" + srcType + ")");
							}
						}
						if((messageLikeType == "likeBook") && db.Query("SELECT * FROM `users_books` WHERE `id`=\"" + messageId + "\";"))
							userIDtoNotify = db.Get(0, "userID");
						if((messageLikeType == "likeCertification") && db.Query("SELECT * FROM `users_certifications` WHERE `id`=\"" + messageId + "\";"))
							userIDtoNotify = db.Get(0, "user_id");
						if((messageLikeType == "likeCourse") && db.Query("SELECT * FROM `users_courses` WHERE `id`=\"" + messageId + "\";"))
							userIDtoNotify = db.Get(0, "user_id");
						if((messageLikeType == "likeUniversityDegree") && db.Query("SELECT * FROM `users_university` WHERE `id`=\"" + messageId + "\";"))
							userIDtoNotify = db.Get(0, "user_id");
						if((messageLikeType == "likeCompany") && db.Query("SELECT * FROM `users_company` WHERE `id`=\"" + messageId + "\";"))
							userIDtoNotify = db.Get(0, "user_id");
						if((messageLikeType == "likeLanguage") && db.Query("SELECT * FROM `users_language` WHERE `id`=\"" + messageId + "\";"))
							userIDtoNotify = db.Get(0, "user_id");

						// --- send notification to message owner
						if(userIDtoNotify.length() && (userIDtoNotify != "0"))
						{
							// --- user notification for message like
							
							if(db.InsertQuery("INSERT INTO `users_notification` (`userId`, `actionTypeId`, `actionId`, `eventTimestamp`) VALUES (\"" + userIDtoNotify + "\", \"49\", \"" + to_string(feed_message_params_id) + "\", UNIX_TIMESTAMP());"))
							{
							}
							else
							{
								MESSAGE_ERROR("", action, "inserting into users_notification table");
							}
						}
						else
						{
							MESSAGE_ERROR("", action, "can't find \"liked message\" owner");
						}
					}
					else
					{
						isSuccess = false;
						failReason = "inserting into feed_message_params table";

						MESSAGE_ERROR("", action, "inserting into feed_message_params table");
					}
				}

				affected = db.Query("SELECT * FROM `feed_message_params` WHERE `parameter`=\"" + messageLikeType + "\" and `messageID`=\"" + messageId + "\";");
				if(affected)
				{
					ost.str("");
					ost << "SELECT * FROM `users` WHERE `id` in (";
					if(affected)
					{
						for(auto i = 0; i < affected; i++)
						{
							if(i > 0) ost << ",";
							ost << db.Get(i, "userID");
						}
					}
					ost << ");";
					userList = GetUserListInJSONFormat(ost.str(), &db, &user);
				}
				else
				{
					{
						MESSAGE_DEBUG("", action, "noone " + messageLikeType + " id:" + messageId + " anymore");
					}

					userList = "";
				}

				ostFinal.str("");
				if(isSuccess)
				{
					ostFinal << "{";
					ostFinal << "\"result\" : \"success\",";
					ostFinal << "\"description\" : \"\",";
					ostFinal << "\"messageLikesUserList\" : [" << userList << "]";
					ostFinal << "}";
				}
				else
				{
					ostFinal << "{";
					ostFinal << "\"result\" : \"error\",";
					ostFinal << "\"description\" : \"" << failReason << "\"";
					ostFinal << "}";
				}

			}
			else
			{
				// --- message ID is not number
				MESSAGE_ERROR("", "", "messageID[" + indexPage.GetVarsHandler()->Get("messageId") + "] is empty")

				ostFinal.str("");
				ostFinal << "{";
				ostFinal << "\"result\" : \"error\",";
				ostFinal << "\"description\" : \"messageID is empty\"";
				ostFinal << "}";
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}
			MESSAGE_DEBUG("", action, "finish");
		}


		// --- JSON returns shake hands in view profile
		// ---		common friends
		// ---		common companies
		if(action == "JSON_getShakeHands")
		{
			MESSAGE_DEBUG("", action, "start");

			ostringstream	ost, ostFinal;
			string			sessid, lookForKey;
			string			handshakeUserStatus = "";
			auto			user1 = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("user1"));
			auto			user2 = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("user2"));
			string			user1Data, user2Data, hopUserList;
			CMysql			db1;
			vector<int>		vectorFriendList1, vectorFriendList2, vectorFriendList3;


			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			if(user1.length() && user2.length())
			{
				if(user1 != user2)
				{
					int				affected;

					ost.str("");
					ost << "SELECT `users_friends`.`friendID` FROM `users_friends` \
							RIGHT JOIN `users` on `users_friends`.`friendID` = `users`.`id` \
							WHERE `users_friends`.`userID`='" << user1 << "' and `users`.`isblocked`='N' and  `users`.`isactivated`='Y' and `users_friends`.`state`='confirmed';";
					affected = db.Query(ost.str());
					if(affected)
					{
						for(auto i = 0; i < affected; i++)
						{
							vectorFriendList1.push_back(stoi(db.Get(i, "friendID")));

							if(user2 == db.Get(i, "friendID"))
							{
								handshakeUserStatus = "directFriends";
							}
						}


						if(handshakeUserStatus == "")
						{

							ost.str("");
							ost << "SELECT `users_friends`.`friendID` FROM `users_friends` \
									RIGHT JOIN `users` on `users_friends`.`friendID` = `users`.`id` \
									WHERE `users_friends`.`userID`='" << user2 << "' and `users`.`isblocked`='N' and  `users`.`isactivated`='Y' and `users_friends`.`state`='confirmed';";
							affected = db.Query(ost.str());
							if(affected)
							{
								for(auto i = 0; i < affected; i++)
								{
									vectorFriendList2.push_back(stoi(db.Get(i, "friendID")));
								}
							}

							for(vector<int>::iterator it1 = vectorFriendList1.begin(); it1 != vectorFriendList1.end(); ++it1)
							{
								for(vector<int>::iterator it2 = vectorFriendList2.begin(); it2 != vectorFriendList2.end(); ++it2)
								{
									if(*it1 == *it2)
									{
										vectorFriendList3.push_back(*it1);
									}
								}

							}

							if(vectorFriendList3.size() > 0)
							{
								handshakeUserStatus = "1hop";
						
								ost.str("");
								ost << "SELECT * FROM `users` WHERE `isActivated`='Y' and `isblocked`='N' and `id` in (";
								for(vector<int>::iterator it = vectorFriendList3.begin(); it != vectorFriendList3.end(); ++it)
								{
									if(it != vectorFriendList3.begin())	ost << ",";
									ost << *it;
								}
								ost << ");";
								hopUserList = GetUserListInJSONFormat(ost.str(), &db, &user);
							}
							else
							{
								handshakeUserStatus = "noConnection";
							}
						}
					}
					else
					{
						handshakeUserStatus = "noConnection";
					}
				}
				else
				{
					handshakeUserStatus = "sameUser";
				}

				ost.str("");
				ost << "SELECT * FROM `users` WHERE `isActivated`='Y' and `isblocked`='N' and `id`='" << user1 << "'";
				user1Data = GetUserListInJSONFormat(ost.str(), &db, &user);

				ost.str("");
				ost << "SELECT * FROM `users` WHERE `isActivated`='Y' and `isblocked`='N' and `id`='" << user2 << "'";
				user2Data = GetUserListInJSONFormat(ost.str(), &db, &user);


				ostFinal.str("");
				ostFinal << "{";
				ostFinal << "\"result\" : \"success\",";
				ostFinal << "\"description\" : \"\",";
				ostFinal << "\"handshakeUserStatus\" : \"" << handshakeUserStatus << "\",";
				ostFinal << "\"handshakeUsers\" : [" << hopUserList << "],";
				ostFinal << "\"user1\" : " << user1Data << ",";
				ostFinal << "\"user2\" : " << user2Data << "";
				ostFinal << "}";
			}
			else
			{
				auto 	error_message = "user1 or user2 is empty"s;

				MESSAGE_ERROR("", "", "user1(" + user1 + ") or user2(" + user2 + ") is empty")

				ostFinal.str("");
				ostFinal << "{";
				ostFinal << "\"result\" : \"error\",";
				ostFinal << "\"description\" : \"" + error_message + "\"";
				ostFinal << "}";

			}
	
			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		// --- JSON FindFriend by ID
		if(action == "JSON_getFindFriendByID")
		{
			ostringstream	ostFinal;
			string			sessid, lookForKey, userList;
			CMysql			db1;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			if(db1.Connect(&config) < 0)
			{
				MESSAGE_ERROR("", "", "Can not connect to mysql database");
				return(1);
			}

			lookForKey = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("lookForKey"));

			userList = GetUserListInJSONFormat("SELECT * FROM `users` WHERE `isActivated`='Y' and `isblocked`='N' and `id`=\"" + lookForKey + "\" ;", &db, &user);

			ostFinal.str("");
			ostFinal << "[" << std::endl << userList << std::endl << "]" << std::endl;

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if((action == "JSON_getFindFriendsListAutocomplete") || (action == "JSON_getFindFriendsListAutocompleteIncludingMyself") || (action == "JSON_getFindFriendsList"))
		{
			MESSAGE_DEBUG("", "", "start");

			auto	lookForKey = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("lookForKey"));

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");
			}
			else
			{
				if(lookForKey.length())
				{
					auto	include_myself = (action == "JSON_getFindFriendsListAutocompleteIncludingMyself");
					indexPage.RegisterVariableForce("result", "[" + GetUserListInJSONFormat_BySearchString(lookForKey, include_myself, &db, &user) + "]");
				}
				else
				{
					MESSAGE_ERROR("", action, "lookForKey is empty");
				}
			}


			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, " template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}
		}

		// --- JSON industry list for autocomplete
		if(action == "JSON_getIndustryListAutocomplete")
		{
			ostringstream   ost, ostFinal;
			string		  sessid, lookForKey, industriesList;
			vector<string>  searchWords;

			MESSAGE_DEBUG("", action, "start");

			// --- Initialization
			ostFinal.str("");

			if(user.GetLogin() == "Guest")
			{
				ostringstream   ost;

				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			lookForKey = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("lookForKey"));

			if(lookForKey.length() >= 2) 
			{
				ostringstream   ost;
				string		 	tmpStr = "";
				auto			affected = db.Query("SELECT * FROM `company_industry` WHERE `name` LIKE \"%" + lookForKey + "%\";");

				for(auto i = 0; i < affected; ++i)
				{
					if(i) tmpStr += ",";
					tmpStr += string("{\"id\":\"") + string(db.Get(i, "id")) + "\",";
					tmpStr += string("\"name\":\"") + string(db.Get(i, "name")) + "\"}";
				}
				// industriesList = GetCompanyListInJSONFormat(ost.str(), &db, &user);
				ostFinal << "{\"status\":\"success\",\"industries\":[" << tmpStr << "]}";

			}
			else
			{
				{
					MESSAGE_DEBUG("", action, "searching key is empty or less than 2");
				}
				ostFinal << "{\"status\":\"error\",\"description\":\"searching key is empty or less then 2\", \"industries\":[]}";
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			{
				MESSAGE_DEBUG("", action, "final response [" + ostFinal.str() + "]");
			}
		}

		// --- AJAX get certification vendors
		if(action == "JSON_getCertificationDetailsByTitle") 
		{
			ostringstream   ostFinal;
			string		  certificationTitle;
			string		  companiesList = "";

			MESSAGE_DEBUG("", action, "start");

			certificationTitle = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("certificationTitle"));

			if(certificationTitle.length()) 
			{

				if(db.Query("SELECT * FROM `certification_tracks` WHERE `title`=\"" + certificationTitle + "\";"))
				{
					string	  certificationTrackID = db.Get(0, "id");
					companiesList = GetCompanyListInJSONFormat("SELECT * FROM `company` WHERE `id` in (SELECT `vendor_id` FROM `certification_tracks` WHERE `id`=\"" + certificationTrackID + "\");", &db, &user);
				}
				else
				{
					MESSAGE_DEBUG("", action, "certification title new (no vendor)");
				}
			}
			else
			{
				MESSAGE_ERROR("", action, "certification title is empty");
			}

			ostFinal.str("");
			ostFinal << "{\"result\":\"success\",\"companies\":[" << companiesList << "]}";

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		// --- AJAX get course vendors
		if(action == "JSON_getCourseDetailsByTitle") 
		{
			ostringstream   ostFinal;
			string		  courseTitle;
			string		  companiesList = "";

			MESSAGE_DEBUG("", action, "start");

			courseTitle = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("courseTitle"));

			if(courseTitle.length()) 
			{

				if(db.Query("SELECT * FROM `certification_tracks` WHERE `title`=\"" + courseTitle + "\";"))
				{
					string	  courseTrackID = db.Get(0, "id");
					companiesList = GetCompanyListInJSONFormat("SELECT * FROM `company` WHERE `id` in (SELECT `vendor_id` FROM `certification_tracks` WHERE `id`=\"" + courseTrackID + "\");", &db, &user);
				}
				else
				{
					MESSAGE_DEBUG("", action, "course title new (no vendor)");
				}
			}
			else
			{
				MESSAGE_ERROR("", action, "course title is empty");
			}

			ostFinal.str("");
			ostFinal << "{\"result\":\"success\",\"companies\":[" << companiesList << "]}";

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_addEditCompanyAddCompanyFounder")
		{
			ostringstream   ostResult;
			MESSAGE_DEBUG("", action, "start");

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				ostringstream   ost;

				MESSAGE_DEBUG("", action, "re-login required");

				ostResult << "{\"result\": \"error\", \"description\": \"session lost. Need to relogin\"}";
			}
			else
			{
				string		  userName = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("userName"));
				string		  userID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("userid"));
				string		  companyID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("companyid"));

				if(companyID.length() && (userName.length() || userID.length()))
				{

					// --- check if company editor is admin
					if(db.Query("SELECT * FROM `company` WHERE `id`=\"" + companyID + "\" and `admin_userID`=\"" + user.GetID() + "\";"))
					{
						bool				isDuplicate = false;
						ostringstream		ost;

						ost.str("");
						ost << "SELECT * FROM `company_founder` WHERE (`companyID`=\"" << companyID << "\") and (`founder_name` LIKE \"%" << userName << "%\" ";
						if(userID.length() && (userID != "0")) ost << " or `founder_userID`=\"" << userID << "\" ";
						ost << ");";
						if(db.Query(ost.str())) isDuplicate = true;

						if(!isDuplicate)
						{
							unsigned long   newFounderID;

							ost.str("");
							if(userID.length()) ost << "INSERT INTO `company_founder` (`companyID`, `founder_userID`) VALUES (\"" << companyID << "\", \"" << userID << "\");";
							else ost << "INSERT INTO `company_founder` (`companyID`, `founder_name`) VALUES (\"" << companyID << "\", \"" << userName << "\");";

							newFounderID = db.InsertQuery(ost.str());
							if(newFounderID)
							{
								// --- Update live feed
								ost.str("");
								ost << "INSERT INTO `feed` (`title`, `userID`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << companyID << "\", \"1001\", \"" << newFounderID << "\", NOW())";
								if(db.InsertQuery(ost.str()))
								{

								}
								else
								{
									MESSAGE_ERROR("", action, "inserting into `feed` (" + ost.str() + ")");
								}

								ostResult << "{\"result\": \"success\", \"id\": \"" << newFounderID << "\"}";

							}
							else
							{
								MESSAGE_DEBUG("", action, "insertion into company_founder");

								ostResult << "{\"result\": \"error\", \"description\": \"ERROR: insertion into company_founder\"}";
							}
						}
						else
						{
							{
								MESSAGE_DEBUG("", action, "founder already exists");
							}

							ostResult << "{\"result\": \"error\", \"description\": \"Этот основатель уже есть\"}";
						}
					}
					else
					{
						MESSAGE_ERROR("", action, "company doesn't belongs to you");

						ostResult << "{\"result\": \"error\", \"description\": \"Вы не можете редактировать данные компании\"}";
					}
				}
				else
				{
					MESSAGE_ERROR("", "", "mandatory parameter missed or empty in HTML request [userName, userID]");

					ostResult << "{\"result\": \"error\", \"description\": \"any mandatory parameter missed\"}";
				}

			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("user not activated");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_addEditCompanyAddCompanyOwner")
		{
			ostringstream   ostResult;
			MESSAGE_DEBUG("", action, "start");

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				ostringstream   ost;

				MESSAGE_DEBUG("", action, "re-login required");

				ostResult << "{\"result\": \"error\", \"description\": \"session lost. Need to relogin\"}";
			}
			else
			{
				string		  userName	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("userName"));
				string		  userID	= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("userid"));
				string		  companyID	= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("companyid"));
				ostringstream   ost;

				if(companyID.length() && (userName.length() || userID.length()))
				{

					// --- check if company editor is admin
					ost.str("");
					ost << "SELECT * FROM `company` WHERE `id`=\"" << companyID << "\" and `admin_userID`=\"" << user.GetID() << "\";";
					if(db.Query(ost.str()))
					{
						bool				isDuplicate = false;

						ost.str("");
						ost << "SELECT * FROM `company_owner` WHERE (`companyID`=\"" << companyID << "\") and (`owner_name` LIKE \"%" << userName << "%\" ";
						if(userID.length() && (userID != "0")) ost << " or `owner_userID`=\"" << userID << "\" ";
						ost << ");";
						if(db.Query(ost.str())) isDuplicate = true;

						if(!isDuplicate)
						{
							unsigned long   newOwnerID;

							ost.str("");
							if(userID.length()) ost << "INSERT INTO `company_owner` (`companyID`, `owner_userID`) VALUES (\"" << companyID << "\", \"" << userID << "\");";
							else ost << "INSERT INTO `company_owner` (`companyID`, `owner_name`) VALUES (\"" << companyID << "\", \"" << userName << "\");";

							newOwnerID = db.InsertQuery(ost.str());
							if(newOwnerID)
							{
								// --- Update live feed
								ost.str("");
								ost << "INSERT INTO `feed` (`title`, `userID`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << companyID << "\", \"1002\", \"" << newOwnerID << "\", NOW())";
								if(db.InsertQuery(ost.str()))
								{

								}
								else
								{
									MESSAGE_ERROR("", action, "inserting into `feed` (" + ost.str() + ")");
								}

								ostResult << "{\"result\": \"success\", \"id\": \"" << newOwnerID << "\"}";

							}
							else
							{
								MESSAGE_DEBUG("", action, "insertion into company_owner");

								ostResult << "{\"result\": \"error\", \"description\": \"ERROR: insertion into company_owner\"}";
							}
						}
						else
						{
							{
								MESSAGE_DEBUG("", action, "owner already exists");
							}

							ostResult << "{\"result\": \"error\", \"description\": \"owner already exists\"}";
						}
					}
					else
					{
						MESSAGE_ERROR("", action, "company doesn't belongs to you");

						ostResult << "{\"result\": \"error\", \"description\": \"ERROR: company doesn't belongs to you\"}";
					}
				}
				else
				{
					MESSAGE_ERROR("", "", "mandatory parameter missed or empty in HTML request [userName, userID]");

					ostResult << "{\"result\": \"error\", \"description\": \"any mandatory parameter missed\"}";
				}

			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("user not activated");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_addEditCompanyAddCompanyIndustry")
		{
			ostringstream   ostResult;
			MESSAGE_DEBUG("", action, "start");

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				ostringstream   ost;

				MESSAGE_DEBUG("", action, "re-login required");

				ostResult << "{\"result\": \"error\", \"description\": \"session lost. Need to relogin\"}";
			}
			else
			{
				auto		  industryTitle		= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("industrytitle"));
				auto		  companyID			= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("companyid"));
				ostringstream   ost;


				if(companyID.length() && industryTitle.length())
				{

					// --- check if company editor is admin
					ost.str("");
					ost << "SELECT * FROM `company` WHERE `id`=\"" << companyID << "\" and `admin_userID`=\"" << user.GetID() << "\";";
					if(db.Query(ost.str()))
					{
						bool				isDuplicate = false;
						long int			industryID = 0;

						if(db.Query("SELECT * FROM `company_industry` WHERE `name`=\"" + industryTitle + "\";"))
						{
							industryID = stol(db.Get(0, "id"));
						}
						else
						{
							industryID = db.InsertQuery("INSERT INTO `company_industry` ( `name`) VALUES (\"" + industryTitle + "\");");
						}

						if(db.Query("SELECT * FROM `company_industry_ref` WHERE `company_id`=\"" + companyID + "\" and `profile_id`=\"" + to_string(industryID) + "\";")) isDuplicate = true;

						if(!isDuplicate)
						{
							unsigned long   newCompanyIndustryRef;

							newCompanyIndustryRef = db.InsertQuery("INSERT INTO `company_industry_ref` (`company_id`,`profile_id`) VALUES (\"" + companyID + "\",\"" + to_string(industryID) + "\");");
							if(newCompanyIndustryRef)
							{
								// --- Update live feed
								ost.str("");
								ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << companyID << "\", \"1003\", \"" << newCompanyIndustryRef << "\", NOW())";
								if(db.InsertQuery(ost.str()))
								{

								}
								else
								{
									MESSAGE_ERROR("", action, "inserting into `feed` (" + ost.str() + ")");
								}

								ostResult << "{\"result\": \"success\", \"company_industry_ref_id\": \"" << newCompanyIndustryRef << "\"}";

							}
							else
							{
								MESSAGE_DEBUG("", action, "insertion into company_owner");

								ostResult << "{\"result\": \"error\", \"description\": \"ERROR: insertion into company_owner\"}";
							}
						}
						else
						{
							{
								MESSAGE_DEBUG("", action, "industry already exists");
							}

							ostResult << "{\"result\": \"error\", \"description\": \"industry already exists\"}";
						}
					}
					else
					{
						MESSAGE_ERROR("", action, "company doesn't belongs to you");

						ostResult << "{\"result\": \"error\", \"description\": \"ERROR: company doesn't belongs to you\"}";
					}
				}
				else
				{
					MESSAGE_ERROR("", "", "mandatory parameter missed or empty in HTML request [industryTitle, industryID]");

					ostResult << "{\"result\": \"error\", \"description\": \"any mandatory parameter missed\"}";
				}

			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("user not activated");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_updateCompanyDescription")
		{
			ostringstream   ostResult;
			MESSAGE_DEBUG("", action, "start");

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				ostringstream   ost;

				MESSAGE_DEBUG("", action, "re-login required");

				ostResult << "{\"result\": \"error\", \"description\": \"session lost. Need to relogin\"}";
			}
			else
			{
				auto			companyDescription	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("description"));
				auto			companyID			= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("companyid"));
				ostringstream   ost;

				if(companyID.length())
				{

					// --- check if company editor is admin
					ost.str("");
					ost << "SELECT * FROM `company` WHERE `id`=\"" << companyID << "\" and `admin_userID`=\"" << user.GetID() << "\";";
					if(db.Query(ost.str()))
					{
						db.Query("UPDATE `company` SET `description` = \"" + companyDescription + "\",`lastActivity`=UNIX_TIMESTAMP() WHERE `id`=\"" + companyID + "\";");
	 
						// --- Update live feed
						if(db.InsertQuery("INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" + companyID + "\", \"1004\", \"0\", NOW())"))
						{
						}
						else
						{
							MESSAGE_ERROR("", action, "updating news_feed");
						}

						ostResult << "{\"result\": \"success\"}";
					}
					else
					{
						MESSAGE_ERROR("", action, "company doesn't belongs to you");

						ostResult << "{\"result\": \"error\", \"description\": \"ERROR: company doesn't belongs to you\"}";
					}
				}
				else
				{
					MESSAGE_ERROR("", "", "mandatory parameter missed or empty in HTML request [companyDescription, industryID]");

					ostResult << "{\"result\": \"error\", \"description\": \"any mandatory parameter missed\"}";
				}

			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("user not activated");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_updateCompanyLink")
		{
			string			value, id;
			ostringstream	ostFinal;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			value = toLower(CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value")));
			value = value.substr(0, 64);
			id = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("companyid"));
			ostFinal.str("");

			if(value.length() && id.length()) 
			{
				if(db.Query("SELECT `link` FROM `company` WHERE `id`=\"" + id + "\" AND `admin_userID`=\"" + user.GetID() + "\";"))
				{
					string	originalLink = db.Get(0, "link");

					if(value.length() < 10)
					{
						MESSAGE_DEBUG("", action, "link[" + value + "] is too short, must be longer than 10 symbols");

						ostFinal.str("");
						ostFinal << "{";
						ostFinal << "\"result\" : \"error\",";
						ostFinal << "\"link\" : \"" + originalLink + "\",";
						ostFinal << "\"description\" : \"ссылка должна быть минимум 10 символов\"";
						ostFinal << "}";
					}
					else if(value.find_first_not_of("abcdefghijklmnopqrstuvwxyz_-1234567890") != string::npos)
					{
						MESSAGE_DEBUG("", action, "link[" + value + "] contains prohibited symbols");

						ostFinal.str("");
						ostFinal << "{";
						ostFinal << "\"result\" : \"error\",";
						ostFinal << "\"link\" : \"" + originalLink + "\",";
						ostFinal << "\"description\" : \"ссылка можеть содержать только английские буквы или цифры\"";
						ostFinal << "}";
					}
					else
					{
						db.Query("UPDATE `company` SET `link`=\"" + value + "\"  WHERE `id`=\"" + id + "\";");

						if(!db.isError())
						{
							ostFinal.str("");
							ostFinal << "{";
							ostFinal << "\"result\" : \"success\",";
							ostFinal << "\"description\" : \"\"";
							ostFinal << "}";
						}
						else
						{
							MESSAGE_ERROR("", action, "updating DB");

							ostFinal.str("");
							ostFinal << "{";
							ostFinal << "\"result\" : \"error\",";
							ostFinal << "\"link\" : \"" + originalLink + "\",";
							ostFinal << "\"description\" : \"ошибка БД\"";
							ostFinal << "}";
						}
					}
				}
				else
				{
					MESSAGE_ERROR("", action, "user.id(" + user.GetID() + ") is not a company(" + id + ") owner");

					ostFinal.str("");
					ostFinal << "{";
					ostFinal << "\"result\" : \"error\",";
					ostFinal << "\"description\" : \"вы не можете изменить данные компании\"";
					ostFinal << "}";
				}
			}
			else
			{
				MESSAGE_ERROR("", action, "id(" + id + ") or value(" + value + ") is empty")

				ostFinal.str("");
				ostFinal << "{";
				ostFinal << "\"result\" : \"error\",";
				ostFinal << "\"description\" : \"пустые параметры id или value\"";
				ostFinal << "}";
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");

		}

		if(action == "AJAX_updateCompanyWebSite")
		{

			ostringstream   ostResult;
			MESSAGE_DEBUG("", action, "start");

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				ostringstream   ost;

				MESSAGE_DEBUG("", action, "re-login required");

				ostResult << "{\"result\": \"error\", \"description\": \"re-login required\"}";
			}
			else
			{
				string			companyWebsite = indexPage.GetVarsHandler()->Get("value");
				string			companyID = indexPage.GetVarsHandler()->Get("companyid");
				ostringstream   ost;

				companyID = CheckHTTPParam_Number(companyID);
				companyWebsite = CheckHTTPParam_Text(companyWebsite);

				if(regex_match(companyWebsite, regex("^https?://.*")))
				{

				}
				else
				{
					{
						MESSAGE_DEBUG("", action, "protocol missed, added prefix: http://");
					}

					companyWebsite = "http://" + companyWebsite;
				}

				if(companyID.length())
				{
					// --- check if company editor is admin
					if(db.Query("SELECT * FROM `company` WHERE `id`=\"" + companyID + "\" and `admin_userID`=\"" + user.GetID() + "\";"))
					{
						db.Query("UPDATE `company` SET `webSite` = \"" + companyWebsite.substr(0, 255) + "\",`lastActivity`=UNIX_TIMESTAMP() WHERE `id`=\"" + companyID + "\";");
	 
						// --- Update live feed
						if(db.InsertQuery("INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" + companyID + "\", \"1005\", \"0\", NOW())"))
						{
						}
						else
						{
							MESSAGE_ERROR("", action, "updating news_feed");
						}

						ostResult << "{\"result\": \"success\"}";
					}
					else
					{
						MESSAGE_ERROR("", action, "company doesn't belongs to you");

						ostResult << "{\"result\": \"error\", \"description\": \"ERROR: company doesn't belongs to you\"}";
					}
				}
				else
				{
					MESSAGE_ERROR("", action, "mandatory parameter missed or empty in HTML request [companyWebsite, companyID]");

					ostResult << "{\"result\": \"error\", \"description\": \"mandatory parameter missed\"}";
				}

			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("user not activated");
			}

			MESSAGE_DEBUG("", action, "finish");

		}

		if(action == "AJAX_updateCompanyEmployeeNumber")
		{

			ostringstream   ostResult;
			MESSAGE_DEBUG("", action, "start");

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				ostringstream   ost;

				MESSAGE_DEBUG("", action, "re-login required");

				ostResult << "{\"result\": \"error\", \"description\": \"session lost. Need to relogin\"}";
			}
			else
			{
				auto			employeeNumber = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("value").c_str());
				auto			companyID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("companyid"));
				ostringstream   ost;

				if(companyID.length())
				{
					// --- check if company editor is admin
					ost.str("");
					ost << "SELECT * FROM `company` WHERE `id`=\"" << companyID << "\" and `admin_userID`=\"" << user.GetID() << "\";";
					if(db.Query(ost.str()))
					{
						db.Query("UPDATE `company` SET `numberOfEmployee` = \"" + employeeNumber + "\",`lastActivity`=UNIX_TIMESTAMP() WHERE `id`=\"" + companyID + "\";");
	 
						// --- Update live feed
						if(db.InsertQuery("INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" + companyID + "\", \"1006\", \"0\", NOW())"))
						{
						}
						else
						{
							MESSAGE_ERROR("", action, "updating news_feed");
						}

						ostResult << "{\"result\": \"success\"}";
					}
					else
					{
						MESSAGE_ERROR("", action, "company doesn't belongs to you");

						ostResult << "{\"result\": \"error\", \"description\": \"ERROR: company doesn't belongs to you\"}";
					}
				}
				else
				{
					MESSAGE_ERROR("", "", "mandatory parameter missed or empty in HTML request [employeeNumber, industryID]");

					ostResult << "{\"result\": \"error\", \"description\": \"any mandatory parameter missed\"}";
				}

			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("user not activated");
			}

			MESSAGE_DEBUG("", action, "finish");

		}

		if(action == "AJAX_updateCompanyFoundationDate")
		{

			ostringstream   ostResult;
			MESSAGE_DEBUG("", action, "start");

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				ostringstream   ost;

				MESSAGE_DEBUG("", action, "re-login required");

				ostResult << "{\"result\": \"error\", \"description\": \"session lost. Need to relogin\"}";
			}
			else
			{
				auto		  foundationDate	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));
				auto		  companyID			= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("companyid"));
				ostringstream	ost;

				if(companyID.length() && foundationDate.length())
				{
					// --- check if company editor is admin
					ost.str("");
					ost << "SELECT * FROM `company` WHERE `id`=\"" << companyID << "\" and `admin_userID`=\"" << user.GetID() << "\";";
					if(db.Query(ost.str()))
					{
						db.Query("UPDATE `company` SET `foundationDate` = \"" + foundationDate + "\",`lastActivity`=UNIX_TIMESTAMP() WHERE `id`=\"" + companyID + "\";");
	 
						// --- Update live feed
						if(db.InsertQuery("INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" + companyID + "\", \"1007\", \"0\", NOW())"))
						{
						}
						else
						{
							MESSAGE_ERROR("", action, "updating news_feed");
						}

						ostResult << "{\"result\": \"success\"}";
					}
					else
					{
						MESSAGE_ERROR("", action, "company doesn't belongs to you");

						ostResult << "{\"result\": \"error\", \"description\": \"ERROR: company doesn't belongs to you\"}";
					}
				}
				else
				{
					MESSAGE_ERROR("", "", "mandatory parameter missed or empty in HTML request [foundationDate, industryID]");

					ostResult << "{\"result\": \"error\", \"description\": \"any mandatory parameter missed\"}";
				}

			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("user not activated");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_updateCompanyType")
		{

			ostringstream   ostResult;
			MESSAGE_DEBUG("", action, "start");

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				ostringstream   ost;

				MESSAGE_DEBUG("", action, "re-login required");

				ostResult << "{\"result\": \"error\", \"description\": \"session lost. Need to relogin\"}";
			}
			else
			{
				auto			companyType	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));
				auto			companyID		= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("companyid"));
				ostringstream   ost;

				if(companyID.length() && companyType.length())
				{
					// --- check if company editor is admin
					ost.str("");
					ost << "SELECT * FROM `company` WHERE `id`=\"" << companyID << "\" and `admin_userID`=\"" << user.GetID() << "\";";
					if(db.Query(ost.str()))
					{
						if(companyType == "___") companyType = "";

						db.Query("UPDATE `company` SET `type` = \"" + companyType + "\",`lastActivity`=UNIX_TIMESTAMP() WHERE `id`=\"" + companyID + "\";");
	 
						// --- Update live feed
						if(db.InsertQuery("INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" + companyID + "\", \"1008\", \"0\", NOW())"))
						{
						}
						else
						{
							MESSAGE_ERROR("", action, "updating news_feed");
						}

						ostResult << "{\"result\": \"success\"}";
					}
					else
					{
						MESSAGE_ERROR("", action, "company doesn't belongs to you");

						ostResult << "{\"result\": \"error\", \"description\": \"ERROR: company doesn't belongs to you\"}";
					}
				}
				else
				{
					MESSAGE_ERROR("", "", "mandatory parameter missed or empty in HTML request [companyType, industryID]");

					ostResult << "{\"result\": \"error\", \"description\": \"any mandatory parameter missed\"}";
				}

			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("user not activated");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		// --- JSON get list of my friends
		if((action == "JSON_getMyNetworkFriendList") || (action == "JSON_getWhoWatchedONMeList"))
		{
			ostringstream	ost, ostFinal, friendsSqlQuery;
			string			sessid, lookForKey, userList = "";
			CMysql			db1;
			int				affected;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			if(db1.Connect(&config) < 0)
			{
				MESSAGE_ERROR("", "", "Can not connect to mysql database");

				return(1);
			}

			friendsSqlQuery.str("");
			ost.str("");
			if(action == "JSON_getMyNetworkFriendList")
				ost << "SELECT `friendID` FROM `users_friends` WHERE `userID`='" << user.GetID() << "';";
			if(action == "JSON_getWhoWatchedONMeList")
				ost << "SELECT `watching_userID` as `friendID` FROM `users_watched` WHERE `watched_userID`='" << user.GetID() << "';";
			affected = db.Query(ost.str());
			if(affected)
			{
				friendsSqlQuery << "SELECT * FROM `users` WHERE `isActivated`='Y' and `isblocked`='N' and `id` IN (";
				for(auto i = 0; i < affected; i++)
				{
					friendsSqlQuery << (i > 0 ? ", " : "") << db.Get(i, "friendID");
				}
				friendsSqlQuery << ");";

				{
					MESSAGE_DEBUG("", action, "query for JSON prepared [" + friendsSqlQuery.str() + "]");
				}

				userList = GetUserListInJSONFormat(friendsSqlQuery.str(), &db, &user);
			}


			ostFinal.str("");
			ostFinal << "[" << std::endl << userList << std::endl << "]" << std::endl;

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}
		}
/*
		if(action == "JSON_chatGetInitialData")
		{
			ostringstream	ost, ostFinal, friendsSqlQuery, chatMessageQuery;
			string			sessid, lookForKey, userArray = "", messageArray = "";

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				ostFinal << "result: fail";
			}
			else
			{
				auto			friends_id = GetValuesFromDB("SELECT `friendID` FROM `users_friends` WHERE `userID`='" + user.GetID() + "';", &db);

				if(friends_id.size())
				{
					userArray = GetUserListInJSONFormat("SELECT * FROM `users` WHERE `isActivated`='Y' and `isblocked`='N' and `id` IN (" + join(friends_id, ",") + ");", &db, &user);
					messageArray = GetChatMessagesInJSONFormat("SELECT * FROM `chat_messages` WHERE `toID`='" + user.GetID() + "' or `fromID`='" + user.GetID() + "';", &db);
				}

				ostFinal.str("");
				ostFinal << "\"result\": \"success\"," << std::endl;
				ostFinal << "\"userArray\": [" << userArray << "]," << std::endl;
				ostFinal << "\"messageArray\": [" << messageArray << "]";
			}


			indexPage.RegisterVariableForce("result", "{" + ostFinal.str() + "}");

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}
*/

		// --- JSON avatar list
		if(action == "JSON_getAvatarList") 
		{
			MESSAGE_DEBUG("", action, "start");

			auto	affected = db.Query("SELECT * FROM `users_avatars` WHERE `userid`=\"" + user.GetID() + "\";");;

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			if(affected > 0)
			{
				ostringstream	ost;

				ost.str("");
				for(auto i = 0; i < affected; i++) 
				{
					if(i) ost << ",";
					ost << "{ \"folder\": \"" << db.Get(i, "folder") << "\", \"filename\": \"" << db.Get(i, "filename") << "\", \"isActive\": \"" << db.Get(i, "isActive") << "\", \"avatarID\": \"" << db.Get(i, "id") << "\" }";
				}

				indexPage.RegisterVariableForce("result", ost.str());
			}
			else
			{
				MESSAGE_DEBUG("", action, "there are no avatars for user " + user.GetLogin());
			}

			if(!indexPage.SetTemplate("ajax_getJobTitles.htmlt"))
			{
				MESSAGE_ERROR("", "", "template file ajax_response.htmlt was missing");

				throw CException("Template file was missing");
			}
		}

		// --- JSON get user CV
		if(action == "JSON_getUserCV") 
		{
			ostringstream	ost, ostResult;
			string		sessid;
			int			affected;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			ost.str("");
			ostResult.str("");
			ost << "SELECT * FROM `users` WHERE `id`=\"" << user.GetID() << "\";";
			affected = db.Query(ost.str());
			if(affected > 0)
			{
				ostResult << "{ \"result\": \"success\", \"cv\": \"" << db.Get(0, "cv") << "\" }";
			}
			else
			{
				ostResult << "{ \"result\": \"fail\", \"description\": \"error receiving CV FROM DB\" }";

				MESSAGE_DEBUG("", action, "issue in getting users(" + user.GetLogin() + ") CV");
			}

			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}
		}

		// --- JSON get comments list on message
		if((action == "JSON_getCommentsOnMessage") || 
			(action == "JSON_getCommentsOnBook") || 
			(action == "JSON_getCommentsOnCertification") || 
			(action == "JSON_getCommentsOnCompany") || 
			(action == "JSON_getCommentsOnLanguage") || 
			(action == "JSON_getCommentsOnScienceDegree"))
		{
			ostringstream	ost, ostList;
			string			messageID;
			string		  commentType;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.RegisterVariableForce("result", "{\"result\":\"error\",\"description\":\"re-login required\",\"link\":\"/autologin?rand=" + GetRandom(10) + "\"}");
			}
			else
			{
				messageID = indexPage.GetVarsHandler()->Get("messageID");

				// --- ensure that it is number
				try {
					messageID = to_string(stol(messageID));
				} catch(...) {
					MESSAGE_ERROR("", action, "can't convert messageID[" + messageID + "] to number");

					messageID = "";
				}

				if(action == "JSON_getCommentsOnMessage") commentType = "\"message\"";
				if(action == "JSON_getCommentsOnBook") commentType = "\"book\"";
				if(action == "JSON_getCommentsOnCertification") commentType = "\"certification\", \"course\"";
				if(action == "JSON_getCommentsOnScienceDegree") commentType = "\"university\"";
				if(action == "JSON_getCommentsOnCompany") commentType = "\"company\"";
				if(action == "JSON_getCommentsOnLanguage") commentType = "\"language\"";

				if(messageID != "")
				{
					int				affected;

					ostList.str("");
					ost.str("");
					ost << "SELECT "
						<< "`feed_message_comment`.`id` as `feed_message_comment_id`, "
						<< "`feed_message_comment`.`messageID` as `feed_message_comment_messageID`, "
						<< "`feed_message_comment`.`userID` as `feed_message_comment_userID`, "
						<< "`feed_message_comment`.`comment` as `feed_message_comment_comment`, "
						<< "`feed_message_comment`.`eventTimestamp` as `feed_message_comment_eventTimestamp`, "
						<< "`users`.`id` as `users_id`, "
						<< "`users`.`name` as `users_name`, "
						<< "`users`.`nameLast` as `users_nameLast` "
						<< "from `feed_message_comment` "
						<< "inner join `users` on `feed_message_comment`.`userID`=`users`.`id` "
						<< "where `messageID`=\"" << messageID << "\" and `feed_message_comment`.`type` in (" + commentType + ") and `users`.`isactivated`='Y' and `users`.`isblocked`='N'";

					affected = db.Query(ost.str());
					if(affected)
					{
						ostList.str("");
						for(auto i = 0; i < affected; i++)
						{
							if(i > 0) ostList << "," << std::endl;
							ostList << "{";
							ostList << "\"id\" : \"" << db.Get(i, "feed_message_comment_id") << "\","; 
							ostList << "\"messageID\" : \"" << db.Get(i, "feed_message_comment_messageID") << "\","; 
							ostList << "\"user\" : {";
							ostList << 		"\"userID\" : \"" << db.Get(i, "users_id") << "\","; 
							ostList << 		"\"name\" : \"" << db.Get(i, "users_name") << "\","; 
							ostList << 		"\"nameLast\" : \"" << db.Get(i, "users_nameLast") << "\""; 
							ostList << "},";
							ostList << "\"comment\" : \"" << db.Get(i, "feed_message_comment_comment") << "\","; 
							ostList << "\"eventTimestamp\" : \"" << db.Get(i, "feed_message_comment_eventTimestamp") << "\","; 
							ostList << "\"eventTimestampDelta\":\"" << GetHumanReadableTimeDifferenceFromNow(db.Get(i,"feed_message_comment_eventTimestamp")) << "\"";
							ostList << "}";
						}
					}

					ost.str("");
					ost << "{" << std::endl;
					ost << "\"result\": \"success\",";
					ost << "\"commentsList\": [" << ostList.str() << "]" << std::endl;
					ost << "}" << std::endl;
				}
				else
				{
					// --- Empty messageID
					ost.str("");
					ost << "{";
					ost << "\"result\": \"error\",";
					ost << "\"description\": \"can't get comments list due to messageID is empty\"";
					ost << "}";

					MESSAGE_ERROR("", action, "can't convert messageID[" + indexPage.GetVarsHandler()->Get("messageID") + "] to number");
				} // --- if(messageID != "")

				indexPage.RegisterVariableForce("result", ost.str());
			}

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("template page missing");
			} // if(!indexPage.SetTemplate("json_response.htmlt"))

			MESSAGE_DEBUG("", action, "finish");
		}
		if(action == "AJAX_chatMarkMessageReadByMessageID")
		{
			ostringstream	ost, ostFinal;
			string			messageID = "";

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.RegisterVariableForce("result", "{"
															"\"result\":\"error\","
															"\"description\":\"re-login required\","
															"\"location\":\"/login?rand=" + GetRandom(10) + "\""
															"}");
			}
			else
			{
				messageID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("messageid"));

				MESSAGE_DEBUG("", action, "message [" + messageID + "]");

				if(messageID.length())
				{
					ost.str("");
					ost << "UPDATE `chat_messages` SET `messageStatus`=\"read\" \
							WHERE `toID`='" << user.GetID() << "' and `id`='" << messageID << "';";

					db.Query(ost.str());

					{
						ostFinal.str("");
						ostFinal << "\"result\": \"success\"" << std::endl;
					}
				}
				else
				{
					MESSAGE_ERROR("", action, "messageID is empty");

					ostFinal.str("");
					ostFinal << "\"result\": \"error\"," << std::endl;
					ostFinal << "\"description\": \"messageID is empty\"" << std::endl;
				}

				indexPage.RegisterVariableForce("result", "{" + ostFinal.str() + "}");
			}


			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_notificationMarkMessageReadByMessageID")
		{
			ostringstream	ost, ostFinal;
			string			notificationID = "";

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					MESSAGE_ERROR("", action, "re-login required");
				}

				indexPage.RegisterVariableForce("result", "{"
															"\"result\":\"error\","
															"\"description\":\"re-login required\","
															"\"location\":\"/login?rand=" + GetRandom(10) + "\""
															"}");
			}
			else
			{
				notificationID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("notificationID"));

				MESSAGE_DEBUG("", action, "message [" + notificationID + "]");

				if(notificationID.length())
				{
					ost.str("");
					ost << "UPDATE `users_notification` SET `notificationStatus`=\"read\" \
							WHERE `userId`='" << user.GetID() << "' and `id`='" << notificationID << "';";

					db.Query(ost.str());

					if(!db.isError())
					{
						ostFinal.str("");
						ostFinal << "\"result\": \"success\"" << std::endl;
					}
					else
					{
						MESSAGE_ERROR("", action, "updating users_notification table");

						ostFinal.str("");
						ostFinal << "\"result\": \"fail\"" << std::endl;
					}
				}
				else
				{
					MESSAGE_ERROR("", action, "notificationID is empty");

					ostFinal.str("");
					ostFinal << "\"result\": \"error\"," << std::endl;
					ostFinal << "\"description\": \"notificationID is empty\"" << std::endl;
				}

				indexPage.RegisterVariableForce("result", "{" + ostFinal.str() + "}");
			}


			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_chatMarkMessageReadByUserID")
		{
			ostringstream	ost, ostFinal;
			string			userID = "";

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.RegisterVariableForce("result", "{"
															"\"result\":\"error\","
															"\"description\":\"re-login required\","
															"\"location\":\"/login?rand=" + GetRandom(10) + "\""
															"}");
			}
			else
			{
				userID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("userID"));

				MESSAGE_DEBUG("", action, "message [" + userID + "]");

				if(userID.length())
				{
					ost.str("");
					ost << "UPDATE `chat_messages` SET `messageStatus`=\"read\" \
							WHERE `fromID`='" << userID << "' AND `toID`='" << user.GetID() << "' AND `messageStatus`='unread';";

					db.Query(ost.str());

					{
						ostFinal.str("");
						ostFinal << "\"result\": \"success\"," << std::endl;
					}
				}
				else
				{
					MESSAGE_ERROR("", action, "userID is empty");

					ostFinal.str("");
					ostFinal << "\"result\": \"error\"," << std::endl;
					ostFinal << "\"description\": \"userID is empty\"" << std::endl;
				}

				indexPage.RegisterVariableForce("result", "{" + ostFinal.str() + "}");
			}


			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}


		// --- cleanup news feed image after closing the modal window 
		if((action == "AJAX_cleanupFeedImages") || (action == "AJAX_editCleanupFeedImages"))
		{
			string		imageTempSet;

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.RegisterVariableForce("result", "{"
															"\"result\":\"error\","
															"\"description\":\"re-login required\","
															"\"location\":\"/login?rand=" + GetRandom(10) + "\""
															"}");
			}
			else
			{
				imageTempSet = indexPage.GetVarsHandler()->Get("imageTempSet");
				if(imageTempSet.length() > 0)
				{
					RemoveMessageImages("`tempSet`=\"" + imageTempSet + "\" AND `userID`=\"" + user.GetID() + "\" AND `setID`=\"0\"", &db);

					// --- cleanup DB tempSet for edit images 
					db.Query("UPDATE `feed_images` SET `tempSet`='0', `removeFlag`=\"keep\" WHERE `tempSet`=\"" + imageTempSet + "\" AND `userID`=\"" + user.GetID() + "\";");
					if(db.isError())
					{
						MESSAGE_ERROR("", action, "DB error message(" + db.GetErrorMessage() + ")");
					}
				}
				else
				{
					MESSAGE_ERROR("", action, "imageTempSet is empty");
				}


				indexPage.RegisterVariableForce("result", "{\"result\" : \"success\"}");
			}


			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}
		}


		// --- prepare new feed image to new image set loading
		if(action == "AJAX_prepareFeedImages")
		{
			string		currentEmployment, companyId;
			ostringstream	ost;

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.RegisterVariableForce("result", "{"
															"\"result\":\"error\","
															"\"description\":\"re-login required\","
															"\"location\":\"/autologin?rand=" + GetRandom(10) + "\""
															"}");

			}
			else
			{

				db.Query("update `feed_images` set `tempSet`='0' WHERE `userID`='" + user.GetID() + "';");

				indexPage.RegisterVariableForce("result", "{"
															"\"result\":\"success\","
															"\"companies\":[" + GetCompanyListInJSONFormat("SELECT * FROM `company` WHERE `isBlocked`='N' and (`admin_userID`=\"" + user.GetID() + "\");", &db, &user) + "]"
															"}");
			}


			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}
		}

		// --- prepare edit feed image to new image set loading
		if(action == "AJAX_prepareEditFeedImages")
		{
			ostringstream	ost;

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.RegisterVariableForce("result", "{"
															"\"result\":\"error\","
															"\"description\":\"re-login required\","
															"\"location\":\"/login?rand=" + GetRandom(10) + "\""
															"}");
			}
			else
			{
				
				auto	messageID = indexPage.GetVarsHandler()->Get("messageID");
				auto	imageTempSet = indexPage.GetVarsHandler()->Get("imageTempSet");

				if((messageID.length() > 0) and (messageID != "0") and (imageTempSet.length() > 0) and (imageTempSet != "0"))
				{
					if(AmIMessageOwner(messageID, &user, &db))
					{
						auto 	messageOwner = GetMessageOwner(messageID, &user, &db);
						auto	messageOwnerType = messageOwner.first;
						auto	messageOwnerID = messageOwner.second;

						if(messageOwnerType.length() && messageOwnerID.length())
						{
							// --- "where `tempSet`!='0'" using to speed up lookup through `feed_images` table
							db.Query("UPDATE `feed_images` set `tempSet`='0' WHERE `srcType`=\"" + messageOwnerType + "\" AND `userID`=\"" + messageOwnerID + "\" and `tempSet`!=\"0\";");

							if(db.Query("SELECT  `imageSetID` FROM  `feed_message` WHERE  `id` = \"" + messageID + "\";"))
							{
								auto	imageSetID = db.Get(0, "imageSetID");

								// --- this condition avoid assigning "lost pictures" to a new message
								// --- how to reproduce:
								// --- 1) create "lost picture" (`feedImages` table: `imageSet`=0, `userID`=XXXX)
								// --- 2) craft post without images
								// --- 3) edit post title
								// ---		PROBLEM: :lost pictures" having imageSet=0 which will be reassigned to edited post 
								if(imageSetID.length() && (imageSetID != "0"))
								{
									db.Query("UPDATE `feed_images` SET `tempSet`=\"" + imageTempSet + "\", `removeFlag`=\"keep\" WHERE `setID`=\"" + imageSetID + "\";");
								}
								else
								{
									MESSAGE_DEBUG("", action, "there is no media in this post [messageID: " + messageID + string("]"));
								}
							}

							indexPage.RegisterVariableForce("result", "{\"result\" : \"success\"}");
						}
						else
						{
							MESSAGE_ERROR("", action, "message owner error (type:" + messageOwnerType + ", id:" + messageOwnerID + ")");

							ost.str("");
							ost << "{";
							ost << "\"result\" : \"error\",";
							ost << "\"description\" : \"Ошибка: Не получилось определить владельца сообщения\"";
							ost << "}";
						}
					}
					else
					{
						MESSAGE_ERROR("", action, "message.id(" + messageID + ") doesn't belongs to user.id(" + user.GetID() + ") or his companies");
						indexPage.RegisterVariableForce("result", "{\"result\" : \"error\", \"description\" : \"вы не можете редактировать сообщение\"}");
					}
				}
				else
				{
					MESSAGE_ERROR("", action, "messageID(" + messageID + ") or imageTempSet(" + imageTempSet + ") is empty");
					indexPage.RegisterVariableForce("result", "{\"result\" : \"error\", \"description\" : \"issue with messageID or imageTempSet parameters (empty or '0')\"}");
				}

			}

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}
		}

		// --- JSON get current employment
		if(action == "JSON_getCurrentEmployment")
		{
			string		currentEmployment, companyId;
			ostringstream	ost;
			int			affected;

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			ost.str("");
			ost << "SELECT `company`.`name` as `company_name` FROM `company`, `users_company` WHERE `users_company`.`occupation_finish`=\"0000-00-00\" and `users_company`.`company_id`=`company`.`id`;";
			if((affected = db.Query(ost.str())) > 0) 
			{
				ost.str("");
				for(auto i = 0; i < affected; i++)
				{
					ost << "{ \"folder\": \"" << db.Get(i, "folder") << "\", \"filename\": \"" << db.Get(i, "filename") << "\", \"isActive\": \"" << db.Get(i, "isActive") << "\" }";
					if(i < (affected-1)) ost << ", ";
				}
			}
			else
			{
				MESSAGE_DEBUG("", action, "there are no company of current employment of user " + user.GetID());
			}
			indexPage.RegisterVariableForce("result", ost.str());

			if(!indexPage.SetTemplate("ajax_getJobTitles.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file ajax_response.htmlt was missing");
				throw CException("Template file was missing");
			}

		}
		// --- JSON part has ended


		// --- AJAX part has started

		// --- AJAX post message to news feed
		if(action == "AJAX_postNewsFeedMessage")
		{
			MESSAGE_DEBUG("", action, "start");

			auto			error_message = ""s;

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				LogoutIfGuest(action, &config, &indexPage, &db, &user);
			}
			else
			{
				auto			prohibitReason = "prohibited to post message"s;
				bool			admittedToPost = false;

				// --- Authorized user
				auto	newsFeedMessageDstType			= CheckHTTPParam_Text	(indexPage.GetVarsHandler()->Get("newsFeedMessageDstType"));
				auto	newsFeedMessageDstID			= CheckHTTPParam_Number	(indexPage.GetVarsHandler()->Get("newsFeedMessageDstID"));
				auto	newsFeedMessageSrcType			= CheckHTTPParam_Text	(indexPage.GetVarsHandler()->Get("newsFeedMessageSrcType"));
				auto	newsFeedMessageSrcID			= CheckHTTPParam_Number	(indexPage.GetVarsHandler()->Get("newsFeedMessageSrcID"));
				auto	newsFeedMessageTitle			= CheckHTTPParam_Text	(indexPage.GetVarsHandler()->Get("newsFeedMessageTitle"));
				auto	newsFeedMessageLink				= CheckHTTPParam_Text	(indexPage.GetVarsHandler()->Get("newsFeedMessageLink"));
				auto	newsFeedMessageText				= CheckHTTPParam_Text	(indexPage.GetVarsHandler()->Get("newsFeedMessageText"));
				auto	newsFeedMessageRights			= CheckHTTPParam_Text	(indexPage.GetVarsHandler()->Get("newsFeedMessageRights"));
				auto	newsFeedMessageImageTempSet		= CheckHTTPParam_Number	(indexPage.GetVarsHandler()->Get("newsFeedMessageImageTempSet"));
				auto	newsFeedMessageImageSet = ""s;

				if(newsFeedMessageDstID == "") newsFeedMessageDstID = "0";

				// --- check src if permitted to post message
				if((newsFeedMessageSrcType == "") || (newsFeedMessageSrcType == "user"))
				{
					if(newsFeedMessageSrcID != user.GetID())	
					{
						MESSAGE_ERROR("", action, "spoofed userID");
					}

					newsFeedMessageSrcType = "user";
					newsFeedMessageSrcID = user.GetID();
					admittedToPost = true;
				}
				else if(newsFeedMessageSrcType == "company")
				{
					if(db.Query("SELECT `id`, `admin_userID` FROM `company` WHERE `id`=\"" + newsFeedMessageSrcID + "\" AND `admin_userID`=\"" + user.GetID() + "\";"))
						admittedToPost = true;
					else
					{
						prohibitReason = gettext("company or occupation not found");
						MESSAGE_ERROR("", action, "userID[" + user.GetID() + "] spoofed companyID[" + newsFeedMessageSrcID + "]");
					}
				}
				else
				{
					prohibitReason = gettext("message owner is not known");
					MESSAGE_ERROR("", action, "unknown newsFeedMessageSrcType[" + newsFeedMessageSrcType + "]");
				}

				// --- check dst if permitted to post message
				if(newsFeedMessageDstType == "group")
				{
					if(db.Query("SELECT `id` FROM `groups` WHERE `id`=\"" + newsFeedMessageDstID + "\" AND `isBlocked`=\"N\";"))
					{
						// --- no change in post admittance
						db.Query("UPDATE `groups` SET `eventTimestampLastPost`=UNIX_TIMESTAMP() WHERE `id`=\"" + newsFeedMessageDstID + "\";");
						if(db.isError())
						{
							MESSAGE_ERROR("", action, "DB error message(" + db.GetErrorMessage() + ")");
						}
					}
					else
					{
						prohibitReason = gettext("group not found or blocked");
						MESSAGE_ERROR("", action, "group.id[" + newsFeedMessageDstID + "] not found or blocked");

						admittedToPost = false;
					}
				}

				// --- check adverse words
				if(isAdverseWordsHere(newsFeedMessageText, &db) || isAdverseWordsHere(newsFeedMessageTitle, &db))
				{
					prohibitReason = gettext("adverse words present in message");
					MESSAGE_DEBUG("", action, prohibitReason);

					admittedToPost = false;
				}


				if(admittedToPost)
				{
					// --- handle images assigned to message
					if(newsFeedMessageImageTempSet.length() && (newsFeedMessageImageTempSet != "0"))
					{
						// --- remove all images subjected to removal
						RemoveMessageImages("`tempSet`=\"" + newsFeedMessageImageTempSet + "\" and `userID`=\"" + user.GetID() + "\" and `removeFlag`=\"remove\";", &db);

						// --- update images assigned to message
						if(newsFeedMessageImageTempSet.length() && (newsFeedMessageImageTempSet != "0") && db.Query("SELECT `id` FROM `feed_images` WHERE `tempSet`=\"" + newsFeedMessageImageTempSet + "\" and `userID`=\"" + user.GetID() + "\";"))
						{
							newsFeedMessageImageSet = db.Get(0, "id");

							db.Query("UPDATE  `feed_images` SET "
										"`setID`=\"" + newsFeedMessageImageSet + "\","
										"`tempSet`=\"0\","
										"`srcType`=\"" + newsFeedMessageSrcType + "\","
										"`userID`=\"" + newsFeedMessageSrcID + "\" "
										"WHERE  `tempSet`=\"" + newsFeedMessageImageTempSet + "\" AND `userID`=\"" + user.GetID() + "\";");
						}
						else
						{
							MESSAGE_DEBUG("", action, "DEBUG: there are no images associated with `tempSet`='" + newsFeedMessageImageTempSet + "' and `userID`='" + user.GetID() + "'");
						}
					}
					else
					{
						MESSAGE_ERROR("", action, "imageTempSet is empty or 0;");
					}

					if(!((newsFeedMessageTitle == "") && (newsFeedMessageText == "") && (newsFeedMessageImageSet == "")))
					{
						auto	feed_messageID = db.InsertQuery(
																"INSERT INTO `feed_message` (`title`, `link`, `message`, `imageSetID`, `access`) "
																"VALUES ("
																"\"" + newsFeedMessageTitle + "\", "
																"\"" + newsFeedMessageLink + "\", "
																"\"" + newsFeedMessageText + "\", "
																"\"" + (newsFeedMessageImageSet.length() ? newsFeedMessageImageSet : "0") + "\", "
																"\"" + newsFeedMessageRights + "\" "
																");"
																);
						if(feed_messageID)
						{
							auto	feed_id = db.InsertQuery("INSERT INTO `feed` (`title`, `dstType`, `dstID`, `srcType`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" + newsFeedMessageDstType + "\",\"" + newsFeedMessageDstID + "\",\"" + newsFeedMessageSrcType + "\",\"" + newsFeedMessageSrcID + "\", \"11\", " + to_string(feed_messageID) + ", NOW());");
							if(feed_id)
							{
								// --- message has been posted

								// --- attach ribbon
								error_message = AttachRibbon(to_string(feed_id), &db, &user);
								if(error_message.length())
								{
									MESSAGE_ERROR("", action, error_message);
								}
							}
							else
							{
								error_message = gettext("SQL syntax error");
								MESSAGE_ERROR("", action, error_message);
							}
						}
						else
						{
							error_message = gettext("SQL syntax error");
							MESSAGE_ERROR("", action, error_message);
						}

						MESSAGE_DEBUG("", action, "end (message FROM " + newsFeedMessageSrcType + ".id[" + newsFeedMessageSrcID + "] has been posted)");
					} // if(!((newsFeedMessageTitle == "") && (newsFeedMessageText == "") && (newsFeedMessageImage == "")))
					else
					{
						// --- Empty title, message and image
						error_message = gettext("can't post message due to title, text and image is empty");
						MESSAGE_ERROR("", action, error_message);
					} // if(!((newsFeedMessageTitle == "") && (newsFeedMessageText == "") && (newsFeedMessageImage == "")))
				}
				else
				{
					error_message = prohibitReason;

					MESSAGE_ERROR("", action, "" + newsFeedMessageSrcType + ".id[" + newsFeedMessageSrcID + "] prohibited to post");
				}
			}

			AJAX_ResponseTemplate(&indexPage, "", error_message);

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_updateUserCV")
		{
			ostringstream	ostResult;

			MESSAGE_DEBUG("", action, "start");


			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				ostResult << "{\"result\": \"error\", \"description\": \"session lost. Need to relogin\"}";
			}
			else
			{
				string			cvText = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("cv"));
				ostringstream	ost;

				ost.str("");
				ost << "update `users` set `cv`=\"" << cvText << "\" WHERE `id`=\"" << user.GetID() << "\";";
				db.Query(ost.str());
				if(!db.isError())
				{

					// --- Update live feed
					ost.str("");
					ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"21\", \"0\", NOW())";
					if(db.InsertQuery(ost.str()))
					{

						ostResult << "{\"result\": \"success\"}";
					}
					else
					{
						ostResult << "{\"result\": \"error\", \"description\": \"error updating live feed\"}";

						MESSAGE_ERROR("", action, "can't update feed table");
					}
				}
				else
				{
					ostResult << "{\"result\": \"error\", \"description\": \"error updating table users\"}";

					MESSAGE_ERROR("", action, "can't update users table");
				}


			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("user not activated");
			}

			MESSAGE_DEBUG("", action, "finish");

		}

		if(action == "AJAX_changeEditProfileCompanyEmploymentEndDateStatus")
		{
			ostringstream	ostResult;

			MESSAGE_DEBUG("", action, "start");


			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				ostResult << "{\"result\": \"error\", \"description\": \"session lost. Need to relogin\"}";
			}
			else
			{
				auto		companyID = stol(indexPage.GetVarsHandler()->Get("companyID"));
				ostringstream	ost;

				ost.str("");
				ost << "SELECT * FROM `users_company` WHERE `id`=\"" << companyID << "\";";
				if(db.Query(ost.str()))
				{
					if(db.Get(0, "user_id") == user.GetID())
					{
						string	currentStatus = db.Get(0, "current_company");

						ost.str("");
						ost << "update `users_company` set `current_company`='" << (currentStatus == "0" ? "1" : "0") << "' WHERE `id`=\"" << companyID << "\";";
						db.Query(ost.str());

						// --- Update live feed
						ost.str("");
						ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"2\", \"0\", NOW())";
						if(db.InsertQuery(ost.str()))
						{
							ostResult << "{\"result\": \"success\"}";
						}
						else
						{
							ostResult << "{\"result\": \"error\", \"description\": \"error updating live feed\"}";

							MESSAGE_ERROR("", action, "can't update feed table");
						}
					}
					else
					{
						MESSAGE_DEBUG("", action, "user trying update company not belonging to him");

						ostResult << "{\"result\": \"error\", \"description\": \"you didn't works in that company\"}";
					}
				}
				

			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("user not activated");
			}

			MESSAGE_DEBUG("", action, "finish");

		}

		if(action == "AJAX_updateRecommendationTitle")
		{

			ostringstream	ostResult;

			MESSAGE_DEBUG("", action, "start");


			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				ostResult << "{\"result\": \"error\", \"description\": \"session lost. Need to relogin\"}";
			}
			else
			{
				auto			recommendationID	= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));
				auto			recommendationTitle	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("content"));
				ostringstream	ost;

				ost.str("");
				ost << "SELECT * FROM `users_recommendation` WHERE `id`=\"" << recommendationID << "\" and `recommending_userID`='" << user.GetID() << "';";
				if(db.Query(ost.str()))
				{
					string 		recommended_userID = db.Get(0, "recommended_userID");

					ost.str("");
					ost << "update `users_recommendation` set `title`=\"" << recommendationTitle << "\" WHERE `id`=\"" << recommendationID << "\" and `recommending_userID`='" << user.GetID() << "';";
					db.Query(ost.str());
					if(!db.isError())
					{

						ostringstream	dictionaryStatement;
						int				affected;

						// --- check on adverse dictionary words
						dictionaryStatement.str("");
						ost.str("");
						ost << "SELECT * FROM `dictionary_adverse`;";
						affected = db.Query(ost.str());
						if(affected)
						{
							for(auto i = 0; i < affected; i++)
							{
								if(i) dictionaryStatement << " or ";
								dictionaryStatement << "(`title` like \"%" << db.Get(i, "word") << "%\")";
							}
						}

						ost.str("");
						ost << "SELECT * FROM `users_recommendation` WHERE `id`=\"" << recommendationID << "\" and (" << dictionaryStatement.str() << ");";
						if(db.Query(ost.str()))
						{
							ost.str("");
							ost << "update `users_recommendation` set `state`='potentially adverse' WHERE `id`=\"" << recommendationID << "\";";
							db.Query(ost.str());
							if(db.isError())
							{
								MESSAGE_ERROR("", action, "can't update recommendation status to 'potentially adverse' (" + db.GetErrorMessage() + ")");
							}
						}
						else
						{
							MESSAGE_DEBUG("", action, "adverse words not found in new `users_recommendation` (" + recommendationID + ")");
						}


						// --- Update live feed
						ost.str("");
						ost << "INSERT INTO `users_notification` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << recommended_userID << "\", \"48\", \"" << recommendationID << "\", UNIX_TIMESTAMP())";
						if(db.InsertQuery(ost.str()))
						{

							ostResult << "{\"result\": \"success\"}";
						}
						else
						{
							ostResult << "{\"result\": \"error\", \"description\": \"error updating live feed\"}";

							MESSAGE_ERROR("", action, "can't update feed table");
						}
					}
					else
					{
						ostResult << "{\"result\": \"error\", \"description\": \"error updating table users\"}";

						MESSAGE_ERROR("", action, "can't update users table");
					}
				}
				else
				{
					ostResult << "{\"result\": \"error\", \"description\": \"recommendation not found or not belongs to you\"}";

					MESSAGE_ERROR("", action, "recommendation not found or not belongs to you");
				}
			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("user not activated");
			}

			MESSAGE_DEBUG("", action, "finish");

		}

		if(action == "AJAX_updateUserResponsibilities")
		{
			ostringstream	ostResult;

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				ostResult << "{\"result\": \"error\", \"description\": \"session lost. Need to relogin\"}";
			}
			else
			{
				auto		respID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));
				auto		respText = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("content"));

				if(db.Query("SELECT * FROM `users_company` WHERE `id`='" + respID + "' and `user_id`='" + user.GetID() + "';"))
				{
					if(db.Get(0, "responsibilities") != respText)
					{
						db.Query("update `users_company` set `responsibilities`=\"" + respText + "\" WHERE `id`='" + respID + "';");

						if(db.InsertQuery("INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" + user.GetID() + "\", \"20\", \"" + respID + "\", NOW())"))
						{

							ostResult << "{\"result\": \"success\"}";
						}
						else
						{
							ostResult << "{\"result\": \"error\", \"description\": \"error updating live feed\"}";

							MESSAGE_ERROR("", action, "can't update feed table");
						}
					}
					else
					{
						// --- no changes in responsibilities
						ostResult << "{\"result\": \"success\"}";
					}
				}
				else
				{
					MESSAGE_ERROR("", action, "changing alien profile");

					ostResult << "{\"result\": \"error\", \"description\": \"changing alien profile\"}";
				}
			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("user not activated");
			}
		}

		if(action == "AJAX_addEditProfileAddCarrierCompany")
		{
			auto			error_message = ""s;
			ostringstream	ostResult;

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				error_message = string(gettext("re-login required"));

				MESSAGE_DEBUG("", action, error_message);

				ostResult << "{\"result\": \"error\", \"description\": \"" + error_message + "\"}";
			}
			else
			{
				auto			title				= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("title"));
				auto			companyName			= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("companyName"));
				auto			occupationStart		= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("occupationStart"));
				auto			occupationFinish	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("occupationFinish"));
				auto			currentCompany		= stol(indexPage.GetVarsHandler()->Get("currentCompany"));
				auto			responsibilities	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("responsibilities"));
				ostringstream	ost;

				if(title.length() && companyName.length() && occupationStart.length() && (currentCompany || occupationFinish.length()))
				{
					auto		titleID = 0u, companyID = 0u, newCarrierID = 0u;
					auto		logo_filename = ""s, logo_folder = ""s;
					auto		affected = db.Query("SELECT * FROM `company_position` WHERE `title`=\"" + title + "\";");

					if(affected)
					{
						titleID = stol(db.Get(0, "id"));

						MESSAGE_DEBUG("", action, "job title [" + title + "] already exists, no need to update DB.")
					}
					else
					{
						MESSAGE_DEBUG("", action, "new job title [" + title + "] needed to be added to DB")

						titleID = db.InsertQuery("INSERT INTO `company_position` (`title`) VALUES (\"" + title + "\");");
					}

					if((affected = db.Query("SELECT * FROM `company` WHERE `name`=\"" + companyName + "\";")) > 0)
					{
						companyID = stol(db.Get(0, "id"));
						logo_folder = db.Get(0, "logo_folder");
						logo_filename = db.Get(0, "logo_filename");

						MESSAGE_DEBUG("", action, "company [" + companyName + "] already exists, no need to update DB.")
					}
					else
					{
						MESSAGE_DEBUG("", action, "new company [" + companyName + "] needed to be added to DB");

						companyID = db.InsertQuery("INSERT INTO `company` (`name`) VALUES (\"" + companyName + "\");");
						if(companyID)
						{
							db.Query("UPDATE `company` SET `link`=\"" + to_string(companyID) + "\" WHERE `id`=\"" + to_string(companyID) + "\";");

							if(db.isError())
							{
								MESSAGE_ERROR("", action, "can't update `company`.`link`=" + to_string(companyID) + " WHERE `company`.`id`=" + to_string(companyID) + ", error message (" + db.GetErrorMessage() + ")");
							}
						}
						else
						{
							MESSAGE_ERROR("", action, "insertion company (" + companyName + ") to table `company`");
						}
					}

					if(titleID && companyID)
					{

						ost.str("");
						ost << "INSERT INTO `users_company` (`user_id`, `company_id`, `position_title_id`, `occupation_start`, `occupation_finish`, `current_company`, `responsibilities`) "
							<< "VALUES(\"" << user.GetID() << "\",\"" << companyID << "\", \"" << titleID << "\", \"" << occupationStart << "\", " << (currentCompany ? "NOW()" : "\"" + occupationFinish + "\"") << ", \"" << currentCompany << "\",  \"" << responsibilities << "\")";
						
						newCarrierID = db.InsertQuery(ost.str());					
						if(newCarrierID)
						{
							SubscribeToCompany(to_string(companyID), &user, &db);

							// --- Updating the live feed
							ost.str("");
							ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) VALUES(\"\",\"" << user.GetID() << "\", " << (currentCompany ? "\"1\"" : "\"51\"") << ", \"" << newCarrierID << "\", NOW())";
							if(db.InsertQuery(ost.str()))
							{
							}
							else
							{
								error_message = gettext("SQL syntax error");
								MESSAGE_ERROR("", action, error_message);
							}
							ostResult	<< "{\"result\": \"success\","
										<< "\"logo_folder\": \"" << logo_folder << "\","
										<< "\"logo_filename\": \"" << logo_filename << "\","
										<< "\"carrierID\": \"" << newCarrierID << "\""
										<< "}";

						}
						else
						{
							error_message = gettext("fail to insert to db");

							ostResult << "{\"result\": \"error\", \"description\": \"" + error_message + "\"}";
						}
					}
					else
					{
						error_message = gettext("company or occupation not found");
						MESSAGE_ERROR("", action, error_message);

						ostResult << "{\"result\": \"error\", \"description\": \"" + error_message + "\"}";
					}
				}
				else
				{
					error_message = gettext("mandatory parameter missed");
					MESSAGE_ERROR("", action, error_message);

					ostResult << "{\"result\": \"error\", \"description\": \"" + error_message + "\"}";
				}
			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("user not activated");
			}
		}

		if(action == "AJAX_addEditProfileAddCertificate")
		{
			ostringstream	ostResult;

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				ostResult << "{\"result\": \"error\", \"description\": \"session lost. Need to relogin\"}";
			}
			else
			{
				auto		newVendorName	 = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("vendor"));
				auto		track			 = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("track"));
				auto		number			 = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("number"));

				if(newVendorName.length() && track.length() && number.length())
				{
					unsigned long		newVendorID = 0, trackID = 0;
					string			  existingVendorID = "", existingVendorName = "";
					// int					affected;
					bool				okToAdd = false;
					string			  certificationPhotoFolder = "";
					string			  certificationPhotoFilename = "";
					string			  certificationTrack = track;
					string			  certificationVendor = newVendorName;
					string			  certificationNumber = number;

					// --- certification_track.name must be unique
					// --- to avoid: CCIE(Cisco) && CCIE(Juniper)
					if(db.Query("SELECT * FROM `certification_tracks` WHERE `title`=\"" + track + "\";"))
					{
						certificationPhotoFolder = db.Get(0, "logo_folder");
						certificationPhotoFilename = db.Get(0, "logo_filename");

						try {
							trackID = stol(db.Get(0, "id"));
						} catch (...) {
							MESSAGE_ERROR("", action, "can't convert trackID[" + to_string(trackID) + "] to number");

							trackID = 0;
						}
						existingVendorID = db.Get(0, "vendor_id");
						if(existingVendorID.length() && db.Query("SELECT * FROM `company` WHERE `id`=\"" + existingVendorID + "\";"))
						{
							existingVendorName = db.Get(0, "name");
							if(existingVendorName == newVendorName)
								okToAdd = true;
							else
							{
								MESSAGE_ERROR("", action, "incorrect certification[" + track + "] vendor[" + newVendorName + "] trying to add");

								ostResult << "{\"result\": \"error\", \"description\": \"ОШИБКА: некорректно указан вендор\"}";
							}
						}
						else
						{
							// --- should not come here
							// --- if you get here then DB-structure is wrong 
							// --- particularly "certification_track" have non-existent or empty vendorID
							MESSAGE_ERROR("", action, "incorrect DB structure of certification[" + track + "]");

							newVendorID = db.InsertQuery("INSERT INTO `company` (`name`) VALUES (\"" + newVendorName + "\");");
							db.Query("UPDATE `certification_tracks` SET `vendor_id`=\"" + to_string(newVendorID) + "\" WHERE `id`=\"" + to_string(trackID) + "\";");
							okToAdd = true;
						}
					}
					else
					{
						MESSAGE_DEBUG("", "", "track [" + track + "] needed to be added to DB");

						if(db.Query("SELECT * FROM `company` WHERE `name`=\"" + newVendorName + "\";"))
							newVendorID = stol(db.Get(0, "id"));
						else
							newVendorID = db.InsertQuery("INSERT INTO `company` (`name`) VALUES (\"" + newVendorName + "\");");

						if(newVendorID)
						{
							trackID = db.InsertQuery("INSERT INTO `certification_tracks` (`title`, `vendor_id`) VALUES (\"" + track + "\", \"" + to_string(newVendorID) + "\");");
							if(trackID)
								okToAdd = true;
							else
							{
								MESSAGE_ERROR("", action, "inserting into `certification_tracks`");
							}
						}
						else
						{
							MESSAGE_ERROR("", "", "newVendorID is empty");
						}
					}

					if(okToAdd && trackID)
					{
						unsigned long	newCertificationID = 0;
						ostringstream   ost;

						ost.str("");
						ost << "INSERT INTO `users_certifications` (`user_id`, `track_id`, `certification_number`) "
							<< "VALUES (\"" << user.GetID() << "\", \"" << trackID << "\", \"" << number << "\");";
						
						newCertificationID = db.InsertQuery(ost.str());				 
						if(newCertificationID)
						{
							// --- Update live feed
							if(db.InsertQuery("INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) VALUES (\"\",\"" + user.GetID() + "\", \"22\", \"" + to_string(newCertificationID) + "\", NOW());"))
							{
							}
							else
							{
								MESSAGE_ERROR("", action, "insertion into table news_feed");
							}

							ostResult << "{\"result\": \"success\", "
									  << "\"certificationID\": \"" << newCertificationID << "\","
									  << "\"certificationInternalID\":\"" << trackID << "\","
									  << "\"certificationPhotoFolder\":\"" << certificationPhotoFolder << "\","
									  << "\"certificationPhotoFilename\":\"" << certificationPhotoFilename << "\","
									  << "\"certificationVendor\":\"" << certificationVendor << "\","
									  << "\"certificationTrack\":\"" << certificationTrack << "\","
									  << "\"certificationNumber\":\"" << certificationNumber << "\""
									  << "}";
						}
						else
						{
							MESSAGE_ERROR("", "", "issue with inserting into DB table [users_certification]");

							ostResult << "{\"result\": \"error\", \"description\": \"issue with DB operations on adding certification\"}";
						}
					}
					else
					{
						MESSAGE_DEBUG("", "", "issue with DB operations on vendor or track");
					}
				}
				else
				{
					MESSAGE_ERROR("", "", "mandatory parameter missed or empty in HTML request [vendor, track, number]");

					ostResult << "{\"result\": \"error\", \"description\": \"any mandatory parameter missed\"}";
				}
			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("user not activated");
			}
		}


		if(action == "AJAX_addEditProfileAddCourse")
		{
			ostringstream	ostResult;

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				ostResult << "{\"result\": \"error\", \"description\": \"session lost. Need to relogin\"}";
			}
			else
			{
				auto			newVendorName = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("vendor"));
				auto			track = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("track"));
				ostringstream	ost;

				if(newVendorName.length() && track.length())
				{
					auto			newVendorID = 0, trackID = 0;
					auto			existingVendorID = ""s, existingVendorName = ""s;
					auto			coverPhotoFolder = ""s, coverPhotoFilename = ""s;
					auto			okToAdd = false;

					// --- certification_track.name must be unique
					// --- to avoid: CCIE(Cisco) && CCIE(Juniper)
					if(db.Query("SELECT * FROM `certification_tracks` WHERE `title`=\"" + track + "\";"))
					{
						try {
							trackID = stol(db.Get(0, "id"));
						} catch (...) {
							MESSAGE_ERROR("", action, "can't convert trackID[" + to_string(trackID) + "] to number");

							trackID = 0;
						}
						coverPhotoFolder = db.Get(0, "logo_folder");
						coverPhotoFilename = db.Get(0, "logo_filename");
						existingVendorID = db.Get(0, "vendor_id");
						if(existingVendorID.length() && db.Query("SELECT * FROM `company` WHERE `id`=\"" + existingVendorID + "\";"))
						{
							existingVendorName = db.Get(0, "name");
							if(existingVendorName == newVendorName)
								okToAdd = true;
							else
							{
								MESSAGE_ERROR("", action, "incorrect certification[" + track + "] vendor[" + newVendorName + "] trying to add");

								ostResult << "{\"result\": \"error\", \"description\": \"ОШИБКА: некорректно указан вендор\"}";
							}
						}
						else
						{
							// --- should not come here
							// --- if you get here then DB-structure is wrong 
							// --- particularly "certification_track" have non-existent or empty vendorID
							MESSAGE_ERROR("", action, "incorrect DB structure of certification[" + track + "]");

							newVendorID = db.InsertQuery("INSERT INTO `company` (`name`) VALUES (\"" + newVendorName + "\");");
							db.Query("UPDATE `certification_tracks` SET `vendor_id`=\"" + to_string(newVendorID) + "\" WHERE `id`=\"" + to_string(trackID) + "\";");
							okToAdd = true;
						}
					}
					else
					{
						MESSAGE_DEBUG("", "", "track [" + track + "] needed to be added to DB");

						if(db.Query("SELECT * FROM `company` WHERE `name`=\"" + newVendorName + "\";"))
							newVendorID = stol(db.Get(0, "id"));
						else
							newVendorID = db.InsertQuery("INSERT INTO `company` (`name`) VALUES (\"" + newVendorName + "\");");

						if(newVendorID)
						{
							trackID = db.InsertQuery("INSERT INTO `certification_tracks` (`title`, `vendor_id`) VALUES (\"" + track + "\", \"" + to_string(newVendorID) + "\");");
							if(trackID)
								okToAdd = true;
							else
							{
								MESSAGE_ERROR("", action, "inserting into `certification_tracks`");
							}
						}
						else
						{
							MESSAGE_ERROR("", "", "newVendorID is empty");
						}
					}

					if(okToAdd && trackID)
					{
						int		newCertificationID = 0;

						ost.str("");
						ost << "INSERT INTO `users_courses` (`user_id`, `track_id`, `eventTimestamp`) "
							<< "VALUES(\"" << user.GetID() << "\", \"" << trackID << "\", UNIX_TIMESTAMP());";
						
						newCertificationID = db.InsertQuery(ost.str());					
						if(newCertificationID)
						{
							// --- Update live feed
							ost.str("");
							ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"23\", \"" << newCertificationID << "\", NOW())";
							if(db.InsertQuery(ost.str()))
							{

							}
							else
							{
								MESSAGE_ERROR("", action, "inserting into `feed` (" + ost.str() + ")");
							}

							ostResult << "{\"result\": \"success\", "
									  << "\"courseID\": \"" << newCertificationID << "\","
									  << "\"courseInternalID\": \"" << trackID << "\","
									  << "\"courseTrack\": \"" << track << "\","
									  << "\"courseVendor\": \"" << newVendorName << "\","
									  << "\"coursePhotoFolder\": \"" << coverPhotoFolder << "\","
									  << "\"coursePhotoFilename\": \"" << coverPhotoFilename << "\""
									  << "}";
						}
						else
						{
							MESSAGE_ERROR("", "", "issue with inserting into DB table [users_courses]");

							ostResult << "{\"result\": \"error\", \"description\": \"issue with DB operations on adding course\"}";
						}
					}
					else
					{
						MESSAGE_ERROR("", "", "issue with DB operations on vendor or track");
					}
				}
				else
				{
					MESSAGE_ERROR("", "", "mandatory parameter missed or empty in HTML request [vendor, track]");

					ostResult << "{\"result\": \"error\", \"description\": \"any mandatory parameter missed\"}";
				}
			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("user not activated");
			}
		}
		if(action == "AJAX_setCourseRating")
		{
			ostringstream   ostResult;
			string		  id, courseID, rating;

			MESSAGE_DEBUG("", action, "start");

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

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

					MESSAGE_DEBUG("", action, "can't convert id to number");
				}

				try
				{
					courseID = to_string(stol(indexPage.GetVarsHandler()->Get("courseID")));
				}
				catch(...)
				{
					courseID = "";

					MESSAGE_DEBUG("", action, "can't convert courseID to number");
				}

				try
				{
					int	 temp = stoi(indexPage.GetVarsHandler()->Get("rating"));
					if(temp <= 5) rating = to_string(temp); else rating = "0";
				}
				catch(...)
				{
					rating = "";

					MESSAGE_ERROR("", action, "fail to convert rating to number");
				}

				if(id.length() && db.Query("select `id`, `track_id`, `rating` from `users_courses` WHERE `user_id`=\"" + user.GetID() + "\" and `id`=\"" + id + "\";"))
				{
					courseID = db.Get(0, "track_id");
					db.Query("UPDATE `users_courses` SET `rating`=\"" + rating + "\" WHERE `id`=\"" + id + "\";");
					isSuccess = true;
				}
				else if(courseID.length() && db.Query("select `id`, `rating` from `users_courses` WHERE `user_id`=\"" + user.GetID() + "\" and `track_id`=\"" + courseID + "\";"))
				{
					id = db.Get(0, "id");
					db.Query("UPDATE `users_courses` SET `rating`=\"" + rating + "\" WHERE `id`=\"" + id + "\";");
					isSuccess = true;
				}
				else
				{
					if(courseID.length())
					{
						id = to_string(db.InsertQuery("INSERT INTO `users_courses` SET `user_id`=\"" + user.GetID() + "\", `track_id`=\"" + courseID + "\", `rating`=\"" + rating + "\", `eventTimestamp`=UNIX_TIMESTAMP();"));
						if(id.length() && (id != "0"))
							isSuccess = true;
						else
						{
							MESSAGE_ERROR("", action, "inserting  into `users_courses` fail");

							ostResult << "{\"result\":\"error\",\"description\":\"ошибка: не указан courseID\"}";
						}
					}
					else
					{
						MESSAGE_ERROR("", action, "courseID is not defined");

						ostResult << "{\"result\":\"error\",\"description\":\"ошибка: не указан courseID\"}";
					} // --- if(courseID.length())
				} // --- insert or update liked course

				if(isSuccess)
				{
					if(!db.Query("SELECT * FROM `feed` WHERE `userId`=\"" + user.GetID() + "\" and (`actionTypeId`=\"23\" or `actionTypeId`=\"25\" or `actionTypeId`=\"57\") and `actionId`=\"" + id + "\""))
					{
						if(db.InsertQuery("INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" + user.GetID() + "\", \"57\", \"" + id + "\", NOW());"))
						{

						}
						else
						{
							MESSAGE_ERROR("", action, "inserting into `feed`");
						}
					}

					ostResult << "{\"result\":\"success\",\"id\":\"" << id << "\",\"courseReadersRatingList\":["<< GetCourseRatingList(courseID, &db) << "]}";
				}
			} // --- if(user != Guest)

			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file json_response.htmlt was missing");
			}  // if(!indexPage.SetTemplate("AJAX_setCourseRating.htmlt"))

			MESSAGE_DEBUG("", action, "finish");
		}   // if(action == "AJAX_setCourseRating")

		if(action == "AJAX_addEditProfileAddLanguage")
		{
			ostringstream	ostResult;

			MESSAGE_DEBUG("", action, "start");

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				ostResult << "{\"result\": \"error\", \"description\": \"session lost. Need to relogin\"}";
			}
			else
			{
				auto			title = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("title"));
				auto			level = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("level"));
				ostringstream	ost;

				if(title.length() && level.length())
				{
					unsigned long		languageID = 0;
					string			  language_logo_folder = "";
					string			  language_logo_filename = "";
					int					affected;

					ost.str("");
					ost << "SELECT * FROM `language` WHERE `title`=\"" << title << "\";";
					if((affected = db.Query(ost.str())) > 0)
					{
						languageID = stol(db.Get(0, "id"));
						language_logo_folder = db.Get(0, "logo_folder");
						language_logo_filename = db.Get(0, "logo_filename");

						MESSAGE_DEBUG("", "", "title [" + title + "] already exists, no need to update DB.");
					}
					else
					{
						MESSAGE_DEBUG("", "", "title [" + title + "] needed to be added to DB");

						ost.str("");
						ost << "INSERT INTO `language` (`title`) VALUES (\"" << title << "\");";
						languageID = db.InsertQuery(ost.str());
					}

					if(languageID)
					{
						int		newLanguageID = 0;

						ost.str("");
						ost << "INSERT INTO `users_language` (`user_id`, `language_id`, `level`) "
							<< "values(\"" << user.GetID() << "\",\"" << languageID << "\", \"" << level << "\");";
						
						newLanguageID = db.InsertQuery(ost.str());					
						if(newLanguageID)
						{
							// --- Update live feed
							ost.str("");
							ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"29\", \"" << newLanguageID << "\", NOW())";
							if(db.InsertQuery(ost.str()))
							{

							}
							else
							{
								MESSAGE_ERROR("", action, "inserting into `feed` (" + ost.str() + ")");
							}

							ostResult << "{\"result\": \"success\","
									  << "\"languageID\": \"" << newLanguageID << "\","
									  << "\"languageInternalID\":\"" << languageID << "\","
									  << "\"languagePhotoFolder\":\"" << language_logo_folder << "\","
									  << "\"languagePhotoFilename\":\"" << language_logo_filename << "\""
									  << "}";
						}
						else
						{
							MESSAGE_ERROR("", "", "issue with inserting into DB table [users_Languages]");

							ostResult << "{\"result\": \"error\", \"description\": \"issue with DB operations on adding Language\"}";
						}
					}
					else
					{
						MESSAGE_ERROR("", "", "issue with DB operations on title or level");

						ostResult << "{\"result\": \"error\", \"description\": \"issue with DB operations on title or level\"}";
					}
				}
				else
				{
					MESSAGE_ERROR("", "", "mandatory parameter missed or empty in HTML request [title, level]");

					ostResult << "{\"result\": \"error\", \"description\": \"any mandatory parameter missed\"}";
				}
			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("user not activated");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_addEditProfileAddSkill")
		{
			ostringstream	ostResult;

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				ostResult << "{\"result\": \"error\", \"description\": \"session lost. Need to relogin\"}";
			}
			else
			{
				string			title = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("title"));
				ostringstream	ost;

				if(title.length())
				{
					unsigned long		skillID = 0;
					int					affected;

					ost.str("");
					ost << "SELECT * FROM `skill` WHERE `title`=\"" << title << "\";";
					if((affected = db.Query(ost.str())) > 0)
					{
						skillID = stol(db.Get(0, "id"));

						MESSAGE_DEBUG("", "", "title [" + title + "] already exists, no need to update DB.");
					}
					else
					{
						MESSAGE_DEBUG("", "", "title [" + title + "] needed to be added to DB");

						ost.str("");
						ost << "INSERT INTO `skill` (`title`) VALUES (\"" << title << "\");";
						skillID = db.InsertQuery(ost.str());
					}

					if(skillID)
					{
						int		newSkillID = 0;

						ost.str("");
						ost << "INSERT INTO `users_skill` (`user_id`, `skill_id`) "
							<< "values(\"" << user.GetID() << "\",\"" << skillID << "\");";
						
						newSkillID = db.InsertQuery(ost.str());					
						if(newSkillID)
						{
							// --- Update live feed
							ost.str("");
							ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"41\", \"" << newSkillID << "\", NOW())";
							if(db.InsertQuery(ost.str()))
							{

							}
							else
							{
								MESSAGE_ERROR("", action, "inserting into `feed` (" + ost.str() + ")");
							}

							ostResult << "{\"result\": \"success\", \"skillID\": \"" << newSkillID << "\"}";
						}
						else
						{
							MESSAGE_ERROR("", "", "issue with inserting into DB table [users_skill]");

							ostResult << "{\"result\": \"error\", \"description\": \"issue with DB operations on adding skill\"}";
						}
					}
					else
					{
						MESSAGE_ERROR("", "", "issue with DB operations on title or level");

						ostResult << "{\"result\": \"error\", \"description\": \"issue with DB operations on title or level\"}";
					}
				}
				else
				{
					MESSAGE_ERROR("", "", "mandatory parameter missed or empty in HTML request [title]");

					ostResult << "{\"result\": \"error\", \"description\": \"any mandatory parameter missed\"}";
				}
			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("user not activated");
			}
		}


		if(action == "AJAX_addEditProfileAddSchool")
		{
			ostringstream	ostResult;

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				ostResult << "{\"result\": \"error\", \"description\": \"session lost. Need to relogin\"}";
			}
			else
			{
				auto			locality		= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("locality"));
				auto			title			= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("title"));
				auto			periodStart		= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("periodStart"));
				auto			periodFinish	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("periodFinish"));
				ostringstream	ost;

				if(locality.length() && title.length() && periodStart.length() && periodFinish.length() && (stoi(periodStart) <= stoi(periodFinish)))
				{
					unsigned long		localityID = 0, schoolInternalID = 0;
					string			  school_logo_folder = "";
					string			  school_logo_filename = "";
					int					affected;

					ost.str("");
					ost << "SELECT * FROM `geo_locality` WHERE `title`=\"" << locality << "\";";
					if((affected = db.Query(ost.str())) > 0)
					{
						localityID = stol(db.Get(0, "id"));

						MESSAGE_DEBUG("", "", "locality [" + locality + "] already exists, no need to update DB.");
					}
					else
					{
						MESSAGE_DEBUG("", "", "locality [" + locality + "] needed to be added to DB");

						ost.str("");
						ost << "INSERT INTO `geo_locality` (`title`) VALUES (\"" << locality << "\");";
						localityID = db.InsertQuery(ost.str());
					}

					ost.str("");
					ost << "SELECT * FROM `school` WHERE `title`=\"" << title << "\" and `geo_locality_id`=\"" << localityID << "\";";
					if((affected = db.Query(ost.str())) > 0)
					{
						schoolInternalID = stol(db.Get(0, "id"));
						school_logo_folder = db.Get(0, "logo_folder");
						school_logo_filename = db.Get(0, "logo_filename");

						MESSAGE_DEBUG("", "", "school [" + title + "] already exists, no need to update DB.");
					}
					else
					{
						MESSAGE_DEBUG("", "", "school [" + title + "] needed to be added to DB");

						ost.str("");
						ost << "INSERT INTO `school` (`geo_locality_id`, `title`) VALUES (\"" << localityID << "\",\"" << title << "\");";
						schoolInternalID = db.InsertQuery(ost.str());
					}

					if(localityID && schoolInternalID)
					{
						int		newSchoolID = 0;

						ost.str("");
						ost << "INSERT INTO `users_school` (`user_id`, `school_id`, `occupation_start`, `occupation_finish`) "
							<< "values(\"" << user.GetID() << "\", \"" << schoolInternalID << "\", \"" << periodStart << "\", \"" << periodFinish << "\");";
						
						newSchoolID = db.InsertQuery(ost.str());					
						if(newSchoolID)
						{
							// --- Update live feed
							ost.str("");
							ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"27\", \"" << newSchoolID << "\", NOW())";
							if(db.InsertQuery(ost.str()))
							{

							}
							else
							{
								MESSAGE_ERROR("", action, "inserting into `feed` (" + ost.str() + ")");
							}

							ostResult << "{\"result\": \"success\", "
									  << "\"schoolID\": \"" << newSchoolID << "\","
									  << "\"schoolInternalID\":\"" << schoolInternalID << "\","
									  << "\"schoolPhotoFolder\":\"" << school_logo_folder << "\","
									  << "\"schoolPhotoFilename\":\"" << school_logo_filename << "\""
									  << "}";
						}
						else
						{
							MESSAGE_ERROR("", "", "issue with inserting into DB table [users_school]");

							ostResult << "{\"result\": \"error\", \"description\": \"issue with DB operations on adding school\"}";
						}
					}
					else
					{
						MESSAGE_ERROR("", "", "issue with DB operations on locality or school");

						ostResult << "{\"result\": \"error\", \"description\": \"issue with DB operations on locality or school\"}";
					}
				}
				else
				{
					MESSAGE_ERROR("", "", "mandatory parameter missed or empty in HTML request [locality, school, periodStart, periodFinish] or (periodStart > periodFinish)");

					ostResult << "{\"result\": \"error\", \"description\": \"any mandatory parameter missed\"}";
				}
			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("user not activated");
			}
		}

		if(action == "AJAX_addEditProfileAddUniversity")
		{
			ostringstream	ostResult;

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				ostResult << "{\"result\": \"error\", \"description\": \"session lost. Need to relogin\"}";
			}
			else
			{
				auto			region			= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("region"));
				auto			title			= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("title"));
				auto			degree			= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("degree"));
				auto			periodStart		= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("periodStart"));
				auto			periodFinish	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("periodFinish"));
				ostringstream	ost;

				if(region.length() && title.length() && degree.length() && periodStart.length() && periodFinish.length() && (stoi(periodStart) <= stoi(periodFinish)))
				{
					unsigned long		regionID = 0, universityInternalID = 0;
					auto				university_logo_folder = ""s;
					auto				university_logo_filename = ""s;
					auto				affected = db.Query("SELECT * FROM `geo_region` WHERE `title`=\"" + region + "\";");

					if(affected > 0)
					{
						regionID = stol(db.Get(0, "id"));

						MESSAGE_DEBUG("", "", "region [" + region + "] already exists, no need to update DB.");
					}
					else
					{
						MESSAGE_DEBUG("", "", "region [" + region + "] needed to be added to DB");

						regionID = db.InsertQuery("INSERT INTO `geo_region` (`title`) VALUES (\"" + region + "\");");
					}

					ost.str("");
					ost << "SELECT * FROM `university` WHERE `title`=\"" << title << "\" and  `geo_region_id`=\"" << regionID << "\";";
					if((affected = db.Query(ost.str())) > 0)
					{
						universityInternalID = stol(db.Get(0, "id"));
						university_logo_folder = db.Get(0, "logo_folder");
						university_logo_filename = db.Get(0, "logo_filename");

						MESSAGE_DEBUG("", "", "university [" + title + "] already exists, no need to update DB.");
					}
					else
					{
						MESSAGE_DEBUG("", "", "university [" + title + "] needed to be added to DB");

						ost.str("");
						ost << "INSERT INTO `university` (`title`, `geo_region_id`) VALUES (\"" << title << "\", \"" << regionID << "\");";
						universityInternalID = db.InsertQuery(ost.str());
					}

					if(regionID && universityInternalID)
					{
						int		newUniversityID = 0;

						ost.str("");
						ost << "INSERT INTO `users_university` (`user_id`, `university_id`, `degree`, `occupation_start`, `occupation_finish`) "
							<< "values(\"" << user.GetID() << "\", \"" << universityInternalID << "\", \"" << degree << "\", \"" << periodStart << "\", \"" << periodFinish << "\");";
						
						newUniversityID = db.InsertQuery(ost.str());					
						if(newUniversityID)
						{
							// --- Update live feed
							ost.str("");
							ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"28\", \"" << newUniversityID << "\", NOW())";
							if(db.InsertQuery(ost.str()))
							{

							}
							else
							{
								MESSAGE_ERROR("", action, "inserting into `feed` (" + ost.str() + ")");
							}

							ostResult << "{\"result\": \"success\", "
									  << "\"universityID\": \"" << newUniversityID << "\","
									  << "\"universityInternalID\":\"" << universityInternalID << "\","
									  << "\"universityPhotoFolder\":\"" << university_logo_folder << "\","
									  << "\"universityPhotoFilename\":\"" << university_logo_filename << "\""
									  << "}";
						}
						else
						{
							MESSAGE_ERROR("", "", "issue with inserting into DB table [users_university]");

							ostResult << "{\"result\": \"error\", \"description\": \"issue with DB operations on adding university\"}";
						}
					}
					else
					{
						MESSAGE_ERROR("", "", "issue with DB operations on region or university");

						ostResult << "{\"result\": \"error\", \"description\": \"issue with DB operations on region or university\"}";
					}
				}
				else
				{
					MESSAGE_ERROR("", "", "mandatory parameter missed or empty in HTML request [region, university, degree, period] or (periodStart > periodFinish)");

					ostResult << "{\"result\": \"error\", \"description\": \"any mandatory parameter missed\"}";
				}
			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("user not activated");
			}
		}

		if(action == "AJAX_addViewProfileAddRecommendation")
		{
			ostringstream	ostResult;

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				ostResult << "{\"result\": \"error\", \"description\": \"session lost. Need to relogin\"}";
			}
			else
			{
				auto			title = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("title"));
				auto			recommendedUserID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("recommendedUserID"));
				auto			eventTimestamp = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("eventTimestamp"));
				ostringstream	ost;

				if(title.length() && eventTimestamp.length() && recommendedUserID.length())
				{
					unsigned long		newRecommendationID = 0;

					ost.str("");
					ost << "INSERT INTO `users_recommendation` (`recommended_userID`, `recommending_userID`, `title`, `eventTimestamp`) VALUES \
					(\"" << recommendedUserID << "\",\"" << user.GetID() << "\",\"" << title << "\",\"" << eventTimestamp << "\");";
					newRecommendationID = db.InsertQuery(ost.str());

					if(newRecommendationID)
					{
						ostringstream	dictionaryStatement;
						int				affected;

						// --- check on adverse dictionary words
						dictionaryStatement.str("");
						ost.str("");
						ost << "SELECT * FROM `dictionary_adverse`;";
						affected = db.Query(ost.str());
						if(affected)
						{
							for(auto i = 0; i < affected; i++)
							{
								if(i) dictionaryStatement << " or ";
								dictionaryStatement << "(`title` like \"%" << db.Get(i, "word") << "%\")";
							}
						}

						if(db.Query("SELECT * FROM `users_recommendation` WHERE `id`=\"" + to_string(newRecommendationID) + "\" and (" + dictionaryStatement.str() + ");"))
						{
							db.Query("UPDATE `users_recommendation` SET `state`='potentially adverse' WHERE `id`=\"" + to_string(newRecommendationID) + "\";");
							if(db.isError())
							{
								MESSAGE_ERROR("", action, "can't update recommendation status to 'potentially adverse' (" + db.GetErrorMessage() + ")");
							}
						}
						else
						{
							MESSAGE_DEBUG("", action, "adverse words not found in new `users_recommendation` (" + to_string(newRecommendationID) + ")");
						}

						// --- Update live feed
						ost.str("");
						ost << "INSERT INTO `users_notification` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << recommendedUserID << "\", \"45\", \"" << newRecommendationID << "\", UNIX_TIMESTAMP() );";
						if(db.InsertQuery(ost.str()))
						{
							ostResult << "{\"result\": \"success\", \"recommendationID\": \"" << newRecommendationID << "\"}";
						}
						else
						{
							ostResult << "{\"result\": \"error\", \"description\": \"error updating live feed\"}";

							MESSAGE_ERROR("", action, "can't update feed table");
						}

					}
					else
					{
						MESSAGE_ERROR("", action, "inserting into users_recommendation table");

						ostResult << "{\"result\": \"error\", \"description\": \"inserting into users_recommendation table\"}";
					}
				}
				else
				{
					MESSAGE_ERROR("", action, "mandatory parameter missed or empty in HTML request [title or eventTimestamp or recommendedUserID]");

					ostResult << "{\"result\": \"error\", \"description\": \"any mandatory parameter missed\"}";
				}
			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("user not activated");
			}
		}

		// --- AJAX update message to news feed
		if(action == "AJAX_updateNewsFeedMessage")
		{
			MESSAGE_DEBUG("", action, "start");

			auto			error_message = ""s;

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				LogoutIfGuest(action, &config, &indexPage, &db, &user);
			}
			else
			{
				// --- Authorized user
				auto	newsFeedMessageID				= CheckHTTPParam_Number	(indexPage.GetVarsHandler()->Get("newsFeedMessageID"));
				auto	newsFeedMessageDstType			= CheckHTTPParam_Text	(indexPage.GetVarsHandler()->Get("newsFeedMessageDstType"));
				auto	newsFeedMessageDstID			= CheckHTTPParam_Number	(indexPage.GetVarsHandler()->Get("newsFeedMessageDstID"));
				auto	newsFeedMessageTitle			= CheckHTTPParam_Text	(indexPage.GetVarsHandler()->Get("newsFeedMessageTitle"));
				auto	newsFeedMessageLink				= CheckHTTPParam_Text	(indexPage.GetVarsHandler()->Get("newsFeedMessageLink"));
				auto	newsFeedMessageText				= CheckHTTPParam_Text	(indexPage.GetVarsHandler()->Get("newsFeedMessageText"));
				auto	newsFeedMessageRights			= CheckHTTPParam_Text	(indexPage.GetVarsHandler()->Get("newsFeedMessageRights"));
				auto	newsFeedMessageImageTempSet		= CheckHTTPParam_Number	(indexPage.GetVarsHandler()->Get("newsFeedMessageImageTempSet"));
				auto	newsFeedMessageImageSet			= ""s;

				// --- messageID defined
				if(newsFeedMessageID.length() && (AmIMessageOwner(newsFeedMessageID, &user, &db)))
				{
					auto 	messageOwner = GetMessageOwner(newsFeedMessageID, &user, &db);
					auto	messageOwnerType = messageOwner.first;
					auto	messageOwnerID = messageOwner.second;


					if(messageOwnerType.length() && messageOwnerID.length())
					{
						if(!isAdverseWordsHere(newsFeedMessageText, &db) && !isAdverseWordsHere(newsFeedMessageTitle, &db))
						{
							// --- handle images assigned to message
							// --- remove all images subjected to removal
							RemoveMessageImages(" `tempSet`=\"" + newsFeedMessageImageTempSet + "\" AND `srcType`=\"" + messageOwnerType + "\" AND `userID`=\"" + messageOwnerID + "\" AND `removeFlag`=\"remove\" ", &db);
	
							// --- define new SetID FROM existing images or newly uploaded
							if(db.Query("SELECT `id`,`setID` FROM `feed_images` WHERE `tempSet`=\"" + newsFeedMessageImageTempSet + "\" AND `srcType`=\"" + messageOwnerType + "\" AND `userID`=\"" + messageOwnerID + "\" LIMIT 0,1;"))
							{
								newsFeedMessageImageSet = db.Get(0, "setID");
	
								// --- if there were no media uploaded during message crafting, but new media uploaded in message editing
								// --- creating new uniq imageSetID
								if(newsFeedMessageImageSet == "0") newsFeedMessageImageSet = db.Get(0, "id");
							}
							else
							{
								MESSAGE_DEBUG("", action, "there is no media attached to the message.id [" + newsFeedMessageID + "]");
							}
	
							if(newsFeedMessageImageSet.length() && (newsFeedMessageImageSet != "0") && newsFeedMessageImageTempSet.length() && (newsFeedMessageImageTempSet != "0"))
								db.Query("UPDATE `feed_images` SET `setID`=\"" + newsFeedMessageImageSet + "\", `tempSet`=\"0\" WHERE `tempSet`=\"" + newsFeedMessageImageTempSet + "\" AND `srcType`=\"" + messageOwnerType + "\" AND `userID`=\"" + messageOwnerID + "\";");
							else
							{
								MESSAGE_DEBUG("", action, "tempSet(" + newsFeedMessageImageTempSet + ") empty or zero, no changes in feed_images");
							}
	
							if(!((newsFeedMessageTitle == "") && (newsFeedMessageText == "") && (newsFeedMessageImageTempSet == "") && (newsFeedMessageImageSet == "")))
							{
								error_message = UpdateMessageDst(newsFeedMessageID, newsFeedMessageDstType, newsFeedMessageDstID, &db, &user);

								if(error_message.empty())
								{
									db.Query(
											"UPDATE `feed_message` SET "
											"`title`=\"" + newsFeedMessageTitle + "\", "
											"`link`=\"" + newsFeedMessageLink + "\", "
											"`message`=\"" + newsFeedMessageText + "\", "
											"`imageSetID`=\"" + (newsFeedMessageImageSet == "" ? "0" : newsFeedMessageImageSet) + "\", "
											"`access`=\"" + newsFeedMessageRights + "\" "
											"WHERE `id`='" + newsFeedMessageID + "';"
										);
									MESSAGE_DEBUG("", action, "message [id = " + newsFeedMessageID + "] FROM " + messageOwnerType + ".id[" + messageOwnerID + "] has been posted");
								}
								else
								{
									MESSAGE_ERROR("", action, error_message);
								}
							}
							else
							{
								error_message = gettext("can't post message due to title, text and image is empty");
								MESSAGE_ERROR("", action, error_message);
							}
						}
						else
						{
							error_message = gettext("adverse words present in message");
							MESSAGE_ERROR("", action, "can't post message due to adverse words");
						}
					}
					else
					{
						error_message = gettext("you are not authorized");
						MESSAGE_ERROR("", action, "message owner error (type:" + messageOwnerType + ", id:" + messageOwnerID + ")");
					}
				}
				else
				{
					error_message = gettext("message not found");
					MESSAGE_ERROR("", action, "can't update message due to messageID(" + newsFeedMessageID + ") is empty or not belongs to you");
				}
			}

			AJAX_ResponseTemplate(&indexPage, "", error_message);

			MESSAGE_DEBUG("", action, "finish");
		}

		// --- AJAX comment on message in news feed
		if((action == "AJAX_commentOnMessageInNewsFeed") ||
			(action == "AJAX_commentOnBookInNewsFeed") ||
			(action == "AJAX_commentOnCertificationInNewsFeed") || 
			(action == "AJAX_commentOnCourseInNewsFeed") || 
			(action == "AJAX_commentOnCompanyInNewsFeed") || 
			(action == "AJAX_commentOnLanguageInNewsFeed") || 
			(action == "AJAX_commentOnScienceDegreeInNewsFeed"))
		{
			ostringstream	ost;
			string			strPageToGet, strNewsOnSinglePage;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				ostringstream   ost;

				MESSAGE_DEBUG("", action, "re-login required");

				ost.str("");
				ost << "{\"result\": \"error\", \"description\": \"session lost. Need to relogin\"}";

				indexPage.RegisterVariableForce("result", ost.str());

				if(!indexPage.SetTemplate("json_response.htmlt"))
				{
					MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
					throw CExceptionHTML("user not activated");
				} // if(!indexPage.SetTemplate("json_response.htmlt"))
			}
			else
			{
				string			newsFeedMessageID;
				string			newsFeedMessageComment;
				string			newsFeedMessageCommentType;

				// --- "new" used due to possibility of drop "standard exception"
				char			*convertBuffer = new char[ 1024 * 1024];

				// --- This line will not be reached in case of error in memory allocation 
				// --- To avoid throw std exception use char *a = new(std::nothrow) char[ 0x7FFFFFFF ];
				if(!convertBuffer)
				{
					MESSAGE_ERROR("", action, "can't allocate memory");
				}

				// --- Authorized user
				newsFeedMessageID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("messageID"));
				newsFeedMessageComment = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("comment"));

				// --- messageID defined
				if(newsFeedMessageID.length())
				{
					unsigned long	feed_message_comment_id;
					string			messageOwnerID = "";
					string			messageOwnerType = "user";
					
					if(action == "AJAX_commentOnMessageInNewsFeed")
					{
						newsFeedMessageCommentType = "message";

						// --- check that message exists
						if(db.Query("SELECT `id` FROM `feed_message` WHERE `id`='" + newsFeedMessageID + "';"))
						{
							// --- looking for message owner
							if(db.Query("SELECT * FROM `feed` WHERE `actionTypeId`=\"11\" and `actionId`=\"" + newsFeedMessageID + "\";"))
							{
								messageOwnerID = db.Get(0, "userId");
								messageOwnerType = db.Get(0, "srcType");
							}
							else
							{
								MESSAGE_ERROR("", action, "finding messageID in feed");

								ost.str("");
								ost << "{";
								ost << "\"result\": \"error\",";
								ost << "\"description\": \"ERROR finding messageID in feed\"";
								ost << "}";
							}
						}
						else
						{
							// --- Empty title, message and image
							ost.str("");
							ost << "{";
							ost << "\"result\": \"error\",";
							ost << "\"description\": \"messageID[" << newsFeedMessageID << "] is not exists\"";
							ost << "}";

							MESSAGE_ERROR("", action, "can't post message due to missing messageID");
						}
					} // --- action == "AJAX_commentOnMessageInNewsFeed") 
					if(action == "AJAX_commentOnBookInNewsFeed")
					{
						newsFeedMessageCommentType = "book";

						// --- check that message exists
						if(db.Query("SELECT * FROM `users_books` WHERE `id`=\"" + newsFeedMessageID + "\";"))
						{
							messageOwnerID = db.Get(0, "userID");

							// --- comments must belongs to book rather than users/book
							newsFeedMessageID = db.Get(0, "bookID");
						}
						else
						{
							// --- Empty title, message and image
							ost.str("");
							ost << "{";
							ost << "\"result\": \"error\",";
							ost << "\"description\": \"usersBookID[" << newsFeedMessageID << "] is not exists\"";
							ost << "}";

							MESSAGE_ERROR("", action, "can't find ID in users_books table");
						}
					} // --- action == "AJAX_commentOnBookInNewsFeed"
					if(action == "AJAX_commentOnCertificationInNewsFeed")
					{
						newsFeedMessageCommentType = "certification";

						// --- check that message exists
						if(db.Query("SELECT * FROM `users_certifications` WHERE `id`='" + newsFeedMessageID + "';"))
						{
							messageOwnerID = db.Get(0, "user_id");

							// --- comments must belongs to certification rather than users/certification
							newsFeedMessageID = db.Get(0, "track_id");
						}
						else
						{
							// --- Empty title, message and image
							ost.str("");
							ost << "{";
							ost << "\"result\": \"error\",";
							ost << "\"description\": \"users_certifications.id [" << newsFeedMessageID << "] is not exists\"";
							ost << "}";

							MESSAGE_ERROR("", action, "can't find ID in users_certifications table");
						}
					} // --- action == "AJAX_commentOnCertificationInNewsFeed"
					if(action == "AJAX_commentOnCourseInNewsFeed")
					{
						newsFeedMessageCommentType = "course";

						// --- check that message exists
						if(db.Query("SELECT * FROM `users_courses` WHERE `id`='" + newsFeedMessageID + "';"))
						{
							messageOwnerID = db.Get(0, "user_id");

							// --- comments must belongs to course rather than users/course
							newsFeedMessageID = db.Get(0, "track_id");
						}
						else
						{
							// --- Empty title, message and image
							ost.str("");
							ost << "{";
							ost << "\"result\": \"error\",";
							ost << "\"description\": \"users_courses.id [" << newsFeedMessageID << "] is not exists\"";
							ost << "}";

							MESSAGE_ERROR("", action, "can't find ID in users_courses table");
						}
					} // --- action == "AJAX_commentOnCourseInNewsFeed"
					if(action == "AJAX_commentOnLanguageInNewsFeed")
					{
						newsFeedMessageCommentType = "language";

						// --- check that message exists
						if(db.Query("SELECT * FROM `users_language` WHERE `id`='" + newsFeedMessageID + "';"))
						{
							messageOwnerID = db.Get(0, "user_id");

							// --- comments must belongs to language rather than users/language
							newsFeedMessageID = db.Get(0, "language_id");
						}
						else
						{
							// --- Empty title, message and image
							ost.str("");
							ost << "{";
							ost << "\"result\": \"error\",";
							ost << "\"description\": \"users_languages.id [" << newsFeedMessageID << "] is not exists\"";
							ost << "}";

							MESSAGE_ERROR("", action, "can't find ID in users_languages table");
						}
					} // --- action == "AJAX_commentOnLanguageInNewsFeed"
					if(action == "AJAX_commentOnCompanyInNewsFeed")
					{
						newsFeedMessageCommentType = "company";

						// --- check that message exists
						if(db.Query("SELECT * FROM `users_company` WHERE `id`='" + newsFeedMessageID + "';"))
						{
							messageOwnerID = db.Get(0, "user_id");

							// --- comments must belongs to company rather than users/company
							newsFeedMessageID = db.Get(0, "company_id");
						}
						else
						{
							// --- Empty title, message and image
							ost.str("");
							ost << "{";
							ost << "\"result\": \"error\",";
							ost << "\"description\": \"users_companies.id [" << newsFeedMessageID << "] is not exists\"";
							ost << "}";

							MESSAGE_ERROR("", action, "can't find ID in users_companies table");
						}
					} // --- action == "AJAX_commentOnCompanyInNewsFeed"
					if(action == "AJAX_commentOnScienceDegreeInNewsFeed")
					{
						newsFeedMessageCommentType = "university";

						// --- check that message exists
						if(db.Query("SELECT * FROM `users_university` WHERE `id`='" + newsFeedMessageID + "';"))
						{
							messageOwnerID = db.Get(0, "user_id");

							// --- comments must belongs to university rather than users/university
							newsFeedMessageID = db.Get(0, "university_id");
						}
						else
						{
							// --- Empty title, message and image
							ost.str("");
							ost << "{";
							ost << "\"result\": \"error\",";
							ost << "\"description\": \"users_university.id [" << newsFeedMessageID << "] is not exists\"";
							ost << "}";

							MESSAGE_ERROR("", action, "can't find ID in users_university table");
						}
					} // --- action == "AJAX_commentOnScienceDegreeInNewsFeed"


					if(messageOwnerType == "company")
					{
						if(db.Query("SELECT `admin_userID` from `company` WHERE `id`=\"" + messageOwnerID + "\";"))
						{
							messageOwnerID = db.Get(0, "admin_userID");
						}
						else
						{
							MESSAGE_ERROR("", action, "defining company(id: " + messageOwnerID + ") admin userID");

							messageOwnerID = "";

							ost.str("");
							ost << "{";
							ost << "\"result\": \"error\",";
							ost << "\"description\": \"ERROR defining company owner\"";
							ost << "}";
						}
					}

					if(messageOwnerID.length())
					{
						// --- insert comment
						ost.str("");
						ost << "INSERT INTO `feed_message_comment` (`messageID`, `userID`, `comment`, `type`, `eventTimestamp`) VALUES (\"" << newsFeedMessageID << "\", \"" << user.GetID() << "\", \"" << newsFeedMessageComment << "\", \"" << newsFeedMessageCommentType << "\", NOW());";
						feed_message_comment_id = db.InsertQuery(ost.str());
						if(feed_message_comment_id)
						{
							vector<string>	  replyToUsers = GetUserTagsFromText(newsFeedMessageComment);

							// --- if comment written by other users to your message
							if(messageOwnerID != user.GetID()) 
							{
								auto	it = find(replyToUsers.begin(), replyToUsers.end(), messageOwnerID);
								if(it == replyToUsers.end())
									replyToUsers.push_back(messageOwnerID);
							}

							for(auto &replyUserID: replyToUsers)
							{
								ost.str("");
								ost << "INSERT INTO `users_notification` (`userID`, `actionTypeId`, `actionId`, `eventTimestamp`) VALUES ('" << replyUserID << "', \"19\", '" << feed_message_comment_id << "', UNIX_TIMESTAMP() );";
								if(db.InsertQuery(ost.str()))
								{
									MESSAGE_DEBUG("", action, "success comment submission to userID[" + replyUserID + "]");

								}
								else
								{
									MESSAGE_ERROR("", action, "inserting into users_notification table");
								}
							}

							ost.str("");
							ost << "{";
							ost << "\"result\": \"success\",";
							ost << "\"description\": \"\"";
							ost << "}";
						}
						else
						{
							MESSAGE_ERROR("", action, "inserting into feed_message_comment");

							ost.str("");
							ost << "{";
							ost << "\"result\": \"error\",";
							ost << "\"description\": \"ERROR inserting into feed_message_comment\"";
							ost << "}";
						}
					}
					else
					{
						MESSAGE_ERROR("", action, "finding commenting message owner");

						ost.str("");
						ost << "{";
						ost << "\"result\": \"error\",";
						ost << "\"description\": \"ERROR finding commenting message owner\"";
						ost << "}";
					}
				} // if(newsFeedMessageID.length() > 0)
				else
				{
					// --- Empty title, message and image
					ost.str("");
					ost << "{";
					ost << "\"result\": \"error\",";
					ost << "\"description\": \"can't update message due to messageID is not defined\"";
					ost << "}";

					MESSAGE_ERROR("", action, "can't update message due to messageID is not defined");
				}
			} // if(user.GetLogin() == "Guest")

			indexPage.RegisterVariableForce("result", ost.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}


		// --- AJAX change user password
		if(action == "AJAX_changeUserPassword")
		{
			string			newPassword, cleanedPassword;
			auto			error_message = ""s;

			if(user.GetLogin() == "Guest")
			{
				error_message = "re-login required";
				MESSAGE_DEBUG("", action, error_message);
			}
			else
			{
				// --- Authorized user
				newPassword = indexPage.GetVarsHandler()->Get("password");
				cleanedPassword = CheckHTTPParam_Text(newPassword);

				if(cleanedPassword != newPassword)
				{
					error_message = "Пароль не должен содержать символов [(кавычки), (перевод строки), '<>]";
					MESSAGE_DEBUG("", action, error_message);
				}
				else
				{
					error_message = user.ChangePasswordTo(newPassword);
				}
			} // if(user.GetLogin() == "Guest")

			AJAX_ResponseTemplate(&indexPage, "", error_message);
		}


		if(action == "ajax_regNewUser_checkUser") {
			ostringstream	ost;
			string		sessid;
			string		randomValue = GetRandom(4);
			string 		userToCheck;

			MESSAGE_DEBUG("", action, "start");

			userToCheck = indexPage.GetVarsHandler()->Get("regEmail"); 

			if(CheckUserEmailExisting(userToCheck, &db)) {
				MESSAGE_DEBUG("", action, "login or email already registered");

				indexPage.RegisterVariableForce("result", "already used");
			}
			else {
				MESSAGE_DEBUG("", action, "login or email not yet exists");
				indexPage.RegisterVariableForce("result", "free");
			}

			if(!indexPage.SetTemplate("ajax_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file ajax_response.htmlt was missing");
				throw CException("Template file was missing");
			}
		}

		if(action == "view_profile")
		{
			ostringstream	ost;
			string			sessid, friendID;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					MESSAGE_DEBUG("", action, " " + action + " re-login required");
				}

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			friendID = indexPage.GetVarsHandler()->Get("userid");
			if(friendID.length() && stol(friendID))
			{
				if(friendID != user.GetID())
				{
					// --- update user watched page
					ost.str("");
					ost << "SELECT `id` FROM `users_watched` WHERE `watched_userID`=\"" << friendID << "\" and `watching_userID`=\"" << user.GetID() << "\";";
					if(db.Query(ost.str()))
					{
						auto		profile_watched_id = db.Get(0, "id");

						ost.str("");
						ost << "update `users_watched` set `eventTimestamp`=UNIX_TIMESTAMP() WHERE `id`='" << profile_watched_id << "';";
						db.Query(ost.str());
						if(db.isError())
						{
							MESSAGE_ERROR("", action, "updating table users_watched");
						}
					}
					else
					{
						ost.str("");
						ost << "insert `users_watched` (`watched_userID`, `watching_userID`, `eventTimestamp`) VALUE (\"" << friendID << "\", \"" << user.GetID() << "\", UNIX_TIMESTAMP());";
						if(!db.InsertQuery(ost.str()))
						{
							MESSAGE_ERROR("", action, "inserting into table users_watched");
						}
					}
				}

				// --- get user profile details
				ost.str("");
				ost << "SELECT * FROM `users` WHERE `id`='" << friendID << "' and `isactivated`='Y' and `isblocked`='N';";
				if(db.Query(ost.str()))
				{

					string		friendID = db.Get(0, "id");
					string		friendLogin = db.Get(0, "login");
					string		friendEmail = db.Get(0, "email");
					string		friendName = db.Get(0, "name");
					string		friendNameLast = db.Get(0, "nameLast");
					string		friendCV = db.Get(0, "cv");
					string		friendLastOnline = db.Get(0, "last_online");
					string		friendIP = db.Get(0, "ip");
					string		friendActivated = db.Get(0, "activated");
					string		friendFriendshipState = "empty";
					int			affected = 0;
					string		current_company = "0";

					friendCV = ReplaceCRtoHTML(friendCV); 
					indexPage.RegisterVariableForce("friendID", friendID);
					indexPage.RegisterVariableForce("friendLogin", friendLogin);
					indexPage.RegisterVariableForce("friendEmail", friendEmail);
					indexPage.RegisterVariableForce("friendName", friendName);
					indexPage.RegisterVariableForce("friendNameLast", friendNameLast);
					indexPage.RegisterVariableForce("friendCV", friendCV);
					indexPage.RegisterVariableForce("friendLastOnline", friendLastOnline);
					indexPage.RegisterVariableForce("friendIP", friendIP);
					indexPage.RegisterVariableForce("friendActivated", friendActivated);
					indexPage.RegisterVariableForce("friendActivatedDifferenceFromNow", GetHumanReadableTimeDifferenceFromNow(friendActivated));

					if(GetTimeDifferenceFromNow(friendLastOnline) < 60)
					{
						indexPage.RegisterVariableForce("friendLastOnlineStatement", "менее минуты назад");
					}
					else
					{
						ostringstream		ost;

						ost.str("");
						ost << GetHumanReadableTimeDifferenceFromNow(friendLastOnline);
						indexPage.RegisterVariableForce("friendLastOnlineStatement", ost.str());
					}


					if(db.Query("SELECT * FROM `users_avatars` WHERE `userID`=\"" + friendID + "\" and `isActive`=\"1\";"))
						indexPage.RegisterVariableForce("friendAvatar", "/images/avatars/avatars" + string(db.Get(0, "folder")) + "/" + string(db.Get(0, "filename")));

					ost.str("");
					ost << "SELECT * FROM `users_friends` WHERE `userid`='" << user.GetID() << "' and `friendID`='" << friendID << "';";
					if(db.Query(ost.str()))
					{
						friendFriendshipState = db.Get(0, "state");
					}
					indexPage.RegisterVariableForce("friendFriendshipState", friendFriendshipState);


					ost.str("");
					ost << "SELECT `users_company`.`id` as `users_company_id`, `company`.`name` as `company_name`, `occupation_start`, `occupation_finish`, `current_company`, `responsibilities`, `company_position`.`title` as `title` \
							FROM  `company` ,  `users_company` ,  `company_position` \
							WHERE  `user_id` =  '" << friendID << "' \
							AND  `company`.`id` =  `users_company`.`company_id`  \
							AND  `company_position`.`id` =  `users_company`.`position_title_id`  \
							ORDER BY  `users_company`.`occupation_start` DESC ";
					affected = db.Query(ost.str());
					if(affected > 0) {
							ostringstream	ost1, ost2, ost3, ost4;
							string			occupationFinish;
							ost1.str("");
							ost2.str("");
							ost3.str("");
							ost4.str("");

							for(auto i = 0; i < affected; i++, current_company = "0") {
								occupationFinish = db.Get(i, "occupation_finish");
								if(occupationFinish == "0000-00-00") {
									current_company = "1";
									ost2.str("");
									ost2 << indexPage.GetVarsHandler()->Get("currentCompany");
									if(ost2.str().length() > 1) ost2 << ", ";
									ost2 << db.Get(i, "company_name");
									indexPage.RegisterVariableForce("currentCompany", ost2.str());
								}

								ost1 << "<div class='row'>\n";
								ost1 << "<div class='col-md-4'>";
								ost1 << "<p" << (current_company == "1" ? " class=\"current_company\"" : "") << ">с ";
								ost1 << "<span data-id='" << db.Get(i, "users_company_id") << "' data-action='update_occupation_start' class='occupation_start datePick'>" << db.Get(i, "occupation_start") << "</span> по ";
								ost1 << "<span data-id='" << db.Get(i, "users_company_id") << "' data-action='update_occupation_finish' class='occupation_finish editableSpan'>" << (occupationFinish == "0000-00-00" ? "настоящее время" : occupationFinish)  << "</span></p>";
								ost1 << "</div>\n";
								ost1 << "<div class='col-md-8'>";
								ost1 << "<p" << (current_company == "1" ? " class=\"current_company\" " : "") << "> \
								<span data-id='" << db.Get(i, "users_company_id") << "' data-action='updateJobTitle' class='jobTitle editableSpan'>"  << db.Get(i, "title") << "</span> в \
								<span data-id='" << db.Get(i, "users_company_id") << "' data-action='updateCompanyName' class='companyName editableSpan'>" << db.Get(i, "company_name") << "</span>";
								// ost1 << (current_company == "1" ? " (текущее место работы)" : "") << "</p>";
								ost1 << "</div>\n";
								ost1 << "</div> <!-- row -->\n\n";
								ost1 << "<div class='row'>\n";
								ost1 << "<div class='col-md-1'>";
								ost1 << "</div>\n";
								ost1 << "<div class='col-md-9'>";
								ost1 << "<p>"  << db.Get(i,"responsibilities") << "</p>";
								ost1 << "</div>\n";
								ost1 << "<div class='col-md-1'>";
								ost1 << "</div>\n";
								ost1 << "</div>\n\n";
							}
							indexPage.RegisterVariableForce("carrierPath", ost1.str());
					}
					else 
					{
						indexPage.RegisterVariableForce("carrierPath", "Нет данных");
					}
				}
				else
				{
					MESSAGE_ERROR("", action, "user not found , not activated or blocked");
				}
			}
			else
			{
				MESSAGE_ERROR("", action, "userID is missing or equal zero");
			}


			if(db.Query("SELECT * FROM `users_avatars` WHERE `userID`=\"" + user.GetID() + "\" and `isActive`=\"1\";"))
				indexPage.RegisterVariableForce("myUserAvatar", "/images/avatars/avatars" + string(db.Get(0, "folder")) + "/" + string(db.Get(0, "filename")));

			if(!indexPage.SetTemplate("view_profile.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file view_profile.htmlt was missing");
				throw CException("Template file was missing");
			}
		}

		// --- JSON get user notifications
		if(action == "AJAX_getUserNotification")
		{
			ostringstream	ost;
			string			strPageToGet, strNewsOnSinglePage, strFriendList;

			MESSAGE_DEBUG("", action, "start");

			strNewsOnSinglePage	= indexPage.GetVarsHandler()->Get("NewsOnSinglePage");
			strPageToGet 		= indexPage.GetVarsHandler()->Get("page");
			if(strPageToGet.empty()) strPageToGet = "0";
			{
				MESSAGE_DEBUG("", action, "page " + strPageToGet + " requested");
			}

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}


			ost.str("");
			ost << "SELECT `users_notification`.`eventTimestamp` as `feed_eventTimestamp`, `users_notification`.`actionId` as `feed_actionId` , `users_notification`.`actionTypeId` as `feed_actionTypeId`, "
				" `users_notification`.`id` as `users_notification_id`, `users_notification`.`notificationStatus` as `users_notification_notificationStatus`, "
				" `users`.`id` as `user_id`, `users`.`name` as `user_name`, `users`.`nameLast` as `user_nameLast`, `users`.`sex` as `user_sex`, `users`.`email` as `user_email`, "
				" `action_types`.`title` as `action_types_title`, `action_types`.`title_male` as `action_types_title_male`, `action_types`.`title_female` as `action_types_title_female`, "
				" `action_category`.`title` as `action_category_title`, `action_category`.`title_male` as `action_category_title_male`, `action_category`.`title_female` as `action_category_title_female`, `action_category`.`id` as `action_category_id` "
				" FROM `users_notification` "
				" INNER JOIN  `action_types` 		ON `users_notification`.`actionTypeId`=`action_types`.`id` "
				" INNER JOIN  `action_category` 	ON `action_types`.`categoryID`=`action_category`.`id` "
				" INNER JOIN  `users` 			ON `users_notification`.`userId`=`users`.`id` "
				" WHERE `users_notification`.`userId`=\"" << user.GetID() << "\" AND `action_types`.`isShowNotification`='1' "
				" ORDER BY  `users_notification`.`eventTimestamp` DESC LIMIT " << stoi(strPageToGet, nullptr, 10) * stoi(strNewsOnSinglePage, nullptr, 10) << " , " << stoi(strNewsOnSinglePage, nullptr, 10);

			indexPage.RegisterVariableForce("result", GetUserNotificationInJSONFormat(ost.str(), &db, &user));

			ost.str("");
			ost << "update `users_notification` set `notificationStatus`=\"read\" WHERE `userId`=\"" << user.GetID() << "\";";
			db.Query(ost.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("user not activated");
			} // if(!indexPage.SetTemplate("json_response.htmlt"))

			MESSAGE_DEBUG("", action, "finish");
		}


		if(action == "user_notifications")
		{
			ostringstream	ost;
			string			sessid, activeUserID;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					MESSAGE_DEBUG("", action, " " + action + " re-login required");
				}

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			if(!indexPage.SetTemplate("user_notifications.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file user_notifications.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "chat")
		{
			ostringstream	ost;
			string			sessid, activeUserID;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					MESSAGE_DEBUG("", action, " " + action + " re-login required");
				}

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			if(!indexPage.SetTemplate("chat.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file chat.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "login")
		{
			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;
				string		sessid;
				string		randomValue = GetRandom(4);
				string 		captchaFile = GenerateImage(randomValue);
				int 		affected;

				sessid = indexPage.GetCookie("sessid");

				if(sessid.length() < 5) {
					MESSAGE_ERROR("", action, "session.id [" + sessid + "] must be 20 symbols long");
					throw CException("Please enable cookie in browser.");
				}

				{
					MESSAGE_DEBUG("", action, "get login captcha for session " + sessid);
				}


				ost.str("");
				ost << "SELECT id FROM captcha WHERE `session`=\"" << sessid << "\" and `purpose`='regNewUser'";

				if((affected = db.Query(ost.str())) > 0) {
					// ------ Update session
					{
						MESSAGE_DEBUG("", action, "update session.id(" + sessid + ") captcha");
					}

					ost.str("");
					ost << "UPDATE `captcha` SET `code`='" << randomValue << "', `filename`='" << captchaFile << "', `timestamp`=NOW() WHERE `session`=\"" << sessid << "\" AND `purpose`='regNewUser'";
				}
				else {
					// ------ Create new session
					{
						MESSAGE_DEBUG("", action, "create new session.id(" + sessid + ") captcha");
					}


					ost.str("");
					ost << "INSERT INTO  `captcha` (`session` ,`code` ,`filename` ,`purpose`, `timestamp`) VALUES ('" << sessid << "',  '" << randomValue << "',  '" << captchaFile << "',  'regNewUser', NOW());";
				}
				db.Query(ost.str());

				{
					MESSAGE_DEBUG("", action, "register variables");
				}

				indexPage.RegisterVariableForce("noun_list", GetPasswordNounsList(&db));
				indexPage.RegisterVariableForce("adjectives_list", GetPasswordAdjectivesList(&db));

				indexPage.RegisterVariableForce("title", "Добро пожаловать");
				indexPage.RegisterVariable("regEmail_checked", "0");


				indexPage.RegisterVariableForce("securityFile", captchaFile);


				if(!indexPage.SetTemplate("login.htmlt"))
				{
					MESSAGE_ERROR("", action, "template file login.htmlt was missing");
					throw CException("Template file was missing");
				}
			}
			else
			{
				MESSAGE_ERROR("", action, "(not an error, severity should be monitor) registered user(" + user.GetLogin() + ") attempts to access login page, redirect to default page");

				indexPage.Redirect("/" + config.GetFromFile("default_action", user.GetType()) + "?rand=" + GetRandom(10));
			}

			MESSAGE_DEBUG("", action, "finish");
		}
		if(action == "logout")
		{
			ostringstream	ost;
			string		sessid;

			MESSAGE_DEBUG("", action, "start");

			sessid = indexPage.GetCookie("sessid");
			if(sessid.length() > 0)
			{
				db.Query("UPDATE `sessions` SET `user_id`=(SELECT `id` FROM `users` WHERE `login`=\"Guest\"), `expire`=\"1\" WHERE `id`=\"" + sessid + "\";");

				if(!indexPage.Cookie_Expire())
				{
					MESSAGE_ERROR("", action, "session expiration");
				} // --- if(!indexPage.Cookie_Expire())
			}

			if(!indexPage.SetTemplate("logout.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file logout.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "forget_password_page")
		{
			MESSAGE_DEBUG("", action, "start");

			if(!indexPage.SetTemplate("forget_password.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file forgot_password_page.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");

		}

		if(action == "AJAX_forgetPassword")
		{
			string		login, lng, sessid;
			CUser		user;
			ostringstream	ost1, ostResult;

			MESSAGE_DEBUG("", action, "start");

			sessid = indexPage.GetCookie("sessid");
			if(sessid.length() < 5)
			{
				MESSAGE_DEBUG("", action, "session id derived FROM cookies");

				ostResult.str("");
				ostResult << "{";
				ostResult << "\"result\": \"error\",";
				ostResult << "\"description\": \"session ID derived FROM cookie is wrong\",";
				ostResult << "\"type\": \"redirect\",";
				ostResult << "\"url\": \"/login?rand=" << GetRandom(10) << "\"";
				ostResult << "}";
			}
			else
			{
				login = indexPage.GetVarsHandler()->Get("email");
				lng = indexPage.GetLanguage();

				login = ReplaceDoubleQuoteToQuote(login);
				login = DeleteHTML(login);
				login = SymbolReplace(login, "\r\n", "<br>");
				login = SymbolReplace(login, "\r", "<br>");
				login = SymbolReplace(login, "\n", "<br>");

				user.SetDB(&db);
				if(!user.GetFromDBbyEmail(login))
				{
					MESSAGE_DEBUG("", action, "" + action + ": user [" + user.GetLogin() + "] not found");

					// --- don't alert that user is missing, it make reconnaissance attack easier
					ostResult.str("");
					ostResult << "{";
					ostResult << "\"result\": \"error\",";
					ostResult << "\"description\": \"Проверьте свой e-mail\"";
					ostResult << "}";
				}
				else
				{

					if(!user.isActive())
					{
						MESSAGE_ERROR("", action, "user [" + user.GetLogin() + "] not activated");

						ostResult.str("");
						ostResult << "{";
						ostResult << "\"result\": \"error\",";
						ostResult << "\"description\": \"пользователь неактивирован. Проверьте e-mail и активируйте аккаунт.\"";
						ostResult << "}";
					}
					else
					{
						CMailLocal	mail;
						string		activator_id = GetRandom(20);

						MESSAGE_DEBUG("", action, "" + action + ": sending mail message with password to user (" + user.GetLogin() + ")");

						db.Query("INSERT INTO `activators` SET `id`=\"" + activator_id + "\", `user`=\"" + user.GetEmail() + "\", `type`=\"password_recovery\", `date`=NOW();");
						if(db.isError())
						{
							MESSAGE_ERROR("", action, "fail to insert activator for user.login(" + user.GetLogin() + ")");

							ostResult.str("");
							ostResult << "{";
							ostResult << "\"result\": \"error\",";
							ostResult << "\"description\": \"Ошибка БД. Мы исправим ее в течение 24 часов.\"";
							ostResult << "}";
						}
						else
						{
							indexPage.RegisterVariableForce("login", user.GetLogin());
							indexPage.RegisterVariableForce("name", user.GetName());
							indexPage.RegisterVariableForce("nameLast", user.GetNameLast());
							indexPage.RegisterVariableForce("activator_id", activator_id);
							mail.Send(user.GetLogin(), "forget", indexPage.GetVarsHandler(), &db);

							ost1.str("");
							ost1 << "/login?rand=" << GetRandom(10) << "&signinInputEmail=" << user.GetEmail();

							ostResult.str("");
							ostResult << "{";
							ostResult << "\"result\": \"success\",";
							ostResult << "\"description\": \"\",";
							ostResult << "\"url\": \"" << ost1.str() << "\",";
							ostResult << "\"email\": \"" << user.GetEmail() << "\"";
							ostResult << "}";
						}

					}  // if(!user.isActive())
				}  // if(!user.isFound())
			} // if(sessid.length() < 5)

			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("Template file was missing");
			}

			MESSAGE_DEBUG("", "", "finish");
		}

		if(action == "AJAX_recoverPassword")
		{
			string			login, lng, sessid;
			CUser			user;
			ostringstream	ost1, ostResult;
			string			error_message;
			string			redirect_url;

			MESSAGE_DEBUG("", action, "start");

			sessid = indexPage.GetCookie("sessid");
			if(sessid.length() < 5)
			{
				error_message = "Устаревшая сессия";
				redirect_url = "/login?rand=" + GetRandom(10) + "\"";
				MESSAGE_ERROR("", action, "with session id derived FROM cookies");
			}
			else
			{
				auto		activator_id = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("activator_id"));
				auto		password_hash = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("password_hash"));

				if(activator_id.length())
				{
					if(db.Query("SELECT `user` FROM `activators` WHERE `id`=\"" + activator_id + "\" and `type`=\"password_recovery\";"))
					{
						auto		user_email = db.Get(0, "user");
						CUser		user(&db);

						if(user.GetFromDBbyEmail(user_email))
						{
							if((error_message = user.ChangePasswordTo(password_hash)).empty())
							{
								db.Query("DELETE FROM `activators` WHERE `user`=\"" + user_email + "\" AND `type`=\"password_recovery\";");
								if(db.isError())
								{
									MESSAGE_ERROR("", "", "fail to clean-up table activators with user.email(" + user_email + ") and type(password_recovery)");
								}

								redirect_url = "/login?rand=" + GetRandom(10) + "&signinInputEmail=" + user_email;
							}
							else
							{
								MESSAGE_ERROR("", action, error_message);
							}
						}
						else
						{
							error_message = gettext("user not found");
							MESSAGE_ERROR("", action, error_message);
						}
					}
					else
					{
						error_message = "Неизвестный токен активации";
						MESSAGE_ERROR("", action, "activator_id(" + activator_id + ") with type(password_recovery) not found in the activator table");
					}
				}
				else
				{
					error_message = "Токен активации пустой";
					MESSAGE_ERROR("", action, "activator_id is empty");
				}
			} // if(sessid.length() < 5)

			if(error_message.empty())
			{
				ostResult.str("");
				ostResult << "{";
				ostResult << "\"result\": \"success\",";
				ostResult << "\"redirect_url\": \"" + redirect_url + "\"";
				ostResult << "}";
			}
			else
			{
				ostResult.str("");
				ostResult << "{";
				ostResult << "\"result\": \"error\",";
				ostResult << "\"description\": \"" + error_message + "\",";
				ostResult << "\"type\": \"redirect\",";
				ostResult << "\"redirect_url\": \"" + redirect_url + "\"";
				ostResult << "}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_loginUser")
		{
			MESSAGE_DEBUG("", action, "start");

			vector<pair<string, string>>	error_message;
			auto							success_message = ""s;
			auto							sessid = indexPage.GetCookie("sessid");

			if(sessid.length() < 5)
			{
				MESSAGE_DEBUG("", action, "with session id derived FROM cookies");

				error_message.push_back(make_pair("description", "session ID derived FROM cookie is wrong"));
				error_message.push_back(make_pair("type", "redirect"));
				error_message.push_back(make_pair("url", "/login?rand=" + GetRandom(10) + ""));
			}
			else
			{
				auto	login				= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("login"));
				auto	password			= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("password"));
				auto	confirmation_code	= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("confirmation_code"));
				auto	rememberMe			= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("signinRemember"));
				auto	country_code		= ""s;
				auto	phone_number		= ""s;
				CUser	user;

				user.SetDB(&db);

				if(confirmation_code.length())
				{
					tie(country_code, phone_number, password) = GetCountryCodeAndPhoneNumberBySMSCode(confirmation_code, indexPage.SessID_Get_FromHTTP() , &db);
					error_message = CheckPhoneConfirmationCode(confirmation_code, indexPage.SessID_Get_FromHTTP(), &db, &user);
				}

				if(error_message.empty())
				{
					if	(
							((login.find("@") != string::npos) && user.GetFromDBbyEmail(login)) ||
							(country_code.length() && phone_number.length() && password.length() && user.GetFromDBbyPhone(country_code, phone_number)) || 
							(user.GetFromDBbyLogin(login))
						)
					{

						if(!user.isActive())
						{
							error_message.push_back(make_pair("description", "пользователь неактивирован, необходима активация"));
							MESSAGE_ERROR("", action, "user [" + user.GetLogin() + "] not activated");
						}
						else
						{
							if((password != user.GetPasswd()) || (user.GetPasswd() == ""))
							{
								auto	passwd_change_timestamp = GetValueFromDB("SELECT `eventTimestamp` FROM `users_passwd` WHERE `userID`=" + quoted(user.GetID()) + " AND `eventTimestamp`>(SELECT `eventTimestamp` FROM `users_passwd` WHERE `passwd`=" + quoted(password) + " and `userID`=" + quoted(user.GetID()) + ") ORDER BY `eventTimestamp` ASC LIMIT 0,1", &db);
								if(passwd_change_timestamp.length())
								{
									// --- earlier password is user for user login
									error_message.push_back(make_pair("description", "этот пароль был изменен " + GetHumanReadableTimeDifferenceFromNow(passwd_change_timestamp)));
									MESSAGE_DEBUG("", action, "old password has been used for user [" + user.GetLogin() + "] login");
								}
								else
								{
									// --- password is wrong for user
									error_message.push_back(make_pair("description", "логин или пароль указаны не верно"));
									MESSAGE_DEBUG("", action, "user [" + user.GetLogin() + "] failed to login due to passwd error");
								}

							}
							else
							{
								MESSAGE_DEBUG("", action, "" + action + ": switching session (" + sessid + ") FROM Guest to user (" + user.GetLogin() + ")");

								// FlawFinder: ignore
								auto ra = getenv("REMOTE_ADDR");    /* Flawfinder: ignore */

								db.Query("UPDATE `sessions` SET `user_id`=\"" + user.GetID() + "\", `ip`=\"" + ra + "\", `expire`=\"" + (rememberMe == "remember-me" ? "0" : to_string(SESSION_LEN * 60)) + "\" WHERE `id`=\"" + sessid + "\";");

								if(db.isError())
								{
									error_message.push_back(make_pair("description", gettext("SQL syntax error")));
									MESSAGE_ERROR("", action, "updating `sessions` table");
								}
								else
								{
									if(rememberMe == "remember-me")
									{
										if(!indexPage.CookieUpdateTS("sessid", 0))
										{
											MESSAGE_ERROR("", action, "in setting up expiration sessid cookie to infinite");
										}
									}

									indexPage.RegisterVariableForce("loginUser", user.GetLogin());
									indexPage.RegisterVariableForce("menu_main_active", "active");

									MESSAGE_DEBUG("", action, "redirect to \"/" + config.GetFromFile("default_action", user.GetType()) + "?rand=xxxxxx\"");

									success_message = 	"\"result\": \"success\","
														"\"description\": \"\","
														"\"url\": \"/" + config.GetFromFile("default_action", user.GetType()) + "?rand=" + GetRandom(10) + "\"";
								}
							}
						}
					}
					else
					{
						error_message.push_back(make_pair("description", "Почта или Пароль указаны не верно."));
						MESSAGE_DEBUG("", action, "user [" + user.GetLogin() + "] not found");
					}
				}
				else
				{
					MESSAGE_DEBUG("", action, "error_message is not empty");
				}

			} // if(sessid.length() < 5)

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);

			MESSAGE_DEBUG("", action, "finish");
		}
/*		
		if(action == "AJAX_loginUser")
		{
			string		login, password, lng, sessid, rememberMe;
			CUser		user;
			ostringstream	ost1, ostResult;

			{
				
				MESSAGE_DEBUG("", action, "start");
			}

			sessid = indexPage.GetCookie("sessid");
			if(sessid.length() < 5)
			{
				
				MESSAGE_DEBUG("", action, "with session id derived FROM cookies");

				ostResult.str("");
				ostResult << "{";
				ostResult << "\"result\": \"error\",";
				ostResult << "\"description\": \"session ID derived FROM cookie is wrong\",";
				ostResult << "\"type\": \"redirect\",";
				ostResult << "\"url\": \"/login?rand=" << GetRandom(10) << "\"";
				ostResult << "}";
			}
			else
			{
				login		= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("login"));
				password	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("password"));
				rememberMe	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("remember"));
				lng			= CheckHTTPParam_Text(indexPage.GetLanguage());

				user.SetDB(&db);

				if	(
						((login.find("@") != string::npos) && user.GetFromDBbyEmail(login)) ||
						((login.find_first_not_of("1234567890+()- ") == string::npos) && user.GetFromDBbyPhone(login)) || 
						(user.GetFromDBbyLogin(login))
					)
				{

					if(!user.isActive())
					{
						MESSAGE_ERROR("", action, "user [" + user.GetLogin() + "] not activated");

						ostResult.str("");
						ostResult << "{"
									"\"result\": \"error\","
									"\"description\": \"пользователь неактивирован, необходима активация\""
									"}";
					}
					else
					{
						if((password != user.GetPasswd()) || (user.GetPasswd() == ""))
						{
							if(db.Query("SELECT * FROM `users_passwd` WHERE `userID`=\"" + user.GetID() + "\" and `passwd`=\"" + password + "\";"))
							{
								// --- earlier password is user for user login
								MESSAGE_DEBUG("", action, "old password has been used for user [" + user.GetLogin() + "] login");

								ostResult.str("");
								ostResult << "{";
								ostResult << "\"result\": \"error\",";
								ostResult << "\"description\": \"этот пароль был изменен " << GetHumanReadableTimeDifferenceFromNow(db.Get(0, "eventTimestamp")) << "\"";
								ostResult << "}";
							}
							else
							{
								// --- password is wrong for user
								MESSAGE_DEBUG("", action, "user [" + user.GetLogin() + "] failed to login due to passwd error");

								ostResult.str("");
								ostResult << "{";
								ostResult << "\"result\": \"error\",";
								ostResult << "\"description\": \"логин или пароль указаны не верно\"";
								ostResult << "}";
							}

						}
						else
						{
							MESSAGE_DEBUG("", action, "" + action + ": switching session (" + sessid + ") FROM Guest to user (" + user.GetLogin() + ")");

							db.Query("UPDATE `sessions` SET `user_id`=\"" + user.GetID() + "\", `ip`=\"" + getenv("REMOTE_ADDR") + "\", `expire`=\"" + (rememberMe == "remember-me" ? "0" : to_string(SESSION_LEN * 60)) + "\" WHERE `id`=\"" + sessid + "\";");

							if(db.isError())
							{
								MESSAGE_ERROR("", action, "updating `sessions` table");

								ostResult.str("");
								ostResult << "{";
								ostResult << "\"result\": \"error\",";
								ostResult << "\"description\": \"ошибка БД\"";
								ostResult << "}";
							}
							else
							{
								if(rememberMe == "remember-me")
								{
									if(!indexPage.CookieUpdateTS("sessid", 0))
									{
										MESSAGE_ERROR("", action, "in setting up expiration sessid cookie to infinite");
									}
								}

								indexPage.RegisterVariableForce("loginUser", user.GetLogin());
								indexPage.RegisterVariableForce("menu_main_active", "active");

								MESSAGE_DEBUG("", action, "redirect to \"/" + config.GetFromFile("default_action", "guest") + "?rand=xxxxxx\"");

								ostResult.str("");
								ostResult << "{";
								ostResult << "\"result\": \"success\",";
								ostResult << "\"description\": \"\",";
								ostResult << "\"url\": \"/" + config.GetFromFile("default_action", "guest") + "?rand=" << GetRandom(10) << "\"";
								ostResult << "}";
							}
						} // if(password != user.GetPasswd())
					}  // if(!user.isActive())
				}
				else
				{
					MESSAGE_DEBUG("", action, "user [" + user.GetLogin() + "] not found");

					ostResult.str("");
					ostResult << "{";
					ostResult << "\"result\": \"error\",";
					ostResult << "\"description\": \"Почта или Пароль указаны не верно.\"";
					ostResult << "}";
				}
			} // if(sessid.length() < 5)

			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("Template file was missing");
			}

			{
				
				MESSAGE_DEBUG("", action, "finish");
			}
		}
*/
		if(action == "regNewUser")
		{
			string			actID, sessid;
			CActivator		act;
			ostringstream	ost;
			int 			affected;
			string			error_message = "";
			string			redirect_url = "";
			string			result = "";

			MESSAGE_DEBUG("", action, "start");

			sessid = indexPage.GetCookie("sessid");
			if(sessid.length() < 5)
			{
				MESSAGE_ERROR("", action, "in session id [" + sessid + "]");
				error_message = "Разрешите cookie в браузере";
			}
			else
			{
					string 			regSecurityCode, regPassword, regEmail;

					regEmail = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("regEmail"));
					regPassword = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("regPassword"));
					regSecurityCode = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("regSecurityCode"));

					if(regEmail.length() <= 3)
					{
						MESSAGE_DEBUG("", action, "email is incorrect [" + regEmail + "]");
						error_message = "Введен некорректный email";
					}
					else
					{
						if(CheckUserEmailExisting(regEmail, &db))
						{
							MESSAGE_DEBUG("", action, "login or email already registered");
							error_message = "Пользователь с этим адресом уже зарегистрирован";
						}
						else
						{
							// ----- Check captcha and session coincidence
							if((affected = db.Query("SELECT id FROM `captcha` WHERE `purpose`='regNewUser' and `code`='" + regSecurityCode + "' and `session`='" + sessid + "' and `timestamp` > NOW() - INTERVAL " + to_string(SESSION_LEN) + " MINUTE")) == 0)
							{
								{
									MESSAGE_DEBUG("", action, "check captcha fail");
								}

								error_message = "Некорректный код с картинки";
								redirect_url = "/cgi-bin/index.cgi?action=weberror_captcha_template&regEmail=" + regEmail + "&rand=" + GetRandom(10);
							}
							else
							{
								CActivator 	act;
								CMailLocal	mail;
								CUser		userTemporary;
								string		remoteIP;

								{
									MESSAGE_DEBUG("", action, "check captcha success");
								}

								remoteIP = getenv("REMOTE_ADDR");    /* Flawfinder: ignore */

								affected = db.Query("DELETE FROM `captcha` WHERE `purpose`='regNewUser' and `code`=\"" + regSecurityCode + "\" and `session`=\"" + sessid + "\";");
								if(affected != 0)
								{
									MESSAGE_ERROR("", action, "in cleanup captcha table for type=regNewUser and captcha=" + regSecurityCode + ", [affected rows = " + to_string(affected) + "]");
								}

								// --- Create temporarily user
								userTemporary.SetLogin(regEmail);
								userTemporary.SetEmail(regEmail);
								userTemporary.SetPasswd(regPassword);
								userTemporary.SetType("user");
								userTemporary.SetIP(getenv("REMOTE_ADDR"));    /* Flawfinder: ignore */
								userTemporary.SetLng(indexPage.GetLanguage());
								userTemporary.SetDB(&db);
								userTemporary.Create();


								// -----  Create activator for new user
								act.SetCgi(&indexPage);
								act.SetDB(&db);
								act.SetUser(indexPage.GetVarsHandler()->Get("regEmail"));
								act.SetType("regNewUser");
								act.Save();
								// act.Activate();

								indexPage.RegisterVariableForce("activator_regNewUser", act.GetID());
								mail.Send(regEmail, "activator_regNewUser", indexPage.GetVarsHandler(), &db);

								if(!indexPage.SetTemplate("activator_regNewUser.htmlt"))
								{
									MESSAGE_ERROR("", action, "template file index.htmlt was missing");
									throw CException("Template file was missing");
								} // if(!indexPage.SetTemplate("activator_regNewUser.htmlt"))

							} // if captcha correct

						} // if(CheckUserEmailExisting(regEmail))
					}

			}

			if(error_message.empty())
			{
				result = "{\"result\":\"success\",\"redirect_url\":\"/cgi-bin/index.cgi?action=activator_regNewUser_template&rand=" + GetRandom(10) + "\"}";
			}
			else
			{
				result = "{\"result\":\"error\",\"description\":\"" + error_message + "\", \"redirect_url\":\"" + redirect_url + "\"}";
			}


			// --- scoping
			{
				string			template_name = "json_response.htmlt";
				indexPage.RegisterVariableForce("result", result);

				if(!indexPage.SetTemplate(template_name))
				{
					MESSAGE_DEBUG("", action, "can't find template " + template_name);
				} // if(!indexPage.SetTemplate("my_network.htmlt"))
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "activateNewUser") 
		{
			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				CActivator 		act;
				auto			activatorID = indexPage.GetVarsHandler()->Get("activator");


				act.SetCgi(&indexPage);
				act.SetDB(&db);
				if(!act.Load(activatorID)) 
				{
					MESSAGE_DEBUG("", action, "failed to Load activator [" + activatorID + "]");

					if(!indexPage.SetTemplate("weberror_activator_notfound.htmlt"))
					{
						MESSAGE_ERROR("", action, "template file weberror_activator_notfound.htmlt was missing");
						throw CExceptionHTML("Template file was missing");
					}

				}
				else 
				{
					// --- account activated
					act.Activate();

					// --- improve the user experience by automatically sign-in user
					// --- automatic sing-in
					string		sessid, login, rememberMe, lng;
					CUser		user;

					sessid = indexPage.GetCookie("sessid");
					if(sessid.length() < 5)
					{
						MESSAGE_DEBUG("", action, "with session id derived FROM cookies");

						if(!indexPage.SetTemplate("weberror_cookie_disabled.htmlt.htmlt"))
						{
							MESSAGE_ERROR("", action, "template weberror_cookie_disabled.htmlt can't be found");
							throw CExceptionHTML("cookies");
						}

					} // --- if(sessid.length() < 5)
					else 
					{
						login = act.GetUser();
						rememberMe = "remember-me";
						lng = indexPage.GetLanguage();

						user.SetDB(&db);
						if(!user.GetFromDBbyEmail(login)) 
						{
							MESSAGE_DEBUG("", action, "user [" + user.GetLogin() + "] not found");

							if(!indexPage.SetTemplate("weberror_user_not_found.htmlt"))
							{
								throw CExceptionHTML("template page missing");
							}

						}
						else 
						{

							if(!user.isActive()) 
							{
								MESSAGE_ERROR("", action, "user [" + user.GetLogin() + "] not activated");

								if(!indexPage.SetTemplate("weberror_user_not_activated.htmlt"))
								{
									throw CExceptionHTML("template page missing");
								}
							}
							else 
							{
								ostringstream	ost1;

								{
									MESSAGE_DEBUG("", action, "switching session (" + sessid + ") FROM Guest to user (" + user.GetLogin() + ")");
								}

								// --- 2delete if login works till Nov 1
								// ost1.str("");
								// ost1 << "update `users` set `last_online`=NOW(), `ip`='" << getenv("REMOTE_ADDR") << "' WHERE `login`='" << user.GetLogin() << "';";    /* Flawfinder: ignore */
								// db.Query(ost1.str());

								ost1.str("");
								ost1 << "update `sessions` set `user_id`='" << user.GetID() << "', `ip`='" << getenv("REMOTE_ADDR") << "', `expire`=" << (rememberMe == "remember-me" ? 0 : SESSION_LEN * 60) << " WHERE `id`='" << sessid << "';";    /* Flawfinder: ignore */
								db.Query(ost1.str());

								if(rememberMe == "remember-me") 
								{
									if(!indexPage.CookieUpdateTS("sessid", 0))
									{
										MESSAGE_ERROR("", action, "sets expiration sessid cookie to infinite");
									}
								}

								indexPage.RegisterVariableForce("loginUser", user.GetLogin());
								indexPage.RegisterVariableForce("menu_main_active", "active");


								{
									MESSAGE_DEBUG("", action, "redirection to \"news_feed?rand=xxxxxx\"");
								}
								ost1.str("");
								ost1 << "/news_feed?rand=" << GetRandom(10);
								indexPage.Redirect(ost1.str());

							}  // if(!user.isActive()) 
						}  // if(!user.isFound()) 
					} // if(sessid.length() < 5)
				} // if(!act.Load(activatorID))
			}
			else
			{
				MESSAGE_ERROR("", action, "(not an error, severity should be monitor) registered user(" + user.GetLogin() + ") attempts to access activateNewUser page, redirect to default page");

				indexPage.Redirect("/" + config.GetFromFile("default_action", user.GetType()) + "?rand=" + GetRandom(10));
			}


			MESSAGE_DEBUG("", action, "finish");
		}

		// --- Account properties
		if(action == "user_account_properties")
		{
			ostringstream	ost;
			int		affected;
			string		userID, name, nameLast, age, cv, pass, address, phone, email, isBlocked, avatarFileName, avatarFolderName, current_company;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			ost.str("");
			ost << "SELECT \
						`users`.`id` 				as `users_id`, \
						`users`.`name` 				as `users_name`, \
						`users`.`nameLast`			as `users_nameLast`, \
						`users`.`cv` 				as `users_cv`, \
						`users_passwd`.`passwd` 	as `users_passwd_passwd`, \
						`users`.`address`			as `users_address`, \
						`users`.`phone`				as `users_phone`, \
						`users`.`email`				as `users_email`, \
						`users`.`isblocked`			as `users_isblocked` \
					FROM `users` \
					INNER JOIN `users_passwd` ON `users_passwd`.`userID`=`users`.`id` \
					WHERE `users`.`id`='" << user.GetID() << "' AND `users_passwd`.`isActive`='true';";
			affected = db.Query(ost.str());
			if(affected)
			{
				indexPage.RegisterVariableForce("menu_profile_active", "active");


				userID = db.Get(0, "users_id");
				name = db.Get(0, "users_name"); 		indexPage.RegisterVariableForce("name", name);
										  				indexPage.RegisterVariableForce("myFirstName", name);
				nameLast = db.Get(0, "users_nameLast"); indexPage.RegisterVariableForce("nameLast", nameLast);
														indexPage.RegisterVariableForce("myLastName", nameLast);
				cv = db.Get(0, "users_cv");				indexPage.RegisterVariableForce("cv", cv);
				pass = db.Get(0, "users_passwd_passwd");indexPage.RegisterVariableForce("pass", pass);
				address = db.Get(0, "users_address");	indexPage.RegisterVariableForce("address", address);
				phone = db.Get(0, "users_phone");		indexPage.RegisterVariableForce("phone", phone);
				email = db.Get(0, "users_email"); 		indexPage.RegisterVariableForce("email", email);
				isBlocked = db.Get(0, "users_isblocked");  

				if(isBlocked == "Y")
				{
					indexPage.RegisterVariableForce("isblocked", "<button type=\"button\" class=\"btn btn-danger user-account-properties-visible form-control\" id=\"ButtonAccountEnable1\">Аккаунт заблокирован</button> <button type=\"button\" class=\"btn btn-success user-account-properties-hidden form-control\" id=\"ButtonAccountBlock1\">Аккаунт активен</button>");
				}
				else
				{
					indexPage.RegisterVariableForce("isblocked", "<button type=\"button\" class=\"btn btn-danger user-account-properties-hidden form-control\" id=\"ButtonAccountEnable1\">Аккаунт заблокирован</button> <button type=\"button\" class=\"btn btn-success user-account-properties-visible form-control\" id=\"ButtonAccountBlock1\">Аккаунт активен</button>");
				}

				{ 
					MESSAGE_DEBUG("", action, " user_account_properties: user details isBlocked:[" + isBlocked + "]"); 
				}
			}

			if(!indexPage.SetTemplate("user_account_properties.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file user_account_properties.htmlt was missing");
				throw CExceptionHTML("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "edit_profile")
		{

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}
			else
			{
				auto		affected = db.Query(
						"SELECT \
							`users`.`id` 				as `users_id`, \
							`users`.`name` 				as `users_name`, \
							`users`.`nameLast`			as `users_nameLast`, \
							`users`.`cv` 				as `users_cv`, \
							`users_passwd`.`passwd` 	as `users_passwd_passwd`, \
							`users`.`address`			as `users_address`, \
							`users`.`phone`				as `users_phone`, \
							`users`.`email`				as `users_email`, \
							`users`.`isblocked`			as `users_isblocked` \
						FROM `users` \
						INNER JOIN `users_passwd` ON `users_passwd`.`userID`=`users`.`id` \
						WHERE `users`.`id`='" + user.GetID() + "' AND `users_passwd`.`isActive`='true';"
					);
				if(affected)
				{
					ostringstream	ost;

					auto userID = db.Get(0, "users_id");
					auto name = db.Get(0, "users_name"); 		indexPage.RegisterVariableForce("name", name);
												  				indexPage.RegisterVariableForce("myFirstName", name);
					auto nameLast = db.Get(0, "users_nameLast"); indexPage.RegisterVariableForce("nameLast", nameLast);
																indexPage.RegisterVariableForce("myLastName", nameLast);
					auto cv = db.Get(0, "users_cv");				
					if(cv == "") cv = "Напишите несколько слов о себе";
																indexPage.RegisterVariableForce("cv", cv);
					auto pass = db.Get(0, "users_passwd_passwd");indexPage.RegisterVariableForce("pass", pass);
					auto address = db.Get(0, "users_address");	indexPage.RegisterVariableForce("address", address);
					auto phone = db.Get(0, "users_phone");		indexPage.RegisterVariableForce("phone", phone);
					auto email = db.Get(0, "users_email"); 		indexPage.RegisterVariableForce("email", email);
					auto isBlocked = db.Get(0, "users_isblocked");  

					indexPage.RegisterVariableForce("menu_profile_active", "active");

					MESSAGE_DEBUG("", action, " edit_profile: user details isBlocked:[" + isBlocked + "]"); 

					ost.str("");
					ost << "SELECT `users_company`.`id` as `users_company_id`, `company`.`name` as `company_name`, `occupation_start`, `occupation_finish`, `current_company`, `responsibilities`, `company_position`.`title` as `title` \
			FROM  `company` ,  `users_company` ,  `company_position` \
			WHERE  `user_id` =  '" << userID << "' \
			AND  `company`.`id` =  `users_company`.`company_id`  \
			AND  `company_position`.`id` =  `users_company`.`position_title_id`  \
			ORDER BY  `users_company`.`occupation_start` DESC ";
					affected = db.Query(ost.str());
					if(affected > 0)
					{
							ostringstream	ost1, ost2;
							auto			occupationFinish = ""s;
							auto			current_company = ""s;

							ost1.str("");

							for(auto i = 0; i < affected; i++, current_company = "0") {
								occupationFinish = db.Get(i, "occupation_finish");
								if(occupationFinish == "0000-00-00") 
								{
									current_company = "1";
									ost2.str("");
									ost2 << indexPage.GetVarsHandler()->Get("currentCompany");
									if(ost2.str().length() > 1) ost2 << ", ";
									ost2 << db.Get(i, "company_name");
									indexPage.RegisterVariableForce("currentCompany", ost2.str());
								}

								ost1 << "<div class='row'>\n";
								ost1 << "<div class='col-xs-4'>";
								ost1 << "<p" << (current_company == "1" ? " class=\"current_company\"" : "") << ">с ";
								ost1 << "<span data-id='" << db.Get(i, "users_company_id") << "' data-action='update_occupation_start' class='occupation_start datePick'>" << db.Get(i, "occupation_start") << "</span> по ";
								ost1 << "<span data-id='" << db.Get(i, "users_company_id") << "' data-action='update_occupation_finish' class='occupation_finish editableSpan'>" << (occupationFinish == "0000-00-00" ? "настоящее время" : occupationFinish)  << "</span></p>";
								ost1 << "</div>\n";
								ost1 << "<div class='col-xs-6'>";
								ost1 << "<p" << (current_company == "1" ? " class=\"current_company\" " : "") << "> \
								<span data-id='" << db.Get(i, "users_company_id") << "' data-action='updateJobTitle' class='jobTitle editableSpan'>"  << db.Get(i, "title") << "</span> в \
								<span data-id='" << db.Get(i, "users_company_id") << "' data-action='updateCompanyName' class='companyName editableSpan'>" << db.Get(i, "company_name") << "</span>";
								// ost1 << (current_company == "1" ? " (текущее место работы)" : "") << "</p>";
								ost1 << "</div>\n";
								ost1 << "<div class='col-xs-1'>";
								ost1 << "<span class=\"glyphicon glyphicon-remove animateClass removeCompanyExperience\" aria-hidden=\"true\" data-id='" << db.Get(i, "users_company_id") << "' data-action='AJAX_removeCompanyExperience'></span>";
								ost1 << "</div>\n";
								ost1 << "</div> <!-- row -->\n\n";
								ost1 << "<div class='row'>\n";
								ost1 << "<div class='col-xs-1'>";
								ost1 << "</div>\n";
								ost1 << "<div class='col-xs-9'>";
								ost1 << "<p id=\"responsibilities" << db.Get(i, "users_company_id") << "\" class=\"editableParagraph\" data-action=\"update_responsibilities\" data-id=\"" << db.Get(i, "users_company_id") << "\">";
								if((db.Get(i,"responsibilities")).length())
									ost1 << db.Get(i,"responsibilities");
								else
									ost1 << "Опишите круг своих обязанностей работы в компании";
								ost1 << "</p>";
								ost1 << "</div>\n";
								ost1 << "<div class='col-xs-1'>";
								ost1 << "</div>\n";
								ost1 << "</div>\n\n";
							}
							indexPage.RegisterVariableForce("carrierPath", ost1.str());
					}
					else 
					{
						indexPage.RegisterVariableForce("carrierPath", "Вы не заполнили опыт работы");
					}
				}
				else
				{
					MESSAGE_DEBUG("", action, "there is no user in DB [" + indexPage.GetVarsHandler()->Get("loginUser") + "]");
					CExceptionHTML("no user");
				}
			}

			indexPage.RegisterVariableForce("title", "Моя страница");

			if(!indexPage.SetTemplate("edit_profile.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file edit_profile.htmlt was missing");
				throw CException("Template file edit_profile.htmlt was missing");
			}  // if(!indexPage.SetTemplate("edit_profile.htmlt"))
			MESSAGE_DEBUG("", action, "finish");
		} 	// if(action == "edit_profile")

		if(action == "edit_company")
		{
			ostringstream	ost;
			string		userID, name, nameLast, age, cv, pass, address, phone, email, isBlocked, avatarFileName, avatarFolderName, current_company;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			indexPage.RegisterVariableForce("title", "Редактирование данных компании");

			if(!indexPage.SetTemplate("edit_company.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file edit_company.htmlt was missing");
				throw CException("Template file edit_company.htmlt was missing");
			}  // if(!indexPage.SetTemplate("edit_company.htmlt"))

			MESSAGE_DEBUG("", action, "finish");
		} 	// if(action == "edit_company")

		if(action == "JSON_getUserProfile")
		{
			MESSAGE_DEBUG("", action, "start");

			auto	temp_userID			= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));
			auto	userID				= (temp_userID.length() ? temp_userID : user.GetID());
			auto	success_message		= "\"users\":[" + GetUserListInJSONFormat("SELECT * FROM `users` WHERE `id`=\"" + userID + "\";", &db, &user) + "]";
			auto	error_message		= ""s;

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);

			MESSAGE_DEBUG("", action, "finish");
		}
		if(action == "updateJobTitle")
		{
			string		newJobTitle, companyId;
			string	  result = "";

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			newJobTitle = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));
			companyId	= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

			if((newJobTitle.length() > 0) && (companyId.length() > 0)) 
			{
				ostringstream	ost;
				int				affected;

				ost.str("");
				ost << "SELECT * FROM `company_position` WHERE `title`='" << newJobTitle << "';";
				if((affected = db.Query(ost.str())) > 0)
				{
					string	titleId = db.Get(0, "id");

					MESSAGE_DEBUG("", "", "job title [" + newJobTitle + "] already exists, no need to update DB.");

					ost.str("");
					ost << "update users_company set `position_title_id`='" << titleId << "' WHERE `user_id`='" << user.GetID() << "' and `id`='" << companyId << "'";
					db.Query(ost.str());

					result = "{\"result\":\"success\"}";

					// --- Update live feed
					ost.str("");
					ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"3\", \"" << companyId << "\", NOW())";
					if(db.InsertQuery(ost.str()))
					{
					}
					else
					{
						MESSAGE_ERROR("", "", "updating news_feed.");
					}

				}
				else
				{
					MESSAGE_DEBUG("", "", "new job title [" + newJobTitle + "] needed to be added to DB");

					auto	titleId = db.InsertQuery("INSERT INTO `company_position` (`title`) VALUES (\"" + newJobTitle + "\");");
					if(titleId)
					{
						ost.str("");
						ost << "update users_company set `position_title_id`=\"" << titleId << "\" WHERE `user_id`='" << user.GetID() << "' and `id`='" << companyId << "'";
						db.Query(ost.str());

						result = "{\"result\":\"success\"}";

						// --- Update live feed
						ost.str("");
						ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"3\", \"" << companyId << "\", NOW())";
						if(db.InsertQuery(ost.str()))
						{
						}
						else
						{
							MESSAGE_ERROR("", "", "updating news_feed.");
						}
					}
					else
					{
						MESSAGE_ERROR("", "", "insertion into company_position.ID (occupation [" + newJobTitle + "] can't be inserted)");
		
						result = "{\"error\":\"success\",\"description\":\"occupation insertion error\"}";
					}

				}

			}
			else
			{
				MESSAGE_DEBUG("", "", "newJobTitle [" + newJobTitle + "] or companyId [" + companyId + "] is unknown/empty");
			}

			indexPage.RegisterVariableForce("result", result);

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file json_response.htmlt was missing");
			}  // if(!indexPage.SetTemplate("JSON_getCompanyProfile.htmlt"))

			MESSAGE_DEBUG("", action, "finish");

		}

		// --- updateCompanyName
		if(action == "updateCompanyName")
		{
			MESSAGE_DEBUG("", action, "start");

			ostringstream	ostFinal;
			auto 			newCompanyName = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));
			auto 			user_company_id = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			if(newCompanyName.length() && user_company_id.length()) 
			{
				ostringstream	ost;
				int				affected;

				ost.str("");
				ost << "SELECT * FROM `company` WHERE `name`=\"" << newCompanyName << "\";";
				if((affected = db.Query(ost.str())) > 0)
				{
					auto	companyName = db.Get(0, "id");

					MESSAGE_DEBUG("", "", "company [" + newCompanyName + "] already exists, no need to update DB.");

					ost.str("");
					ost << "update `users_company` set `company_id`=\"" << companyName << "\" WHERE `user_id`='" << user.GetID() << "' and `id`='" << user_company_id << "'";
					db.Query(ost.str());

					// --- Update live feed
					ost.str("");
					ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"52\", \"" << user_company_id << "\", NOW())";
					if(db.InsertQuery(ost.str()))
					{
						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"success\"," << std::endl;
						ostFinal << "\"description\" : \"\"" << std::endl;
						ostFinal << "}" << std::endl;
					}
					else
					{
						MESSAGE_ERROR("", "", gettext("SQL syntax error"));

						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"error\"," << std::endl;
						ostFinal << "\"description\" : \"error inserting into user feed\"" << std::endl;
						ostFinal << "}" << std::endl;
					}

				}
				else
				{
					MESSAGE_DEBUG("", "", "new job title [" + newCompanyName + "] needed to be added to DB");

					auto	company_id = db.Query("INSERT INTO `company` (`name`) VALUES (\"" + newCompanyName + "\");");

					if(company_id)
					{
						db.Query("update users_company set `company_id`=\"" + to_string(company_id) + "\" WHERE `user_id`='" + user.GetID() + "' and `id`='" + user_company_id + "'");

						// --- Update live feed
						if(db.InsertQuery("INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" + user.GetID() + "\", \"52\", \"" + user_company_id + "\", NOW())"))
						{
							ostFinal.str("");
							ostFinal << "{" << std::endl;
							ostFinal << "\"result\" : \"success\"," << std::endl;
							ostFinal << "\"description\" : \"\"" << std::endl;
							ostFinal << "}" << std::endl;
						}
						else
						{

							MESSAGE_ERROR("", "", gettext("SQL syntax error"));

							ostFinal.str("");
							ostFinal << "{" << std::endl;
							ostFinal << "\"result\" : \"error\"," << std::endl;
							ostFinal << "\"description\" : \"error inserting into user feed\"" << std::endl;
							ostFinal << "}" << std::endl;
						}
					}
					else
					{
						MESSAGE_ERROR("", "", "id of newly inserted job [" + newCompanyName + "] is not defined");
					}
				}

			}
			else
			{
				MESSAGE_DEBUG("", "", "newCompanyName [" + newCompanyName + "] or user_company_id [" + user_company_id + "] is unknown/empty");
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		// --- updateCertificationTrack
		if(action == "updateCertificationTrack")
		{
			ostringstream	ostFinal;
			string			newCertificationTrack, userCertificationID;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			newCertificationTrack	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));
			userCertificationID		= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

			if((newCertificationTrack.length() > 0) && (userCertificationID.length() > 0)) 
			{
				ostringstream	ost;
				int				affected;

				ost.str("");
				ost << "SELECT * FROM `certification_tracks` WHERE `title`=\"" << newCertificationTrack << "\";";
				if((affected = db.Query(ost.str())) > 0)
				{
					auto	trackID = db.Get(0, "id");

					MESSAGE_DEBUG("", "", "company [" + newCertificationTrack + "] already exists, no need to update DB.");

					ost.str("");
					ost << "update `users_certifications` set `track_id`=\"" << trackID << "\" WHERE `user_id`='" << user.GetID() << "' and `id`='" << userCertificationID << "'";
					db.Query(ost.str());

					// --- Update live feed
					ost.str("");
					ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"24\", \"" << userCertificationID << "\", NOW())";
					if(db.InsertQuery(ost.str()))
					{
						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"success\"," << std::endl;
						ostFinal << "\"description\" : \"\"" << std::endl;
						ostFinal << "}" << std::endl;
					}
					else
					{

						MESSAGE_ERROR("", "", gettext("SQL syntax error"));

						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"error\"," << std::endl;
						ostFinal << "\"description\" : \"error inserting into user feed\"" << std::endl;
						ostFinal << "}" << std::endl;
					}

				}
				else
				{
					auto	trackID = 0;

					MESSAGE_DEBUG("", "", "new certification title [" + newCertificationTrack + "] needed to be added to DB");

					ost.str("");
					ost << "INSERT INTO `certification_tracks` (`title`) VALUES (\"" << newCertificationTrack << "\");";
					trackID = db.InsertQuery(ost.str());

					if(trackID)
					{
						ost.str("");
						ost << "update users_certifications set `track_id`=\"" << trackID << "\" WHERE `user_id`='" << user.GetID() << "' and `id`='" << userCertificationID << "'";
						db.Query(ost.str());

						// --- Update live feed
						ost.str("");
						ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"24\", \"" << userCertificationID << "\", NOW())";
						if(db.InsertQuery(ost.str()))
						{

						}
						else
						{
							MESSAGE_ERROR("", action, "inserting into `feed` (" + ost.str() + ")");
						}
					}
					else
					{
						MESSAGE_ERROR("", "", "id of newly inserted job [" + newCertificationTrack + "] is not defined");
					}
				}

			}
			else
			{
				MESSAGE_DEBUG("", "", "newCertificationTrack [" + newCertificationTrack + "] or userCertificationID [" + userCertificationID + "] is unknown/empty");
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		// --- updateCourseTrack
		if(action == "updateCourseTrack")
		{
			ostringstream	ostFinal;
			string			newCourseTrack, userCourseID;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			newCourseTrack	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));
			userCourseID	= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

			if((newCourseTrack.length() > 0) && (userCourseID.length() > 0)) 
			{
				ostringstream	ost;
				int				affected;

				ost.str("");
				ost << "SELECT * FROM `certification_tracks` WHERE `title`=\"" << newCourseTrack << "\";";
				if((affected = db.Query(ost.str())) > 0)
				{
					string	trackID = db.Get(0, "id");

					MESSAGE_DEBUG("", "", "company [" + newCourseTrack + "] already exists, no need to update DB.");

					ost.str("");
					ost << "update `users_courses` set `track_id`=\"" << trackID << "\" WHERE `user_id`='" << user.GetID() << "' and `id`='" << userCourseID << "'";
					db.Query(ost.str());

					// --- Update live feed
					ost.str("");
					ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"25\", \"" << userCourseID << "\", NOW())";
					if(db.InsertQuery(ost.str()))
					{
						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"success\"," << std::endl;
						ostFinal << "\"description\" : \"\"" << std::endl;
						ostFinal << "}" << std::endl;
					}
					else
					{
						MESSAGE_ERROR("", action, "inserting into `feed` (" + ost.str() + ")");

						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"error\"," << std::endl;
						ostFinal << "\"description\" : \"error inserting into user feed\"" << std::endl;
						ostFinal << "}" << std::endl;
					}

				}
				else
				{
					MESSAGE_DEBUG("", "", "new certification title [" + newCourseTrack + "] needed to be added to DB");

					auto	trackID = db.InsertQuery("INSERT INTO `certification_tracks` (`title`) VALUES (\"" + newCourseTrack + "\");");

					if(trackID)
					{
						ost.str("");
						ost << "update `users_courses` set `track_id`=\"" << trackID << "\" WHERE `user_id`='" << user.GetID() << "' and `id`='" << userCourseID << "'";
						db.Query(ost.str());

						// --- Update live feed
						ost.str("");
						ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"25\", \"" << userCourseID << "\", NOW())";
						if(db.InsertQuery(ost.str()))
						{

						}
						else
						{
							MESSAGE_ERROR("", action, "inserting into `feed` (" + ost.str() + ")");
						}

					}
					else
					{
						MESSAGE_ERROR("", "", "id of newly inserted job [" + newCourseTrack + "] is not defined");
					}
				}

			}
			else
			{
				MESSAGE_DEBUG("", "", "newCourseTrack [" + newCourseTrack + "] or userCourseID [" + userCourseID + "] is unknown/empty");
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		// --- updateSchoolTitle
		if(action == "updateSchoolTitle")
		{
			ostringstream	ostFinal;
			string			newSchoolNumber, userSchoolID;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			newSchoolNumber = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));
			userSchoolID	= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

			if((newSchoolNumber.length() > 0) && (userSchoolID.length() > 0)) 
			{
				ostringstream	ost;

				ost.str("");
				ost << "SELECT * FROM `users_school` WHERE `user_id`=\"" << user.GetID() << "\" and `id`=\"" << userSchoolID << "\";";
				if(db.Query(ost.str()))
				{
					unsigned long 	existingSchoolID = stol(db.Get(0, "school_id"));

					ost.str("");
					ost << "SELECT * FROM `school` WHERE `id`=\"" << existingSchoolID << "\";";
					if(db.Query(ost.str()))
					{
						auto	existingLocalityID = stol(db.Get(0, "geo_locality_id"));
						auto	newSchoolID = 0l;

						ost.str("");
						ost << "SELECT * FROM `school` WHERE `geo_locality_id`=\"" << existingLocalityID << "\" and `title`=\"" << newSchoolNumber << "\";";
						if(db.Query(ost.str()))
						{
							MESSAGE_DEBUG("", "", "school [locality " + to_string(existingLocalityID) + ", newSchoolNumber " + newSchoolNumber + "] already exists, no need to update DB.");

							newSchoolID = stol(db.Get(0, "id"));
						}
						else
						{
							MESSAGE_DEBUG("", "", "require to add new school [locality " + to_string(existingLocalityID) + ", newSchoolNumber " + newSchoolNumber + "]");

							if(!(newSchoolID = db.InsertQuery("INSERT INTO `school` (`geo_locality_id`, `title`) VALUES (\"" + to_string(existingLocalityID) + "\",\"" + newSchoolNumber + "\");")))
							{
								MESSAGE_ERROR("", "", "inserting new school (" + newSchoolNumber + ")");

								ostFinal.str("");
								ostFinal << "{" << std::endl;
								ostFinal << "\"result\" : \"error\"," << std::endl;
								ostFinal << "\"description\" : \"error inserting new school\"" << std::endl;
								ostFinal << "}" << std::endl;
							}
						}

						if(newSchoolID)
						{
							ost.str("");
							ost << "update `users_school` set `school_id`=\"" << newSchoolID << "\" WHERE `user_id`='" << user.GetID() << "' and `id`='" << userSchoolID << "'";
							db.Query(ost.str());

							// --- Update live feed
							ost.str("");
							ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"32\", \"" << userSchoolID << "\", NOW())";
							if(db.InsertQuery(ost.str()))
							{
								ostFinal.str("");
								ostFinal << "{" << std::endl;
								ostFinal << "\"result\" : \"success\"," << std::endl;
								ostFinal << "\"description\" : \"\"" << std::endl;
								ostFinal << "}" << std::endl;
							}
							else
							{

								MESSAGE_ERROR("", "", gettext("SQL syntax error"));

								ostFinal.str("");
								ostFinal << "{" << std::endl;
								ostFinal << "\"result\" : \"error\"," << std::endl;
								ostFinal << "\"description\" : \"error inserting into user feed\"" << std::endl;
								ostFinal << "}" << std::endl;
							}

						}
						else
						{
							MESSAGE_ERROR("", "", "school can't be found");

							ostFinal.str("");
							ostFinal << "{" << std::endl;
							ostFinal << "\"result\" : \"error\"," << std::endl;
							ostFinal << "\"description\" : \"school can't be found\"" << std::endl;
							ostFinal << "}" << std::endl;
						}
					}
					else
					{
						MESSAGE_ERROR("", "", "finding school (id=" + to_string(existingSchoolID) + ")");

						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"error\"," << std::endl;
						ostFinal << "\"description\" : \"error finding school in DB\"" << std::endl;
						ostFinal << "}" << std::endl;
					}
				}
				else
				{

					MESSAGE_ERROR("", "", "selecting FROM users_school (id=" + userSchoolID + ")");

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"error\"," << std::endl;
					ostFinal << "\"description\" : \"error finding user schools in DB\"" << std::endl;
					ostFinal << "}" << std::endl;

				}

			}
			else
			{
				MESSAGE_DEBUG("", "", "either newSchoolNumber [" + newSchoolNumber + "] or userSchoolID [" + userSchoolID + "] is unknown/empty");
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		// --- updateUniversityTitle
		if(action == "updateUniversityTitle")
		{
			ostringstream	ostFinal;
			string			newUniversityTitle, userUniversityID;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			newUniversityTitle	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));
			userUniversityID	= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

			if((newUniversityTitle.length() > 0) && (userUniversityID.length() > 0)) 
			{
				ostringstream	ost;

				ost.str("");
				ost << "SELECT * FROM `users_university` WHERE `user_id`=\"" << user.GetID() << "\" and `id`=\"" << userUniversityID << "\";";
				if(db.Query(ost.str()))
				{
					unsigned long 	existingUniversityID = stol(db.Get(0, "university_id"));

					ost.str("");
					ost << "SELECT * FROM `university` WHERE `id`=\"" << existingUniversityID << "\";";
					if(db.Query(ost.str()))
					{
						auto	existingRegionID = stol(db.Get(0, "geo_region_id"));
						auto	newUniversityID = 0l;

						if(db.Query("SELECT * FROM `university` WHERE `geo_region_id`=\"" + to_string(existingRegionID) + "\" and `title`=\"" + newUniversityTitle + "\";"))
						{
							MESSAGE_DEBUG("", "", "university [locality " + to_string(existingRegionID) + ", newUniversityTitle " + newUniversityTitle + "] already exists, no need to update DB.");

							newUniversityID = stol(db.Get(0, "id"));
						}
						else
						{
							MESSAGE_DEBUG("", "", "require to add new university [locality " + to_string(existingRegionID) + ", newUniversityTitle " + newUniversityTitle + "]");

							if(!(newUniversityID = db.InsertQuery("INSERT INTO `university` (`geo_region_id`, `title`) VALUES (\"" + to_string(existingRegionID) + "\",\"" + newUniversityTitle + "\");")))
							{
								MESSAGE_ERROR("", "", "inserting new university (" + newUniversityTitle + ")");

								ostFinal.str("");
								ostFinal << "{" << std::endl;
								ostFinal << "\"result\" : \"error\"," << std::endl;
								ostFinal << "\"description\" : \"error inserting new university\"" << std::endl;
								ostFinal << "}" << std::endl;
							}
						}

						if(newUniversityID)
						{
							ost.str("");
							ost << "update `users_university` set `university_id`=\"" << newUniversityID << "\" WHERE `user_id`='" << user.GetID() << "' and `id`='" << userUniversityID << "'";
							db.Query(ost.str());

							// --- Update live feed
							ost.str("");
							ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"33\", \"" << userUniversityID << "\", NOW())";
							if(db.InsertQuery(ost.str()))
							{
								ostFinal.str("");
								ostFinal << "{" << std::endl;
								ostFinal << "\"result\" : \"success\"," << std::endl;
								ostFinal << "\"description\" : \"\"" << std::endl;
								ostFinal << "}" << std::endl;
							}
							else
							{

								MESSAGE_ERROR("", "", gettext("SQL syntax error"));

								ostFinal.str("");
								ostFinal << "{" << std::endl;
								ostFinal << "\"result\" : \"error\"," << std::endl;
								ostFinal << "\"description\" : \"error inserting into user feed\"" << std::endl;
								ostFinal << "}" << std::endl;
							}

						}
						else
						{
							MESSAGE_ERROR("", "", "university can't be found");

							ostFinal.str("");
							ostFinal << "{" << std::endl;
							ostFinal << "\"result\" : \"error\"," << std::endl;
							ostFinal << "\"description\" : \"university can't be found\"" << std::endl;
							ostFinal << "}" << std::endl;
						}
					}
					else
					{
						MESSAGE_ERROR("", "", "finding university (id=" + to_string(existingUniversityID) + ")");

						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"error\"," << std::endl;
						ostFinal << "\"description\" : \"error finding university in DB\"" << std::endl;
						ostFinal << "}" << std::endl;
					}
				}
				else
				{

					MESSAGE_ERROR("", "", "selecting FROM users_university (id=" + userUniversityID + ")");

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"error\"," << std::endl;
					ostFinal << "\"description\" : \"error finding user university in DB\"" << std::endl;
					ostFinal << "}" << std::endl;

				}

			}
			else
			{
				MESSAGE_DEBUG("", "", "either newUniversityTitle [" + newUniversityTitle + "] or userUniversityID [" + userUniversityID + "] is unknown/empty");
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		// --- updateLanguageTitle
		if(action == "updateLanguageTitle")
		{
			ostringstream	ostFinal;
			string			newLanguageTitle, userLanguageID;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			newLanguageTitle	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));
			userLanguageID		= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

			if((newLanguageTitle.length() > 0) && (userLanguageID.length() > 0)) 
			{
				ostringstream	ost;
				int				affected;

				ost.str("");
				ost << "SELECT * FROM `language` WHERE `title`=\"" << newLanguageTitle << "\";";
				if((affected = db.Query(ost.str())) > 0)
				{
					auto	languageID = db.Get(0, "id");

					MESSAGE_DEBUG("", "", "company [" + newLanguageTitle + "] already exists, no need to update DB.");

					ost.str("");
					ost << "update `users_language` set `language_id`=\"" << languageID << "\" WHERE `user_id`='" << user.GetID() << "' and `id`='" << userLanguageID << "'";
					db.Query(ost.str());

					// --- Update live feed
					ost.str("");
					ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"34\", \"" << userLanguageID << "\", NOW())";
					if(db.InsertQuery(ost.str()))
					{
						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"success\"," << std::endl;
						ostFinal << "\"description\" : \"\"" << std::endl;
						ostFinal << "}" << std::endl;
					}
					else
					{
						MESSAGE_ERROR("", "", gettext("SQL syntax error"));

						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"error\"," << std::endl;
						ostFinal << "\"description\" : \"error inserting into user feed\"" << std::endl;
						ostFinal << "}" << std::endl;
					}

				}
				else
				{
					MESSAGE_DEBUG("", "", "new language title [" + newLanguageTitle + "] needed to be added to DB");

					auto	languageID = db.InsertQuery("INSERT INTO `language` (`title`) VALUES (\"" + newLanguageTitle + "\");");

					if(languageID)
					{
						ost.str("");
						ost << "update users_language set `language_id`=\"" << languageID << "\" WHERE `user_id`='" << user.GetID() << "' and `id`='" << userLanguageID << "'";
						db.Query(ost.str());

						// --- Update live feed
						ost.str("");
						ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"34\", \"" << userLanguageID << "\", NOW())";
						if(db.InsertQuery(ost.str()))
						{
							ostFinal.str("");
							ostFinal << "{" << std::endl;
							ostFinal << "\"result\" : \"success\"," << std::endl;
							ostFinal << "\"description\" : \"\"" << std::endl;
							ostFinal << "}" << std::endl;
						}
						else
						{
							MESSAGE_ERROR("", "", gettext("SQL syntax error"));

							ostFinal.str("");
							ostFinal << "{" << std::endl;
							ostFinal << "\"result\" : \"error\"," << std::endl;
							ostFinal << "\"description\" : \"error inserting into user feed\"" << std::endl;
							ostFinal << "}" << std::endl;
						}
					}
					else
					{
						MESSAGE_ERROR("", "", "id of newly inserted language [" + newLanguageTitle + "] is not defined");

						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"error\"," << std::endl;
						ostFinal << "\"description\" : \"error creating new language\"" << std::endl;
						ostFinal << "}" << std::endl;
					}
				}

			}
			else
			{
				MESSAGE_DEBUG("", "", "either newLanguageTitle [" + newLanguageTitle + "] or userLanguageID [" + userLanguageID + "] is unknown/empty");

				ostFinal.str("");
				ostFinal << "{" << std::endl;
				ostFinal << "\"result\" : \"error\"," << std::endl;
				ostFinal << "\"description\" : \"error parameters sent to server\"" << std::endl;
				ostFinal << "}" << std::endl;
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		// --- updateSkillTitle
		if(action == "updateSkillTitle")
		{
			ostringstream	ostFinal;
			string			newSkillTitle, userSkillID;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			newSkillTitle	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));
			userSkillID		= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

			if((newSkillTitle.length() > 0) && (userSkillID.length() > 0)) 
			{
				ostringstream	ost;
				int				affected;

				ost.str("");
				ost << "SELECT * FROM `skill` WHERE `title`=\"" << newSkillTitle << "\";";
				if((affected = db.Query(ost.str())) > 0)
				{
					auto	skillID = db.Get(0, "id");

					MESSAGE_DEBUG("", "", "company [" + newSkillTitle + "] already exists, no need to update DB.");

					ost.str("");
					ost << "update `users_skill` set `skill_id`=\"" << skillID << "\" WHERE `user_id`='" << user.GetID() << "' and `id`='" << userSkillID << "'";
					db.Query(ost.str());

					// --- Update live feed
					ost.str("");
					ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"42\", \"" << userSkillID << "\", NOW())";
					if(db.InsertQuery(ost.str()))
					{
						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"success\"," << std::endl;
						ostFinal << "\"description\" : \"\"" << std::endl;
						ostFinal << "}" << std::endl;
					}
					else
					{
						MESSAGE_ERROR("", "", gettext("SQL syntax error"));

						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"error\"," << std::endl;
						ostFinal << "\"description\" : \"error inserting into user feed\"" << std::endl;
						ostFinal << "}" << std::endl;
					}

				}
				else
				{
					MESSAGE_DEBUG("", "", "new skill title [" + newSkillTitle + "] needed to be added to DB");

					auto skillID = db.InsertQuery("INSERT INTO `skill` (`title`) VALUES (\"" + newSkillTitle + "\");");

					if(skillID)
					{
						ost.str("");
						ost << "update users_skill set `skill_id`=\"" << skillID << "\" WHERE `user_id`='" << user.GetID() << "' and `id`='" << userSkillID << "'";
						db.Query(ost.str());

						// --- Update live feed
						ost.str("");
						ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"42\", \"" << userSkillID << "\", NOW())";
						if(db.InsertQuery(ost.str()))
						{

						}
						else
						{
							MESSAGE_ERROR("", action, "inserting into `feed` (" + ost.str() + ")");
						}

						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"success\"," << std::endl;
						ostFinal << "\"description\" : \"\"" << std::endl;
						ostFinal << "}" << std::endl;
					}
					else
					{
						MESSAGE_ERROR("", "", "id of newly inserted skill [" + newSkillTitle + "] is not defined");

						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"error\"," << std::endl;
						ostFinal << "\"description\" : \"error creating new skill\"" << std::endl;
						ostFinal << "}" << std::endl;
					}
				}

			}
			else
			{
				MESSAGE_DEBUG("", "", "either newSkillTitle [" + newSkillTitle + "] or userSkillID [" + userSkillID + "] is unknown/empty");

				ostFinal.str("");
				ostFinal << "{" << std::endl;
				ostFinal << "\"result\" : \"error\"," << std::endl;
				ostFinal << "\"description\" : \"error parameters sent to server\"" << std::endl;
				ostFinal << "}" << std::endl;
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}


		// --- updateCertificationNumber
		if(action == "updateCertificationNumber")
		{
			ostringstream	ostFinal;
			string			newCertificationNumber, userCertificationID;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			newCertificationNumber	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));
			userCertificationID		= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

			if((newCertificationNumber.length() > 0) && (userCertificationID.length() > 0)) 
			{
				ostringstream	ost;

				ost.str("");
				ost << "update `users_certifications` set `certification_number`=\"" << newCertificationNumber << "\" WHERE `user_id`='" << user.GetID() << "' and `id`='" << userCertificationID << "'";
				db.Query(ost.str());

				// --- Update live feed
				ost.str("");
				ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"28\", \"" << userCertificationID << "\", NOW())";
				if(db.InsertQuery(ost.str()))
				{
					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"success\"," << std::endl;
					ostFinal << "\"description\" : \"\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
				else
				{
					MESSAGE_ERROR("", "", gettext("SQL syntax error"));

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"error\"," << std::endl;
					ostFinal << "\"description\" : \"error inserting into user feed\"" << std::endl;
					ostFinal << "}" << std::endl;
				}

			}
			else
			{
				MESSAGE_DEBUG("", "", "newCertificationNumber [" + newCertificationNumber + "] or userCertificationID [" + userCertificationID + "] is unknown/empty");
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		// --- updateSchoolLocality
		if(action == "updateSchoolLocality")
		{
			ostringstream	ostFinal;
			string			newSchoolLocality, userSchoolID;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			newSchoolLocality	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));
			userSchoolID		= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

			if((newSchoolLocality.length() > 0) && (userSchoolID.length() > 0)) 
			{
				ostringstream	ost;
				int				affected;

				ost.str("");
				ost << "SELECT * FROM `users_school` WHERE `id`=\"" << userSchoolID << "\" and `user_id`=\"" << user.GetID() << "\";";
				if((affected = db.Query(ost.str())) > 0)
				{
					string 	existingSchoolID = db.Get(0, "school_id");

					ost.str("");
					ost << "SELECT * FROM `school` WHERE `id`=\"" << existingSchoolID << "\";";
					if((affected = db.Query(ost.str())) > 0)
					{
						long int	localityID = 0;
						string 	existingSchoolTitle = db.Get(0, "title");

						ost.str("");
						ost << "SELECT * FROM `geo_locality` WHERE `title`=\"" << newSchoolLocality << "\";";
						if((affected = db.Query(ost.str())) > 0)
						{
							localityID = stol(db.Get(0, "id"));

							MESSAGE_DEBUG("", "", "locality [" + newSchoolLocality + "] already exists, no need to update DB.");
						}
						else
						{
							MESSAGE_DEBUG("", "", "new locality title [" + newSchoolLocality + "] needed to be added to DB");

							ost.str("");
							ost << "INSERT INTO `geo_locality` (`title`) VALUES (\"" << newSchoolLocality << "\");";
							localityID = db.InsertQuery(ost.str());
						}

						if(localityID && existingSchoolTitle.length())
						{
							long int	newSchoolID = 0;

							ost.str("");
							ost << "SELECT `id` FROM `school` WHERE `geo_locality_id`=\"" << localityID << "\" and `title`=\"" << existingSchoolTitle << "\";";
							if(db.Query(ost.str()))
							{
								newSchoolID = stol(db.Get(0, "id"));
							}
							else
							{
								ost.str("");
								ost << "INSERT INTO `school` (`geo_locality_id`, `title`) VALUES (\"" << localityID << "\", \"" << existingSchoolTitle << "\");";
								newSchoolID = db.InsertQuery(ost.str());
							}

							if(newSchoolID)
							{
								ost.str("");
								ost << "update `users_school` set `school_id`=\"" << newSchoolID << "\" WHERE `user_id`='" << user.GetID() << "' and `id`='" << userSchoolID << "';";
								db.Query(ost.str());

								// --- Update live feed
								ost.str("");
								ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"30\", \"" << userSchoolID << "\", NOW())";
								if(db.InsertQuery(ost.str()))
								{

								}
								else
								{
									MESSAGE_ERROR("", "", gettext("SQL syntax error"));
								}

								ostFinal.str("");
								ostFinal << "{";
								ostFinal << "\"result\" : \"success\",";
								ostFinal << "\"description\" : \"\"";
								ostFinal << "}";
							}
							else
							{
								MESSAGE_ERROR("", "", "id of newly inserted school [" + to_string(localityID) + ", " + existingSchoolTitle + "] is not defined");

								ostFinal.str("");
								ostFinal << "{";
								ostFinal << "\"result\" : \"error\",";
								ostFinal << "\"description\" : \"ERROR id of newly inserted school [" << localityID << ", " << existingSchoolTitle << "] is not defined\"";
								ostFinal << "}";

							}

						}
						else
						{
							MESSAGE_ERROR("", "", "localityID [" + to_string(localityID) + "] or existingSchoolTitle [" + existingSchoolTitle + "] is not defined");

							ostFinal.str("");
							ostFinal << "{";
							ostFinal << "\"result\" : \"error\",";
							ostFinal << "\"description\" : \" localityID [" << localityID << "] or existingSchoolTitle [" << existingSchoolTitle << "] is not defined\"";
							ostFinal << "}";

						}
					}
					else
					{
						MESSAGE_ERROR("", "", "id of existing schoolID [" + existingSchoolID + "] is not defined");

						ostFinal.str("");
						ostFinal << "{";
						ostFinal << "\"result\" : \"error\",";
						ostFinal << "\"description\" : \"ERROR id of existing schoolID [" << existingSchoolID << "] is not defined\"";
						ostFinal << "}";

					}

				}
				else
				{
					MESSAGE_ERROR("", "", "ERROR: userSchoolID [" + userSchoolID + "] is unknown/empty in DB or belongs to another user");

					ostFinal.str("");
					ostFinal << "{";
					ostFinal << "\"result\" : \"error\",";
					ostFinal << "\"description\" : \"userSchoolID unknown/empty in DB or belongs to another user\"";
					ostFinal << "}";
				}



			}
			else
			{
				MESSAGE_ERROR("", "", "newSchoolLocality [" + newSchoolLocality + "] or userSchoolID [" + userSchoolID + "] is unknown/empty");

				ostFinal.str("");
				ostFinal << "{";
				ostFinal << "\"result\" : \"error\",";
				ostFinal << "\"description\" : \"newSchoolLocality [" << newSchoolLocality << "] or userSchoolID [" << userSchoolID << "] is unknown/empty\"";
				ostFinal << "}";
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		// --- updateUniversityRegion
		if(action == "updateUniversityRegion")
		{
			ostringstream	ostFinal;
			string			newUniversityRegion, userUniversityID;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			newUniversityRegion = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));
			userUniversityID	= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

			if((newUniversityRegion.length() > 0) && (userUniversityID.length() > 0)) 
			{
				ostringstream	ost;
				int				affected;

				ost.str("");
				ost << "SELECT * FROM `users_university` WHERE `id`=\"" << userUniversityID << "\" and `user_id`=\"" << user.GetID() << "\";";
				if((affected = db.Query(ost.str())) > 0)
				{
					string 	existingUniversityID = db.Get(0, "university_id");

					ost.str("");
					ost << "SELECT * FROM `university` WHERE `id`=\"" << existingUniversityID << "\";";
					if((affected = db.Query(ost.str())) > 0)
					{
						long int	regionID = 0;
						string 		existingUniversityTitle = db.Get(0, "title");

						ost.str("");
						ost << "SELECT * FROM `geo_region` WHERE `title`=\"" << newUniversityRegion << "\";";
						if((affected = db.Query(ost.str())) > 0)
						{
							regionID = stol(db.Get(0, "id"));

							MESSAGE_DEBUG("", "", "region [" + newUniversityRegion + "] already exists, no need to update DB.");
						}
						else
						{
							MESSAGE_DEBUG("", "", "new region title [" + newUniversityRegion + "] needed to be added to DB");

							ost.str("");
							ost << "INSERT INTO `geo_region` (`title`) VALUES (\"" << newUniversityRegion << "\");";
							regionID = db.InsertQuery(ost.str());
						}

						if(regionID && existingUniversityTitle.length())
						{
							long int	newUniversityID = 0;

							ost.str("");
							ost << "SELECT `id` FROM `university` WHERE `geo_region_id`=\"" << regionID << "\" and `title`=\"" << existingUniversityTitle << "\";";
							if(db.Query(ost.str()))
							{
								newUniversityID = stol(db.Get(0, "id"));
							}
							else
							{
								ost.str("");
								ost << "INSERT INTO `university` (`geo_region_id`, `title`) VALUES (\"" << regionID << "\", \"" << existingUniversityTitle << "\");";
								newUniversityID = db.InsertQuery(ost.str());
							}

							if(newUniversityID)
							{
								ost.str("");
								ost << "update `users_university` set `university_id`=\"" << newUniversityID << "\" WHERE `user_id`='" << user.GetID() << "' and `id`='" << userUniversityID << "';";
								db.Query(ost.str());

								// --- Update live feed
								ost.str("");
								ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"31\", \"" << userUniversityID << "\", NOW())";
								if(db.InsertQuery(ost.str()))
								{
								}
								else
								{
									MESSAGE_ERROR("", action, "inserting into `feed` (" + ost.str() + ")");
								}

								ostFinal.str("");
								ostFinal << "{" << std::endl;
								ostFinal << "\"result\" : \"success\"," << std::endl;
								ostFinal << "\"description\" : \"\"" << std::endl;
								ostFinal << "}" << std::endl;
							}
							else
							{
								MESSAGE_ERROR("", "", "id of newly inserted university [" + to_string(regionID) + ", " + existingUniversityTitle + "] is not defined");

								ostFinal.str("");
								ostFinal << "{" << std::endl;
								ostFinal << "\"result\" : \"error\"," << std::endl;
								ostFinal << "\"description\" : \"ERROR id of newly inserted university [" << regionID << ", " << existingUniversityTitle << "] is not defined\"" << std::endl;
								ostFinal << "}" << std::endl;

							}

						}
						else
						{
							MESSAGE_ERROR("", "", "regionID [" + to_string(regionID) + "] or existingUniversityTitle [" + existingUniversityTitle + "] is not defined");

							ostFinal.str("");
							ostFinal << "{" << std::endl;
							ostFinal << "\"result\" : \"error\"," << std::endl;
							ostFinal << "\"description\" : \" regionID [" << regionID << "] or existingUniversityTitle [" << existingUniversityTitle << "] is not defined\"" << std::endl;
							ostFinal << "}" << std::endl;

						}
					}
					else
					{
						MESSAGE_ERROR("", "", "id of existing universityID [" + existingUniversityID + "] is not defined");

						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"error\"," << std::endl;
						ostFinal << "\"description\" : \"ERROR id of existing universityID [" << existingUniversityID << "] is not defined\"" << std::endl;
						ostFinal << "}" << std::endl;

					}

				}
				else
				{
					MESSAGE_ERROR("", "", "ERROR: userUniversityID [" + userUniversityID + "] is unknown/empty in DB or belongs to another user");

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"error\"," << std::endl;
					ostFinal << "\"description\" : \"userUniversityID unknown/empty in DB or belongs to another user\"" << std::endl;
					ostFinal << "}" << std::endl;
				}



			}
			else
			{
				MESSAGE_ERROR("", "", "newUniversityRegion [" + newUniversityRegion + "] or userUniversityID [" + userUniversityID + "] is unknown/empty");

				ostFinal.str("");
				ostFinal << "{" << std::endl;
				ostFinal << "\"result\" : \"error\"," << std::endl;
				ostFinal << "\"description\" : \"newUniversityRegion [" << newUniversityRegion << "] or userUniversityID [" << userUniversityID << "] is unknown/empty\"" << std::endl;
				ostFinal << "}" << std::endl;
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if((action == "AJAX_editProfile_setRecommendationAdverse") || (action == "AJAX_editProfile_setRecommendationClean"))
		{
			ostringstream	ostFinal;
			string			userRecommendationID;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			userRecommendationID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

			if(userRecommendationID.length())
			{
				ostringstream	ost;
				int				affected;

				if((affected = db.Query("SELECT * FROM `users_recommendation` WHERE `id`=\"" + userRecommendationID + "\" and `recommended_userID`=\"" + user.GetID() + "\";")) > 0)
				{
					db.Query("update `users_recommendation` set `state`=\""s + (action == "AJAX_editProfile_setRecommendationAdverse" ? "adverse" : "clean") + "\" WHERE `id`=\"" + userRecommendationID + "\";");
					if(!db.isError())
					{
						ostFinal.str("");
						ostFinal << "{";
						ostFinal << "\"result\" : \"success\",";
						ostFinal << "\"description\" : \"\"";
						ostFinal << "}";
					}
					else
					{
						MESSAGE_ERROR("", "", "updating recommendation [" + userRecommendationID + "] state");

						ostFinal.str("");
						ostFinal << "{";
						ostFinal << "\"result\" : \"error\",";
						ostFinal << "\"description\" : \"ERROR updating recommendation[" << userRecommendationID << "] state\"";
						ostFinal << "}";
					}
				}
				else
				{
					MESSAGE_ERROR("", "", "userRecommendationID [" + userRecommendationID + "] is unknown/empty in DB or belongs to another user");

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"error\"," << std::endl;
					ostFinal << "\"description\" : \"userRecommendationID unknown/empty in DB or belongs to another user\"" << std::endl;
					ostFinal << "}" << std::endl;
				}



			}
			else
			{
				MESSAGE_ERROR("", "", "userRecommendationID [" + userRecommendationID + "] is unknown/empty");

				ostFinal.str("");
				ostFinal << "{" << std::endl;
				ostFinal << "\"result\" : \"error\"," << std::endl;
				ostFinal << "\"description\" : \"userRecommendationID [" << userRecommendationID << "] is unknown/empty\"" << std::endl;
				ostFinal << "}" << std::endl;
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if((action == "AJAX_editProfile_setAppliedVacanciesAll") || (action == "AJAX_editProfile_setAppliedVacanciesInprogress"))
		{
			ostringstream	ostFinal;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			{
				db.Query("update `users` set `appliedVacanciesRender`=\"" + (action == "AJAX_editProfile_setAppliedVacanciesAll" ? string("all") : string("inprogress")) + "\" WHERE `id`=\"" + user.GetID() + "\";");
				if(!db.isError())
				{
					ostFinal.str("");
					ostFinal << "{"
							 << "\"result\" : \"success\","
							 << "}";
				}
				else
				{
					MESSAGE_ERROR("", action, "updating recommendation [" + user.GetID() + "] state");

					ostFinal.str("");
					ostFinal << "{"
							 << "\"result\" : \"error\","
							 << "\"description\" : \"ERROR updating recommendation[" << user.GetID() << "] state\""
							 << "}";
				}
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_newsFeedMarkImageToRemove")
		{
			ostringstream	ostFinal;
			string			imageIDMarkToRemove;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			imageIDMarkToRemove = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("imageID"));

			if(imageIDMarkToRemove.length() > 0) 
			{
				ostringstream	ost;
				string			messageID = "";

				// --- defining messageID by image
				if(db.Query("SELECT `setID` FROM `feed_images` WHERE `id`=\"" + imageIDMarkToRemove + "\";"))
				{
					string		setID = db.Get(0, "setID");

					if(db.Query("SELECT `id` FROM `feed_message` WHERE `imageSetID`=\"" + setID + "\";"))
					{
						messageID = db.Get(0, "id");
					}
					else
					{
						MESSAGE_ERROR("", action, "can't find message by imageSetID(" + setID + ")");
					}
				}
				else
				{
					MESSAGE_ERROR("", action, "can't find imageSet by image.id(" + imageIDMarkToRemove + ")");
				}

				if(AmIMessageOwner(messageID, &user, &db))
				{
					int				affected;

					affected = db.Query("SELECT * FROM `feed_images` WHERE `id`=\"" + imageIDMarkToRemove + "\";");
					if(affected > 0)
					{
						string	  mediaType = db.Get(0, "mediaType");
						string	  dbQuery;

						if((mediaType == "video") || (mediaType == "youtube_video"))
						{
							string	  setID = db.Get(0, "setID");
							string	  tempSet = db.Get(0, "tempSet");

							if(tempSet != "0") 
								dbQuery = "update `feed_images` set `removeFlag`=\"remove\" WHERE `tempSet`=\"" + tempSet + "\";";
							else if(setID != "0")
								dbQuery = "update `feed_images` set `removeFlag`=\"remove\" WHERE `setID`=\"" + setID + "\";";
							else
							{
								MESSAGE_ERROR("", action, "video media can't be found because tempSet=0 and setID=0");
							}
						} else if(mediaType == "image")
							dbQuery = "update `feed_images` set `removeFlag`=\"remove\" WHERE `id`=\"" + imageIDMarkToRemove + "\";";
						else
						{
							MESSAGE_ERROR("", action, "unknown mediaType [" + mediaType + "]");
						}

						if(dbQuery.length())
						{
							db.Query(dbQuery);
							if(!db.isError())
							{
								ostFinal.str("");
								ostFinal << "{" << std::endl;
								ostFinal << "\"result\" : \"success\"," << std::endl;
								ostFinal << "\"description\" : \"\"" << std::endl;
								ostFinal << "}" << std::endl;
							}
							else
							{
								MESSAGE_ERROR("", action, "marking imageID for removal (" + db.GetErrorMessage() + ")");

								ostFinal.str("");
								ostFinal << "{";
								ostFinal << "\"result\" : \"error\",";
								ostFinal << "\"description\" : \"ERROR marking imageID to removal\"";
								ostFinal << "}";

							}
						}
						else
						{
							MESSAGE_ERROR("", action, "update dbQuery is empty");

							ostFinal.str("");
							ostFinal << "{";
							ostFinal << "\"result\" : \"error\",";
							ostFinal << "\"description\" : \"ERROR with DB operations\"";
							ostFinal << "}";
						}

					}
					else
					{
						MESSAGE_ERROR("", action, "`feed_images`.`tempSet` [" + imageIDMarkToRemove + "]  is empty or doesn't belongs to you");

						ostFinal.str("");
						ostFinal << "{";
						ostFinal << "\"result\" : \"success\",";
						ostFinal << "\"description\" : \"there is no media files assigned to message or they belongs to another user\"";
						ostFinal << "}";
					}

				}
				else
				{
					MESSAGE_ERROR("", action, "can't edit message.id(" + messageID + ") such as neither you(user.id: " + user.GetID() + ") nor your companies are not owning message");

					ostFinal.str("");
					ostFinal << "{";
					ostFinal << "\"result\" : \"error\",";
					ostFinal << "\"description\" : \"Вам запрещено редактировать это сообщение\"";
					ostFinal << "}";
				}

			}
			else
			{
				MESSAGE_ERROR("", action, "imageIDMarkToRemove [" + imageIDMarkToRemove + "]  is unknown/empty");

				ostFinal.str("");
				ostFinal << "{" << std::endl;
				ostFinal << "\"result\" : \"error\"," << std::endl;
				ostFinal << "\"description\" : \"imageIDMarkToRemove [" << imageIDMarkToRemove << "]  is unknown/empty\"" << std::endl;
				ostFinal << "}" << std::endl;
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}


		// --- viewProfile_SkillApprove
		if(action == "viewProfile_SkillApprove")
		{
			MESSAGE_DEBUG("", action, "start");

			ostringstream	ostFinal;
			auto			userSkillID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			if((userSkillID.length() > 0)) 
			{
				ostringstream	ost;
				auto			affected = db.Query("SELECT * FROM `skill_confirmed` WHERE `users_skill_id`=\"" + userSkillID + "\" and `approver_userID`=\"" + user.GetID() + "\";");

				if(affected == 0)
				{
					auto	skill_confirmed_id = db.InsertQuery("INSERT INTO `skill_confirmed` (`users_skill_id`, `approver_userID`) VALUE (\"" + userSkillID + "\", \"" + user.GetID() + "\");");

					if(skill_confirmed_id)
					{
						if(db.Query("SELECT * FROM `users_skill` WHERE `id`=\"" + userSkillID + "\";"))
						{
							auto		skillOwnerUserID = db.Get(0, "user_id");

							// --- Update live feed
							if(db.InsertQuery("INSERT INTO `users_notification` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" + skillOwnerUserID + "\", \"43\", \"" + to_string(skill_confirmed_id) + "\", UNIX_TIMESTAMP())"))
							{
								ostFinal.str("");
								ostFinal << "{";
								ostFinal << "\"result\" : \"success\",";
								ostFinal << "\"description\" : \"\"";
								ostFinal << "}";
							}
							else
							{
								MESSAGE_ERROR("", "", gettext("SQL syntax error"));

								ostFinal.str("");
								ostFinal << "{";
								ostFinal << "\"result\" : \"error\",";
								ostFinal << "\"description\" : \"error inserting into user feed\"";
								ostFinal << "}";
							}
						}
						else
						{
							MESSAGE_ERROR("", "", "skill(" + userSkillID + ") not found in users_skill");

							ostFinal.str("");
							ostFinal << "{";
							ostFinal << "\"result\" : \"error\",";
							ostFinal << "\"description\" : \"error finding skill\"";
							ostFinal << "}";
						}
					}
					else
					{
						MESSAGE_ERROR("", "", gettext("SQL syntax error"));

						ostFinal.str("");
						ostFinal << "{";
						ostFinal << "\"result\" : \"error\",";
						ostFinal << "\"description\" : \"error inserting into skill DB\"";
						ostFinal << "}";
					}
				}
				else
				{
						ostFinal.str("");
						ostFinal << "{";
						ostFinal << "\"result\" : \"success\",";
						ostFinal << "\"description\" : \"already approved by this user\"";
						ostFinal << "}";
				}

			}
			else
			{
				MESSAGE_DEBUG("", "", "userSkillID [" + userSkillID + "] is unknown/empty");

				ostFinal.str("");
				ostFinal << "{";
				ostFinal << "\"result\" : \"error\",";
				ostFinal << "\"description\" : \"error parameters sent to server\"";
				ostFinal << "}";
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		// --- viewProfile_SkillReject
		if(action == "viewProfile_SkillReject")
		{
			ostringstream	ostFinal;
			string			userSkillID;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			userSkillID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

			if((userSkillID.length() > 0)) 
			{
				ostringstream	ost;
				int				affected;

				ost.str("");
				ost << "SELECT * FROM `skill_confirmed` WHERE `users_skill_id`=\"" << userSkillID << "\" and `approver_userID`=\"" << user.GetID() << "\";";
				if( (affected = db.Query(ost.str())) ) 
				{
					ost.str("");
					ost << "DELETE FROM `users_notification` WHERE `actionTypeId`='43' and `actionId` in (SELECT `id` FROM `skill_confirmed` WHERE `users_skill_id`=\"" << userSkillID << "\" and `approver_userID`=\"" << user.GetID() << "\");";
					db.Query(ost.str());

					ost.str("");
					ost << "DELETE FROM `skill_confirmed` WHERE `users_skill_id`=\"" << userSkillID << "\" and `approver_userID`=\"" << user.GetID() << "\";";
					db.Query(ost.str());

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"success\"," << std::endl;
					ostFinal << "\"description\" : \"\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
				else
				{
					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"success\"," << std::endl;
					ostFinal << "\"description\" : \"already rejected by this user\"" << std::endl;
					ostFinal << "}" << std::endl;
				}

			}
			else
			{
				MESSAGE_DEBUG("", "", "userSkillID [" + userSkillID + "] is unknown/empty");

				ostFinal.str("");
				ostFinal << "{" << std::endl;
				ostFinal << "\"result\" : \"error\"," << std::endl;
				ostFinal << "\"description\" : \"error parameters sent to server\"" << std::endl;
				ostFinal << "}" << std::endl;
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}





		// --- AJAX_updateFirstName
		if(action == "AJAX_updateFirstName")
		{
			string			firstName;
			ostringstream	ostFinal;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			firstName = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));
			ostFinal.str("");

			if((firstName.length() > 0)) 
			{
				ostringstream	ost;

				MESSAGE_DEBUG("", "", "name [" + firstName + "]");

				ost.str("");
				ost << "update `users` set `name`=\"" << firstName << "\" , `sex`=\"" << AutodetectSexByName(firstName, &db) << "\"  WHERE `id`=\"" << user.GetID() << "\";";
				db.Query(ost.str());

				// --- Update live feed
				ost.str("");
				ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"5\", \"0\", NOW())";
				if(db.InsertQuery(ost.str()))
				{
					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"success\"," << std::endl;
					ostFinal << "\"description\" : \"\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
				else
				{

					MESSAGE_ERROR("", "", gettext("SQL syntax error"));

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"error\"," << std::endl;
					ostFinal << "\"description\" : \"error inserting into user feed\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
			}
			else
			{
				MESSAGE_DEBUG("", "", "firstName [" + firstName + "] is empty");

				ostringstream	ost;

				ost.str("");
				ost << "update `users` set `name`=\"\" WHERE `id`='" << user.GetID() << "'";
				db.Query(ost.str());

				// --- Update live feed
				ost.str("");
				ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"6\", \"0\", NOW())";
				if(db.InsertQuery(ost.str()))
				{
					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"success\"," << std::endl;
					ostFinal << "\"description\" : \"\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
				else
				{
					MESSAGE_ERROR("", "", gettext("SQL syntax error"));

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"error\"," << std::endl;
					ostFinal << "\"description\" : \"error inserting into user feed\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");

		}

		// --- AJAX_updateLastName
		if(action == "AJAX_updateLastName")
		{
			string			lastName;
			ostringstream	ostFinal;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			lastName = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));

			if((lastName.length() > 0)) 
			{
				ostringstream	ost;

				MESSAGE_DEBUG("", "", "name [" + lastName + "]");

				ost.str("");
				ost << "update users set `nameLast`=\"" << lastName << "\" WHERE `id`='" << user.GetID() << "'";
				db.Query(ost.str());

				// --- Update live feed
				ost.str("");
				ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"4\", \"0\", NOW())";
				if(db.InsertQuery(ost.str()))
				{
					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"success\"," << std::endl;
					ostFinal << "\"description\" : \"\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
				else
				{
					MESSAGE_ERROR("", "", gettext("SQL syntax error"));

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"error\"," << std::endl;
					ostFinal << "\"description\" : \"error inserting into user feed\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
			}
			else
			{
				MESSAGE_DEBUG("", "", "lastName [" + lastName + "] is empty");

				ostringstream	ost;

				ost.str("");
				ost << "update users set `nameLast`=\"\" WHERE `id`='" << user.GetID() << "'";
				db.Query(ost.str());

				// --- Update live feed
				ost.str("");
				ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"7\", \"0\", NOW())";
				if(db.InsertQuery(ost.str()))
				{
					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"success\"," << std::endl;
					ostFinal << "\"description\" : \"\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
				else
				{
					MESSAGE_ERROR("", "", gettext("SQL syntax error"));

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"error\"," << std::endl;
					ostFinal << "\"description\" : \"error inserting into user feed\"" << std::endl;
					ostFinal << "}" << std::endl;
				}

			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		// --- AJAX_updateFirstLastName
		if(action == "AJAX_updateFirstLastName")
		{
			ostringstream	ostFinal;
			string			firstName, lastName;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			firstName	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("firstName"));
			lastName	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("lastName"));

			if((lastName.length() > 0)) 
			{
				MESSAGE_DEBUG("", "", "name [" + lastName + "]");

				ostringstream	ost;

				ost.str("");
				ost << "update users set `nameLast`=\"" << lastName << "\" WHERE `id`='" << user.GetID() << "'";
				db.Query(ost.str());

				// --- Update live feed
				ost.str("");
				ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"4\", \"0\", NOW())";
				if(db.InsertQuery(ost.str()))
				{
				}
				else
				{
					MESSAGE_ERROR("", "", gettext("SQL syntax error"));
				}
			}
			else
			{
				MESSAGE_DEBUG("", "", "lastName [" + lastName + "] is empty");

				ostringstream	ost;

				ost.str("");
				ost << "update users set `nameLast`=\"\" WHERE `id`='" << user.GetID() << "'";
				db.Query(ost.str());

				// --- Update live feed
				ost.str("");
				ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"7\", \"0\", NOW())";
				if(db.InsertQuery(ost.str()))
				{
				}
				else
				{
					MESSAGE_ERROR("", "", gettext("SQL syntax error"));
				}

			}

			if((firstName.length() > 0)) 
			{
				ostringstream	ost;

				MESSAGE_DEBUG("", "", "name [" + firstName + "]");

				ost.str("");
				ost << "update `users` set `name`=\"" << firstName << "\", `sex`=\"" << AutodetectSexByName(firstName, &db) << "\" WHERE `id`='" << user.GetID() << "'";
				db.Query(ost.str());

				// --- Update live feed
				ost.str("");
				ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"5\", \"0\", NOW())";
				if(db.InsertQuery(ost.str()))
				{
					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"success\"," << std::endl;
					ostFinal << "\"description\" : \"\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
				else
				{
					MESSAGE_ERROR("", "", gettext("SQL syntax error"));

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"error\"," << std::endl;
					ostFinal << "\"description\" : \"error inserting into user feed\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
			}
			else
			{
				MESSAGE_DEBUG("", "", "firstName [" + firstName + "] is empty");
				ostringstream	ost;

				ost.str("");
				ost << "update `users` set `name`=\"\" WHERE `id`='" << user.GetID() << "'";
				db.Query(ost.str());

				// --- Update live feed
				ost.str("");
				ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"6\", \"0\", NOW())";
				if(db.InsertQuery(ost.str()))
				{
					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"success\"," << std::endl;
					ostFinal << "\"description\" : \"\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
				else
				{
					MESSAGE_ERROR("", "", gettext("SQL syntax error"));

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"error\"," << std::endl;
					ostFinal << "\"description\" : \"error inserting into user feed\"" << std::endl;
					ostFinal << "}" << std::endl;
				}


			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		// --- AJAX_update_occupation_start
		if(action == "update_occupation_start")
		{
			string		occupationStart, companyId;
			ostringstream	ostFinal;

			ostFinal.str("");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			occupationStart = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));
			companyId		= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

			if((occupationStart.length() > 0) && (companyId.length() > 0)) 
			{
				ostringstream	ost;

				MESSAGE_DEBUG("", "", "date [" + occupationStart + "]");

				db.Query("update `users_company` set `occupation_start`=\"" + occupationStart + "\" WHERE `id`=\"" + companyId + "\";");

				if(!db.isError())
				{
					// --- Update live feed
					ost.str("");
					ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"2\", \"" << companyId << "\", NOW())";
					if(db.InsertQuery(ost.str()))
					{
						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"success\"" << std::endl;
						ostFinal << "}" << std::endl;
					}
					else
					{
						MESSAGE_ERROR("", "", gettext("SQL syntax error"));

						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"error\"," << std::endl;
						ostFinal << "\"description\" : \"error inserting into user feed\"" << std::endl;
						ostFinal << "}" << std::endl;
					}
				}
				else
				{
					MESSAGE_ERROR("", "", "updating DB (" + db.GetErrorMessage() + ")");

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"error\"," << std::endl;
					ostFinal << "\"description\" : \"error updating occupation start in company\"" << std::endl;
					ostFinal << "}" << std::endl;

				}

			}
			else
			{
				ostFinal << "{\"result\":\"error\", \"description\":\"issue with input parameters\"}";

				MESSAGE_DEBUG("", "", "occupationStart [" + occupationStart + "] or companyId [" + companyId + "] is unknown/empty");
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		// --- updateSchoolOccupationStart
		if(action == "updateSchoolOccupationStart")
		{
			string		occupationStart, userSchoolID;
			ostringstream	ostFinal;

			ostFinal.str("");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			occupationStart = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));
			userSchoolID	= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

			if((occupationStart.length() > 0) && (userSchoolID.length() > 0)) 
			{
				ostringstream	ost;

				MESSAGE_DEBUG("", "", "date [" + occupationStart + "]");

				if(db.Query("SELECT * FROM `users_school` WHERE `user_id`=\"" + user.GetID() + "\" and `id`=\"" + userSchoolID + "\";"))
				{
					if(stoi(occupationStart) <= stoi(db.Get(0, "occupation_finish")))
					{

						ost.str("");
						ost << "update `users_school` set `occupation_start`=\"" << occupationStart << "\" WHERE `id`='" << userSchoolID << "'";
						db.Query(ost.str());

						if(!db.isError())
						{
							// --- Update live feed
							ost.str("");
							ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"35\", \"" << userSchoolID << "\", NOW())";
							if(db.InsertQuery(ost.str()))
							{
								ostFinal.str("");
								ostFinal << "{";
								ostFinal << "\"result\" : \"success\"";
								ostFinal << "}";
							}
							else
							{
								MESSAGE_ERROR("", "", gettext("SQL syntax error"));

								ostFinal.str("");
								ostFinal << "{";
								ostFinal << "\"result\" : \"error\",";
								ostFinal << "\"description\" : \"error inserting into user feed\"";
								ostFinal << "}";
							}
						}
						else
						{
							MESSAGE_ERROR("", "", "updating DB (" + db.GetErrorMessage() + ")");

							ostFinal.str("");
							ostFinal << "{";
							ostFinal << "\"result\" : \"error\",";
							ostFinal << "\"description\" : \"error updating occupation start in company\"";
							ostFinal << "}";

						}

					}
					else
					{
						MESSAGE_DEBUG("", "", "updating period because of error in start (" + occupationStart + ") and finish dates");

						ostFinal.str("");
						ostFinal << "{";
						ostFinal << "\"result\" : \"error\",";
						ostFinal << "\"description\" : \"error start and finish date\"";
						ostFinal << "}";
					}
					
				}
				else
				{
					MESSAGE_ERROR("", "", "user not found");

					ostFinal.str("");
					ostFinal << "{";
					ostFinal << "\"result\" : \"error\",";
					ostFinal << "\"description\" : \"error finding user school entry in DB\"";
					ostFinal << "}";
				}
			}
			else
			{
				ostFinal << "{\"result\":\"error\", \"description\":\"issue with input parameters\"}";

				MESSAGE_DEBUG("", "", "occupationStart [" + occupationStart + "] or userSchoolID [" + userSchoolID + "] is unknown/empty");
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		// --- updateSchoolOccupationFinish
		if(action == "updateSchoolOccupationFinish")
		{
			string		occupationFinish, userSchoolID;
			ostringstream	ostFinal;

			ostFinal.str("");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			occupationFinish	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));
			userSchoolID		= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

			if((occupationFinish.length() > 0) && (userSchoolID.length() > 0)) 
			{
				MESSAGE_DEBUG("", "", "date [" + occupationFinish + "]");

				ostringstream	ost;

				ost.str("");
				ost << "SELECT * FROM `users_school` WHERE `user_id`=\"" << user.GetID() << "\" and `id`=\"" << userSchoolID << "\";";
				if(db.Query(ost.str()))
				{
					if(stoi(db.Get(0, "occupation_start")) <= stoi(occupationFinish))
					{

						ost.str("");
						ost << "update `users_school` set `occupation_finish`=\"" << occupationFinish << "\" WHERE `id`='" << userSchoolID << "'";
						db.Query(ost.str());

						// --- Update live feed
						ost.str("");
						ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"36\", \"" << userSchoolID << "\", NOW())";
						if(db.InsertQuery(ost.str()))
						{
							ostFinal.str("");
							ostFinal << "{" << std::endl;
							ostFinal << "\"result\" : \"success\"" << std::endl;
							ostFinal << "}" << std::endl;
						}
						else
						{
							MESSAGE_ERROR("", "", gettext("SQL syntax error"));

							ostFinal.str("");
							ostFinal << "{" << std::endl;
							ostFinal << "\"result\" : \"error\"," << std::endl;
							ostFinal << "\"description\" : \"error inserting into user feed\"" << std::endl;
							ostFinal << "}" << std::endl;
						}
					}
					else
					{
						MESSAGE_DEBUG("", "", "updating period because of error in start and finish (" + occupationFinish + ") dates");

						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"error\"," << std::endl;
						ostFinal << "\"description\" : \"error start and finish date\"" << std::endl;
						ostFinal << "}" << std::endl;
					}
					
				}
				else
				{
					MESSAGE_ERROR("", "", "user not found");

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"error\"," << std::endl;
					ostFinal << "\"description\" : \"error finding user school entry in DB\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
			}
			else
			{
				ostFinal << "{\"result\":\"error\", \"description\":\"issue with input parameters\"}";
				MESSAGE_DEBUG("", "", "occupationFinish [" + occupationFinish + "] or userSchoolID [" + userSchoolID + "] is unknown/empty");
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		// --- updateUniversityOccupationStart
		if(action == "updateUniversityOccupationStart")
		{
			string		occupationStart, userUniversityID;
			ostringstream	ostFinal;

			ostFinal.str("");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			occupationStart		= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));
			userUniversityID	= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

			if((occupationStart.length() > 0) && (userUniversityID.length() > 0)) 
			{
				MESSAGE_DEBUG("", "", "date [" + occupationStart + "]");

				ostringstream	ost;

				ost.str("");
				ost << "SELECT * FROM `users_university` WHERE `user_id`=\"" << user.GetID() << "\" and `id`=\"" << userUniversityID << "\";";
				if(db.Query(ost.str()))
				{
					if(stoi(occupationStart) <= stoi(db.Get(0, "occupation_finish")))
					{

						ost.str("");
						ost << "update `users_university` set `occupation_start`=\"" << occupationStart << "\" WHERE `id`='" << userUniversityID << "'";
						db.Query(ost.str());

						// --- Update live feed
						ost.str("");
						ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"37\", \"" << userUniversityID << "\", NOW())";
						if(db.InsertQuery(ost.str()))
						{
							ostFinal.str("");
							ostFinal << "{" << std::endl;
							ostFinal << "\"result\" : \"success\"" << std::endl;
							ostFinal << "}" << std::endl;
						}
						else
						{
							MESSAGE_ERROR("", "", gettext("SQL syntax error"));

							ostFinal.str("");
							ostFinal << "{" << std::endl;
							ostFinal << "\"result\" : \"error\"," << std::endl;
							ostFinal << "\"description\" : \"error inserting into user feed\"" << std::endl;
							ostFinal << "}" << std::endl;
						}
					}
					else
					{
						MESSAGE_DEBUG("", "", "updating period because of error in start (" + occupationStart + ") and finish dates");

						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"error\"," << std::endl;
						ostFinal << "\"description\" : \"error start and finish date\"" << std::endl;
						ostFinal << "}" << std::endl;
					}
					
				}
				else
				{
					MESSAGE_ERROR("", "", "entry not found in users_university");

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"error\"," << std::endl;
					ostFinal << "\"description\" : \"error finding user university entry in DB\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
			}
			else
			{
				ostFinal << "{\"result\":\"error\", \"description\":\"issue with input parameters\"}";

				MESSAGE_DEBUG("", "", "occupationStart [" + occupationStart + "] or userUniversityID [" + userUniversityID + "] is unknown/empty");
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		// --- updateUniversityOccupationFinish
		if(action == "updateUniversityOccupationFinish")
		{
			string		occupationFinish, userUniversityID;
			ostringstream	ostFinal;

			ostFinal.str("");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			occupationFinish = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));
			userUniversityID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

			if((occupationFinish.length() > 0) && (userUniversityID.length() > 0)) 
			{
				MESSAGE_DEBUG("", "", "date [" + occupationFinish + "]");

				ostringstream	ost;

				ost.str("");
				ost << "SELECT * FROM `users_university` WHERE `user_id`=\"" << user.GetID() << "\" and `id`=\"" << userUniversityID << "\";";
				if(db.Query(ost.str()))
				{
					if(stoi(db.Get(0, "occupation_start")) <= stoi(occupationFinish))
					{

						ost.str("");
						ost << "update `users_university` set `occupation_finish`=\"" << occupationFinish << "\" WHERE `id`='" << userUniversityID << "'";
						db.Query(ost.str());

						// --- Update live feed
						ost.str("");
						ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"38\", \"" << userUniversityID << "\", NOW())";
						if(db.InsertQuery(ost.str()))
						{
							ostFinal.str("");
							ostFinal << "{" << std::endl;
							ostFinal << "\"result\" : \"success\"" << std::endl;
							ostFinal << "}" << std::endl;
						}
						else
						{
							MESSAGE_ERROR("", "", gettext("SQL syntax error"));

							ostFinal.str("");
							ostFinal << "{" << std::endl;
							ostFinal << "\"result\" : \"error\"," << std::endl;
							ostFinal << "\"description\" : \"error inserting into user feed\"" << std::endl;
							ostFinal << "}" << std::endl;
						}
					}
					else
					{
						MESSAGE_DEBUG("", "", "updating period because of error in start and finish (" + occupationFinish + ") dates");

						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"error\"," << std::endl;
						ostFinal << "\"description\" : \"error start and finish date\"" << std::endl;
						ostFinal << "}" << std::endl;
					}
					
				}
				else
				{
					MESSAGE_ERROR("", "", "entry not found in users_university");

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"error\"," << std::endl;
					ostFinal << "\"description\" : \"error finding user university entry in DB\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
			}
			else
			{
				ostFinal << "{\"result\":\"error\", \"description\":\"issue with input parameters\"}";
				MESSAGE_DEBUG("", "", "occupationFinish [" + occupationFinish + "] or userUniversityID [" + userUniversityID + "] is unknown/empty");
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		// --- AJAX_update_occupation_finish
		if(action == "update_occupation_finish")
		{
			string		occupationFinish, companyId;
			ostringstream	ostFinal;

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			occupationFinish	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));
			companyId			= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));
			ostFinal.str("");

			if((occupationFinish.length() > 0) && (companyId.length() > 0)) 
			{
				ostringstream	ost;

				MESSAGE_DEBUG("", "", "date [" + occupationFinish + "]");

				ost.str("");
				ost << "update users_company set `occupation_finish`=\"" << occupationFinish << "\" WHERE `id`='" << companyId << "'";
				db.Query(ost.str());
				if(!db.isError())
				{
					// --- Update live feed
					ost.str("");
					ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"2\", \"" << companyId << "\", NOW())";
					if(db.InsertQuery(ost.str()))
					{
						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"success\"" << std::endl;
						ostFinal << "}" << std::endl;
					}
					else
					{
						MESSAGE_ERROR("", "", gettext("SQL syntax error"));

						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"error\"," << std::endl;
						ostFinal << "\"description\" : \"error inserting into user feed\"" << std::endl;
						ostFinal << "}" << std::endl;
					}
				}
				else
				{
					MESSAGE_ERROR("", "", "updating DB (" + db.GetErrorMessage() + ")");

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"error\"," << std::endl;
					ostFinal << "\"description\" : \"error updating occupation finish in company\"" << std::endl;
					ostFinal << "}" << std::endl;

				}

			}
			else
			{
				ostFinal << "{\"result\":\"error\", \"description\":\"issue with input parameters\"}";
				MESSAGE_DEBUG("", "", "occupationFinish [" + occupationFinish + "] or companyId [" + companyId + "] is unknown/empty");
			}


			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		// --- updateUniversityDegree
		if(action == "updateUniversityDegree")
		{
			string			  universityDegree, userUniversityID;
			ostringstream	 ostFinal;

			ostFinal.str("");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			universityDegree = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));
			userUniversityID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

			if((universityDegree.length() > 0) && (userUniversityID.length() > 0)) 
			{
				MESSAGE_DEBUG("", "", "degree [" + universityDegree + "]");

				ostringstream	ost;

				ost.str("");
				ost << "SELECT * FROM `users_university` WHERE `user_id`=\"" << user.GetID() << "\" and `id`=\"" << userUniversityID << "\";";
				if(db.Query(ost.str()))
				{
					ost.str("");
					ost << "update `users_university` set `degree`=\"" << universityDegree << "\" WHERE `id`='" << userUniversityID << "'";
					db.Query(ost.str());

					// --- Update live feed
					// --- remove previous instances
					db.Query("DELETE FROM  `feed` WHERE  `userId`=\"" + user.GetID() + "\" AND `actionTypeId`=\"39\" AND `actionId`=\"" + userUniversityID + "\";");

					// --- remove previous instances
					if(db.InsertQuery("INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" + user.GetID() + "\", \"39\", \"" + userUniversityID + "\", NOW())"))
					{
						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"success\"" << std::endl;
						ostFinal << "}" << std::endl;
					}
					else
					{
						MESSAGE_ERROR("", "", gettext("SQL syntax error"));

						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"error\"," << std::endl;
						ostFinal << "\"description\" : \"error inserting into user feed\"" << std::endl;
						ostFinal << "}" << std::endl;
					}
				}
				else
				{
					MESSAGE_ERROR("", "", "entry not found in users_university");

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"error\"," << std::endl;
					ostFinal << "\"description\" : \"error finding user university entry in DB\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
			}
			else
			{
				ostFinal << "{\"result\":\"error\", \"description\":\"issue with input parameters\"}";

				MESSAGE_DEBUG("", "", "universityDegree [" + universityDegree + "] or userUniversityID [" + userUniversityID + "] is unknown/empty")
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		// --- updateLanguageLevel
		if(action == "updateLanguageLevel")
		{
			string		languageLevel, userLanguageID;
			ostringstream	ostFinal;

			ostFinal.str("");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			languageLevel	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));
			userLanguageID	= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

			if((languageLevel.length() > 0) && (userLanguageID.length() > 0)) 
			{
				ostringstream	ost;

				MESSAGE_DEBUG("", action, " level [" + languageLevel + "]");


				ost.str("");
				ost << "SELECT * FROM `users_language` WHERE `user_id`=\"" << user.GetID() << "\" and `id`=\"" << userLanguageID << "\";";
				if(db.Query(ost.str()))
				{
					ost.str("");
					ost << "update `users_language` set `level`=\"" << languageLevel << "\" WHERE `id`='" << userLanguageID << "'";
					db.Query(ost.str());
					if(!db.isError())
					{
						// --- Update live feed
						ost.str("");
						ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"40\", \"" << userLanguageID << "\", NOW())";
						if(db.InsertQuery(ost.str()))
						{
							ostFinal.str("");
							ostFinal << "{" << std::endl;
							ostFinal << "\"result\" : \"success\"" << std::endl;
							ostFinal << "}" << std::endl;
						}
						else
						{
							MESSAGE_ERROR("", "", gettext("SQL syntax error"));

							ostFinal.str("");
							ostFinal << "{" << std::endl;
							ostFinal << "\"result\" : \"error\"," << std::endl;
							ostFinal << "\"description\" : \"error inserting into user feed\"" << std::endl;
							ostFinal << "}" << std::endl;
						}
					}
					else
					{
						MESSAGE_ERROR("", "", "updating DB [" + db.GetErrorMessage() + "]");

						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"error\"," << std::endl;
						ostFinal << "\"description\" : \"error updating language degree\"" << std::endl;
						ostFinal << "}" << std::endl;

					}

				}
				else
				{
					MESSAGE_ERROR("", "", "user language not found");

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"error\"," << std::endl;
					ostFinal << "\"description\" : \"error finding user language entry in DB\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
			}
			else
			{
				ostFinal << "{\"result\":\"error\", \"description\":\"issue with input parameters\"}";
				{
					MESSAGE_DEBUG("", action, " languageLevel [" + languageLevel + "] or userLanguageID [" + userLanguageID + "] is unknown/empty");
				}
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}



		if(action == "AJAX_updateActiveAvatar")
		{
			MESSAGE_DEBUG("", action, "start");

			auto 			avatarID = (indexPage.GetVarsHandler()->Get("id") == "-1" ? "" : CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id")));
			auto			success_message = ""s;
			auto			error_message = ""s;

			if(user.GetLogin() == "Guest")
			{
				error_message = gettext("re-login required");
				MESSAGE_DEBUG("", action, error_message);
			}

			if(avatarID.length() > 0)
			{
				db.Query("update `users_avatars` set `isActive`=\"0\" WHERE `userid`='" + user.GetID() + "';");
				db.Query("update `users_avatars` set `isActive`=\"1\" WHERE `id`=\"" + avatarID + "\" and `userid`=\"" + user.GetID() + "\";");
				db.Query("INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" + user.GetID() + "\", \"8\", \"" + avatarID + "\", NOW())");
			}
			else
			{
				error_message = gettext("mandatory parameter missed");
				MESSAGE_DEBUG("", action, error_message);
			}

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);

			MESSAGE_DEBUG("", action, "" + action + ": finish");
		}

		if(action == "AJAX_updateSiteTheme")
		{
			string			strPageToGet, strFriendsOnSinglePage;
			ostringstream	ostResult;

			MESSAGE_DEBUG("", action, "start");

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				ostResult << "{\"result\":\"error\",\"description\":\"re-login required\"}";
			}
			else
			{
				string		template_name = "json_response.htmlt";
				string		error_message = "";
				string		theme_id = "";

				theme_id = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("theme_id"));

				if(theme_id.length())
				{
					if(db.Query("SELECT `id` FROM `site_themes` WHERE `id`=\"" + theme_id + "\";"))
					{
						db.Query("UPDATE `users` SET `site_theme_id`=\"" + theme_id + "\" WHERE `id`=\"" + user.GetID() + "\";");

						if(db.isError())
						{
							MESSAGE_ERROR("", action, "DB update failed (" + db.GetErrorMessage() + ")");
							error_message = "Ошибка БД";
						}
					}
					else
					{
						MESSAGE_DEBUG("", action, "theme_id(" + theme_id + ") not found");
						error_message = "Некорректный идентификатор темы";
					}
				}
				else
				{
					MESSAGE_DEBUG("", action, "theme_id is empty");
					error_message = "Не указан идентификатор";
				}


				if(error_message.empty())
				{
					ostResult << "{\"status\":\"success\"}";
				}
				else
				{
					MESSAGE_DEBUG("", action, "business trip validity check failed");
					ostResult << "{\"status\":\"error\",\"description\":\"" + error_message + "\"}";
				}

				indexPage.RegisterVariableForce("result", ostResult.str());

				if(!indexPage.SetTemplate(template_name))
				{
					MESSAGE_DEBUG("", action, "can't find template " + template_name);
				}
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		// --- !!! IMPORTANT !!!
		// --- action _news_feed_ MUST BE BELOW action _login_user_
		if((action == "news_feed") || (action == "getUserWall"))
		{
			MESSAGE_DEBUG("", action, "start");
			
			auto	strNewsOnSinglePage	= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("NewsOnSinglePage"));
			auto	strPageToGet 		= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("page"));

			if((user.GetLogin() == "Guest") && (action == "news_feed"))
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}	

			if(strPageToGet.empty()) strPageToGet = "0";
			{
				MESSAGE_DEBUG("", action, "page " + strPageToGet + " requested");
			}

			if(db.Query("SELECT * FROM `users_avatars` WHERE `userID`=\"" + user.GetID() + "\" and `isActive`=\"1\";"))
				indexPage.RegisterVariableForce("myUserAvatar", "/images/avatars/avatars" + string(db.Get(0, "folder")) + "/" + string(db.Get(0, "filename")));

			indexPage.RegisterVariableForce("userLogin", user.GetLogin());
			indexPage.RegisterVariableForce("myFirstName", user.GetName());
			indexPage.RegisterVariableForce("myLastName", user.GetNameLast());

			indexPage.RegisterVariableForce("action", action);

			if(!indexPage.SetTemplate("news_feed.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template news_feed.htmlt");
				throw CExceptionHTML("user not activated");
			} // if(!indexPage.SetTemplate("news_feed.htmlt"))

			MESSAGE_DEBUG("", action, "finish");
		}

		if((action == "my_network") || (action == "who_watched_on_me"))
		{
			ostringstream	ost;
			string			strPageToGet, strFriendsOnSinglePage;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			if(action == "my_network") indexPage.RegisterVariableForce("title_head", "Мои друзья");
			if(action == "who_watched_on_me") indexPage.RegisterVariableForce("title_head", "Кто просматривал мой профиль");
			if(action == "companies_i_own_list") indexPage.RegisterVariableForce("title_head", "Мои компании");

			strFriendsOnSinglePage	= indexPage.GetVarsHandler()->Get("FriendsOnSinglePage");
			strPageToGet 			= indexPage.GetVarsHandler()->Get("page");
			if(strPageToGet.empty()) strPageToGet = "0";
			{
				MESSAGE_DEBUG("", action, "page " + strPageToGet + " requested");
			}

			indexPage.RegisterVariableForce("myFirstName", user.GetName());
			indexPage.RegisterVariableForce("myLastName", user.GetNameLast());


			if(!indexPage.SetTemplate("my_network.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template my_network.htmlt");
				throw CExceptionHTML("user not activated");
			} // if(!indexPage.SetTemplate("my_network.htmlt"))

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "companies_i_own_list")
		{
			ostringstream	ost;
			string			strPageToGet, strFriendsOnSinglePage;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			indexPage.RegisterVariableForce("title_head", "Мои компании");

			strFriendsOnSinglePage	= indexPage.GetVarsHandler()->Get("FriendsOnSinglePage");
			strPageToGet 			= indexPage.GetVarsHandler()->Get("page");
			if(strPageToGet.empty()) strPageToGet = "0";
			{
				MESSAGE_DEBUG("", action, "page " + strPageToGet + " requested");
			}

			indexPage.RegisterVariableForce("myFirstName", user.GetName());
			indexPage.RegisterVariableForce("myLastName", user.GetNameLast());


			if(!indexPage.SetTemplate("companies_i_own_list.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template my_network.htmlt");
				throw CExceptionHTML("user not activated");
			} // if(!indexPage.SetTemplate("my_network.htmlt"))

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "find_friends")
		{
			ostringstream	ost;
			string			strPageToGet, strFriendsOnSinglePage, searchText;

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/autologin?rand=" + GetRandom(10));
			}

			indexPage.RegisterVariableForce("title_head", "Поиск друзей");
			strFriendsOnSinglePage	= indexPage.GetVarsHandler()->Get("FriendsOnSinglePage");
			strPageToGet 			= indexPage.GetVarsHandler()->Get("page");
			if(strPageToGet.empty()) strPageToGet = "0";
			{
				MESSAGE_DEBUG("", action, "page " + strPageToGet + " requested");
			}

			indexPage.RegisterVariableForce("myFirstName", user.GetName());
			indexPage.RegisterVariableForce("myLastName", user.GetNameLast());


			if(!indexPage.SetTemplate("find_friends.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template find_friends.htmlt");
				throw CExceptionHTML("user not activated");
			} // if(!indexPage.SetTemplate("find_friends.htmlt"))
		}

		if(action == "delete_picture")
		{
			ostringstream	ost;
			int				affected;
			auto			pic = indexPage.GetVarsHandler()->Get("pic");

			if(indexPage.GetVarsHandler()->Get("loginUser").length() <= 0)
			{
				MESSAGE_ERROR("", action, "attempt to fake system");

				indexPage.Redirect("/");
			}

			affected = db.Query("SELECT * FROM users WHERE  login='" + indexPage.GetVarsHandler()->Get("loginUser") + "'");
			if(affected <= 0)
			{
				MESSAGE_ERROR("", action, "there is no user " + indexPage.GetVarsHandler()->Get("loginUser"));

				throw CExceptionHTML("no user");
			}

			if(pic.length() > 5)
			{
				db.Query("DELETE FROM `users_photo` WHERE `file`='" + pic + "'");

				ost.str("");
				ost << "Фотография удалена.<br>";
				ost << "<img src=/images/photo/" << pic << "><br>";
				ost << "Вы можете вернуться к <a href=\"/editpersonalinfo?rand=" << GetRandom(10) << "\">изменению личных параметров</a>.<br>";
			}
			else
			{
				ost.str("");
				ost << "Ошибка регистрации фотографии.<br>";
				ost << "Вы можете вернуться к <a href=\"/editpersonalinfo?rand=" << GetRandom(10) << "\">изменению личных параметров</a>.<br>";
			}

			indexPage.RegisterVariableForce("content", ost.str());
		}

		if(action == "showmain")
		{
			if(user.GetLogin() == "Guest")
			{
				indexPage.RegisterVariableForce("title", "Добро пожаловать");
				if(!indexPage.SetTemplate("main.htmlt"))
				{
					MESSAGE_ERROR("", action, "template main.htmlt not found");
					throw CException("Template file was missing");
				}		
			}
			else
			{
				MESSAGE_ERROR("", action, "(not an error, severity should be monitor) registered user(" + user.GetLogin() + ") attempts to access showmain page, redirect to default page");

				indexPage.Redirect("/" + config.GetFromFile("default_action", user.GetType()) + "?rand=" + GetRandom(10));
			}
		}

		if(action == "forget")
		{
			MESSAGE_DEBUG("", action, "start");

			ostringstream	ost;
			CMailLocal		mail;
			auto			login = RemoveQuotas(indexPage.GetVarsHandler()->Get("login"));

			if(login.length() > 0)
			{
				int		affected;

				ost.str("");
				ost << "SELECT \
							`users`.`login` 			as `users_login`, \
							`users`.`email` 			as `users_email`, \
							`users_passwd`.`passwd`		as `users_passwd_passwd` \
						FROM `users` \
						INNER JOIN `users_passwd` ON `users_passwd`.`userID`=`users`.`id` \
						WHERE (`users`.`login`=\"" << login << "\"  OR  `users`.`email`=\"" << login << "\")  AND (`users_passwd`.`isActive`='true');";

				affected = db.Query(ost.str());
				if(affected)
				{
					indexPage.RegisterVariableForce("login", db.Get(0, "users_login"));
					indexPage.RegisterVariableForce("passwd", db.Get(0, "users_passwd_passwd"));
					indexPage.RegisterVariableForce("ip", getenv("REMOTE_ADDR"));    /* Flawfinder: ignore */
					mail.Send(db.Get(0, "users_email"), "forget", indexPage.GetVarsHandler(), &db);
				}
			}
			else
			{
				{
					MESSAGE_ERROR("", action, "login is not defined");
				}
			}

			indexPage.RegisterVariableForce("content", "На ваш почтовый ящик выслан пароль !");
		}


		MESSAGE_DEBUG("", action, " end (action's == \"" + action + "\") condition");

		indexPage.OutTemplate();
	}
	catch(CExceptionHTML &c)
	{
		c.SetLanguage(indexPage.GetLanguage());
		c.SetDB(&db);

		MESSAGE_DEBUG("", action, "catch CExceptionHTML: exception reason: [" + c.GetReason() + "]");

		if(!indexPage.SetTemplate(c.GetTemplate())) return(-1);

		indexPage.RegisterVariable("content", c.GetReason());
		indexPage.OutTemplate();
		return(0);
	}
	catch(CException &c)
	{
		MESSAGE_ERROR("", action, "catch CException: exception reason: [" + c.GetReason() + "]");

		if(!indexPage.SetTemplateFile("templates/error.htmlt")) return(-1);

		indexPage.RegisterVariable("content", c.GetReason());
		indexPage.OutTemplate();
		return(-1);
	}
	catch(exception& e)
	{
		MESSAGE_ERROR("", action, "catch standard exception: [" + e.what() + "]");

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
