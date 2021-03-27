#include "utilities.h"

string	GetDefaultActionFromUserType(CUser *user, CMysql *db)
{
	MESSAGE_DEBUG("", "", "start");
	
	string	result = GUEST_USER_DEFAULT_ACTION;

	if(user->GetType() == "guest")		result = GUEST_USER_DEFAULT_ACTION;
	else if(user->GetType() == "user")	result = LOGGEDIN_USER_DEFAULT_ACTION;
	else if(user->GetType() == "helpdesk") result = LOGGEDIN_HELPDESK_DEFAULT_ACTION;
	else
	{
		MESSAGE_ERROR("", "", "unknown user type (" + user->GetType() + ")");
	}

	MESSAGE_DEBUG("", "", "finish (result = " + result + ")");

	return result;
}

int GetSpecificData_GetNumberOfFolders(string itemType)
{
	int	  result = 0;

	MESSAGE_DEBUG("", "", "start");

	if(itemType == "certification")					result = CERTIFICATIONSLOGO_NUMBER_OF_FOLDERS;
	else if(itemType == "course")					result = CERTIFICATIONSLOGO_NUMBER_OF_FOLDERS;
	else if(itemType == "university")				result = UNIVERSITYLOGO_NUMBER_OF_FOLDERS;
	else if(itemType == "school")					result = SCHOOLLOGO_NUMBER_OF_FOLDERS;
	else if(itemType == "language")					result = FLAG_NUMBER_OF_FOLDERS;
	else if(itemType == "book")						result = BOOKCOVER_NUMBER_OF_FOLDERS;
	else if(itemType == "company")					result = COMPANYLOGO_NUMBER_OF_FOLDERS;
	else if(itemType == "gift")						result = GIFTIMAGE_NUMBER_OF_FOLDERS;
	else if(itemType == "event")					result = EVENTIMAGE_NUMBER_OF_FOLDERS;
	else if(itemType == "helpdesk_ticket_attach")	result = HELPDESK_TICKET_ATTACHES_NUMBER_OF_FOLDERS;
	else
	{
		MESSAGE_ERROR("", "", "itemType [" + itemType + "] unknown");
	}

	MESSAGE_DEBUG("", "", "finish (result = " + to_string(result) + ")");
	
	return result;
}

int GetSpecificData_GetMaxFileSize(string itemType)
{
	int	  result = 0;

	MESSAGE_DEBUG("", "", "start");

	if(itemType == "certification")					result = CERTIFICATIONSLOGO_MAX_FILE_SIZE;
	else if(itemType == "course")					result = CERTIFICATIONSLOGO_MAX_FILE_SIZE;
	else if(itemType == "university")				result = UNIVERSITYLOGO_MAX_FILE_SIZE;
	else if(itemType == "school")					result = SCHOOLLOGO_MAX_FILE_SIZE;
	else if(itemType == "language")					result = FLAG_MAX_FILE_SIZE;
	else if(itemType == "book")						result = BOOKCOVER_MAX_FILE_SIZE;
	else if(itemType == "company")					result = COMPANYLOGO_MAX_FILE_SIZE;
	else if(itemType == "gift")						result = GIFTIMAGE_MAX_FILE_SIZE;
	else if(itemType == "event")					result = EVENTIMAGE_MAX_FILE_SIZE;
	else if(itemType == "helpdesk_ticket_attach")	result = HELPDESK_TICKET_ATTACHES_MAX_FILE_SIZE;
	else
	{
		MESSAGE_ERROR("", "", "itemType [" + itemType + "] unknown");
	}

	MESSAGE_DEBUG("", "", "finish (result = " + to_string(result) + ")");
	
	return result;
}

unsigned int GetSpecificData_GetMaxWidth(string itemType)
{
	int	  result = 0;

	MESSAGE_DEBUG("", "", "start");

	if(itemType == "certification")		result = CERTIFICATIONSLOGO_MAX_WIDTH;
	else if(itemType == "course")		result = CERTIFICATIONSLOGO_MAX_WIDTH;
	else if(itemType == "university")	result = UNIVERSITYLOGO_MAX_WIDTH;
	else if(itemType == "school")		result = SCHOOLLOGO_MAX_WIDTH;
	else if(itemType == "language")		result = FLAG_MAX_WIDTH;
	else if(itemType == "book")			result = BOOKCOVER_MAX_WIDTH;
	else if(itemType == "company")		result = COMPANYLOGO_MAX_WIDTH;
	else if(itemType == "gift")			result = GIFTIMAGE_MAX_WIDTH;
	else if(itemType == "event")		result = EVENTIMAGE_MAX_WIDTH;
	else
	{
		MESSAGE_ERROR("", "", "itemType [" + itemType + "] unknown");
	}

	MESSAGE_DEBUG("", "", "finish (result = " + to_string(result) + ")");
	
	return result;
}

