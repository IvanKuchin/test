#include "chtml.h"

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	CHTML	*obj = (CHTML *)userp;

	{
		CLog	log;
		log.Write(DEBUG, string("CHTML::") + string(__func__) + "(data len " + to_string(size * nmemb) + string(")[") + to_string(__LINE__) +  "]:start");
	}

	size_t				bufferLength = size * nmemb;
	// --- don't use first option
	// unique_ptr<char[]>	tempSmartPointer(new char[bufferLength + 1]); // "+1" must host last '\0'	
	unique_ptr<char[]>	tempSmartPointer = make_unique<char[]>(bufferLength + 1); // "+1" must host last '\0'	
	char				*bufferToWrite = tempSmartPointer.get();

	memcpy(bufferToWrite, contents, bufferLength);
	bufferToWrite[bufferLength] = 0;

	obj->AppendToHTMLPage(bufferToWrite);

	MESSAGE_DEBUG("", "", "finish (size of allocated buffer is " + to_string(bufferLength) + ")");

	return bufferLength;
}

CHTML::CHTML()
{
	MESSAGE_DEBUG("", "", "start");

	// --- Repeated calls to curl_global_init and curl_global_cleanup should be avoided. They should only be called once each.
	curl_global_init(CURL_GLOBAL_ALL);

	MESSAGE_DEBUG("", "", "finish");
}

CHTML::CHTML(string _type_): type(_type_)
{
	MESSAGE_DEBUG("", "", "start");

	// --- Repeated calls to curl_global_init and curl_global_cleanup should be avoided. They should only be called once each.
	curl_global_init(CURL_GLOBAL_ALL);

	MESSAGE_DEBUG("", "", "finish");
}

string CHTML::GetTagContent(string tag_name)
{
	MESSAGE_DEBUG("", "", "start(" + tag_name + ")");

    auto        result = ""s;
    smatch      sm1, sm2;

    if(regex_search(headTag, sm1, regex("<" + tag_name + ".*>(.*)</" + tag_name + ">")) )
    {
        result = sm1[1];
    }
    else
    {
    	MESSAGE_DEBUG("", "", "tag <" + tag_name + "> not found");
    }

	MESSAGE_DEBUG("", "", "finish (result length is " + to_string(result.length()) + ")");

    return result;
}

string CHTML::GetAttributeValue(string tag_name, string attr_name, string name_content, string attr_content_name)
{
	MESSAGE_DEBUG("", "", "start( " + tag_name + ", " + attr_name + ", " + name_content + ", " + attr_content_name + ")");

    auto        result = ""s;
    auto        tag_start = headTag.find("<" + tag_name);
    auto        tag_close = tag_start; // --- type derivation only
    

    while(tag_start != string::npos)
    {
        tag_close = headTag.find(">", tag_start);

        if(tag_close != string::npos)
        {
            auto    tag_attrs = headTag.substr(tag_start, tag_close - tag_start);
            auto    name_start = tag_attrs.find(attr_name + "=");
            
            if(name_start != string::npos)
            {
                if(tag_attrs.find(name_content, name_start) != string::npos)
                {
                    auto    content_attr_start = tag_attrs.find(attr_content_name + "=");
                    
                    if(content_attr_start != string::npos)
                    {
                        auto content_start = content_attr_start + string(attr_content_name + "=").length();

                        if(content_start + 1 < tag_attrs.length())
                        {
                            auto attr_separate_symbol = tag_attrs.at(content_start);
                            auto content_end_pos = tag_attrs.find(attr_separate_symbol, content_start + 1);
                            
                            if(content_end_pos != string::npos)
                            {
                                result = tag_attrs.substr(content_start + 1, content_end_pos - (content_start + 2) + 1);
                            }
                            else
                            {
                            	MESSAGE_ERROR("", "", "can't find separator symbol("s + attr_separate_symbol + ")");
                            }
                        }
                        else
                        {
                        	MESSAGE_ERROR("", "", "content start position at the end of attribute list");
                        }
                    }
                    else
                    {
	                	MESSAGE_DEBUG("", "", attr_content_name + " not found");
                    }
                }
                else
                {
                	MESSAGE_DEBUG("", "", name_content + " not found");
                }
            }
            else
            {
            	MESSAGE_DEBUG("", "", attr_name + " not found");
            }
        }
        else
        {
        	MESSAGE_DEBUG("", "", "closing brace not found");
        }

        tag_start = headTag.find("<" + tag_name, tag_close);
    }

	MESSAGE_DEBUG("", "", "finish (result length is " + to_string(result.length()) + ")");
    
    return result;
}

