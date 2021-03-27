#include "avataruploader.h"	 

bool ImageSaveAsJpg (const string src, const string dst)
{
	{
		CLog	log;
		ostringstream   ost;

		ost.str("");
		ost << "ImageSaveAsJpg (" << src << ", " << dst << "): enter";
		log.Write(DEBUG, ost.str());
	}

#ifndef IMAGEMAGICK_DISABLE
	// Construct the image object. Separating image construction from the
	// the read operation ensures that a failure to read the image file
	// doesn't render the image object useless.
	try {
		Magick::Image		   image;
		Magick::OrientationType imageOrientation;
		Magick::Geometry		imageGeometry;

		// Read a file into image object
		image.read( src );

		imageGeometry = image.size();
		imageOrientation = image.orientation();

		{
			CLog	log;
			ostringstream   ost;

			ost.str("");
			ost << "ImageSaveAsJpg (" << src << ", " << dst << "): imageOrientation = " << imageOrientation << ", xRes = " << imageGeometry.width() << ", yRes = " << imageGeometry.height();
			log.Write(DEBUG, ost.str());
		}

		if(imageOrientation == Magick::TopRightOrientation) image.flop();
		if(imageOrientation == Magick::BottomRightOrientation) image.rotate(180);
		if(imageOrientation == Magick::BottomLeftOrientation) { image.flop(); image.rotate(180); }
		if(imageOrientation == Magick::LeftTopOrientation) { image.flop(); image.rotate(-90); }
		if(imageOrientation == Magick::RightTopOrientation) image.rotate(90);
		if(imageOrientation == Magick::RightBottomOrientation) { image.flop(); image.rotate(90); }
		if(imageOrientation == Magick::LeftBottomOrientation) image.rotate(-90);

		if((imageGeometry.width() > AVATAR_MAX_WIDTH) || (imageGeometry.height() > AVATAR_MAX_HEIGHT))
		{
			int   newHeight, newWidth;
			if(imageGeometry.width() >= imageGeometry.height())
			{
				newWidth = AVATAR_MAX_WIDTH;
				newHeight = newWidth * imageGeometry.height() / imageGeometry.width();
			}
			else
			{
				newHeight = AVATAR_MAX_HEIGHT;
				newWidth = newHeight * imageGeometry.width() / imageGeometry.height();
			}

			image.resize(Magick::Geometry(newWidth, newHeight, 0, 0));
		}

		// Write the image to a file
		image.write( dst );
	}
	catch( Magick::Exception &error_ )
	{
		{
			CLog	log;
			ostringstream   ost;

			ost.str("");
			ost << "ImageSaveAsJpg (" << src << ", " << dst << "): exception in read/write operation [" << error_.what() << "]";
			log.Write(DEBUG, ost.str());
		}
		return false;
	}
	{
		CLog	log;
		ostringstream   ost;

		ost.str("");
		ost << "ImageSaveAsJpg (" << src << ", " << dst << "): image has been successfully converted to .jpg format";
		log.Write(DEBUG, ost.str());
	}
	return true;
#else
	{
		CLog	log;
		ostringstream   ost;

		ost.str("");
		ost << "ImageSaveAsJpg (" << src << ", " << dst << "): simple file coping cause ImageMagick++ is not activated";
		log.Write(DEBUG, ost.str());
	}
	CopyFile(src, dst);
	return  true;
#endif
}

