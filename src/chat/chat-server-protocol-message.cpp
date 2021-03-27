#include "chat-server-protocol-message.h"

/* message protocol */
extern	CMysql						db;
extern	double						dbConnectionInitTimestamp;
extern  CPresenceCache				presenceCache;
		CChatRequestRatelimiter		requestRatelimiter; 
struct per_session_data__message	*connectionsList = NULL;

inline bool isFileExists(const std::string& name) {
	struct stat buffer;
	return (stat (name.c_str(), &buffer) == 0);
}

bool ImageSaveAsJpg (const string src, const string dst, struct ExifInfo *exifInfo = NULL)
{
	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream   ost;

		ost.str("");
		ost << __func__ << "[" << __LINE__ << "]:(" << src << ", " << dst << "): start";
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

{
	CLog	log(CHAT_LOG_FILE_NAME);
	ostringstream   ost;

	ost.str("");
	ost << __func__ << "[" << __LINE__ << "]:(" << src << ", " << dst << "): ";
	log.Write(DEBUG, ost.str());
}

		// Read a file into image object
		image.read( src );

{
	CLog	log(CHAT_LOG_FILE_NAME);
	ostringstream   ost;

	ost.str("");
	ost << __func__ << "[" << __LINE__ << "]:(" << src << ", " << dst << "): ";
	log.Write(DEBUG, ost.str());
}

		imageGeometry = image.size();
{
	CLog	log(CHAT_LOG_FILE_NAME);
	ostringstream   ost;

	ost.str("");
	ost << __func__ << "[" << __LINE__ << "]:(" << src << ", " << dst << "): ";
	log.Write(DEBUG, ost.str());
}

		imageOrientation = image.orientation();

		{
			CLog	log(CHAT_LOG_FILE_NAME);
			ostringstream   ost;

			ost.str("");
			ost << __func__ << "[" << __LINE__ << "]:(" << src << ", " << dst << "): imageOrientation = " << imageOrientation << ", xRes = " << imageGeometry.width() << ", yRes = " << imageGeometry.height();
			log.Write(DEBUG, ost.str());
		}

		if((imageGeometry.width() > CHAT_IMAGE_MAX_WIDTH) || (imageGeometry.height() > CHAT_IMAGE_MAX_HEIGHT))
		{
			int   newHeight, newWidth;
			if(imageGeometry.width() >= imageGeometry.height())
			{
				newWidth = CHAT_IMAGE_MAX_WIDTH;
				newHeight = newWidth * imageGeometry.height() / imageGeometry.width();
			}
			else
			{
				newHeight = CHAT_IMAGE_MAX_HEIGHT;
				newWidth = newHeight * imageGeometry.width() / imageGeometry.height();
			}

			image.resize(Magick::Geometry(newWidth, newHeight, 0, 0));
		}

		// --- save exifInfo, if exists
		if(exifInfo)
		{
			if(imageOrientation == Magick::TopRightOrientation) image.flop();
			if(imageOrientation == Magick::BottomRightOrientation) image.rotate(180);
			if(imageOrientation == Magick::BottomLeftOrientation) { image.flop(); image.rotate(180); }
			if(imageOrientation == Magick::LeftTopOrientation) { image.flop(); image.rotate(-90); }
			if(imageOrientation == Magick::RightTopOrientation) image.rotate(90);
			if(imageOrientation == Magick::RightBottomOrientation) { image.flop(); image.rotate(90); }
			if(imageOrientation == Magick::LeftBottomOrientation) image.rotate(-90);

			// --- Exif information save
			exifInfo->DateTime = "";
			exifInfo->DateTime = image.attribute("exif:DateTime");
			exifInfo->GPSAltitude = "";
			exifInfo->GPSAltitude = image.attribute("exif:GPSAltitude");
			exifInfo->GPSLatitude = "";
			exifInfo->GPSLatitude = image.attribute("exif:GPSLatitude");
			exifInfo->GPSLongitude = "";
			exifInfo->GPSLongitude = image.attribute("exif:GPSLongitude");
			exifInfo->GPSSpeed = "";
			exifInfo->GPSSpeed = image.attribute("exif:GPSSpeed");
			exifInfo->Model = "";
			exifInfo->Model = image.attribute("exif:Model");
			exifInfo->Authors = "";
			exifInfo->Authors = image.attribute("exif:Authors");
			exifInfo->ApertureValue = "";
			exifInfo->ApertureValue = image.attribute("exif:ApertureValue");
			exifInfo->BrightnessValue = "";
			exifInfo->BrightnessValue = image.attribute("exif:BrightnessValue");
			exifInfo->ColorSpace = "";
			exifInfo->ColorSpace = image.attribute("exif:ColorSpace");
			exifInfo->ComponentsConfiguration = "";
			exifInfo->ComponentsConfiguration = image.attribute("exif:ComponentsConfiguration");
			exifInfo->Compression = "";
			exifInfo->Compression = image.attribute("exif:Compression");
			exifInfo->DateTimeDigitized = "";
			exifInfo->DateTimeDigitized = image.attribute("exif:DateTimeDigitized");
			exifInfo->DateTimeOriginal = "";
			exifInfo->DateTimeOriginal = image.attribute("exif:DateTimeOriginal");
			exifInfo->ExifImageLength = "";
			exifInfo->ExifImageLength = image.attribute("exif:ExifImageLength");
			exifInfo->ExifImageWidth = "";
			exifInfo->ExifImageWidth = image.attribute("exif:ExifImageWidth");
			exifInfo->ExifOffset = "";
			exifInfo->ExifOffset = image.attribute("exif:ExifOffset");
			exifInfo->ExifVersion = "";
			exifInfo->ExifVersion = image.attribute("exif:ExifVersion");
			exifInfo->ExposureBiasValue = "";
			exifInfo->ExposureBiasValue = image.attribute("exif:ExposureBiasValue");
			exifInfo->ExposureMode = "";
			exifInfo->ExposureMode = image.attribute("exif:ExposureMode");
			exifInfo->ExposureProgram = "";
			exifInfo->ExposureProgram = image.attribute("exif:ExposureProgram");
			exifInfo->ExposureTime = "";
			exifInfo->ExposureTime = image.attribute("exif:ExposureTime");
			exifInfo->Flash = "";
			exifInfo->Flash = image.attribute("exif:Flash");
			exifInfo->FlashPixVersion = "";
			exifInfo->FlashPixVersion = image.attribute("exif:FlashPixVersion");
			exifInfo->FNumber = "";
			exifInfo->FNumber = image.attribute("exif:FNumber");
			exifInfo->FocalLength = "";
			exifInfo->FocalLength = image.attribute("exif:FocalLength");
			exifInfo->FocalLengthIn35mmFilm = "";
			exifInfo->FocalLengthIn35mmFilm = image.attribute("exif:FocalLengthIn35mmFilm");
			exifInfo->GPSDateStamp = "";
			exifInfo->GPSDateStamp = image.attribute("exif:GPSDateStamp");
			exifInfo->GPSDestBearing = "";
			exifInfo->GPSDestBearing = image.attribute("exif:GPSDestBearing");
			exifInfo->GPSDestBearingRef = "";
			exifInfo->GPSDestBearingRef = image.attribute("exif:GPSDestBearingRef");
			exifInfo->GPSImgDirection = "";
			exifInfo->GPSImgDirection = image.attribute("exif:GPSImgDirection");
			exifInfo->GPSImgDirectionRef = "";
			exifInfo->GPSImgDirectionRef = image.attribute("exif:GPSImgDirectionRef");
			exifInfo->GPSInfo = "";
			exifInfo->GPSInfo = image.attribute("exif:GPSInfo");
			exifInfo->GPSTimeStamp = "";
			exifInfo->GPSTimeStamp = image.attribute("exif:GPSTimeStamp");
			exifInfo->ISOSpeedRatings = "";
			exifInfo->ISOSpeedRatings = image.attribute("exif:ISOSpeedRatings");
			exifInfo->JPEGInterchangeFormat = "";
			exifInfo->JPEGInterchangeFormat = image.attribute("exif:JPEGInterchangeFormat");
			exifInfo->JPEGInterchangeFormatLength = "";
			exifInfo->JPEGInterchangeFormatLength = image.attribute("exif:JPEGInterchangeFormatLength");
			exifInfo->Make = "";
			exifInfo->Make = image.attribute("exif:Make");
			exifInfo->MeteringMode = "";
			exifInfo->MeteringMode = image.attribute("exif:MeteringMode");
			exifInfo->Orientation = "";
			exifInfo->Orientation = image.attribute("exif:Orientation");
			exifInfo->ResolutionUnit = "";
			exifInfo->ResolutionUnit = image.attribute("exif:ResolutionUnit");
			exifInfo->SceneCaptureType = "";
			exifInfo->SceneCaptureType = image.attribute("exif:SceneCaptureType");
			exifInfo->SceneType = "";
			exifInfo->SceneType = image.attribute("exif:SceneType");
			exifInfo->SensingMethod = "";
			exifInfo->SensingMethod = image.attribute("exif:SensingMethod");
			exifInfo->ShutterSpeedValue = "";
			exifInfo->ShutterSpeedValue = image.attribute("exif:ShutterSpeedValue");
			exifInfo->Software = "";
			exifInfo->Software = image.attribute("exif:Software");
			exifInfo->SubjectArea = "";
			exifInfo->SubjectArea = image.attribute("exif:SubjectArea");
			exifInfo->SubSecTimeDigitized = "";
			exifInfo->SubSecTimeDigitized = image.attribute("exif:SubSecTimeDigitized");
			exifInfo->SubSecTimeOriginal = "";
			exifInfo->SubSecTimeOriginal = image.attribute("exif:SubSecTimeOriginal");
			exifInfo->WhiteBalance = "";
			exifInfo->WhiteBalance = image.attribute("exif:WhiteBalance");
			exifInfo->XResolution = "";
			exifInfo->XResolution = image.attribute("exif:XResolution");
			exifInfo->YCbCrPositioning = "";
			exifInfo->YCbCrPositioning = image.attribute("exif:YCbCrPositioning");
			exifInfo->YResolution = "";
			exifInfo->YResolution = image.attribute("exif:YResolution");

			exifInfo->GPSAltitude = image.attribute("exif:GPSAltitudeRef") + ": " + exifInfo->GPSAltitude;
			exifInfo->GPSLatitude = image.attribute("exif:GPSLatitudeRef") + ": " + exifInfo->GPSLatitude;
			exifInfo->GPSLongitude = image.attribute("exif:GPSLongitudeRef") + ": " + exifInfo->GPSLongitude;
			exifInfo->GPSSpeed = image.attribute("exif:GPSSpeedRef") + ": " + exifInfo->GPSSpeed;

			image.strip();
		}

		// Write the image to a file
		image.write( dst );
	}
	catch( Magick::Exception &error_ )
	{
		{
			CLog	log(CHAT_LOG_FILE_NAME);
			ostringstream   ost;

			ost.str("");
			ost << __func__ << "[" << __LINE__ << "]:(" << src << ", " << dst << "): exception in read/write operation [" << error_.what() << "]";
			log.Write(DEBUG, ost.str());
		}
		return false;
	}
	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream   ost;

		ost.str("");
		ost << __func__ << "[" << __LINE__ << " (" << src << ", " << dst << "): image has been successfully converted to .jpg format";
		log.Write(DEBUG, ost.str());
	}
	return true;
#else
	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream   ost;

		ost.str("");
		ost << __func__ << "[" << __LINE__ << "]:(" << src << ", " << dst << "): simple file coping cause ImageMagick++ is not activated";
		log.Write(DEBUG, ost.str());
	}
	CopyFile(src, dst);
	return  true;
#endif

	{
		CLog	log(CHAT_LOG_FILE_NAME);
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:(" + string(src) + ", " + string(dst) + "): end");
	}
}