bool CHTML::ExtractTITLE()
{
	bool			 	result = true;

	MESSAGE_DEBUG("", "", "start");

	metaTitle = GetTagContent("title");
	if(metaTitle.length() == 0) metaTitle = GetAttributeValue("meta",  "name", "title",  "content");

	MESSAGE_DEBUG("", "", "finish (return: " + (result ? "true" : "false") + ")");

	return result;	
}

bool CHTML::isEmbedVideoHostedOnYoutube()
{
	bool			 	result = false;
	regex				regex1("https?://www.youtube.com/(.*)");
	regex				regex2("https?://youtu.be/(.*)");

	MESSAGE_DEBUG("", "", "start");

	result = (regex_match(embedVideoURL, regex1)) || (regex_match(embedVideoURL, regex2));

	MESSAGE_DEBUG("", "", "finish (return: " + (result ? "true" : "false") + ")");

	return result;
}

bool CHTML::ExtractEmbedVideoURL()
{
	MESSAGE_DEBUG("", "", "start");

	embedVideoURL = GetAttributeValue("meta",  "property", "og:video:url", "content");

	MESSAGE_DEBUG("", "", "finish (return: " + (!embedVideoURL.empty() ? "true" : "false") + ")");

	return !embedVideoURL.empty();
}

bool CHTML::ExtractPreviewImage()
{
	bool	result;

	MESSAGE_DEBUG("", "", "start");

	metaPreviewImageURL = GetAttributeValue("meta", "property", "og:image", "content");
	result = !metaPreviewImageURL.empty();

	MESSAGE_DEBUG("", "", "finish (return: " + (result ? "true" : "false") + ")");

	return result;	
}

bool CHTML::ExtractDESCRIPTION()
{
	bool			 	result = true;

	MESSAGE_DEBUG("", "", "start");

	metaDescription = GetAttributeValue("meta", "name", "description", "content");
	if(metaDescription.empty())
		metaDescription = GetAttributeValue("meta", "property", "og:description", "content");

	MESSAGE_DEBUG("", "", "finish (return: " + (result ? "true" : "false") + ")");

	return result;	
}

bool CHTML::ExtractHEAD()
{
	bool			 	result = false;
	string::size_type	openTagPos, closeTagPos;

	MESSAGE_DEBUG("", "", "start");

	openTagPos = htmlPage.find("<head>");
	if(openTagPos == string::npos) openTagPos = htmlPage.find("< head >");
	if(openTagPos == string::npos) openTagPos = htmlPage.find("<HEAD>");
	if(openTagPos == string::npos) openTagPos = htmlPage.find("< HEAD >");
	if(openTagPos == string::npos) openTagPos = htmlPage.find("<head prefix"); // --- yahoo
	if(openTagPos != string::npos)
	{
		closeTagPos = htmlPage.find("</head>", openTagPos + 1);
		if(closeTagPos == string::npos) closeTagPos = htmlPage.find("< /head >", openTagPos + 1);
		if(closeTagPos == string::npos) closeTagPos = htmlPage.find("</HEAD>", openTagPos + 1);
		if(closeTagPos == string::npos) closeTagPos = htmlPage.find("< /HEAD >", openTagPos + 1);
		if(closeTagPos != string::npos)
		{
			headTag = htmlPage.substr(openTagPos, closeTagPos - openTagPos);

			result = true;
			htmlPage = ""; // --- garbage collection
		}
		else
		{
			MESSAGE_DEBUG("", "", " close tag not found");
		}

	}
	else
	{
		MESSAGE_DEBUG("", "", " open tag not found");
	}

	MESSAGE_DEBUG("", "", "finish (return: " + (result ? "true" : "false") + ")");

	return result;
}

int CHTML::GetNumberOfFoldersByType()
{
	int		result = FEEDIMAGE_NUMBER_OF_FOLDERS;

	MESSAGE_DEBUG("", "", "start");

	if(type == "feed") result = FEEDIMAGE_NUMBER_OF_FOLDERS;
	else if(type == "temp") result = TEMPIMAGE_NUMBER_OF_FOLDERS;
	else
	{
		MESSAGE_DEBUG("", "", "ERROR:unknown type(" + type + ")");
	}

	MESSAGE_DEBUG("", "", "finish (result = " + to_string(result) + ")");

	return result;
}

string CHTML::GetDirectoryByType()
{
	string		result = IMAGE_FEED_DIRECTORY;

	MESSAGE_DEBUG("", "", "start");

	if(type == "feed") result = IMAGE_FEED_DIRECTORY;
	else if(type == "temp") result = IMAGE_TEMP_DIRECTORY;
	else
	{
		MESSAGE_DEBUG("", "", "ERROR:unknown type(" + type + ")");
	}

	MESSAGE_DEBUG("", "", "finish (result = " + result + ")");

	return result;
}

