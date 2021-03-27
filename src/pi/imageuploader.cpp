#include "imageuploader.h"	 

// return message content image or video
// message could contain either VIDEO or IMAGES
int GetMessageMedia(string imageTempSet, CMysql *db)
{
	auto	 result = MESSAGE_HAVENO_MEDIA;

	MESSAGE_DEBUG("", "", "start (imageTempSet = " + imageTempSet + ")");

	if(imageTempSet.length() && db->Query("select `mediaType` from `feed_images` where `tempSet`=\"" + imageTempSet + "\";"))
	{
		string  mediaType = db->Get(0, "mediaType");

		if(mediaType == "video")
			result = MESSAGE_HAVING_VIDEO;
		else if(mediaType == "image")
			result = MESSAGE_HAVING_IMAGE;
		else if(mediaType == "youtube_video")
			result = MESSAGE_HAVING_YOUTUBE_VIDEO;
	}
	else
	{
		MESSAGE_DEBUG("", "", "imageTempSet not found or empty");
	}

	MESSAGE_DEBUG("", "", "finish (result = " + to_string(result) + ")");

	return result;
}

bool VideoConverter(const string src, const string dst, struct ExifInfo &exifInfo)
{
	bool	result = true;

	MESSAGE_DEBUG("", "", "start");

	{
		int	 ret;
		setpriority(PRIO_PROCESS, getpid(), 5);
		ret = getpriority(PRIO_PROCESS, getpid());
		if((ret > -20) and (ret < 19))
		{
			MESSAGE_DEBUG("", "", "process priority " + to_string(ret));
		}
		else
		{
			MESSAGE_ERROR("", "", "getting process priority (errno = " + to_string(errno) + ")");
		}
	}

	CopyFile(src, dst);

	MESSAGE_DEBUG("", "", "finish (result = " + to_string(result) + ")");

	return result;
}