int main()
{
	CStatistics	 appStat;  // --- CStatistics must be a first statement to measure end2end param's
	CCgi			indexPage(EXTERNAL_TEMPLATE);
	CUser		   user;
	string		  action, partnerID;
	CMysql		  db;
	struct timeval  tv;
	ostringstream   ostJSONResult/*(static_cast<ostringstream&&>(ostringstream() << "["))*/;


	{
		CLog	log;
		log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: " + __FILE__);
	}

	signal(SIGSEGV, crash_handler); 

	gettimeofday(&tv, NULL);
	srand(tv.tv_sec * tv.tv_usec * 100000);
	ostJSONResult.clear();
		
	try
	{
		indexPage.ParseURL();

		if(!indexPage.SetTemplate("index.htmlt"))
		{
			CLog	log;

			log.Write(ERROR, "template file was missing");
			throw CException("Template file was missing");
		}

		if(db.Connect() < 0)
		{
			CLog	log;

			log.Write(ERROR, "Can not connect to mysql database");
			throw CExceptionHTML("MySql connection");
		}

		indexPage.SetDB(&db);

#ifndef IMAGEMAGICK_DISABLE
		Magick::InitializeMagick(NULL);
#endif
			
		action = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("action"));
		MESSAGE_DEBUG("", "", "action taken from HTTP is " + action);

		// --- generate common parts
		{
			// --- it has to be run before session, because session relay upon Apache environment variable
			if(RegisterInitialVariables(&indexPage, &db, &user)) {}
			else
			{
				MESSAGE_ERROR("", "", "RegisterInitialVariables failed, throwing exception");
				throw CExceptionHTML("environment variable error");
			}

			//------- Generate session
			action = GenerateSession(action, &indexPage, &db, &user);
		}
		// ------------ end generate common parts

		MESSAGE_DEBUG("", "", "pre-condition if(action == \"" + action + "\")");


		if((user.GetID().length()) && (user.GetName() != "Guest"))
		{

			for(int filesCounter = 0; filesCounter < indexPage.GetFilesHandler()->Count(); filesCounter++)
			{
				FILE			*f;
				int				folderID = (int)(rand()/(RAND_MAX + 1.0) * AVATAR_NUMBER_OF_FOLDERS) + 1;
				string			filePrefix = GetRandom(20);
				string			file2Check, tmpFile2Check, tmpImageJPG, fileName, fileExtension;
				ostringstream   ost;
				int			 affected;

				// --- Check the number of files for that user
				affected = db.Query("select id from `users_avatars` where `userid`=" + quoted(user.GetID()) + ";");
				if(affected < 4) // --- 1 text avatar and 3 image avatar
				{ 
					{
						CLog	log;
						ostringstream	ost;

						ost << string(__func__) + ": number of files POST'ed = " << indexPage.GetFilesHandler()->Count();
						log.Write(DEBUG, ost.str());
					}

					if(indexPage.GetFilesHandler()->GetSize(filesCounter) > AVATAR_MAX_FILE_SIZE) 
					{
						CLog			log;
						ostringstream	ost;

						ost.str("");
						ost << string(__func__) + ": ERROR avatar file [" << indexPage.GetFilesHandler()->GetName(filesCounter) << "] size exceed permitted maximum: " << indexPage.GetFilesHandler()->GetSize(filesCounter) << " > " << AVATAR_MAX_FILE_SIZE;

						log.Write(ERROR, ost.str());
						throw CExceptionHTML("avatar file size exceed", indexPage.GetFilesHandler()->GetName(filesCounter));
					}

					//--- check avatar file existing
					do
					{
						ostringstream	ost;
						string		  tmp;
						std::size_t  foundPos;

						folderID = (int)(rand()/(RAND_MAX + 1.0) * AVATAR_NUMBER_OF_FOLDERS) + 1;
						filePrefix = GetRandom(20);
						tmp = indexPage.GetFilesHandler()->GetName(filesCounter);

						if((foundPos = tmp.rfind(".")) != string::npos) 
						{
							fileExtension = tmp.substr(foundPos, tmp.length() - foundPos);
						}
						else
						{
							fileExtension = ".jpg";
						}

						ost.str("");
						ost << IMAGE_AVATAR_DIRECTORY << "/avatars" << folderID << "/" << filePrefix << ".jpg";
						file2Check = ost.str();

						ost.str("");
						ost << "/tmp/tmp_" << filePrefix << fileExtension;
						tmpFile2Check = ost.str();

						ost.str("");
						ost << "/tmp/" << filePrefix << ".jpg";
						tmpImageJPG = ost.str();
					} while(isFileExists(file2Check) || isFileExists(tmpFile2Check) || isFileExists(tmpImageJPG));



					{
						CLog	log;
						ostringstream	ost;

						ost << string(__func__) + ": Save file to /tmp for checking of image validity [" << tmpFile2Check << "]";
						log.Write(DEBUG, ost.str());
					}

					// --- Save file to "/tmp/" for checking of image validity
					f = fopen(tmpFile2Check.c_str(), "w");
					if(f == NULL)
					{
						{
							CLog			log;

							log.Write(ERROR, string(__func__) + ": ERROR writing file:", tmpFile2Check.c_str());
							throw CExceptionHTML("avatar file write error", indexPage.GetFilesHandler()->GetName(filesCounter));
						}
					}
					fwrite(indexPage.GetFilesHandler()->Get(filesCounter), indexPage.GetFilesHandler()->GetSize(filesCounter), 1, f);
					fclose(f);

					if(ImageSaveAsJpg(tmpFile2Check, tmpImageJPG))
					{

						MESSAGE_DEBUG("", "", "chosen filename for avatar [" + file2Check + "]");

						// --- remove previous logo
						if(db.Query("SELECT * FROM `users_avatars` WHERE `userid`=\"" + user.GetID() + "\" AND `isActive`=\"1\";"))
						{
							auto	currLogo = string(IMAGE_AVATAR_DIRECTORY) + "/" + db.Get(0, "folder") + "/" + db.Get(0, "filename");
							auto	id = db.Get(0, "id");

							if(isFileExists(currLogo)) 
							{
								MESSAGE_DEBUG("", "", "remove current logo (" + currLogo + ")");
								unlink(currLogo.c_str());
							}

							db.Query("DELETE FROM `users_avatars` WHERE `id`=" + quoted(id) + ";");
						}
						else
						{
							MESSAGE_DEBUG("", "", gettext("no active avatar found"));
						}

						CopyFile(tmpImageJPG, file2Check);

						if(auto avatarID = db.InsertQuery("insert into `users_avatars` set `userid`='" + user.GetID() + "', `folder`='" + to_string(folderID) + "', `filename`='" + filePrefix + ".jpg', `isActive`=\"1\";"))
						{
							if(filesCounter == 0) ostJSONResult << "[" << std::endl;
							if(filesCounter  > 0) ostJSONResult << ",";
							ostJSONResult << "{" << std::endl;
							ostJSONResult << "\"result\": \"success\"," << std::endl;
							ostJSONResult << "\"textStatus\": \"\"," << std::endl;
							ostJSONResult << "\"fileName\": \"" << indexPage.GetFilesHandler()->GetName(filesCounter) << "\" ," << std::endl;
							ostJSONResult << "\"jqXHR\": \"\"" << std::endl;
							ostJSONResult << "}" << std::endl;
							if(filesCounter == (indexPage.GetFilesHandler()->Count() - 1)) ostJSONResult << "]";

							// --- Update live feed
							if(!db.InsertQuery("insert into `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" + user.GetID() + "\", \"10\", \"" + to_string(avatarID) + "\", NOW())"))
							{
								MESSAGE_ERROR("", "", gettext("SQL syntax error"))
							}
						}
						else
						{
							MESSAGE_ERROR("", "", gettext("SQL syntax error"))

							if(filesCounter == 0) ostJSONResult << "[" << std::endl;
							if(filesCounter  > 0) ostJSONResult << ",";
							ostJSONResult << "{" << std::endl;
							ostJSONResult << "\"result\": \"error\"," << std::endl;
							ostJSONResult << "\"textStatus\": \"ERROR inserting into `user_avatars` table\"," << std::endl;
							ostJSONResult << "\"fileName\": \"\" ," << std::endl;
							ostJSONResult << "\"jqXHR\": \"\"" << std::endl;
							ostJSONResult << "}" << std::endl;
							if(filesCounter == (indexPage.GetFilesHandler()->Count() - 1)) ostJSONResult << "]";

							// --- Delete temporarily files
							unlink(tmpFile2Check.c_str());
							unlink(tmpImageJPG.c_str());
						}

					}
					else
					{
						{
							ostringstream   ost;
							CLog			log;

							ost.clear();
							ost << string(__func__) + ": avatar [" << indexPage.GetFilesHandler()->GetName(filesCounter) << "] is not valid image";
							log.Write(DEBUG, ost.str());
						}

						if(filesCounter == 0) ostJSONResult << "[" << std::endl;
						if(filesCounter  > 0) ostJSONResult << ",";
						ostJSONResult << "{" << std::endl;
						ostJSONResult << "\"result\": \"error\"," << std::endl;
						ostJSONResult << "\"textStatus\": \"wrong format\"," << std::endl;
						ostJSONResult << "\"fileName\": \"" << indexPage.GetFilesHandler()->GetName(filesCounter) << "\" ," << std::endl;
						ostJSONResult << "\"jqXHR\": \"\"" << std::endl;
						ostJSONResult << "}" << std::endl;
						if(filesCounter == (indexPage.GetFilesHandler()->Count() - 1)) ostJSONResult << "]";
					}
					// --- Delete temporarily files
					unlink(tmpFile2Check.c_str());
					unlink(tmpImageJPG.c_str());

				} //--- Check that number of avatars <= 4
				else
				{
					{
						ostringstream   ost;
						CLog			log;

						ost.clear();
						ost << string(__func__) + ": number of avatars for user " << user.GetLogin() << " exceeds 4";
						log.Write(DEBUG, ost.str());
					}

					if(filesCounter == 0) ostJSONResult << "[" << std::endl;
					if(filesCounter  > 0) ostJSONResult << ",";
					ostJSONResult << "{" << std::endl;
					ostJSONResult << "\"result\": \"error\"," << std::endl;
					ostJSONResult << "\"textStatus\": \"number of avatars exceeds\"," << std::endl;
					ostJSONResult << "\"fileName\": \"" << indexPage.GetFilesHandler()->GetName(filesCounter) << "\" ," << std::endl;
					ostJSONResult << "\"jqXHR\": \"\"" << std::endl;
					ostJSONResult << "}" << std::endl;
					if(filesCounter == (indexPage.GetFilesHandler()->Count() - 1)) ostJSONResult << "]";
				}

			} // --- Loop through all uploaded files

			indexPage.RegisterVariableForce("result", ostJSONResult.str());

		}
		else
		{
			ostringstream   ost;
			
			ost.str("");
			ost << "{" << std::endl;
			ost << "\"result\": \"error\"," << std::endl;
			ost << "\"textStatus\": \"" << indexPage.GetVarsHandler()->Get("ErrorDescription") << "\"," << std::endl;
			ost << "\"fileName\": \"\" ," << std::endl;
			ost << "\"jqXHR\": \"\"" << std::endl;
			ost << "}" << std::endl;

			indexPage.RegisterVariableForce("result", ost.str());
		}

		if(!indexPage.SetTemplate("json_response.htmlt"))
		{
			CLog	log;

			log.Write(ERROR, string(__func__) + ": ERROR template file was missing: ", "json_response_with_braces.htmlt");
			throw CException("Template file was missing");
		}

		indexPage.OutTemplate();

	}
	catch(CExceptionHTML &c)
	{
		CLog	log;

		c.SetLanguage(indexPage.GetLanguage());
		c.SetDB(&db);

		log.Write(ERROR, "catch CExceptionHTML: exception reason: [", c.GetReason(), "]");

		if(c.GetReason() == "avatar file write error")
		{
			ostringstream   ost;

			ost.str("");
			ost << "\"result\": \"error\"," << std::endl;
			ost << "\"textStatus\": \"" << c.GetReason() << ", file: (" << c.GetParam1() << ")\"," << std::endl;
			ost << "\"jqXHR\": \"\"" << std::endl;
			indexPage.RegisterVariableForce("result", ost.str());
		}
		if(c.GetReason() == "avatar file size exceed")
		{
			ostringstream   ost;

			ost.str("");
			ost << "\"result\": \"error\"," << std::endl;
			ost << "\"textStatus\": \"" << c.GetReason() << ", file: (" << c.GetParam1() << ")\"," << std::endl;
			ost << "\"jqXHR\": \"\"" << std::endl;
			indexPage.RegisterVariableForce("result", ost.str());
		}


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

		log.Write(ERROR, "catch CException: exception: ", c.GetReason());

		indexPage.RegisterVariable("content", c.GetReason());
		indexPage.OutTemplate();
		return(-1);
	}

	return(0);
}