tuple<string, string, string> CHTML::PickFileName()
{
	string		filePrefix = "", fileExtension = "";
	int			folderID;
	string		finalFile, tmpFile2Check, tmpImageJPG;

	MESSAGE_DEBUG("", "", "start");

	//--- check image file existing
	do
	{
		ostringstream	ost;
        string          tmp;
        std::size_t  	foundPos;

		folderID = (int)(rand()/(RAND_MAX + 1.0) * GetNumberOfFoldersByType()) + 1;
		filePrefix = GetRandom(20);
        tmp = metaPreviewImageURL;

        if((foundPos = tmp.rfind(".")) != string::npos)
        {
            fileExtension = tmp.substr(foundPos, tmp.length() - foundPos);
            
            // --- filter wrong fileExtension (for ex: .com?action=fake_action) 
            if(fileExtension.find("jpeg")) fileExtension = ".jpeg";
            else if(fileExtension.find("png")) fileExtension = ".png";
            else if(fileExtension.find("gif")) fileExtension = ".gif";
            else if(fileExtension.find("svg")) fileExtension = ".svg";
            else fileExtension = ".jpg";
        }
        else
        {
            fileExtension = ".jpg";
        }

		ost.str("");
		ost << GetDirectoryByType() << "/" << folderID << "/" << filePrefix << ".jpg";
        finalFile = ost.str();

        ost.str("");
        ost << "/tmp/tmp_" << filePrefix << fileExtension;
        tmpFile2Check = ost.str();

        ost.str("");
        ost << "/tmp/" << filePrefix << ".jpg";
        tmpImageJPG = ost.str();
	} while(isFileExists(finalFile) || isFileExists(tmpFile2Check) || isFileExists(tmpImageJPG));

	MESSAGE_DEBUG("", "", "finish (return: folderID: " + to_string(folderID) + ", filePrefix:" + filePrefix + ", fileExtension:" + fileExtension + ")");

	return make_tuple(to_string(folderID), filePrefix, fileExtension);
}

bool CHTML::DownloadFile(string urlPreview, FILE *f)
{
	bool		result = false;

	MESSAGE_DEBUG("", "", "start");

	curl = curl_easy_init();
	if(curl)
	{
		// --- specify URL to get
		curl_easy_setopt(curl, CURLOPT_URL, urlPreview.c_str());

		// --- send all data to this file
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, reinterpret_cast<void *>(f));

		// --- enable HTTP redirects 3xx
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		/* some servers don't like requests that are made without a user-agent
		 field, so we provide one */
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

		curlRes = curl_easy_perform(curl);
		if(curlRes == CURLE_OK)
		{
			result = true;
		}
		else
		{
			{
				CLog	log;
				log.Write(ERROR, string("CHTML::") + string(__func__) + string("[") + to_string(__LINE__) +  "]:ERROR: curl_easy_perform() returned error (" + curl_easy_strerror(curlRes) + ")");
			}
			result = false;
		}

		curl_easy_cleanup(curl);
	}
	else
	{
		{
			CLog	log;
			log.Write(ERROR, string("CHTML::") + string(__func__) + string("[") + to_string(__LINE__) +  "]:ERROR: curl_easy_init() returned null");
		}
		result = false;
	}

	MESSAGE_DEBUG("", "", " end (result: " + (result ? "true" : "false"));

	return result;
}