// --- saving blob to /tmp/tmp_(prefix).jpg file
//	 Input: void *content, uint size
//	 Output: (prefix)
string SaveAndCheckPreImageToTmpLocation(const string &fileContent)
{
	// FILE			*f;
	int				folderID = (int)(rand()/(RAND_MAX + 1.0) * FEEDIMAGE_NUMBER_OF_FOLDERS) + 1;
	string			filePrefix = GetRandom(20);
	string			finalFile, tmpFile2Check, tmpImageJPG, fileName, fileExtension;
	string			httpImageLocation = "";
	size_t			base64Prefix = 0;
	string			result = "";

	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost << __func__ << "[" << __LINE__ << "]: start (size of file to be saved is " << fileContent.length() << ")";
		log.Write(DEBUG, ost.str());
	}


	//--- check image file existing
	do
	{
		ostringstream	ost;
		// string		  tmp;
		// std::size_t  	foundPos;

		folderID = (int)(rand()/(RAND_MAX + 1.0) * CHAT_IMAGE_NUMBER_OF_FOLDERS) + 1;
		filePrefix = GetRandom(20);
		fileExtension = ".jpg";

		ost.str("");
		ost << IMAGE_CHAT_DIRECTORY << "/" << folderID << "/" << filePrefix << ".jpg";
		finalFile = ost.str();

		ost.str("");
		ost << "/tmp/tmp_" << filePrefix << fileExtension;
		tmpFile2Check = ost.str();

		ost.str("");
		ost << "/tmp/" << filePrefix << ".jpg";
		tmpImageJPG = ost.str();

		httpImageLocation = to_string(folderID) + "/" + filePrefix + ".jpg";

	} while(isFileExists(finalFile) || isFileExists(tmpFile2Check) || isFileExists(tmpImageJPG));

	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << __func__ << "[" << __LINE__ << "]: Save file to /tmp for checking of image validity [" << tmpFile2Check << "]";
		log.Write(DEBUG, ost.str());
	}

	// --- strip the header "data:image/XXXX;base64,"
	base64Prefix = fileContent.find(";base64,");
	if((base64Prefix != string::npos) && ((base64Prefix + strlen(";base64,")) < fileContent.length()))
	{
		// --- Save file to "/tmp/" for checking of image validity
		ofstream	ofs;
		ofs.open(tmpFile2Check, ofstream::out);
		ofs << base64_decode(fileContent.substr(base64Prefix + strlen(";base64,")));
		ofs.close();

		if(ImageSaveAsJpg(tmpFile2Check, tmpImageJPG))
		{
			CopyFile(tmpImageJPG, finalFile);

			// --- Delete temporarily files
			unlink(tmpFile2Check.c_str());
			unlink(tmpImageJPG.c_str());

			result = httpImageLocation;
		}
		else
		{
			{
				ostringstream   ost;
				CLog			log(CHAT_LOG_FILE_NAME);

				ost.str("");
				ost << __func__ << "[" << __LINE__ << "]:ERROR: image [" << tmpFile2Check << "] is not valid format (looks like attack)";
				log.Write(ERROR, ost.str());
			}

			result = "";
		}
	}
	else
	{
		{	
			CLog	log(CHAT_LOG_FILE_NAME);
			ostringstream	ost;
	
			ost << __func__ << "[" << __LINE__ << "]: received file having problem with base64 encoding (file length " << fileContent.length() << " bytes)";
			log.Write(DEBUG, ost.str());
		}

		result = "";
	}

	{
		CLog			log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost << __func__ << "[" << __LINE__ << "]: end (file saved to :" << tmpFile2Check << ")";
		log.Write(DEBUG, ost.str());
	}

	return result;
}


int	GetConnectionsNumber()
{
	struct per_session_data__message *connectionIterator = connectionsList;
	int			i = 0;
 
	i = 0; 
	while(connectionIterator)
	{
		connectionIterator = connectionIterator->nextConnection;
		i++;
	}
	return i;
}

bool	CloseSingleTextMessageConnection(struct per_session_data__message *connection)
{
	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << string(__func__) + "[" + to_string(__LINE__) + "]: begin (user = " << (connection->user ? connection->user->GetID() : "no-user") << ")";
		log.Write(DEBUG, ost.str());
	}

	for(int i = 0; i < CHAT_MAX_MESSAGE_QUEUE; i++)
		if(connection->messageList[i])
		{
			delete connection->messageList[i];
		}

	if(connection->user) delete connection->user;
	if(connection->packetReassemble) delete connection->packetReassemble;


	{
		CLog	log(CHAT_LOG_FILE_NAME);
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) +  "]: finish");
	}
	return	true;
}

bool	CloseAllTextMessageConnections()
{
	struct per_session_data__message *connectionIterator = connectionsList;
	int			i = 0;

	{
		CLog	log(CHAT_LOG_FILE_NAME);
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) +  "]: start");
	}

	i = 0; 
	while(connectionIterator)
	{
 
		lws_close_reason(connectionIterator->wsi, LWS_CLOSE_STATUS_GOINGAWAY,
				 (unsigned char *)"server shutdown", 15);

		connectionIterator = connectionIterator->nextConnection;
		i++;
	}

	{
		CLog			log(CHAT_LOG_FILE_NAME);
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) +  "]: finish (closed " + to_string(i) + " connections)");
	}

	return true;
}

int GetLengthWriteQueue(struct per_session_data__message *pss)
{
	int		messageQueueLength = 0;
	{
		CLog			log(CHAT_LOG_FILE_NAME);
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) +  "]: start(to user [" + (pss->user ? pss->user->GetID() : "no-user") + "])");
	}
	
	messageQueueLength = (pss->ringBufferHead - pss->ringBufferTail) & (CHAT_MAX_MESSAGE_QUEUE);

	{
		CLog			log(CHAT_LOG_FILE_NAME);
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) +  "]: finish (to user [" + (pss->user ? pss->user->GetID() : "no-user") + "], queue message len [" + to_string(messageQueueLength) + "])");
	}

	return messageQueueLength;
}

string GetMessageInJSONFormat(CSingleMessage *messageObj)
{
	return "{"
				"\"RequestType\":\"" + messageObj->GetRequestType() + "\","
				"\"RequestID\":\"" + messageObj->GetRequestID() + "\","
				"\"id\":\"" + messageObj->GetID() + "\","
				"\"toType\":\"" + messageObj->GetToType() + "\","
				"\"toID\":\"" + messageObj->GetToID() + "\","
				"\"fromType\":\"" + messageObj->GetFromType() + "\","
				"\"fromID\":\"" + messageObj->GetFromID() + "\","
				"\"messageStatus\":\"" + messageObj->GetMessageStatus() + "\","
				"\"eventTimestamp\":\"" + messageObj->GetEventTimestamp() + "\","
				"\"eventTimestampDelta\":\"" + GetHumanReadableTimeDifferenceFromNow(messageObj->GetEventTimestamp()) + "\","
				"\"secondsSinceY2k\":\"" + to_string(messageObj->GetSecondsSince2000()) + "\","
				"\"messageType\":\"" + messageObj->GetMessageType() + "\","
				"\"message\":\"" + messageObj->GetMessage() + 
			"\"}";
}

string GetSimpleTextInJSONFormat(CSingleMessage *messageObj)
{
	return "{"
				"\"RequestType\":\"" + messageObj->GetRequestType() + "\","
				"\"RequestID\":\"" + messageObj->GetRequestID() + "\","
				"\"id\":\"" + messageObj->GetID() + "\","
				"\"toType\":\"" + messageObj->GetToType() + "\","
				"\"toID\":\"" + messageObj->GetToID() + "\","
				"\"fromType\":\"" + messageObj->GetFromType() + "\","
				"\"fromID\":\"" + messageObj->GetFromID() + "\","
				"\"messageStatus\":\"" + messageObj->GetMessageStatus() + "\","
				"\"eventTimestamp\":\"" + messageObj->GetEventTimestamp() + "\","
				"\"eventTimestampDelta\":\"" + GetHumanReadableTimeDifferenceFromNow(messageObj->GetEventTimestamp()) + "\","
				"\"secondsSinceY2k\":\"" + to_string(messageObj->GetSecondsSince2000()) + "\","
				+ messageObj->GetMessage() + 
			"}";
}

string GetStringToWrite(struct per_session_data__message *pss)
{
	string	result = "";

	{
		CLog			log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << string(__func__) + "[" + to_string(__LINE__) + "]: start (to user [" << (pss->user ? pss->user->GetID() : "no-user") << "])";
		log.Write(DEBUG, ost.str());
	}
	
	if(pss->messageList[pss->ringBufferTail])
	{
		if(pss->messageList[pss->ringBufferTail]->GetRequestType() == "SendMessage")
		{
			result = GetMessageInJSONFormat(pss->messageList[pss->ringBufferTail]);
			{
				CLog			log(CHAT_LOG_FILE_NAME);
				ostringstream	ost;

				ost.str("");
				ost << string(__func__) + "[" + to_string(__LINE__) + "]: SendMessage: string (" << result << ")";
				log.Write(DEBUG, ost.str());
			}
		}
		else if(pss->messageList[pss->ringBufferTail]->GetRequestType() == "SendImage")
		{
			result = GetMessageInJSONFormat(pss->messageList[pss->ringBufferTail]);
			{
				CLog			log(CHAT_LOG_FILE_NAME);
				ostringstream	ost;

				ost.str("");
				ost << string(__func__) + "[" + to_string(__LINE__) + "]: SendImage: string (" << result << ")";
				log.Write(DEBUG, ost.str());
			}
		}
		else if(pss->messageList[pss->ringBufferTail]->GetRequestType() == "OpenSession")
		{
			result = GetSimpleTextInJSONFormat(pss->messageList[pss->ringBufferTail]);
			{
				CLog			log(CHAT_LOG_FILE_NAME);
				ostringstream	ost;

				ost.str("");
				ost << string(__func__) + "[" + to_string(__LINE__) + "]: OpenSession: string (" << result << ")";
				log.Write(DEBUG, ost.str());
			}
		}
		else if(pss->messageList[pss->ringBufferTail]->GetRequestType() == "GetInitialData")
		{
			result = GetSimpleTextInJSONFormat(pss->messageList[pss->ringBufferTail]);
			{
				CLog			log(CHAT_LOG_FILE_NAME);
				ostringstream	ost;

				ost.str("");
				ost << string(__func__) + "[" + to_string(__LINE__) + "]: GetInitialData: string (" << (result.length() > LOG_FILE_MAX_LENGTH ? (result.substr(0, LOG_FILE_MAX_LENGTH) + " ... (stripped due to long output)") : result) << ")";
				log.Write(DEBUG, ost.str());
			}
		}
		else if(pss->messageList[pss->ringBufferTail]->GetRequestType() == "GetMessageBlock")
		{
			result = GetSimpleTextInJSONFormat(pss->messageList[pss->ringBufferTail]);
			{
				CLog			log(CHAT_LOG_FILE_NAME);
				ostringstream	ost;

				ost.str("");
				ost << string(__func__) + "[" + to_string(__LINE__) + "]: GetMessageBlock: string (" << result << ")";
				log.Write(DEBUG, ost.str());
			}
		}
		else if(pss->messageList[pss->ringBufferTail]->GetRequestType() == "ChangeMessageStatus")
		{
			result = GetSimpleTextInJSONFormat(pss->messageList[pss->ringBufferTail]);
			{
				CLog			log(CHAT_LOG_FILE_NAME);
				ostringstream	ost;

				ost.str("");
				ost << string(__func__) + "[" + to_string(__LINE__) + "]: ChangeMessageStatus: string (" << result << ")";
				log.Write(DEBUG, ost.str());
			}
		}
		else if(pss->messageList[pss->ringBufferTail]->GetRequestType() == "PresenceUpdate")
		{
			result = GetSimpleTextInJSONFormat(pss->messageList[pss->ringBufferTail]);
			{
				CLog			log(CHAT_LOG_FILE_NAME);
				ostringstream	ost;

				ost.str("");
				ost << string(__func__) + "[" + to_string(__LINE__) + "]: PresenceUpdate: string (" << result << ")";
				log.Write(DEBUG, ost.str());
			}
		}
		else if(pss->messageList[pss->ringBufferTail]->GetRequestType() == "MessageTypingNotification")
		{
			result = GetSimpleTextInJSONFormat(pss->messageList[pss->ringBufferTail]);
			{
				CLog			log(CHAT_LOG_FILE_NAME);
				ostringstream	ost;

				ost.str("");
				ost << string(__func__) + "[" + to_string(__LINE__) + "]: MessageTypingNotification: string (" << result << ")";
				log.Write(DEBUG, ost.str());
			}
		}
		else
		{
			{
				CLog			log(CHAT_LOG_FILE_NAME);
				ostringstream	ost;

				ost.str("");
				ost << string(__func__) + "[" + to_string(__LINE__) + "]: ERROR: unsupported RequestType [" << pss->messageList[pss->ringBufferTail]->GetRequestType() << "]";
				log.Write(ERROR, ost.str());
			}
		}
	}
	else
	{
		result = "";
		// pss->ringBufferTail = (pss->ringBufferTail + 1) & (CHAT_MAX_MESSAGE_QUEUE);
		{
			CLog			log(CHAT_LOG_FILE_NAME);
			ostringstream	ost;

			ost.str("");
			ost << string(__func__) + "[" + to_string(__LINE__) + "]: ERROR: message in messageList = NULL";
			log.Write(ERROR, ost.str());
		}
	}

	{
		CLog			log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << string(__func__) + "[" + to_string(__LINE__) + "]: end (to user [" << (pss->user ? pss->user->GetID() : "no-user") << "])";
		log.Write(DEBUG, ost.str());
	}

	return result;
}