unsigned int GetSpecificData_GetMaxHeight(string itemType)
{
	int	  result = 0;

	MESSAGE_DEBUG("", "", "start");

	if(itemType == "certification")		result = CERTIFICATIONSLOGO_MAX_HEIGHT;
	else if(itemType == "course")		result = CERTIFICATIONSLOGO_MAX_HEIGHT;
	else if(itemType == "university")	result = UNIVERSITYLOGO_MAX_HEIGHT;
	else if(itemType == "school")		result = SCHOOLLOGO_MAX_HEIGHT;
	else if(itemType == "language")		result = FLAG_MAX_HEIGHT;
	else if(itemType == "book")			result = BOOKCOVER_MAX_HEIGHT;
	else if(itemType == "company")		result = COMPANYLOGO_MAX_HEIGHT;
	else if(itemType == "gift")	  		result = GIFTIMAGE_MAX_HEIGHT;
	else if(itemType == "event")	  	result = EVENTIMAGE_MAX_HEIGHT;
	else
	{
		MESSAGE_ERROR("", "", "itemType [" + itemType + "] unknown");
	}

	MESSAGE_DEBUG("", "", "finish (result = " + to_string(result) + ")");
	
	return result;
}

string GetSpecificData_GetBaseDirectory(string itemType)
{
	string	  result = "";

	MESSAGE_DEBUG("", "", "start");

	if(itemType == "certification")					result = IMAGE_CERTIFICATIONS_DIRECTORY;
	else if(itemType == "course")					result = IMAGE_CERTIFICATIONS_DIRECTORY;
	else if(itemType == "university")				result = IMAGE_UNIVERSITIES_DIRECTORY;
	else if(itemType == "school")					result = IMAGE_SCHOOLS_DIRECTORY;
	else if(itemType == "language")					result = IMAGE_FLAGS_DIRECTORY;
	else if(itemType == "book")						result = IMAGE_BOOKS_DIRECTORY;
	else if(itemType == "company")					result = IMAGE_COMPANIES_DIRECTORY;
	else if(itemType == "gift")						result = IMAGE_GIFTS_DIRECTORY;
	else if(itemType == "event")					result = IMAGE_EVENTS_DIRECTORY;
	else if(itemType == "helpdesk_ticket_attach")	result = HELPDESK_TICKET_ATTACHES_DIRECTORY;
	else
	{
		MESSAGE_ERROR("", "", "itemType [" + itemType + "] unknown");
	}

	{
		CLog	log;

		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: finish (result: " + result + ")");
	}
	
	return result;
}

string GetSpecificData_SelectQueryItemByID(string itemID, string itemType)
{
	string	  result = "";

	MESSAGE_DEBUG("", "", "start");

	if(itemType == "certification")		result = "select * from `certification_tracks` where `id`=\"" + itemID + "\";";
	else if(itemType == "course")		result = "select * from `certification_tracks` where `id`=\"" + itemID + "\";";
	else if(itemType == "university")	result = "select * from `university` where `id`=\"" + itemID + "\";";
	else if(itemType == "school")		result = "select * from `school` where `id`=\"" + itemID + "\";";
	else if(itemType == "language")		result = "select * from `language` where `id`=\"" + itemID + "\";";
	else if(itemType == "book")			result = "select * from `book` where `id`=\"" + itemID + "\";";
	else if(itemType == "company")		result = "select * from `company` where `id`=\"" + itemID + "\";";
	else if(itemType == "gift")			result = "select * from `gifts` where `id`=\"" + itemID + "\";";
	else if(itemType == "event")		result = "select * from `events` where `id`=\"" + itemID + "\";";
	else
	{
		MESSAGE_ERROR("", "", "itemType [" + itemType + "] unknown");
	}

	{
		CLog	log;

		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: finish (result: " + result + ")");
	}
	
	return result;
}