bool ImageConvertToJpg (const string src, const string dst, struct ExifInfo &exifInfo)
{

	MESSAGE_DEBUG("", "", "start");

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

		MESSAGE_DEBUG("", "", " (" + src + ", " + dst + "): imageOrientation = " + to_string(imageOrientation) + ", xRes = " + to_string(imageGeometry.width()) + ", yRes = " + to_string(imageGeometry.height()));

		if(imageOrientation == Magick::TopRightOrientation) image.flop();
		if(imageOrientation == Magick::BottomRightOrientation) image.rotate(180);
		if(imageOrientation == Magick::BottomLeftOrientation) { image.flop(); image.rotate(180); }
		if(imageOrientation == Magick::LeftTopOrientation) { image.flop(); image.rotate(-90); }
		if(imageOrientation == Magick::RightTopOrientation) image.rotate(90);
		if(imageOrientation == Magick::RightBottomOrientation) { image.flop(); image.rotate(90); }
		if(imageOrientation == Magick::LeftBottomOrientation) image.rotate(-90);

		if((imageGeometry.width() > FEED_IMAGE_MAX_WIDTH) || (imageGeometry.height() > FEED_IMAGE_MAX_HEIGHT))
		{
			int   newHeight, newWidth;

			if(imageGeometry.width() >= imageGeometry.height())
			{
				newWidth = FEED_IMAGE_MAX_WIDTH;
				newHeight = newWidth * imageGeometry.height() / imageGeometry.width();
			}
			else
			{
				newHeight = FEED_IMAGE_MAX_HEIGHT;
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
		MESSAGE_ERROR("", "", "exception in read/write operation [" + error_.what() + "]")
		return false;
	}

	MESSAGE_DEBUG("", "", "finish (image has been successfully converted to .jpg format)")

	return true;
#else

	MESSAGE_DEBUG("", "", "simple file coping cause ImageMagick++ is not activated")

	CopyFile(src, dst);

	return  true;

#endif
}

int main()
{
	CStatistics		appStat;  // --- CStatistics must be first statement to measure end2end param's
	CCgi			indexPage(EXTERNAL_TEMPLATE);
	CUser			user;
	string			action, partnerID, imageTempSet, messageID;
	string			messageOwnerType = "";
	string			messageOwnerID = "";
	CMysql			db;
	struct timeval  tv;
	ostringstream   ostJSONResult/*(static_cast<ostringstream&&>(ostringstream() << "["))*/;
	int				messageMedia; // --- denote message type 
									// --- if type == MESSAGE_VIDEO then no pictures could be assigned
									// --- if type == MESSAGE_IMAGE then no videos could be assigned
	bool			showTemplate = true; // --- trigger displaying or not template at the end of app
										// --- template renders in 99% cases
										// --- the only exception is second stage video converting

	MESSAGE_DEBUG("", "", __FILE__)

	signal(SIGSEGV, crash_handler); 

	gettimeofday(&tv, NULL);
	srand(tv.tv_sec * tv.tv_usec * 100000);
	ostJSONResult.clear();
		
	try
	{
		indexPage.ParseURL();

		if(!indexPage.SetTemplate("index.htmlt"))
		{
			MESSAGE_ERROR("", "", "template file was missing");
			throw CException("Template file was missing");
		}

		if(db.Connect() < 0)
		{
			MESSAGE_ERROR("", "", "Can not connect to mysql database");
			throw CExceptionHTML("MySql connection");
		}

		indexPage.SetDB(&db);

#ifndef IMAGEMAGICK_DISABLE
		Magick::InitializeMagick(NULL);
#endif

		imageTempSet = indexPage.GetVarsHandler()->Get("imageTempSet");
		try { // --- validity check
			imageTempSet = to_string(stol(imageTempSet)); 
		} catch (...) {
			imageTempSet = "0";
		}
		messageID = indexPage.GetVarsHandler()->Get("messageID");
		try { // --- validity check
			messageID = to_string(atol(messageID.c_str())); 
		} catch (...) {
			messageID = "";
		}

		{
			MESSAGE_DEBUG("", "", "imageTempSet = " + imageTempSet + ",messageID = " + messageID);
		}

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


		if(messageID.length() && (AmIMessageOwner(messageID, &user, &db)))
		{
			pair<string, string> 	messageOwner = GetMessageOwner(messageID, &user, &db);

			messageOwnerType = messageOwner.first;
			messageOwnerID = messageOwner.second;
		}
		else
		{
			messageOwnerType = "user";
			messageOwnerID = user.GetID();
		}

		// --- just scoping
		{
			if(messageOwnerType.length() && messageOwnerID.length())
			{
				messageMedia = GetMessageMedia(imageTempSet, &db);
			
				MESSAGE_DEBUG("", "", "number of files POST'ed = " + to_string(indexPage.GetFilesHandler()->Count()));

				for(auto filesCounter = 0; filesCounter < indexPage.GetFilesHandler()->Count(); filesCounter++)
				{
					FILE			*f;
					auto			folderID = (int)(rand()/(RAND_MAX + 1.0) * FEEDIMAGE_NUMBER_OF_FOLDERS) + 1;
					auto			filePrefix = GetRandom(20);
					string			finalFile, tmpFile2Check, tmpImageJPG, fileName, fileExtension;
					ostringstream   ost;
					struct ExifInfo exifInfo;
					int			 currFileType = FILETYPE_UNDEFINED;

					if(isFilenameImage(indexPage.GetFilesHandler()->GetName(filesCounter))) currFileType = FILETYPE_IMAGE;
					else if(isFilenameVideo(indexPage.GetFilesHandler()->GetName(filesCounter))) currFileType = FILETYPE_VIDEO;

					if(
						((currFileType == FILETYPE_IMAGE) && (indexPage.GetFilesHandler()->GetSize(filesCounter) > FEEDIMAGE_MAX_FILE_SIZE)) 
						||
						((currFileType == FILETYPE_VIDEO) && (indexPage.GetFilesHandler()->GetSize(filesCounter) > FEEDVIDEO_MAX_FILE_SIZE))
					  )
					{
						ostringstream   ost;

						MESSAGE_ERROR("", "", "feed image file [" + indexPage.GetFilesHandler()->GetName(filesCounter) + "] size exceed permitted maximum: " + to_string(indexPage.GetFilesHandler()->GetSize(filesCounter)));


						if(filesCounter == 0) ostJSONResult << "[" << std::endl;
						if(filesCounter  > 0) ostJSONResult << ",";
						ostJSONResult << "{" << std::endl;
						ostJSONResult << "\"result\": \"error\"," << std::endl;
						ostJSONResult << "\"textStatus\": \"file size exceeded\"," << std::endl;
						ostJSONResult << "\"fileName\": \"" << indexPage.GetFilesHandler()->GetName(filesCounter) << "\" ," << std::endl;
						ostJSONResult << "\"jqXHR\": \"\"" << std::endl;
						ostJSONResult << "}" << std::endl;
						if(filesCounter == (indexPage.GetFilesHandler()->Count() - 1)) ostJSONResult << "]";
					} // --- if file size exceed
					else
					{
						if( 
							(currFileType == FILETYPE_IMAGE) 
							&& 
							((messageMedia == MESSAGE_HAVENO_MEDIA) || (messageMedia == MESSAGE_HAVING_IMAGE)) 
						  )
						{
							// --- image upload
							{
								MESSAGE_DEBUG("", "", "image uploading");
							}
							messageMedia = MESSAGE_HAVING_IMAGE;

							//--- check image file existing
							do
							{
								ostringstream   ost;
								string		  tmp;
								std::size_t	 foundPos;

								folderID = (int)(rand()/(RAND_MAX + 1.0) * FEEDIMAGE_NUMBER_OF_FOLDERS) + 1;
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
								ost << IMAGE_FEED_DIRECTORY << "/" << folderID << "/" << filePrefix << ".jpg";
								finalFile = ost.str();

								ost.str("");
								ost << "/tmp/tmp_" << filePrefix << fileExtension;
								tmpFile2Check = ost.str();

								ost.str("");
								ost << "/tmp/" << filePrefix << ".jpg";
								tmpImageJPG = ost.str();
							} while(isFileExists(finalFile) || isFileExists(tmpFile2Check) || isFileExists(tmpImageJPG));

							MESSAGE_DEBUG("", "", "Save file to /tmp for checking of image validity [" + tmpFile2Check + "]");

							// --- Save file to "/tmp/" for checking of image validity
							f = fopen(tmpFile2Check.c_str(), "w");
							if(f == NULL)
							{
								MESSAGE_ERROR("", "", "writing file:" + tmpFile2Check.c_str());

								throw CExceptionHTML("image file write error", indexPage.GetFilesHandler()->GetName(filesCounter));
							}
							fwrite(indexPage.GetFilesHandler()->Get(filesCounter), indexPage.GetFilesHandler()->GetSize(filesCounter), 1, f);
							fclose(f);

							if(ImageConvertToJpg(tmpFile2Check, tmpImageJPG, exifInfo))
							{
								auto	  feed_imagesID = 0;

								MESSAGE_DEBUG("", "", "chosen filename for feed image [" + finalFile + "]");

								CopyFile(tmpImageJPG, finalFile);

								ost.str("");
								ost << "INSERT INTO `feed_images` set \
								`tempSet`='" << imageTempSet << "', \
								`srcType`='" << messageOwnerType << "',  \
								`userID`='" << messageOwnerID << "',  \
								`folder`='" << folderID << "', \
								`filename`='" << filePrefix << ".jpg', "
								<< "`mediaType`=\"image\", "
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
								feed_imagesID = db.InsertQuery(ost.str());

								if(filesCounter == 0) ostJSONResult << "[" << std::endl;
								if(filesCounter  > 0) ostJSONResult << ",";
								if(feed_imagesID)
								{
									ostJSONResult << "{" << std::endl;
									ostJSONResult << "\"result\": \"success\"," << std::endl;
									ostJSONResult << "\"textStatus\": \"\"," << std::endl;
									ostJSONResult << "\"fileName\": \"" << indexPage.GetFilesHandler()->GetName(filesCounter) << "\" ," << std::endl;
									ostJSONResult << "\"imageID\": \"" << feed_imagesID << "\" ," << std::endl;
									ostJSONResult << "\"mediaType\": \"image\" ," << std::endl;
									ostJSONResult << "\"jqXHR\": \"\"" << std::endl;
									ostJSONResult << "}" << std::endl;
								}
								else
								{
									MESSAGE_ERROR("", "", "inserting image info into feed_images table");

									ostJSONResult << "{" << std::endl;
									ostJSONResult << "\"result\": \"error\"," << std::endl;
									ostJSONResult << "\"textStatus\": \"error inserting into table\"," << std::endl;
									ostJSONResult << "\"fileName\": \"" << indexPage.GetFilesHandler()->GetName(filesCounter) << "\" ," << std::endl;
									ostJSONResult << "\"imageID\": \"" << feed_imagesID << "\" ," << std::endl;
									ostJSONResult << "\"mediaType\": \"image\" ," << std::endl;
									ostJSONResult << "\"jqXHR\": \"\"" << std::endl;
									ostJSONResult << "}" << std::endl;
								}
								if(filesCounter == (indexPage.GetFilesHandler()->Count() - 1)) ostJSONResult << "]";

								// --- Delete temporarily files
								unlink(tmpFile2Check.c_str());
								unlink(tmpImageJPG.c_str());
							}
							else
							{
								MESSAGE_ERROR("", "", "image [" + indexPage.GetFilesHandler()->GetName(filesCounter) + "] is not valid format (looks like attack)");

								if(filesCounter == 0) ostJSONResult << "[" << std::endl;
								if(filesCounter  > 0) ostJSONResult << ",";
								ostJSONResult << "{" << std::endl;
								ostJSONResult << "\"result\": \"error\"," << std::endl;
								ostJSONResult << "\"textStatus\": \"некорректное изображение\"," << std::endl;
								ostJSONResult << "\"fileName\": \"" << indexPage.GetFilesHandler()->GetName(filesCounter) << "\" ," << std::endl;
								ostJSONResult << "\"jqXHR\": \"\"" << std::endl;
								ostJSONResult << "}" << std::endl;
								if(filesCounter == (indexPage.GetFilesHandler()->Count() - 1)) ostJSONResult << "]";
							} // --- if image successfully converted and saved to image folder
						} // --- if uploaded file image 
						else if( (messageMedia == MESSAGE_HAVENO_MEDIA) && (currFileType == FILETYPE_VIDEO) )
						{
							MESSAGE_DEBUG("", "", "video uploading");

							CVideoConverter	 videoConverter(indexPage.GetFilesHandler()->GetName(filesCounter));
							
							messageMedia = MESSAGE_HAVING_VIDEO;

							tmpFile2Check = videoConverter.GetTempFullFilename();
							tmpImageJPG = videoConverter.GetPreFinalFullFilename(0);
							finalFile = videoConverter.GetFinalFullFilename(0);

							// --- video upload
							MESSAGE_DEBUG("", "", "Save file to /tmp for checking of video validity [" + tmpFile2Check + "]");

							// --- Save file to "/tmp/" for checking of video validity
							f = fopen(tmpFile2Check.c_str(), "w");
							if(f == NULL)
							{
								MESSAGE_ERROR("", "", "writing file:" + tmpFile2Check);
								throw CExceptionHTML("image file write error", indexPage.GetFilesHandler()->GetName(filesCounter));
							}
							fwrite(indexPage.GetFilesHandler()->Get(filesCounter), indexPage.GetFilesHandler()->GetSize(filesCounter), 1, f);
							fclose(f);

							if(videoConverter.FirstPhase())
							{
								auto	  feed_mediaID = 0;

								ost.str("");
								ost << "insert into `feed_images` set "
									<< "`tempSet`='" << imageTempSet << "', "
									<< "`srcType`='" << messageOwnerType << "',  "
									<< "`userID`='" << messageOwnerID << "',  "
									<< "`folder`='" << videoConverter.GetFinalFolder() << "', "
									<< "`filename`='" << videoConverter.GetFinalFilename(0) << "',"
									<< "`exifGPSAltitude`='" << ParseGPSAltitude(videoConverter.GetMetadataLocationAltitude()) << "',"
									<< "`exifGPSLongitude`='" << ParseGPSLongitude(videoConverter.GetMetadataLocationLongitude()) << "',"
									<< "`exifGPSLatitude`='" << ParseGPSLatitude(videoConverter.GetMetadataLocationLatitude()) << "',"
									<< "`exifDateTime`='" << videoConverter.GetMetadataDateTime() << "',"
									<< "`exifMake`='" << videoConverter.GetMetadataMake() << "',"
									<< "`exifModel`='" << videoConverter.GetMetadataModel() << "',"
									<< "`exifSoftware`='" << videoConverter.GetMetadataSW() << "',"
									<< "`mediaType`=\"video\";";
								feed_mediaID = db.InsertQuery(ost.str());

								if(filesCounter == 0) ostJSONResult << "[" << std::endl;
								if(filesCounter  > 0) ostJSONResult << ",";
								if(feed_mediaID)
								{
									pid_t	   forkPID;

									ostJSONResult << "{";
									ostJSONResult << "\"result\": \"success\",";
									ostJSONResult << "\"textStatus\": \"\",";
									ostJSONResult << "\"fileName\": \"" << indexPage.GetFilesHandler()->GetName(filesCounter) << "\" ,";
									ostJSONResult << "\"imageID\": \"" << feed_mediaID << "\" ,";
									ostJSONResult << "\"mediaType\": \"video\" ,";
									ostJSONResult << "\"jqXHR\": \"\"";
									ostJSONResult << "}";


									// --- fork here
									// --- parent process: return response to user (to avoid long waiting)
									// --- child process: continue video converting to webm-format (very long conversion)
									// --- !!! IMPORTANT !!!
									// --- no need to add additional functionality here
									forkPID = fork();
									if(forkPID == 0)
									{
										pid_t	   sid;

										// --- child process
										{
											MESSAGE_DEBUG("", "", "child process after fork");
										}
										
										try
										{
											{
												MESSAGE_DEBUG("", "", "daemonize process");
											}
											// --- daemonize process
											// --- this allows apache not to wait until child process finished
											sid = setsid();
											if(sid < 0) 
											{
												MESSAGE_ERROR("", "", "can't daemonize SecondStageVideoConversion-process setsid returns error [" + to_string(EXIT_FAILURE) + "]");

												// --- it must be caught by "catch" in child process
												throw;
											}

											MESSAGE_DEBUG("", "", "redirect stdin/stdout/stderr -> /dev/null");

											// --- Redirect standard files to /dev/null 
											// --- otherwise apache waits until stdout and stderr will be closed
											if(!freopen( "/dev/null", "r", stdin))
											{
												MESSAGE_ERROR("", "", "redirect error stdin -> /dev/null");

												throw;  
											} 
											if(!freopen( "/dev/null", "w", stdout))
											{
												MESSAGE_ERROR("", "", "redirect error stdout -> /dev/null");

												throw;  
											} 
											if(!freopen( "/dev/null", "w", stderr))
											{
												MESSAGE_ERROR("", "", "redirect error stderr -> /dev/null");

												throw;  
											} 
											

											showTemplate = false;

											if(videoConverter.SecondPhase())
											{
												// --- create new DB-connection, due to parent will close the old one
												CMysql  db1;		

												if(db1.Connect() < 0)
												{
													MESSAGE_ERROR("", "", "Can not connect to mysql database");
													throw CExceptionHTML("MySql connection");
												}

												if(db1.Query("SELECT * FROM `feed_images` where `id`=\"" + to_string(feed_mediaID) + "\";"))
												{
													ost.str("");
													ost << "insert into `feed_images` set "
														<< "`setID`='" << db1.Get(0, "setID") << "', "
														<< "`tempSet`='" << db1.Get(0, "tempSet") << "', "
														<< "`srcType`='" << messageOwnerType << "',  "
														<< "`userID`='" << messageOwnerID << "',  "
														<< "`folder`='" << videoConverter.GetFinalFolder() << "', "
														<< "`filename`='" << videoConverter.GetFinalFilename(1) << "',"
														<< "`exifGPSAltitude`='" << ParseGPSAltitude(videoConverter.GetMetadataLocationAltitude()) << "',"
														<< "`exifGPSLongitude`='" << ParseGPSLongitude(videoConverter.GetMetadataLocationLongitude()) << "',"
														<< "`exifGPSLatitude`='" << ParseGPSLatitude(videoConverter.GetMetadataLocationLatitude()) << "',"
														<< "`exifDateTime`='" << videoConverter.GetMetadataDateTime() << "',"
														<< "`exifMake`='" << videoConverter.GetMetadataMake() << "',"
														<< "`exifModel`='" << videoConverter.GetMetadataModel() << "',"
														<< "`exifSoftware`='" << videoConverter.GetMetadataSW() << "',"
														<< "`mediaType`=\"video\";";
													feed_mediaID = db1.InsertQuery(ost.str());
												}
												else
												{
													MESSAGE_DEBUG("", "", "remove newly converted video [" + videoConverter.GetFinalFullFilename(1) + "] because user canceled message submission");
													
													unlink(videoConverter.GetFinalFullFilename(1).c_str());
												}

												videoConverter.Cleanup();
											}
											else
											{
												MESSAGE_ERROR("", "", "2-st phase failed, video [" + indexPage.GetFilesHandler()->GetName(filesCounter) + "] hasn't been converted");
											}

										}
										catch(...)
										{
											MESSAGE_DEBUG("", "", "exception caught in SecondStageVideoConverting-process, can't handle it, just exit");

											exit(1);
										}

										// --- exit from video converting here.
										// --- if continue many things can be broken, 
										// --- parent process already close and clear all structures
										MESSAGE_DEBUG("", "", "exit from child process");

										exit(0);
									}

									MESSAGE_DEBUG("", "", "parent process after fork");
								}
								else
								{
									MESSAGE_ERROR("", "", "inserting image info into feed_images table");

									ostJSONResult << "{" << std::endl;
									ostJSONResult << "\"result\": \"error\"," << std::endl;
									ostJSONResult << "\"textStatus\": \"error inserting into table\"," << std::endl;
									ostJSONResult << "\"fileName\": \"" << indexPage.GetFilesHandler()->GetName(filesCounter) << "\" ," << std::endl;
									ostJSONResult << "\"imageID\": \"" << feed_mediaID << "\" ," << std::endl;
									ostJSONResult << "\"mediaType\": \"video\" ," << std::endl;
									ostJSONResult << "\"jqXHR\": \"\"" << std::endl;
									ostJSONResult << "}" << std::endl;
								}
								if(filesCounter == (indexPage.GetFilesHandler()->Count() - 1)) ostJSONResult << "]";
							}
							else
							{
								MESSAGE_ERROR("", "", "1-st phase failed, video [" + indexPage.GetFilesHandler()->GetName(filesCounter) + "] is not valid format")

								if(filesCounter == 0) ostJSONResult << "[" << std::endl;
								if(filesCounter  > 0) ostJSONResult << ",";
								ostJSONResult << "{" << std::endl;
								ostJSONResult << "\"result\": \"error\"," << std::endl;
								ostJSONResult << "\"textStatus\": \"некорректное изображение\"," << std::endl;
								ostJSONResult << "\"fileName\": \"" << indexPage.GetFilesHandler()->GetName(filesCounter) << "\" ," << std::endl;
								ostJSONResult << "\"jqXHR\": \"\"" << std::endl;
								ostJSONResult << "}" << std::endl;
								if(filesCounter == (indexPage.GetFilesHandler()->Count() - 1)) ostJSONResult << "]";
							}
						} // --- if uploaded file video
						else
						{
							{
								if(currFileType == FILETYPE_UNDEFINED) 
								{
									MESSAGE_ERROR("", "", "file type (" + indexPage.GetFilesHandler()->GetName(filesCounter) + "): undefined");
								}
								else 
								{
									MESSAGE_DEBUG("", "", "video and images can't be mixed in a single message  or  more than one video file uploaded");
								}
							}

							if(filesCounter == 0) ostJSONResult << "[" << std::endl;
							if(filesCounter  > 0) ostJSONResult << ",";
							ostJSONResult << "{" << std::endl;
							ostJSONResult << "\"result\": \"error\"," << std::endl;
							ostJSONResult << "\"textStatus\": \"<br>картинки, видео, youtube видео - не могут быть в одном сообщении<br>в одном сообщении не может быть больше одного видео\"," << std::endl;
							ostJSONResult << "\"fileName\": \"" << indexPage.GetFilesHandler()->GetName(filesCounter) << "\" ," << std::endl;
							ostJSONResult << "\"jqXHR\": \"\"" << std::endl;
							ostJSONResult << "}" << std::endl;
							if(filesCounter == (indexPage.GetFilesHandler()->Count() - 1)) ostJSONResult << "]";
						}  // --- if uploaded file neither image nor video
					} // --- if file size acceptable
				} // --- Loop through all uploaded files
			}
			else
			{
				MESSAGE_ERROR("", "", "message owner error (type:" + messageOwnerType + ", id:" + messageOwnerID + ")")

				ostJSONResult.str("");
				ostJSONResult << "{";
				ostJSONResult << "\"result\" : \"error\",";
				ostJSONResult << "\"description\" : \"Ошибка: Не получилось определить владельца сообщения\"";
				ostJSONResult << "}";
			}
		}

		// --- template shown in 99%
		// --- not demonstrated only for second stage of video converting.
		if(showTemplate)
		{
			indexPage.RegisterVariableForce("result", ostJSONResult.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", "", "template file was missing: json_response.htmlt");
				throw CException("Template file was missing");
			}

			indexPage.OutTemplate();
		}
	}
	catch(CExceptionHTML &c)
	{
		ostringstream   ost;

		c.SetLanguage(indexPage.GetLanguage());
		c.SetDB(&db);

		MESSAGE_ERROR("", "", "catch CExceptionHTML: exception reason: [" + c.GetReason() + "]");

		ost.str("");
		ost << "[{" << std::endl;
		ost << "\"result\": \"error\"," << std::endl;
		ost << "\"textStatus\": \"" << c.GetReason();
		if(c.GetParam1().length() > 0) ost << " (" << c.GetParam1() << ")";
		ost << "\"," << std::endl;
		ost << "\"jqXHR\": \"\"" << std::endl;
		ost << "}]" << std::endl;
		indexPage.RegisterVariableForce("result", ost.str());

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
		if(!indexPage.SetTemplateFile("templates/error.htmlt"))
		{
			return(-1);
		}

		MESSAGE_ERROR("", "", "catch CException: exception: " + c.GetReason());

		indexPage.RegisterVariable("content", c.GetReason());
		indexPage.OutTemplate();
		return(-1);
	}

	return(0);
}