bool PopSingleMessageFromUserQueue(struct per_session_data__message *pss)
{
	{
		CLog			log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << string(__func__) + "[" + to_string(__LINE__) + "]: start";
		log.Write(DEBUG, ost.str());
	}
	if(pss->messageList[pss->ringBufferTail])
	{
		delete pss->messageList[pss->ringBufferTail];
		pss->messageList[pss->ringBufferTail] = NULL;
		pss->ringBufferTail = (pss->ringBufferTail + 1) & (CHAT_MAX_MESSAGE_QUEUE);
	}
	{
		CLog			log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << string(__func__) + "[" + to_string(__LINE__) + "]: finish";
		log.Write(DEBUG, ost.str());
	}

	return true;	
}

bool PushSingleMessageToUserQueue(CSingleMessage *singleMessage, struct per_session_data__message *pss)
{
	bool			result = false;
	unsigned int	queueSize;

	{
		CLog			log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << __func__ << "[" << __LINE__ << "]: start (RequestID [" << singleMessage->GetRequestID() << "], message queue length [" << ((pss->ringBufferHead - pss->ringBufferTail) & (CHAT_MAX_MESSAGE_QUEUE)) << "], user [" << (pss->user ? pss->user->GetID() : "no-user") << "])";
		log.Write(DEBUG, ost.str());
	}

	queueSize = ((pss->ringBufferHead - pss->ringBufferTail) & (CHAT_MAX_MESSAGE_QUEUE));
	if(queueSize < (CHAT_MAX_MESSAGE_QUEUE - 1))
	{
		try
		{
			CSingleMessage	*tempSingleMessage = new CSingleMessage;

			*tempSingleMessage = *singleMessage;
			pss->messageList[pss->ringBufferHead] = tempSingleMessage;
			pss->ringBufferHead = (pss->ringBufferHead + 1) & (CHAT_MAX_MESSAGE_QUEUE);
			lws_callback_on_writable(pss->wsi);
			
			result = true;
		}
		catch (std::bad_alloc& ba)
		{
			// --- if you get here, you are in trouble. 
			// --- highly possible memory leak.
			result = false;

			{
				CLog			log(CHAT_LOG_FILE_NAME);
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: bad alloc exception caught [" + ba.what() + "]");
			}
		} // --- try / catch

	}
	else
	{
		lws_rx_flow_control(pss->wsi, 0); // --- libwebsocket throttling enabled, receiving from socket prohibited
		{
			CLog			log(CHAT_LOG_FILE_NAME);
			ostringstream	ost;

			ost.str("");
			ost << __func__ << "[" << __LINE__ << "]:ERROR: drop due to queue overflow (queue size = " << queueSize << "), throttling enabled";
			log.Write(ERROR, ost.str());
		}
	}
	
	{
		CLog			log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << __func__ << "[" << __LINE__ << "]: end (result = " << result << ")";
		log.Write(DEBUG, ost.str());
	}

	return result;
}

bool ReplicateMessageToAllConnectionsSrcUser(CSingleMessage *singleMessage)
{
	bool								result = true;
	struct per_session_data__message 	*connectionIterator = connectionsList;
	int									i;

	{
		CLog			log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << __func__ << "[" << __LINE__ << "]:userID[" << singleMessage->GetFromID() << "]): start";
		log.Write(DEBUG, ost.str());
	}

	i = 0; 
	while(connectionIterator)
	{
		if(connectionIterator->user)
		{

	 		if(singleMessage->GetFromID() == connectionIterator->user->GetID()) 
			{
				if(PushSingleMessageToUserQueue(singleMessage, connectionIterator))
				{

				}
				else
				{
					{
						CLog			log(CHAT_LOG_FILE_NAME);
						ostringstream	ost;

						ost.str("");
						ost << __func__ << "[" << __LINE__ << "]: ERROR can't replicate message to user [" << singleMessage->GetFromID() << "] connection";
						log.Write(ERROR, ost.str());
					} // --- CLog scoping

				} // --- if(PushSingleMessageToUserQueue)
			} // --- if(singleMessage->GetFromID() == connectionIterator->user->GetID())
		}
		else
		{
			CLog			log(CHAT_LOG_FILE_NAME);
			string			remote_ip = getenv("REMOTE_ADDR");
			struct timeval	stm;

			gettimeofday(&stm, NULL);

			log.Write(ERROR, "cnx without user assigned, might be user do not OpenSession yet, if cnx will be alive long it must be dropped (lifetime " + to_string(stm.tv_sec - connectionIterator->tv_established.tv_sec) + " sec, IP " + remote_ip + ")");
		} // --- if(connectionIterator->user)

		connectionIterator = connectionIterator->nextConnection;
		i++;
	}


	{
		CLog			log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << __func__ << "[" << __LINE__ << "]: end (" << (result ? "true" : "false") << ")";
		log.Write(DEBUG, ost.str());
	}

	return result;
}

bool ReplicateMessageToAllConnectionsDstUser(CSingleMessage *singleMessage)
{
	bool								result = true;
	struct per_session_data__message 	*connectionIterator = connectionsList;
	int									i;

	{
		CLog			log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << __func__ << "(userID: " << singleMessage->GetFromID() << ")[" << __LINE__ << "]: start";
		log.Write(DEBUG, ost.str());
	}

	i = 0; 
	while(connectionIterator)
	{
		if(connectionIterator->user)
		{

	 		if(singleMessage->GetToID() == connectionIterator->user->GetID()) 
			{
				if(PushSingleMessageToUserQueue(singleMessage, connectionIterator))
				{

				}
				else
				{
					{
						CLog			log(CHAT_LOG_FILE_NAME);
						ostringstream	ost;

						ost.str("");
						ost << __func__ << "[" << __LINE__ << "]: ERROR can't replicate message to user [" << singleMessage->GetFromID() << "] connection";
						log.Write(ERROR, ost.str());
					} // --- CLog scoping

				} // --- if(PushSingleMessageToUserQueue(singleMessage, connectionIterator))
			} // --- if message_recipient == destination recipient
		}
		else
		{
			CLog			log(CHAT_LOG_FILE_NAME);
			string			remote_ip = getenv("REMOTE_ADDR");
			struct timeval	stm;

			gettimeofday(&stm, NULL);

			log.Write(ERROR, "cnx without user assigned, might be user do not OpenSession yet, if cnx will be alive long it must be dropped (lifetime " + to_string(stm.tv_sec - connectionIterator->tv_established.tv_sec) + " sec, IP " + remote_ip + ")");
		} // --- if(connectionIterator->user)

		connectionIterator = connectionIterator->nextConnection;
		i++;
	}


	{
		CLog			log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << __func__ << "[" << __LINE__ << "]: end (" << (result ? "true" : "false") << ")";
		log.Write(DEBUG, ost.str());
	}

	return result;
}

// "toUser", inputParams.isNameExists("toID"), "fromUser", pss->user->GetID(), inputParams.isNameExists("msg")
string SubmitMessageToDB(string toType, string toID, string fromType, string fromID, string messageUTF8, string messageStatus, string messageType)
{
	string			result = "";
	ostringstream	ost;
	unsigned long	messageID;

	{
		CLog			log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << __func__ << "[" << __LINE__ << "]: start";
		log.Write(DEBUG, ost.str());
	}
	
	if(messageType == MESSAGE_TYPE_TEXT)
	{
		messageUTF8 = CleanUPText(messageUTF8, KEEP_BR);
	}
	else if(messageType == MESSAGE_TYPE_IMAGE)
	{
	}

	ost.str("");
	ost << "INSERT INTO `chat_messages` (`messageType`, `message`, `fromType`, `fromID`, `toType`, `toID`, `messageStatus`, `eventTimestamp`, `secondsSinceY2k`) \
			VALUES (\"" << messageType << "\",\"" << messageUTF8 << "\",\"" << fromType << "\", \"" << fromID << "\", \"" << toType << "\",\"" << toID << "\", \"" << messageStatus << "\", NOW(), \"" << to_string((double)GetSecondsSinceY2k()) << "\")";
	messageID = db.InsertQueryDB(ost.str());
	if(messageID == 0)
	{
		CLog			log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << __func__ << "[" << __LINE__ << "]: ERROR: message has not been saved in DB";
		log.Write(DEBUG, ost.str());
	}
	else
	{
		ost.str("");
		ost << messageID;
		result = ost.str();
	}

	{
		CLog			log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << __func__ << "[" << __LINE__ << "]: end (messageID: " << result << ")";
		log.Write(DEBUG, ost.str());
	}

	return result;
}

auto GetChatMessagesInJSONFormat(string dbQuery, CMysql *db)
{
	{
		CLog	log(CHAT_LOG_FILE_NAME);
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: start");
	}

	ostringstream	result, ost;
	auto			affected = db->Query(dbQuery);

	result.str("");

	if(affected)
	{
		for(auto i = 0; i < affected; i++)
		{
			result << (i ? "," : "") << "{"
				"\"id\": \""					<< db->Get(i, "id") << "\", "
				"\"message\": \"" 				<< ReplaceDoubleQuoteToQuote(db->Get(i, "message")) << "\", "
				"\"fromType\": \"" 				<< db->Get(i, "fromType") << "\","
				"\"fromID\": \""				<< db->Get(i, "fromID") << "\","
				"\"toType\": \""			 	<< db->Get(i, "toType") << "\","
				"\"toID\": \""	 				<< db->Get(i, "toID") << "\","
				"\"messageStatus\": \""		 	<< db->Get(i, "messageStatus") << "\","
				"\"messageType\": \""			<< db->Get(i, "messageType") << "\","
				"\"eventTimestampDelta\": \""	<< GetHumanReadableTimeDifferenceFromNow(db->Get(i, "eventTimestamp")) << "\","
				"\"secondsSinceY2k\": \""		<< db->Get(i, "secondsSinceY2k") << "\","
				"\"eventTimestamp\": \""		<< db->Get(i, "eventTimestamp") << "\""
			"}";
		}
	}
	
	{
		CLog	log(CHAT_LOG_FILE_NAME);
		log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + string("]: end"));
	}

	return  result.str();
}