string GetSpecificData_UpdateQueryItemByID(string itemID, string itemType, string folderID, string fileName)
{
	string		result = "";
	string		logo_folder = "";
	string		logo_filename = "";

	MESSAGE_DEBUG("", "", "start");

	logo_folder = GetSpecificData_GetDBCoverPhotoFolderString(itemType);
	logo_filename = GetSpecificData_GetDBCoverPhotoFilenameString(itemType);

	if(logo_folder.length() && logo_filename.length())
	{
		if(itemType == "certification")		result = "update `certification_tracks` set	`" + logo_folder + "`='" + folderID + "', `" + logo_filename + "`='" + fileName + "' where `id`=\"" + itemID + "\";";
		else if(itemType == "course")		result = "update `certification_tracks` set `" + logo_folder + "`='" + folderID + "', `" + logo_filename + "`='" + fileName + "' where `id`=\"" + itemID + "\";";
		else if(itemType == "university")	result = "update `university` set 			`" + logo_folder + "`='" + folderID + "', `" + logo_filename + "`='" + fileName + "' where `id`=\"" + itemID + "\";";
		else if(itemType == "school")		result = "update `school` set 				`" + logo_folder + "`='" + folderID + "', `" + logo_filename + "`='" + fileName + "' where `id`=\"" + itemID + "\";";
		else if(itemType == "language")		result = "update `language` set 			`" + logo_folder + "`='" + folderID + "', `" + logo_filename + "`='" + fileName + "' where `id`=\"" + itemID + "\";";
		else if(itemType == "book")			result = "update `book` set 				`" + logo_folder + "`='" + folderID + "', `" + logo_filename + "`='" + fileName + "' where `id`=\"" + itemID + "\";";
		else if(itemType == "company")		result = "update `company` set 				`" + logo_folder + "`='" + folderID + "', `" + logo_filename + "`='" + fileName + "' where `id`=\"" + itemID + "\";";
		else if(itemType == "gift")			result = "update `gifts` set 				`" + logo_folder + "`='" + folderID + "', `" + logo_filename + "`='" + fileName + "' where `id`=\"" + itemID + "\";";
		else if(itemType == "event")		result = "update `events` set 				`" + logo_folder + "`='" + folderID + "', `" + logo_filename + "`='" + fileName + "' where `id`=\"" + itemID + "\";";
		else
		{
			CLog	log;
			log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: itemType [" + itemType + "] unknown");
		}
	}
	else
	{
		{
			CLog	log;
			log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: logo_folder or logo_filename not found for itemType [" + itemType + "]");
		}
	}


	{
		CLog	log;

		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: finish (result: " + result + ")");
	}
	
	return result;
}

string GetSpecificData_GetDBCoverPhotoFolderString(string itemType)
{
	string	  result = "";

	MESSAGE_DEBUG("", "", "start");

	if(itemType == "certification")	 	result = "logo_folder";
	else if(itemType == "course")	   	result = "logo_folder";
	else if(itemType == "university")   result = "logo_folder";
	else if(itemType == "school")	   	result = "logo_folder";
	else if(itemType == "language")	 	result = "logo_folder";
	else if(itemType == "book")		 	result = "coverPhotoFolder";
	else if(itemType == "company")		result = "logo_folder";
	else if(itemType == "gift")	  		result = "logo_folder";
	else if(itemType == "event")	  	result = "logo_folder";
	else
	{
		MESSAGE_ERROR("", "", "itemType [" + itemType + "] unknown");
	}

	{
		CLog	log;

		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: finish (result: " + result + ")");
	}
	
	return result;
}

string GetSpecificData_GetDBCoverPhotoFilenameString(string itemType)
{
	string	  result = "";

	MESSAGE_DEBUG("", "", "start");

	if(itemType == "certification")		result = "logo_filename";
	else if(itemType == "course")		result = "logo_filename";
	else if(itemType == "university")	result = "logo_filename";
	else if(itemType == "school")		result = "logo_filename";
	else if(itemType == "language")		result = "logo_filename";
	else if(itemType == "book")			result = "coverPhotoFilename";
	else if(itemType == "company")		result = "logo_filename";
	else if(itemType == "gift")			result = "logo_filename";
	else if(itemType == "event")		result = "logo_filename";
	else
	{
		MESSAGE_ERROR("", "", "itemType [" + itemType + "] unknown");
	}

	{
		CLog	log;
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: finish (result: " + result + ")");
	}
	
	return result;
}

