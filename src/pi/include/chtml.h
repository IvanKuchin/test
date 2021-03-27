#ifndef __CHTML__H__
#define __CHTML__H__

#include <memory>
#include <string>
#include <functional>
#include <tuple>
#include <regex>
#include <curl/curl.h>

#include "clog.h"
#include "utilities.h"
#include "localy.h"

using namespace std;

class CHTML
{
private: 
	CURL							*curl;
	CURLcode						curlRes;
	string							url = "";
	string							metaTitle = "";
	string							metaDescription = "";
	string							metaPreviewImageURL = "";
	string							htmlPage = "";
	string							headTag = "";
	string							folderID = "", filePrefix = "", fileExtension = "";
	string							embedVideoURL = "";
	string							type = "feed";		// --- type of files (feed, temp) to save form request
														// --- depends on type different /images/xxxx folders stores images
	bool							isParsingRequired = true;

    struct curl_slist 				*__slist_headers = NULL;
	string							__post_json = "";

	string 							GetTagContent(string tag_name);
	string							GetAttributeValue(string tag_name, string attr_name, string name_content, string attr_content_name);

	void							ResetMetaValues();
	bool							ParseHTMLPage();
	bool							ExtractHEAD();
	bool							ExtractTITLE();
	bool							ExtractDESCRIPTION();
	bool							ExtractPreviewImage();
	tuple<string, string, string>	PickFileName();
	bool							DownloadFile(string urlPreview, FILE *f);
	bool							ExtractEmbedVideoURL();

	int 							GetNumberOfFoldersByType();
	string							GetDirectoryByType();

	bool							isPostJSON() { return __post_json.length(); };


public:
									CHTML();
									CHTML(string _type_);

	void							AppendToHTMLPage(string param)			{ htmlPage += param; };
	bool							PerformRequest(string param);

	void							SetType(const string &tmp)				{ type = tmp; }
	void							SetType(string &&tmp) 		noexcept	{ type = move(tmp); }

	void							SetPostJSON(const string &tmp)			{ __post_json = tmp; }
	void							SetPostJSON(string &&tmp)	noexcept	{ __post_json = move(tmp); }

	void							SetParsingDisable()			noexcept	{ isParsingRequired = false; };
	void							SetParsingEnable()			noexcept	{ isParsingRequired = true; };

	string							GetTitle()						const	{ return metaTitle; }
	string							GetDescription()				const	{ return metaDescription; }
	string							GetPreviewImageURL()			const	{ return metaPreviewImageURL; }
	string							GetPreviewImageFolder()			const	{ return folderID; };
	string							GetPreviewImagePrefix()			const	{ return filePrefix; };
	string							GetPreviewImageExtension()		const	{ return fileExtension; };
	string							GetPostJSON()					const	{ return __post_json; };
	string							GetContent() 					const	{ return htmlPage; };

	bool							isEmbedVideoHostedOnYoutube();
	string							GetEmbedVideoURL() { return embedVideoURL; };

									~CHTML();
};

#endif