string GetChatInitialData(struct per_session_data__message *pss, const string activeUserID)
{
	ostringstream	ost, ostFinal, friendsSqlQuery, chatMessageQuery;
	string			sessid, lookForKey, userArray = "", messageArray = "";

	{
		CLog			log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << string(__func__) + "[" + to_string(__LINE__) + "]: start (" << (pss->user ? pss->user->GetID() : "no-user") << ")";
		log.Write(DEBUG, ost.str());
	}

	if(pss->user)
	{
		// --- build friend list from three sources
		// --- 1) all direct friends (table `users_friends`)
		// --- 2) friends from `contact_list` (table `contact_list`)
		// --- 3) user from WebSocket parameter ActiveUserID
		unordered_map<long int, string>		userList;
		int									affected;


		friendsSqlQuery.str("");
		// --- 1) all direct friends (table `users_friends`)
		ost.str("");
		ost << "select `friendID` from `users_friends` where `userID`='" << pss->user->GetID() << "';";
		affected = db.Query(ost.str());
		if(affected)
		{
			for(int i = 0; i < affected; i++)
			{
				userList.emplace( stol(db.Get(i, "friendID")), "");
			}
		}
		// --- 2) friends from `contact_list` (table `contact_list`)
		ost.str("");
		ost << "select `friendID` from `chat_contact_list` where `userID`='" << pss->user->GetID() << "';";
		affected = db.Query(ost.str());
		if(affected)
		{
			for(int i = 0; i < affected; i++)
			{
				userList.emplace( stol(db.Get(i, "friendID")), "");
			}
		}

		// --- 3) user from WebSocket parameter ActiveUserID
		if(activeUserID.length() && stoul(activeUserID))
			if(userList.find(stoul(activeUserID)) == userList.end())
			{
				// --- add user from WebSocket param to contact_list
				userList.emplace( stol(activeUserID), "");

				ost.str("");
				ost << "INSERT INTO `chat_contact_list`(`userID`, `friendID`) VALUES (\"" << pss->user->GetID() << "\",\"" << activeUserID << "\"),(\"" << activeUserID << "\",\"" << pss->user->GetID() << "\");";
				db.Query(ost.str());
			}

		if(userList.size())
		{
			friendsSqlQuery << "select * from `users` where `isActivated`='Y' and `isblocked`='N' and `id` IN (";
/*
			for(int i = 0; i < affected; i++)
			{
				friendsSqlQuery << (i > 0 ? ", " : "") << db.Get(i, "friendID");
			}
*/
			string	tempString = "";
			for(auto &it : userList)
			{
				tempString += (tempString.length() ? ", " : "");
				tempString += to_string(it.first);
			}
			friendsSqlQuery << tempString << ");";

			{
				CLog	log(CHAT_LOG_FILE_NAME);
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: query for JSON prepared [", friendsSqlQuery.str(), "]");
			}
			// userArray = GetUserListInJSONFormat(friendsSqlQuery.str(), &db, pss->user);
			userArray = GetUsersNameAvatarInJSONFormat(friendsSqlQuery.str(), &db, pss->user);

			// --- get messages for friends users
			affected = db.Query(friendsSqlQuery.str());
			if(affected)
			{
				list<string>	friendList;
				for(int i = 0; i < affected; i++)
				{
					friendList.push_back(db.Get(i, "id"));
				}

				messageArray = "";
				for(auto it = friendList.begin(); it != friendList.end(); it++)
				{
					string 		tempString;
					long int	messageID30th = 0, messageIDLastUnread = 0;

					chatMessageQuery.str("");
					chatMessageQuery << "select `id` from `chat_messages` where \
					((`toID`  ='" << pss->user->GetID() << "') and (`fromID`='" << *it << "')) \
					or \
					((`toID`  ='" << *it << "') and (`fromID`='" << pss->user->GetID() << "')) ORDER BY  `chat_messages`.`id` DESC LIMIT " << (CHAT_MESSAGES_PER_PAGE - 1) << ", 1;";
					if(db.Query(chatMessageQuery.str()))
					{
						messageID30th = stol(db.Get(0, "id"));

						chatMessageQuery.str("");
						chatMessageQuery << "select `id` from `chat_messages` where \
						(((`toID`  ='" << pss->user->GetID() << "') and (`fromID`='" << *it << "')) \
						or \
						((`toID`  ='" << *it << "') and (`fromID`='" << pss->user->GetID() << "'))) \
						and \
						(`messageStatus`='unread' or `messageStatus`='sent' or `messageStatus`='delivered') \
						 ORDER BY  `chat_messages`.`id` ASC LIMIT 0, 1;";
						if(db.Query(chatMessageQuery.str()))
						{
							messageIDLastUnread = stol(db.Get(0, "id"));
						}
						else
						{
							// --- absolute value of messageIDLastUnread is _not_ important
							// --- it must be bigger, to avoid influencing
							messageIDLastUnread = messageID30th + 1;
						}

						if(messageID30th <= messageIDLastUnread)
						{
							chatMessageQuery.str("");
							chatMessageQuery << "select * from `chat_messages` where \
							((`toID`  ='" << pss->user->GetID() << "') and (`fromID`='" << *it << "')) \
							or \
							((`toID`  ='" << *it << "') and (`fromID`='" << pss->user->GetID() << "')) ORDER BY  `chat_messages`.`id` DESC LIMIT 0," << CHAT_MESSAGES_PER_PAGE << ";";
						}
						else
						{
							chatMessageQuery.str("");
							chatMessageQuery << "select * from `chat_messages` where \
							(((`toID`  ='" << pss->user->GetID() << "') and (`fromID`='" << *it << "')) \
							or \
							((`toID`  ='" << *it << "') and (`fromID`='" << pss->user->GetID() << "'))) \
							and \
							(`id`>='" << messageIDLastUnread << "') \
							 ORDER BY  `chat_messages`.`id` DESC;";
						}

					}
					else
					{
						chatMessageQuery.str("");
						chatMessageQuery << "select * from `chat_messages` where \
						((`toID`  ='" << pss->user->GetID() << "') and (`fromID`='" << *it << "')) \
						or \
						((`toID`  ='" << *it << "') and (`fromID`='" << pss->user->GetID() << "')) ORDER BY  `chat_messages`.`id` DESC LIMIT 0, " << CHAT_MESSAGES_PER_PAGE << ";";

					} // --- get 30-th messageID

					tempString = GetChatMessagesInJSONFormat(chatMessageQuery.str(), &db);
					messageArray += ((messageArray.length() && tempString.length()) ? "," : "") + tempString;
				}
			}


			ostFinal.str("");
			ostFinal << "\"result\": \"success\"," << std::endl;
			ostFinal << "\"userArray\": [" << userArray << "]," << std::endl;
			ostFinal << "\"messageArray\": [" << messageArray << "]";

		}
		else
		{
			
			{
				CLog			log(CHAT_LOG_FILE_NAME);
				ostringstream	ost;

				ost.str("");
				ost << string(__func__) + "[" + to_string(__LINE__) + "](): DEBUG: there is no friends on this account";
				log.Write(DEBUG, ost.str());
			}

			ostFinal.str("");
			ostFinal << "\"result\": \"fail\",";
			ostFinal << "\"description\": \"there is no friends on this account\",";
			ostFinal << "\"userArray\": [],";
			ostFinal << "\"messageArray\": []";
		} // --- if(user (pss->user->GetID()) found)
	}
	else
	{
		CLog			log(CHAT_LOG_FILE_NAME);
		string			remote_ip = getenv("REMOTE_ADDR");
		struct timeval	stm;

		gettimeofday(&stm, NULL);

		log.Write(ERROR, "cnx without user assigned, might be user do not OpenSession yet, if cnx will be alive long it must be dropped (lifetime " + to_string(stm.tv_sec - pss->tv_established.tv_sec) + " sec, IP " + remote_ip + ")");
	} // --- if(connectionIterator->user)

	{
		CLog	log(CHAT_LOG_FILE_NAME);
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: end");
	}

	return ostFinal.str();
}



string GetMessageBlock(string friendID, string minMessageID, struct per_session_data__message *pss)
{
	ostringstream	ostFinal, chatMessageQuery;
	string			messageArray = "";

	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << string(__func__) + "( friendID" << friendID << ", minMessageID = " << minMessageID << ", pss )[" + to_string(__LINE__) + "]: start";
		log.Write(DEBUG, ost.str());
	}

	if(pss->user)
	{
		string tempString;
		messageArray = "";

		chatMessageQuery.str("");
		chatMessageQuery << "select * from `chat_messages` where \
		(((`toID`  ='" << pss->user->GetID() << "') and (`fromID`='" << friendID << "')) \
			or \
		((`toID`  ='" << friendID << "') and (`fromID`='" << pss->user->GetID() << "'))) \
		and \
		(`id` < '" << minMessageID << "') \
		ORDER BY  `chat_messages`.`id` DESC LIMIT 0 , 30;";
		messageArray = GetChatMessagesInJSONFormat(chatMessageQuery.str(), &db);

		ostFinal.str("");
		ostFinal << "\"result\": \"success\",";
		ostFinal << "\"friendID\": \"" << friendID << "\",";
		ostFinal << "\"messageArray\": [" << messageArray << "]";
	}
	else
	{
		CLog			log(CHAT_LOG_FILE_NAME);
		string			remote_ip = getenv("REMOTE_ADDR");
		struct timeval	stm;

		gettimeofday(&stm, NULL);

		log.Write(ERROR, "cnx without user assigned, might be user do not OpenSession yet, if cnx will be alive long it must be dropped (lifetime " + to_string(stm.tv_sec - pss->tv_established.tv_sec) + " sec, IP " + remote_ip + ")");
	} // --- if(connectionIterator->user)


	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << __func__ << "[" << __LINE__ << "]: end (response.length() = " << ostFinal.str().length() << ")";
		log.Write(DEBUG, ost.str());
	}

	return ostFinal.str();
}

bool ChangeMessageStatusInDB(string messageID, string messageStatus, struct per_session_data__message *pss)
{
	bool		result = false;

	{
		CLog	log(CHAT_LOG_FILE_NAME);
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: start");
	}

	if(pss->user)
	{
		if(messageID.length() && messageStatus.length())
				{
					if((messageStatus == "unread") || (messageStatus == "read") || (messageStatus == "delivered") || (messageStatus == "sent") || (messageStatus == "unread_const"))
					{
						ostringstream	ost;
		
						ost.str("");
						ost << "select * from `chat_messages` where `id`='" << messageID << "';";
						if(db.Query(ost.str()))
						{
							if(db.Get(0, "toID") == pss->user->GetID())
							{
								result = true;
		
								ost.str("");
								ost << "UPDATE `chat_messages` SET `messageStatus`='" << messageStatus << "' WHERE `id`='" << messageID << "';";
								db.Query(ost.str());
							}
							else
							{
								{
									CLog	log(CHAT_LOG_FILE_NAME);
									ostringstream	ost;
		
									ost.str("");
									ost << string(__func__) + "[" + to_string(__LINE__) + "]: ERROR: user [" << pss->user->GetID() << "] can't update message status [" << messageID << "]. Only ["  << db.Get(0, "toID") << "] permitted to change status";
									log.Write(ERROR, ost.str());
								}
							}
						}
						else
						{
							{
								CLog	log(CHAT_LOG_FILE_NAME);
								log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: ERROR: messageID [", messageID, "] not found in DB");
							}
						}
		
					}
					else
					{
						{
							CLog	log(CHAT_LOG_FILE_NAME);
							log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: ERROR: messageStatus is wrong");
						}
					}
				}
				else
				{
					{
						CLog	log(CHAT_LOG_FILE_NAME);
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: ERROR: one of mandatory parameters empty");
					}
				}
	}
	else
	{
		CLog			log(CHAT_LOG_FILE_NAME);
		string			remote_ip = getenv("REMOTE_ADDR");
		struct timeval	stm;

		gettimeofday(&stm, NULL);
		log.Write(ERROR, "cnx without user assigned, might be user do not OpenSession yet, if cnx will be alive long it must be dropped (lifetime " + to_string(stm.tv_sec - pss->tv_established.tv_sec) + " sec, IP " + remote_ip + ")");
	} // --- if(connectionIterator->user)

	{
		CLog	log(CHAT_LOG_FILE_NAME);
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: end");
	}
	return result;
}

string GetFromDBByMessageID(string messageID)
{
	string		result = "";

	{
		CLog	log(CHAT_LOG_FILE_NAME);
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: start");
	}

	if(messageID.length())
	{
		ostringstream	ost;

		ost.str("");
		ost << "select * from `chat_messages` where `id`='" << messageID << "';";
		if(db.Query(ost.str()))
		{
			result = db.Get(0, "fromID");
		}
		else
		{
			{
				CLog	log(CHAT_LOG_FILE_NAME);
				log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: ERROR: messageID [", messageID, "] not found in DB");
			}
		}
	}
	else
	{
		{
			CLog	log(CHAT_LOG_FILE_NAME);
			log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: ERROR: one of mandatory parameters empty");
		}
	}
	
	{
		CLog	log(CHAT_LOG_FILE_NAME);
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: end");
	}
	return result;
}