string GetSpecificData_GetFinalFileExtension(string itemType)
{
	string	  result = ".jpg";

	MESSAGE_DEBUG("", "", "start");

	if(itemType == "template_sow")						result = ".txt";
	else if(itemType == "template_psow")				result = ".txt";
	else if(itemType == "template_costcenter")			result = ".txt";
	else if(itemType == "template_company")				result = ".txt";
	else if(itemType == "template_agreement_company")	result = ".txt";
	else if(itemType == "template_agreement_sow")		result = ".txt";
	else
	{
		MESSAGE_DEBUG("", "", "default extension(" + result + ") taken");
	}

	MESSAGE_DEBUG("", "", "finish (result = " + result + ")");

	return result;
}

string GetSpecificData_GetDataTypeByItemType(const string &itemType)
{
	auto	result = "image"s;

	MESSAGE_DEBUG("", "", "start");

	if(itemType == "template_sow")					result = "template";
	if(itemType == "template_psow")					result = "template";
	if(itemType == "template_costcenter")			result = "template";
	if(itemType == "template_company")				result = "template";
	if(itemType == "template_agreement_company")	result = "template";
	if(itemType == "template_agreement_sow")		result = "template";

	MESSAGE_DEBUG("", "", "finish (result = " + result + ")");

	return result;
}

// --- Does the owner user allowed to change it ?
// --- For example:
// ---	*) university or school logo can be changed by administrator only.
// ---	*) gift image could be changed by owner
auto GetSpecificData_AllowedToChange(string itemID, string itemType, CMysql *db, CUser *user) -> string
{
	auto	  error_message = ""s;

	MESSAGE_DEBUG("", "", "start");

	if(db->Query(GetSpecificData_SelectQueryItemByID(itemID, itemType))) // --- item itemID exists ?
	{
		if((itemType == "course") || (itemType == "university") || (itemType == "school") || (itemType == "language") || (itemType == "book") || (itemType == "company") || (itemType == "certification"))
		{
			string	  coverPhotoFolder = db->Get(0, GetSpecificData_GetDBCoverPhotoFolderString(itemType).c_str());
			string	  coverPhotoFilename = db->Get(0, GetSpecificData_GetDBCoverPhotoFilenameString(itemType).c_str());

			if(coverPhotoFolder.empty() && coverPhotoFilename.empty()) {}
			else
			{
				error_message = "logo already uploaded";

				MESSAGE_DEBUG("", "", "access to " + itemType + "(" + itemID + ") denied, because logo already uploaded");
			}
		}
		else if(itemType == "event")
		{
			if(user)
			{
				if(db->Query("SELECT `id` FROM `event_hosts` WHERE `event_id`=\"" + itemID + "\" AND `user_id`=\"" + user->GetID() + "\";")) {}
				else
				{
					error_message = "you are not the event host";

					MESSAGE_DEBUG("", "", "access to " + itemType + "(" + itemID + ") denied, you are not the event host");
				}
			}
			else
			{
				error_message = "user object is NULL";

				MESSAGE_ERROR("", "", error_message);
			}
		}
		else if(itemType == "gift")
		{
			string		user_id = db->Get(0, "user_id");

			if(user)
			{
				if(user_id == user->GetID()) {}
				else
				{
					error_message = "you are not the gift owner";

					MESSAGE_DEBUG("", "", "access to " + itemType + "(" + itemID + ") denied, you are not the gift owner");
				}
			}
			else
			{
				error_message = "user object is NULL";

				MESSAGE_ERROR("", "", error_message);
			}
		}
		else
		{
			error_message = "itemType [" + itemType + "] unknown";

			MESSAGE_ERROR("", "", error_message);
		}
	}
	else
	{
		error_message = itemType + "(" + itemID + ") not found";

		MESSAGE_ERROR("", "", error_message);
	}

	MESSAGE_DEBUG("", "", "finish (error_message: " + error_message + ")");
	
	return error_message;
}