bool CHTML::ParseHTMLPage()
{
	bool	result = true;

	MESSAGE_DEBUG("", "", "start");

	if(ExtractHEAD())
	{
/*
		{
			MESSAGE_DEBUG("", "", " dump head " + headTag);
		}
*/		
		if(ExtractTITLE())
		{

		}
		else
		{
			{
				MESSAGE_DEBUG("", "", " TITLE tag not found");
			}
		}

		if(ExtractDESCRIPTION())
		{

		}
		else
		{
			{
				MESSAGE_DEBUG("", "", " DESCRIPTION tag not found");
			}
		}

		if(ExtractEmbedVideoURL() && isEmbedVideoHostedOnYoutube())
		{
			// --- embed video hosted on youtube
			// --- no need to extract previewImage
		}
		else if (ExtractPreviewImage())
		{
			if(metaPreviewImageURL.length() > 10)
			{
				// download it
				FILE	*f;

				tie(folderID, filePrefix, fileExtension) = PickFileName();
	
	    		{
					MESSAGE_DEBUG("", "", "Save preview (" + metaPreviewImageURL + ") to /tmp for checking of image validity [" + "/tmp/tmp_" + filePrefix + fileExtension + "]");
	    		}

	            // --- Save file to "/tmp/" for checking of image validity
	            f = fopen(string("/tmp/tmp_" + filePrefix + fileExtension).c_str(), "w");
	            if(f != NULL)
	            {
	            	if(DownloadFile(metaPreviewImageURL, f))
	            	{
	            		// --- download succeed

	            	}
	            	else
	            	{
	                    MESSAGE_ERROR("CHTML", "", "fail download file (" + metaPreviewImageURL + ") with curl")

		                metaPreviewImageURL = "";
		                folderID = "";
		                filePrefix = "";
		                fileExtension = "";
	            	}

		            fclose(f);
	            }
	            else
	            {
                    MESSAGE_ERROR("CHTML", "", "writing file: /tmp/tmp_" + filePrefix + fileExtension);

	                metaPreviewImageURL = "";
	                folderID = "";
	                filePrefix = "";
	                fileExtension = "";
	            }
			}
		}
		else
		{
			MESSAGE_DEBUG("", "", " (embed video, preview image) not found");
		}
	}
	else
	{
		MESSAGE_DEBUG("", "", " HEAD tag not found");
	}

	MESSAGE_DEBUG("", "", "finish (return: " + (result ? "true" : "false") + ")");

	return result;
}


bool CHTML::PerformRequest(string param)
{
	bool		result = false;

	MESSAGE_DEBUG("", "", "start");

	url = param;
	if(url.length())
	{
		ResetMetaValues();

		curl = curl_easy_init();
		if(curl)
		{
			// --- specify URL to get
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

			// --- send all data to this function
			// curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, bind(&CHTML::WriteMemoryCallback, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4));
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

			curl_easy_setopt(curl, CURLOPT_WRITEDATA, reinterpret_cast<void *>(this));

			// --- enable HTTP redirects 3xx
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

			/* some servers don't like requests that are made without a user-agent
			 field, so we provide one */
			curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

			// --- low security
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);

			if(isPostJSON())
			{
				MESSAGE_DEBUG("", "", "post json: " + GetPostJSON());

				curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
			    __slist_headers = curl_slist_append(__slist_headers, "Accept: application/json");
			    __slist_headers = curl_slist_append(__slist_headers, "Content-Type: text/plain;charset=UTF-8");
				curl_easy_setopt(curl, CURLOPT_HTTPHEADER, __slist_headers);
				curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, GetPostJSON().length());
				curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, GetPostJSON().c_str());
			}

			curlRes = curl_easy_perform(curl);

			if(isPostJSON())
			{
				curl_slist_free_all(__slist_headers);
				__slist_headers = nullptr;
			}

			if(curlRes == CURLE_OK)
			{
				// --- can't move it behind ParseHTMLPage, because ParseHTMLPage also calls culr_easy_cleanup
				curl_easy_cleanup(curl);


				if(isParsingRequired)
				{
					if(ParseHTMLPage())
					{
						result = true;
					}
					else
					{
						MESSAGE_ERROR("", "", "page parsing fail");
						result = false;
					}
				}
				else
				{
					// --- take downloaded result from GetContent() func
					result = true;
				}
			}
			else
			{
				MESSAGE_ERROR("", "", "curl_easy_perform() returned error (" + curl_easy_strerror(curlRes) + ")");

				result = false;

				curl_easy_cleanup(curl);
			}

		}
		else
		{
			{
				CLog	log;
				log.Write(ERROR, string("CHTML::") + string(__func__) + string("[") + to_string(__LINE__) +  "]:ERROR: curl_easy_init() returned null");
			}
			result = false;
		}
	}
	else
	{
		{
			CLog	log;
			log.Write(ERROR, string("CHTML::") + string(__func__) + string("[") + to_string(__LINE__) +  "]:ERROR: url is empty");
		}
		result = false;
	}

	MESSAGE_DEBUG("", "", "finish (return:" + (result ? "true" : "false") + ")");

	return result;
}

void CHTML::ResetMetaValues()
{
	MESSAGE_DEBUG("", "", "start");

	htmlPage = "";
	headTag = "";
	metaTitle = "";
	metaDescription = "";
	metaPreviewImageURL = "";
	folderID = "";
	filePrefix = "";
	fileExtension = "";

	MESSAGE_DEBUG("", "", "finish");
}

CHTML::~CHTML()
{
	MESSAGE_DEBUG("", "", "start");

	// --- Repeated calls to curl_global_init and curl_global_cleanup should be avoided. They should only be called once each.
	curl_global_cleanup();

	MESSAGE_DEBUG("", "", "finish");
}