bool FillinUserIDBySessID(string sessIDFromChat, struct per_session_data__message *pss)
{
	bool			result = true;
	ostringstream	ost;
	CCgi			indexPage;
	CUser			*userTemp;

	{
		CLog			log(CHAT_LOG_FILE_NAME);
		ostringstream   ost;

		ost.str("");
		ost << string(__func__) + "[" + to_string(__LINE__) + "](" << sessIDFromChat << ", *pss): begin";
		log.Write(DEBUG, ost.str());
	}

		indexPage.SetDB(&db);
		if(indexPage.SessID_Load_FromDB(sessIDFromChat)) 
		{
			if(indexPage.SessID_CheckConsistency()) 
			{
				if(indexPage.SessID_Get_UserFromDB() != "Guest") 
				{
					{
						CLog			log(CHAT_LOG_FILE_NAME);
						log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: before new CUser");
					}

					try
					{
						userTemp = new CUser();
						{
							CLog			log(CHAT_LOG_FILE_NAME);
							log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: after new CUser");
						}

						userTemp->SetDB(&db);
						// --- place 2change user from user.email to user.id 
						if(userTemp->GetFromDBbyLogin(indexPage.SessID_Get_UserFromDB()))
						{
							ostringstream   ost1;
							string		  avatarPath;

							pss->user = userTemp;

							{
								CLog	log(CHAT_LOG_FILE_NAME);
								ostringstream   ost;

								ost << string(__func__) + "[" + to_string(__LINE__) + "]: user [" << userTemp->GetLogin() << "] logged in";
								log.Write(DEBUG, ost.str());
							}
						}
						else
						{
							// --- enforce to close session
							result = false;

							{
								CLog	log(CHAT_LOG_FILE_NAME);
								ostringstream   ost;

								ost << string(__func__) + "[" + to_string(__LINE__) + "]: ERROR user [" << indexPage.SessID_Get_UserFromDB() << "] session exists on client device, but not in the DB. Change the [action = logout].";
								log.Write(ERROR, ost.str());
							}

						}
					}
					catch (std::bad_alloc& ba)
					{
						// --- if you get here, you are in trouble. 
						// --- highly possible memory leak.
						result = false;

						{
							CLog			log(CHAT_LOG_FILE_NAME);
							log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: bad alloc exception caught [" + ba.what() + "]");
						}
					} // --- try / catch
					catch (exception& e)
					{
						// --- if you get here, you are in trouble. 
						result = false;

						{
							CLog			log(CHAT_LOG_FILE_NAME);
							log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: standard exception caught [" + e.what() + "]");
						}
					}
				} // --- if(user != "Guest")
				else
				{
					result = false;

					{
						CLog			log(CHAT_LOG_FILE_NAME);
						log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: user Guest not allowed to get into chat");
					}
				} // --- if(user == "Guest")
			}
			else 
			{
				{
					CLog			log(CHAT_LOG_FILE_NAME);
					ostringstream	ost;

					ost.str("");
					ost << string(__func__) + "[" + to_string(__LINE__) + "]: ERROR session consistency check failed";
					log.Write(ERROR, ost.str());
				}

				result = false;
			}
		}
		else 
		{
			ostringstream	ost;

			{
				CLog			log(CHAT_LOG_FILE_NAME);
				ostringstream	ost;

				ost.str("");
				ost << string(__func__) + "[" + to_string(__LINE__) + "]: cookie session and DB session is not equal. Need to recreate session";
				log.Write(DEBUG, ost.str());
			}

			result = false;
		} // --- if(indexPage.SessID_Load_FromDB(sessidHTTP)) 
	

	{
		CLog			log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << string(__func__) + "[" + to_string(__LINE__) + "]: end";
		log.Write(DEBUG, ost.str());
	}

	return result;
} 

// --- Quote Words: split string into vector<string>
// --- input: string, reference to vector
// --- output: number converted words
int BuildUsersMapFromString(const string src, unordered_map<long int, string> &dst)
{
	string		trimmedStr = src;
	int			wordCounter = 0;

	{
		CLog			log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << string(__func__) + "[" + to_string(__LINE__) + "](" << src << "): start";
		log.Write(DEBUG, ost.str());
	}

	trim(trimmedStr);

	if(trimmedStr.length())
	{
		std::size_t	prevPointer = 0, nextPointer;

		prevPointer = 0, wordCounter = 0;
		do
		{
			nextPointer = trimmedStr.find(",", prevPointer);
			if(nextPointer == string::npos)
			{
				dst.emplace( atol(trimmedStr.substr(prevPointer).c_str()), "");
			}
			else
			{
				dst.emplace( atol(trimmedStr.substr(prevPointer, nextPointer - prevPointer).c_str()), "");
			}
			prevPointer = nextPointer + 1;
			wordCounter++;
		} while( (nextPointer != string::npos) );
	}

	{
		CLog			log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << string(__func__) + "[" + to_string(__LINE__) + "](" << src << "): end (number converted words = " << wordCounter << ")";
		log.Write(DEBUG, ost.str());
	}

	return wordCounter;
}

int	PacketReassembly(struct per_session_data__message *pss, void *in, size_t len)
{
	int result = 0;

	{
		CLog	log(CHAT_LOG_FILE_NAME);
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: start (len = ", to_string(len), ")");
	} // --- CLog

	// --- packet has been fragmented
	if(pss->packetReassemble == NULL)
	{
		{
			CLog	log(CHAT_LOG_FILE_NAME);
			ostringstream	ost;

			ost.str("");
			ost << string(__func__) + "[" + to_string(__LINE__) + "]: allocate " << CHAT_MAX_PACKET_SIZE << " bytes memory for fragmented packet";
			log.Write(DEBUG, ost.str());
		} // --- CLog

		pss->packetSize = 0;
		pss->packetAllocatedSize = CHAT_MAX_PACKET_SIZE;
		pss->packetReassemble = (char *)malloc(CHAT_MAX_PACKET_SIZE);
		if(!pss->packetReassemble)
		{
			{
				CLog	log(CHAT_LOG_FILE_NAME);
				ostringstream	ost;

				ost.str("");
				ost << string(__func__) + "[" + to_string(__LINE__) + "]: ERROR: fail to allocate memory for packet reassembly";
				log.Write(ERROR, ost.str());
			} // --- CLog

			result = -1;
		}
		else
		{
			memset(pss->packetReassemble, 0, CHAT_MAX_PACKET_SIZE);
		}
	}
	if((len + pss->packetSize) >= (pss->packetAllocatedSize - 1))
	{
		// reallocate  memory
		{
			CLog	log(CHAT_LOG_FILE_NAME);
			ostringstream	ost;

			ost.str("");
			ost << string(__func__) + "[" + to_string(__LINE__) + "]: reallocate additional chunk (" << CHAT_MAX_PACKET_SIZE << " bytes, final packet size = " << (pss->packetAllocatedSize + CHAT_MAX_PACKET_SIZE) << ")";
			log.Write(DEBUG, ost.str());
		} // --- CLog
		
		pss->packetReassemble = (char *)realloc(pss->packetReassemble, pss->packetAllocatedSize + CHAT_MAX_PACKET_SIZE);
		if(!pss->packetReassemble)
		{
			{
				CLog	log(CHAT_LOG_FILE_NAME);
				ostringstream	ost;

				ost.str("");
				ost << string(__func__) + "[" + to_string(__LINE__) + "]: ERROR: fail to reallocate memory for packet reassembly";
				log.Write(ERROR, ost.str());
			} // --- CLog

			result = -1;
		}
		else
		{
			pss->packetAllocatedSize += CHAT_MAX_PACKET_SIZE;
			memset(pss->packetReassemble + pss->packetSize, 0, pss->packetAllocatedSize - pss->packetSize);
		}
	}
	if((len + pss->packetSize) < (pss->packetAllocatedSize - 1))
	{
		{
			CLog	log(CHAT_LOG_FILE_NAME);
			ostringstream	ost;

			ost.str("");
			ost << string(__func__) + "[" + to_string(__LINE__) + "]: copy fragment at offset " << pss->packetSize;
			log.Write(DEBUG, ost.str());
		} // --- CLog
		memcpy(pss->packetReassemble + pss->packetSize, in, len);
		pss->packetSize += len;
		pss->packetReassemble[pss->packetSize] = 0;

		result = pss->packetSize;
/*
		{
			CLog	log(CHAT_LOG_FILE_NAME);
			ostringstream	ost;

			ost.str("");
			log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: ChunkDump:" (char *)in);
		} // --- CLog
*/
	}
	else
	{
		{
			CLog	log(CHAT_LOG_FILE_NAME);
			ostringstream	ost;

			ost.str("");
			ost << string(__func__) + "[" + to_string(__LINE__) + "]: ERROR: reassembled packet too big (" << len + pss->packetSize << " > " << CHAT_MAX_PACKET_SIZE << ")";
			log.Write(ERROR, ost.str());
		} // --- CLog

		result = -1;
	}

	{
		CLog	log(CHAT_LOG_FILE_NAME);
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: end");
	} // --- CLog

	return result;
}

bool CheckDBConnectionReset()
{
	double 		currentTimestamp;
	bool		result = true;

	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << string(__func__) + "[" + to_string(__LINE__) + "]: start";
		log.Write(DEBUG, ost.str());
	} // --- CLog

	currentTimestamp = GetSecondsSinceY2k();
	if((currentTimestamp - dbConnectionInitTimestamp) > CHAT_DB_CONNECTION_RESET_TIMEOUT)
	{
		// --- reset DB timeout connection
		dbConnectionInitTimestamp = currentTimestamp;
			
		db.CloseDB();
		if(db.Connect() < 0)
		{
			result = false;
			{
				CLog	log(CHAT_LOG_FILE_NAME);
				log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: ERROR: Can not connect to mysql database");
			}
		}

	}

	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << string(__func__) + "[" + to_string(__LINE__) + "]: end (result = " << (result ? "true" : "false") << ")";
		log.Write(DEBUG, ost.str());
	} // --- CLog

	return result;
}


int
callback_lws_message(struct lws *wsi, enum lws_callback_reasons reason,
			void *user, void *in, size_t len)
{
	lwsl_notice("start\n");
	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream   ost;

		ost.str("");
		ost << __func__ << "[" << __LINE__ << "]: start";
		log.Write(DEBUG, ost.str());
	}

	// unsigned char buf[LWS_PRE + 512];
	struct per_session_data__message *pss =
			(struct per_session_data__message *)user;
	// unsigned char *p = &buf[LWS_PRE];
	char name[128], rip[128];


	if(!CheckDBConnectionReset())
	{
		{
			CLog	log(CHAT_LOG_FILE_NAME);
			ostringstream   ost;

			ost.str("");
			ost << __func__ << "[" << __LINE__ << "]: ERROR: error reconnecting to DB, exit from thread.";
			log.Write(DEBUG, ost.str());
		}
		
		return -1;
	}

	switch (reason) {

	case LWS_CALLBACK_ESTABLISHED:

		{
			CLog	log(CHAT_LOG_FILE_NAME);
			ostringstream	ost;

			ost.str("");
			ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_ESTABLISHED: begin";
			log.Write(DEBUG, ost.str());
		}
 
		lws_set_extension_option(wsi, "permessage-deflate", "rx_buf_size", "10");

		pss->nextConnection = connectionsList;
		connectionsList = pss;

		pss->wsi = wsi;
		lws_get_peer_addresses(wsi, lws_get_socket_fd(wsi), name,
			   sizeof(name), rip, sizeof(rip));
		sprintf(pss->ip, "%s (%s)", name, rip);
		gettimeofday(&pss->tv_established, NULL);
		strcpy(pss->user_agent, "unknown");
		lws_hdr_copy(wsi, pss->user_agent, sizeof(pss->user_agent),
				 WSI_TOKEN_HTTP_USER_AGENT);

		memset(pss->sessID, 0, sizeof(pss->sessID));
		pss->user = NULL;
		{
			CLog			log(CHAT_LOG_FILE_NAME);
			ostringstream	ost;

			ost.str("");
			ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_ESTABLISHED: end (# of conn's: " << GetConnectionsNumber() << ")";
			log.Write(DEBUG, ost.str());
		}
		break;

	case LWS_CALLBACK_SERVER_WRITEABLE:
		{
			int		bytesWrittenToSocket = 0;
			{
				CLog	log(CHAT_LOG_FILE_NAME);
				ostringstream	ost;

				ost.str("");
				ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_SERVER_WRITEABLE: start";
				log.Write(DEBUG, ost.str());
			}

			if(!pss->closeFlag)
			{
				bytesWrittenToSocket = 0;
				if(GetLengthWriteQueue(pss))
				{
					string	writeString = GetStringToWrite(pss);
					PopSingleMessageFromUserQueue(pss);

					if(writeString.length())
					{

						int					bufferLength = LWS_SEND_BUFFER_PRE_PADDING + writeString.length() + LWS_SEND_BUFFER_POST_PADDING;
						unique_ptr<char[]>	tempSmartPointer(new char[bufferLength]);	
						char				*bufferToWrite;

						// bufferToWrite = (char *)malloc(bufferLength);
						bufferToWrite = tempSmartPointer.get();

						memset(bufferToWrite, 0, bufferLength);
						memcpy(bufferToWrite + LWS_SEND_BUFFER_PRE_PADDING, writeString.c_str(), bufferLength - (LWS_SEND_BUFFER_PRE_PADDING + LWS_SEND_BUFFER_POST_PADDING));
						bytesWrittenToSocket = lws_write(wsi, (unsigned char *)(bufferToWrite + LWS_SEND_BUFFER_PRE_PADDING), bufferLength - (LWS_SEND_BUFFER_PRE_PADDING + LWS_SEND_BUFFER_POST_PADDING), LWS_WRITE_TEXT);
						if (bytesWrittenToSocket < (bufferLength - (LWS_SEND_BUFFER_PRE_PADDING + LWS_SEND_BUFFER_POST_PADDING)))
						{
							{
								CLog	log(CHAT_LOG_FILE_NAME);
								ostringstream	ost;

								ost.str("");
								ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_SERVER_WRITEABLE: ERROR written " << bytesWrittenToSocket << " bytes, but requires " << (bufferLength - (LWS_SEND_BUFFER_PRE_PADDING + LWS_SEND_BUFFER_POST_PADDING)) << ".";
								log.Write(ERROR, ost.str());
							}
							lwsl_err("ERROR %d of %d writing to di socket\n", bytesWrittenToSocket, bufferLength);
							return -1;
						}

						// free(bufferToWrite);
						tempSmartPointer.reset(nullptr);

						if(GetLengthWriteQueue(pss) == (CHAT_MAX_MESSAGE_QUEUE - 15))
						{
							lws_rx_flow_control(wsi, 1);
							{
								CLog			log(CHAT_LOG_FILE_NAME);
								ostringstream	ost;

								ost.str("");
								ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_SERVER_WRITEABLE: socket throttling disabled (queue length = " << GetLengthWriteQueue(pss) << ")";
								log.Write(ERROR, ost.str());
							}		
						}

						if(GetLengthWriteQueue(pss))
							lws_callback_on_writable(pss->wsi);
					}
					else
					{
						{
							CLog	log(CHAT_LOG_FILE_NAME);
							ostringstream	ost;

							ost.str("");
							ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_SERVER_WRITEABLE: ERROR: message to write is empty";
							log.Write(ERROR, ost.str());
						} // --- CLog scope
					} // --- if(writeString.length())
				} // --- if(GetLengthWriteQueue(pss))
			} // --- if(!pss->closeFlag)
			else
			{
				{
					CLog	log(CHAT_LOG_FILE_NAME);
					ostringstream	ost;

					ost.str("");
					ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_SERVER_WRITEABLE: signal connection to close (cnxID = " << hex << pss->wsi << " )";
					log.Write(DEBUG, ost.str());
				}

				// --- returning negative gracefully closing connection
				return -1;
			}

			{
				CLog	log(CHAT_LOG_FILE_NAME);
				ostringstream	ost;

				ost.str("");
				ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_SERVER_WRITEABLE: end (written " << bytesWrittenToSocket << " bytes.)";
				log.Write(DEBUG, ost.str());
			}
			break;
		}

	case LWS_CALLBACK_RECEIVE:
		{
			CLog	log(CHAT_LOG_FILE_NAME);
			ostringstream	ost;

			ost.str("");
			ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: begin (number received bytes " << len << ", remaining packet payload " <<  lws_remaining_packet_payload(wsi) <<" , final frag " << lws_is_final_fragment(wsi) << ")";
			log.Write(DEBUG, ost.str());
		}

		if(!lws_remaining_packet_payload(wsi) && lws_is_final_fragment(wsi))
		{
			// --- final fragment or packet has not been fragmented
			string	socketRawData = (const char*)in;
			JSONParser inputParams;

			{
				int	result = PacketReassembly(pss, in, len);
				if(result == -1) 
				{
					{
						CLog	log(CHAT_LOG_FILE_NAME);
						ostringstream	ost;

						ost.str("");
						ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: ERROR: memory allocation error";
						log.Write(ERROR, ost.str());
					}
					return -1;
				}
				if(result == 0)
				{
					{
						CLog	log(CHAT_LOG_FILE_NAME);
						ostringstream	ost;

						ost.str("");
						ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: ERROR: packet too big";
						log.Write(ERROR, ost.str());
					}
				}

				socketRawData = (const char *)pss->packetReassemble;
				free(pss->packetReassemble);
				pss->packetReassemble = NULL;
			}


			if(inputParams.ParseJSONObject(socketRawData))
			{
				inputParams.DumpParamHash();

				if(inputParams.GetValue("RequestType") == "CloseSession")
				{
					{
						CLog	log(CHAT_LOG_FILE_NAME);
						ostringstream	ost;

						ost.str("");
						ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: CloseSession: start";
						log.Write(DEBUG, ost.str());
					}
					lws_close_reason(wsi, LWS_CLOSE_STATUS_GOINGAWAY,
							 (unsigned char *)"seeya", 5);
					return -1;
					
				}
				if(inputParams.GetValue("RequestType") == "GetInitialData")
				{
					ostringstream	ostResult;

					{
						CLog	log(CHAT_LOG_FILE_NAME);
						ostringstream	ost;

						ost.str("");
						ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: GetInitialData: start";
						log.Write(DEBUG, ost.str());
					}

					ostResult.str("");

					if(pss->user && (pss->user->GetLogin() != "Guest"))
					{

						if( inputParams.isNameExists("RequestType") && 
							inputParams.isNameExists("RequestID"))
						{
							ostResult.str("");
							ostResult << "\"status\":\"ok\"," << GetChatInitialData(pss, inputParams.GetValue("ActiveUserID"));
						}
						else
						{
							ostResult.str("");
							ostResult << "\"status\":\"fail\", \"description\": \"one of mandatory parameters missed\"";
							{
								CLog	log(CHAT_LOG_FILE_NAME);
								ostringstream	ost;

								ost.str("");
								ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: GetInitialData: one of mandatory parameters missed";
								log.Write(DEBUG, ost.str());
							} // --- clog
						} // --- some mandatory parameters missed
					}
					else
					{
						ostResult.str("");
						ostResult << "\"status\":\"fail\", \"description\": \"user must login before sending messages\"";
						{
							CLog	log(CHAT_LOG_FILE_NAME);
							ostringstream	ost;

							ost.str("");
							ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: GetInitialData: user must login before sending messages";
							log.Write(DEBUG, ost.str());
						} // --- clog
					} // --- user must be logged in

					try // --- catch memory allocation failure
					{
						CSingleMessage	*singleMessage;

						singleMessage = new CSingleMessage;

						singleMessage->SetMessage(ostResult.str());
						singleMessage->SetRequestType(inputParams.GetValue("RequestType"));
						singleMessage->SetRequestID(inputParams.GetValue("RequestID"));
						singleMessage->SetEventTimestamp(GetLocalFormattedTimestamp());
						singleMessage->SetSecondsSince2000(GetSecondsSinceY2k());

						if(PushSingleMessageToUserQueue(singleMessage, pss))
						{

						}
						else
						{
							{
								CLog	log(CHAT_LOG_FILE_NAME);
								ostringstream	ost;

								ost.str("");
								ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: GetInitialData: ERROR: PushSingleMessageToUserQueue returns false";
								log.Write(ERROR, ost.str());
							} // --- end CLog
						}

						delete singleMessage;
					}
					catch (std::bad_alloc& ba)
					{
						// --- if you get here, you are in trouble. 
						// --- highly possible memory leak.
						{
							CLog			log(CHAT_LOG_FILE_NAME);
							log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: bad alloc exception caught [" + ba.what() + "]");
						}
					} // --- try / catch
				} // --- if(inputParams.GetValue("RequestType") == "GetInitialData")
				if(inputParams.GetValue("RequestType") == "GetMessageBlock")
				{
					ostringstream	ostResult;

					{
						CLog	log(CHAT_LOG_FILE_NAME);
						ostringstream	ost;

						ost.str("");
						ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: GetMessageBlock: start";
						log.Write(DEBUG, ost.str());
					}

					ostResult.str("");

					if(pss->user && (pss->user->GetLogin() != "Guest"))
					{

						if( inputParams.isNameExists("RequestType") && 
							inputParams.isNameExists("RequestID") && 
							inputParams.isNameExists("friendUser") && 
							inputParams.isNameExists("minMessageID"))
						{
							ostResult.str("");
							ostResult << "\"status\":\"ok\"," << GetMessageBlock(inputParams.GetValue("friendUser"), inputParams.GetValue("minMessageID"), pss);
						}
						else
						{
							ostResult.str("");
							ostResult << "\"status\":\"fail\", \"description\": \"one of mandatory parameters missed\"";
							{
								CLog	log(CHAT_LOG_FILE_NAME);
								ostringstream	ost;

								ost.str("");
								ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: GetMessageBlock: one of mandatory parameters missed";
								log.Write(DEBUG, ost.str());
							} // --- clog
						} // --- some mandatory parameters missed
					}
					else
					{
						ostResult.str("");
						ostResult << "\"status\":\"fail\", \"description\": \"user must login before sending messages\"";
						{
							CLog	log(CHAT_LOG_FILE_NAME);
							ostringstream	ost;

							ost.str("");
							ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: GetMessageBlock: user must login before sending messages";
							log.Write(DEBUG, ost.str());
						} // --- clog
					} // --- user must be logged in

					try
					{
						CSingleMessage	*singleMessage;

						singleMessage = new CSingleMessage;

						singleMessage->SetMessage(ostResult.str());
						singleMessage->SetRequestType(inputParams.GetValue("RequestType"));
						singleMessage->SetRequestID(inputParams.GetValue("RequestID"));
						singleMessage->SetEventTimestamp(GetLocalFormattedTimestamp());
						singleMessage->SetSecondsSince2000(GetSecondsSinceY2k());

						if(PushSingleMessageToUserQueue(singleMessage, pss))
						{

						}
						else
						{
							{
								CLog	log(CHAT_LOG_FILE_NAME);
								ostringstream	ost;

								ost.str("");
								ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: GetMessageBlock: ERROR: PushSingleMessageToUserQueue returns false";
								log.Write(ERROR, ost.str());
							} // --- end CLog
						}

						delete singleMessage;
					}
					catch (std::bad_alloc& ba)
					{
						// --- if you get here, you are in trouble. 
						// --- highly possible memory leak.
						{
							CLog			log(CHAT_LOG_FILE_NAME);
							log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: bad alloc exception caught [" + ba.what() + "]");
						}
					} // --- try / catch

				} // --- if(inputParams.GetValue("RequestType") == "GetMessageBlock")
				if(inputParams.GetValue("RequestType") == "ChangeMessageStatus")
				{
					ostringstream	ostResult;
					bool			updateStatus = false;

					{
						CLog	log(CHAT_LOG_FILE_NAME);
						ostringstream	ost;

						ost.str("");
						ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: ChangeMessageStatus: start";
						log.Write(DEBUG, ost.str());
					}

					ostResult.str("");

					if(pss->user && (pss->user->GetLogin() != "Guest"))
					{

						if( inputParams.isNameExists("RequestType") && 
							inputParams.isNameExists("RequestID") && 
							inputParams.isNameExists("id") && 
							inputParams.isNameExists("messageStatus"))
						{
							if(ChangeMessageStatusInDB(inputParams.GetValue("id"), inputParams.GetValue("messageStatus"), pss))
							{
								updateStatus = true;
								ostResult.str("");
								ostResult << "\"status\":\"ok\"";
							}
							else
							{
								ostResult.str("");
								ostResult << "\"status\":\"fail\", \"description\": \"failed to update message status in DB\"";
								{
									CLog	log(CHAT_LOG_FILE_NAME);
									ostringstream	ost;

									ost.str("");
									ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: ChangeMessageStatus: failed to update message status in DB";
									log.Write(DEBUG, ost.str());
								} // --- clog
							} // --- DB update fail
							
						}
						else
						{
							ostResult.str("");
							ostResult << "\"status\":\"fail\", \"description\": \"one of mandatory parameters missed\"";
							{
								CLog	log(CHAT_LOG_FILE_NAME);
								ostringstream	ost;

								ost.str("");
								ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: ChangeMessageStatus: one of mandatory parameters missed";
								log.Write(DEBUG, ost.str());
							} // --- clog
						} // --- some mandatory parameters missed
					}
					else
					{
						ostResult.str("");
						ostResult << "\"status\":\"fail\", \"description\": \"user must login before sending messages\"";
						{
							CLog	log(CHAT_LOG_FILE_NAME);
							ostringstream	ost;

							ost.str("");
							ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: ChangeMessageStatus: user must login before sending messages";
							log.Write(DEBUG, ost.str());
						} // --- clog
					} // --- user must be logged in

					if(updateStatus)
					{
						try
						{

							CSingleMessage	*singleMessage;

							singleMessage = new CSingleMessage;

							singleMessage->SetMessage(ostResult.str());
							singleMessage->SetRequestType(inputParams.GetValue("RequestType"));
							singleMessage->SetRequestID(inputParams.GetValue("RequestID"));
							singleMessage->SetID(inputParams.GetValue("id"));
							singleMessage->SetMessageStatus(inputParams.GetValue("messageStatus"));
							singleMessage->SetFromID(GetFromDBByMessageID(inputParams.GetValue("id")));
							singleMessage->SetEventTimestamp(GetLocalFormattedTimestamp());
							singleMessage->SetSecondsSince2000(GetSecondsSinceY2k());

							if(ReplicateMessageToAllConnectionsSrcUser(singleMessage))
							{

							}
							else
							{
								
								{
									CLog	log(CHAT_LOG_FILE_NAME);
									ostringstream	ost;

									ost.str("");
									ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: ChangeMessageStatus: ERROR: PushSingleMessageToUserQueue returns false";
									log.Write(ERROR, ost.str());
								} // --- end CLog
							}

							delete singleMessage;
						}
						catch (std::bad_alloc& ba)
						{
							// --- if you get here, you are in trouble. 
							// --- highly possible memory leak.
							{
								CLog			log(CHAT_LOG_FILE_NAME);
								log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: bad alloc exception caught [" + ba.what() + "]");
							}
						} // --- try / catch
					}
				} // --- if(inputParams.GetValue("RequestType") == "ChangeMessageStatus")
				if(inputParams.GetValue("RequestType") == "MessageTypingNotification")
				{
					ostringstream	ostResult;
					bool			typingStatus = false;

					{
						CLog	log(CHAT_LOG_FILE_NAME);
						ostringstream	ost;

						ost.str("");
						ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: MessageTypingNotification: start";
						log.Write(DEBUG, ost.str());
					}

					ostResult.str("");

					if(pss->user && (pss->user->GetLogin() != "Guest"))
					{

						if( inputParams.isNameExists("RequestType") && 
							inputParams.isNameExists("RequestID") && 
							inputParams.isNameExists("toID"))
						{
							typingStatus = true;
							ostResult.str("");
							ostResult << "\"status\":\"ok\"";
						}
						else
						{
							ostResult.str("");
							ostResult << "\"status\":\"fail\", \"description\": \"one of mandatory parameters missed\"";
							{
								CLog	log(CHAT_LOG_FILE_NAME);
								ostringstream	ost;

								ost.str("");
								ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: MessageTypingNotification: one of mandatory parameters missed";
								log.Write(DEBUG, ost.str());
							} // --- clog
						} // --- some mandatory parameters missed
					}
					else
					{
						ostResult.str("");
						ostResult << "\"status\":\"fail\", \"description\": \"user must login before sending messages\"";
						{
							CLog	log(CHAT_LOG_FILE_NAME);
							ostringstream	ost;

							ost.str("");
							ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: MessageTypingNotification: user must login before sending messages";
							log.Write(DEBUG, ost.str());
						} // --- clog
					} // --- user must be logged in

					if(typingStatus)
					{

						try
						{
							CSingleMessage	*singleMessage;

							singleMessage = new CSingleMessage;

							singleMessage->SetMessage(ostResult.str());
							singleMessage->SetRequestType(inputParams.GetValue("RequestType"));
							singleMessage->SetRequestID(inputParams.GetValue("RequestID"));
							singleMessage->SetToID(inputParams.GetValue("toID"));
							singleMessage->SetFromID(pss->user->GetID());
							singleMessage->SetEventTimestamp(GetLocalFormattedTimestamp());
							singleMessage->SetSecondsSince2000(GetSecondsSinceY2k());

							if(ReplicateMessageToAllConnectionsDstUser(singleMessage))
							{

							}
							else
							{
								
								{
									CLog	log(CHAT_LOG_FILE_NAME);
									ostringstream	ost;

									ost.str("");
									ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: MessageTypingNotification: ERROR: PushSingleMessageToUserQueue returns false";
									log.Write(ERROR, ost.str());
								} // --- end CLog
							}

							delete singleMessage;
						}
						catch (std::bad_alloc& ba)
						{
							// --- if you get here, you are in trouble. 
							// --- highly possible memory leak.
							{
								CLog			log(CHAT_LOG_FILE_NAME);
								log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: bad alloc exception caught [" + ba.what() + "]");
							}
						} // --- try / catch

					}
				} // --- if(inputParams.GetValue("RequestType") == "MessageTypingNotification")
				if(inputParams.GetValue("RequestType") == "PresenceUpdate")
				{
					ostringstream	ostResult;
					bool			result = false;

					{
						CLog	log(CHAT_LOG_FILE_NAME);
						ostringstream	ost;

						ost.str("");
						ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: PresenceUpdate: start";
						log.Write(DEBUG, ost.str());
					}

					ostResult.str("");

					if(pss->user && (pss->user->GetLogin() != "Guest"))
					{

						if( inputParams.isNameExists("RequestType") && 
							inputParams.isNameExists("RequestID") && 
							inputParams.isNameExists("userList"))
						{
							string							strUserList = CleanUPText(inputParams.GetValue("userList"));
							unordered_map<long int, string>	usersMap;

							BuildUsersMapFromString(strUserList, usersMap);

							if(usersMap.size())
							{
								ostringstream	tempStr;
								tempStr.str("");

								for(auto it = usersMap.begin(); it != usersMap.end(); it++)
								{
									string		strSecondsSinceY2k = presenceCache.GetUserLastOnline(it->first);

									if(strSecondsSinceY2k.length())
									{

										tempStr << (tempStr.str().length() ? "," : "");
										tempStr << "{ \"" << it->first << "\": \"" << strSecondsSinceY2k << "\" }";
									}
									else
									{
										
										{
											CLog	log(CHAT_LOG_FILE_NAME);
											ostringstream	ost;

											ost.str("");
											ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: PresenceUpdate: ERROR: user [" << it->first << "] can't get cached data";
											log.Write(ERROR, ost.str());
										} // --- clog
									}
									
								}

								if(tempStr.str().length())
								{
									result = true;
									ostResult.str("");
									ostResult << "\"status\":\"ok\",\"presenceCache\":[" << tempStr.str() << "]";
								}
								else
								{
									ostResult << "\"status\":\"fail\", \"description\": \"failed to get data from cache\"";
									{
										CLog	log(CHAT_LOG_FILE_NAME);
										ostringstream	ost;

										ost.str("");
										ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: PresenceUpdate: ERROR: failed to get data from cache";
										log.Write(ERROR, ost.str());
									} // --- clog
								} // --- DB update fail
							}
							else
							{
								
									ostResult.str("");
									ostResult << "\"status\":\"fail\", \"description\": \"user list empty after parsing\"";
									{
										CLog	log(CHAT_LOG_FILE_NAME);
										ostringstream	ost;

										ost.str("");
										ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: PresenceUpdate: ERROR: user list empty after parsing";
										log.Write(ERROR, ost.str());
									} // --- clog
							}
							
						}
						else
						{
							ostResult.str("");
							ostResult << "\"status\":\"fail\", \"description\": \"one of mandatory parameters missed\"";
							{
								CLog	log(CHAT_LOG_FILE_NAME);
								ostringstream	ost;

								ost.str("");
								ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: PresenceUpdate: ERROR: one of mandatory parameters missed";
								log.Write(ERROR, ost.str());
							} // --- clog
						} // --- some mandatory parameters missed
					}
					else
					{
						ostResult.str("");
						ostResult << "\"status\":\"fail\", \"description\": \"user must login before sending messages\"";
						{
							CLog	log(CHAT_LOG_FILE_NAME);
							ostringstream	ost;

							ost.str("");
							ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: PresenceUpdate: user must login before sending messages";
							log.Write(DEBUG, ost.str());
						} // --- clog
					} // --- user must be logged in

					if(result)
					{
						try
						{
							CSingleMessage	*singleMessage;

							singleMessage = new CSingleMessage;

							singleMessage->SetMessage(ostResult.str());
							singleMessage->SetRequestType(inputParams.GetValue("RequestType"));
							singleMessage->SetRequestID(inputParams.GetValue("RequestID"));
							singleMessage->SetEventTimestamp(GetLocalFormattedTimestamp());
							singleMessage->SetSecondsSince2000(GetSecondsSinceY2k());

							if(PushSingleMessageToUserQueue(singleMessage, pss))
							{

							}
							else
							{
								
								{
									CLog	log(CHAT_LOG_FILE_NAME);
									ostringstream	ost;

									ost.str("");
									ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: PresenceUpdate: ERROR: PushSingleMessageToUserQueue returns false";
									log.Write(ERROR, ost.str());
								} // --- end CLog
							}

							delete singleMessage;
						}
						catch (std::bad_alloc& ba)
						{
							// --- if you get here, you are in trouble. 
							// --- highly possible memory leak.

							{
								CLog			log(CHAT_LOG_FILE_NAME);
								log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: bad alloc exception caught [" + ba.what() + "]");
							}
						} // --- try / catch
					}
				} // --- if(inputParams.GetValue("RequestType") == "PresenceUpdate")
				if(inputParams.GetValue("RequestType") == "OpenSession")
				{
					string	sessid;
					string	result;
					string	remoteAddr = (pss->ip ? pss->ip : "");

					{
						CLog	log(CHAT_LOG_FILE_NAME);
						ostringstream	ost;

						ost.str("");
						ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: OpenSession: start";
						log.Write(DEBUG, ost.str());
					}


					requestRatelimiter.IncreaseCounter(remoteAddr);
					if(!requestRatelimiter.isRatelimited(remoteAddr))
					{
						sessid = inputParams.GetValue("sessid");
						if(sessid.length())
						{
							if(FillinUserIDBySessID(sessid, pss))
							{
								result = "\"status\":\"ok\"";
								{
									CLog	log(CHAT_LOG_FILE_NAME);
									ostringstream	ost;

									ost.str("");
									ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: OpenSession: session [" << sessid << "] created for user " << pss->user->GetID();
									log.Write(DEBUG, ost.str());
								} // --- end CLog
							}
							else
							{
								result = "\"status\":\"fail\",\"description\":\"there is no user matched by sessid\"";
								{
									CLog	log(CHAT_LOG_FILE_NAME);
									ostringstream	ost;

									ost.str("");
									ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: OpenSession: there is no user matched by sessid";
									log.Write(DEBUG, ost.str());
								} // --- end CLog
							}
						}
						else
						{
							result = "\"status\":\"fail\",\"description\":\"sessid empty\"";
							{
								CLog	log(CHAT_LOG_FILE_NAME);
								ostringstream	ost;

								ost.str("");
								ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: OpenSession: sessid empty, can't create session";
								log.Write(DEBUG, ost.str());
							} // --- end CLog
							
						}

						if(result.length())
						{
							try
							{
								CSingleMessage	*singleMessage;

								singleMessage = new CSingleMessage;

								singleMessage->SetMessage(result);
								singleMessage->SetRequestType(inputParams.GetValue("RequestType"));
								singleMessage->SetRequestID(inputParams.GetValue("RequestID"));
								singleMessage->SetEventTimestamp(GetLocalFormattedTimestamp());
								singleMessage->SetSecondsSince2000(GetSecondsSinceY2k());

								if(PushSingleMessageToUserQueue(singleMessage, pss))
								{
								}
								else
								{
									
									{
										CLog	log(CHAT_LOG_FILE_NAME);
										ostringstream	ost;

										ost.str("");
										ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: OpenSession: ERROR: PushSingleMessageToUserQueue returns false";
										log.Write(ERROR, ost.str());
									} // --- end CLog
								}

								delete singleMessage;
							}
							catch (std::bad_alloc& ba)
							{
								// --- if you get here, you are in trouble. 
								// --- highly possible memory leak.
								{
									CLog			log(CHAT_LOG_FILE_NAME);
									log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: bad alloc exception caught [" + ba.what() + "]");
								}
							} // --- try / catch
						}
						else
						{
							
							{
								CLog	log(CHAT_LOG_FILE_NAME);
								ostringstream	ost;

								ost.str("");
								ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: OpenSession: ERROR: response to OpenSession is empty, nothing to send back";
								log.Write(ERROR, ost.str());
							} // --- end CLog
						}
					} // --- open request has been rate limited 
					else
					{
						// close connection due to rate-limiting
						pss->closeFlag = true;
						{
							CLog	log(CHAT_LOG_FILE_NAME);
							ostringstream	ost;

							ost.str("");
							ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: OpenSession: ERROR: request OpenSession has been ratelimited";
							log.Write(ERROR, ost.str());
						} // --- end CLog

						lws_callback_on_writable(pss->wsi);
					} // --- open request has been rate limited 
				} // --- if(inputParams.GetValue("RequestType") == "OpenSession")
				if(inputParams.GetValue("RequestType") == "SendMessage")
				{
					ostringstream	ostResult;

					{
						CLog	log(CHAT_LOG_FILE_NAME);
						ostringstream	ost;

						ost.str("");
						ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: SendMessage: start";
						log.Write(DEBUG, ost.str());
					}

					ostResult.str("");

					if(pss->user && (pss->user->GetLogin() != "Guest"))
					{

						if( inputParams.isNameExists("RequestType") && 
							inputParams.isNameExists("RequestID") && 
							inputParams.isNameExists("toType") && 
							inputParams.isNameExists("toID") && 
							inputParams.isNameExists("messageType") && 
							inputParams.isNameExists("message"))
						{
							if(inputParams.isNameExists("QualityAssuranceOnly"))
							{
								// --- Quality Assurance testing
								// --- messages moved to /dev/null
								// --- response is always "ok"

								try // --- catch memory allocation error
								{
									CSingleMessage  *singleMessage;

									{
										CLog	log(CHAT_LOG_FILE_NAME);
										ostringstream   ost;

										ost.str("");
										ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: SendMessage: QualityAssuranceOnly: need to respond 'ok'";
										log.Write(DEBUG, ost.str());
									}

									singleMessage = new CSingleMessage;

									singleMessage->SetID("0");
									singleMessage->SetMessage("ok");
									singleMessage->SetRequestType(inputParams.GetValue("RequestType"));
									singleMessage->SetRequestID(inputParams.GetValue("RequestID"));
									singleMessage->SetToID(inputParams.GetValue("toID"));
									singleMessage->SetFromID(pss->user->GetID());
									singleMessage->SetToType("toUser");
									singleMessage->SetFromType("fromUser");
									singleMessage->SetMessageStatus("sent");
									singleMessage->SetMessageType(inputParams.GetValue("messageType"));
									singleMessage->SetEventTimestamp(GetLocalFormattedTimestamp());
									singleMessage->SetSecondsSince2000(GetSecondsSinceY2k());

									if(ReplicateMessageToAllConnectionsSrcUser(singleMessage))
									{

									}
									else
									{
										{
											CLog	log(CHAT_LOG_FILE_NAME);
											ostringstream   ost;

											ost.str("");
											ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: SendMessage: can't replicate message to all connections source user";
											log.Write(DEBUG, ost.str());
										}
									}

									delete singleMessage;
								}
								catch (std::bad_alloc& ba)
								{
									// --- if you get here, you are in trouble. 
									// --- highly possible memory leak.
									{
										CLog			log(CHAT_LOG_FILE_NAME);
										log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: bad alloc exception caught [" + ba.what() + "]");
									}
								} // --- try / catch
							}
							else
							{
								string	messageBody;

								if(inputParams.GetValue("messageType") == MESSAGE_TYPE_TEXT) 
									messageBody = CleanUPText(inputParams.GetValue("message"), KEEP_BR);
								else if(inputParams.GetValue("messageType") == MESSAGE_TYPE_IMAGE)
									messageBody = SaveAndCheckPreImageToTmpLocation(inputParams.GetValue("message"));

								if(messageBody.length())
								{
									if(inputParams.GetValue("toType") == "toUser")
									{
										string	messageID = SubmitMessageToDB("toUser", inputParams.GetValue("toID"), "fromUser", pss->user->GetID(), messageBody, "sent", inputParams.GetValue("messageType"));

										if(messageID.length())
										{
											try // --- catch memory allocation error
											{
												CSingleMessage	*singleMessage;

												singleMessage = new CSingleMessage;

												singleMessage->SetID(messageID);
												singleMessage->SetMessage(messageBody);
												singleMessage->SetRequestType(inputParams.GetValue("RequestType"));
												singleMessage->SetRequestID(inputParams.GetValue("RequestID"));
												singleMessage->SetToID(inputParams.GetValue("toID"));
												singleMessage->SetFromID(pss->user->GetID());
												singleMessage->SetToType("toUser");
												singleMessage->SetFromType("fromUser");
												singleMessage->SetMessageStatus("sent");
												singleMessage->SetMessageType(inputParams.GetValue("messageType"));
												singleMessage->SetEventTimestamp(GetLocalFormattedTimestamp());
												singleMessage->SetSecondsSince2000(GetSecondsSinceY2k());

												if(ReplicateMessageToAllConnectionsSrcUser(singleMessage))
												{

												}
												else
												{
													{
														CLog	log(CHAT_LOG_FILE_NAME);
														ostringstream	ost;

														ost.str("");
														ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: SendMessage: can't replicate message to all connections source user";
														log.Write(DEBUG, ost.str());
													}
												}

												if(ReplicateMessageToAllConnectionsDstUser(singleMessage))
												{

												}
												else
												{
													{
														CLog	log(CHAT_LOG_FILE_NAME);
														ostringstream	ost;

														ost.str("");
														ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: SendMessage: can't replicate message to all connections destination user";
														log.Write(DEBUG, ost.str());
													}
												}

												delete singleMessage;
											}
											catch (std::bad_alloc& ba)
											{
												// --- if you get here, you are in trouble. 
												// --- highly possible memory leak.
												{
													CLog			log(CHAT_LOG_FILE_NAME);
													log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: bad alloc exception caught [" + ba.what() + "]");
												}
											} // --- try / catch
										}
										else
										{
											ostResult.str("");
											ostResult << "{ \"ResponseType\":\"SendMessage\", \"RequestID\":\"" << inputParams.GetValue("RequestID") << "\", \"status\":\"fail\", \"description\": \"error updating DB\" }";
											{
												CLog	log(CHAT_LOG_FILE_NAME);
												ostringstream	ost;

												ost.str("");
												ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: SendMessage: ERROR: error updating DB";
												log.Write(ERROR, ost.str());
											}
										}
									}
									else
									{
										ostResult.str("");
										ostResult << "{ \"ResponseType\":\"SendMessage\", \"RequestID\":\"" << inputParams.GetValue("RequestID") << "\", \"status\":\"fail\", \"description\": \"the only supported message type is [toUser]\" }";
										{
											CLog	log(CHAT_LOG_FILE_NAME);
											ostringstream	ost;

											ost.str("");
											ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: SendMessage: ERROR: the only supported message type is [toUser]";
											log.Write(ERROR, ost.str());
										} // --- clog
									} // --- supported message types
								}
								else
								{
									ostResult.str("");
									ostResult << "{ \"ResponseType\":\"SendMessage\", \"RequestID\":\"" << inputParams.GetValue("RequestID") << "\", \"status\":\"fail\", \"description\": \"message is empty\" }";
									{
										CLog	log(CHAT_LOG_FILE_NAME);
										ostringstream	ost;

										ost.str("");
										ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: SendMessage: empty message";
										log.Write(DEBUG, ost.str());
									} // --- clog
								} // --- message is empty ?
							} // --- if QualityAssuranceOnly
						} // --- if all necessary parameters exists
						else
						{
							ostResult.str("");
							ostResult << "{ \"ResponseType\":\"SendMessage\", \"RequestID\":\"" << inputParams.GetValue("RequestID") << "\", \"status\":\"fail\", \"description\": \"one of mandatory parameters missed\" }";
							{
								CLog	log(CHAT_LOG_FILE_NAME);
								ostringstream	ost;

								ost.str("");
								ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE:SendMessage:ERROR: one of mandatory parameters missed";
								log.Write(ERROR, ost.str());
							} // --- clog
						} // --- some mandatory parameters missed
					}
					else
					{
						ostResult.str("");
						ostResult << "{ \"ResponseType\":\"SendMessage\", \"RequestID\":\"" << inputParams.GetValue("RequestID") << "\", \"status\":\"fail\", \"description\": \"user must login before sending messages\" }";
						{
							CLog	log(CHAT_LOG_FILE_NAME);
							ostringstream	ost;

							ost.str("");
							ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: SendMessage: user must login before sending messages";
							log.Write(DEBUG, ost.str());
						} // --- clog
					} // --- user must be logged in
				} // --- if(inputParams.GetValue("RequestType") == "SendMessage")
			}
			else
			{
				{
					CLog	log(CHAT_LOG_FILE_NAME);
					ostringstream	ost;

					ost.str("");
					ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: fail to parse client request in json-format";
					log.Write(DEBUG, ost.str());
				} // --- CLog
			} // --- JSON parsing
		} // --- packet has been fragmented
		else
		{
			int	result = PacketReassembly(pss, in, len);
			if(result == -1) 
			{
				{
					CLog	log(CHAT_LOG_FILE_NAME);
					ostringstream	ost;

					ost.str("");
					ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: ERROR: memory allocation error";
					log.Write(ERROR, ost.str());
				}
				return -1;
			}
			if(result == 0)
			{
				{
					CLog	log(CHAT_LOG_FILE_NAME);
					ostringstream	ost;

					ost.str("");
					ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_RECEIVE: ERROR: packet too big";
					log.Write(ERROR, ost.str());
				}
			}
		}

		{
			CLog	log(CHAT_LOG_FILE_NAME);
			log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: LWS_CALLBACK_RECEIVE: end");
		}
		break;
	/*
	 * this just demonstrates how to use the protocol filter. If you won't
	 * study and reject connections based on header content, you don't need
	 * to handle this callback
	 */
	case LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION:
		dump_handshake_info(wsi);
		/* you could return non-zero here and kill the connection */
		break;

	/*
	 * this just demonstrates how to handle
	 * LWS_CALLBACK_WS_PEER_INITIATED_CLOSE and extract the peer's close
	 * code and auxiliary data.  You can just not handle it if you don't
	 * have a use for this.
	 */
	case LWS_CALLBACK_WS_PEER_INITIATED_CLOSE:
		{
			CLog	log(CHAT_LOG_FILE_NAME);
			ostringstream	ost;

			ost.str("");
			ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_WS_PEER_INITIATED_CLOSE: begin (len = " << len << ")";
			log.Write(DEBUG, ost.str());
		}
		lwsl_notice("LWS_CALLBACK_WS_PEER_INITIATED_CLOSE: len %d\n", len);
		{
			CLog	log(CHAT_LOG_FILE_NAME);
			ostringstream	ost;

			ost.str("");
			ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_WS_PEER_INITIATED_CLOSE: begin";
			log.Write(DEBUG, ost.str());
		}
		break;

	case LWS_CALLBACK_CLOSED:
		{
			CLog	log(CHAT_LOG_FILE_NAME);
			ostringstream	ost;

			ost.str("");
			ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_CLOSED: begin";
			log.Write(DEBUG, ost.str());
		}

		{
			struct per_session_data__message **pp;

			pp = &connectionsList;
			while (*pp) {
				if (*pp == pss) {
					*pp = pss->nextConnection;
					pss->nextConnection = NULL;
					break;
				}
				pp = &((*pp)->nextConnection);
			}

			CloseSingleTextMessageConnection(pss);
		}


		{
			CLog			log(CHAT_LOG_FILE_NAME);
			ostringstream	ost;

			ost.str("");
			ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_CLOSED: end (remaining # of conn's: " << GetConnectionsNumber() << ")";
			log.Write(DEBUG, ost.str());
		}
		break;

	default:
		{
			CLog	log(CHAT_LOG_FILE_NAME);
			ostringstream   ost;

			ost.str("");
			ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_DEFAULT: begin";
			log.Write(DEBUG, ost.str());
		}
		{
			CLog	log(CHAT_LOG_FILE_NAME);
			ostringstream   ost;

			ost.str("");
			ost << __func__ << "[" << __LINE__ << "]: LWS_CALLBACK_DEFAULT: end";
			log.Write(DEBUG, ost.str());
		}
		break;
	}

	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream   ost;

		ost.str("");
		ost << __func__ << "[" << __LINE__ << "]: finish";
		log.Write(DEBUG, ost.str());
	}

	return 0;
}

