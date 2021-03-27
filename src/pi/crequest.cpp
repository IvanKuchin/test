#include "cexception.h"
#include "crequest.h"

// --- file upload example
// POST /cgi-bin/index.cgi HTTP/1.1
// Host: mydomain.ru
// Connection: keep-alive
// Content-Length: 311726
// Accept: application/json, text/javascript, */*; q=0.01
// Origin: http://mydomain.ru
// X-Requested-With: XMLHttpRequest
// User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/45.0.2454.101 Safari/537.36
// Content-Type: multipart/form-data; boundary=----WebKitFormBoundarydt825G9wbuSO99vt
// Referer: http://mydomain.ru/edit_profile?rand=1759182532
// Accept-Encoding: gzip, deflate
// Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.6,en;q=0.4
// Cookie: sessid=253723801762733484377614362777192474855032305644181843370558; lng=ru
// Request Payload
// ------WebKitFormBoundarydt825G9wbuSO99vt
// Content-Disposition: form-data; name="files[]"; filename="cloud_strife_and_noctis_lucis_caelum_vs_sephiroth_by_siegxionhart-d6bmp50.jpg"
// Content-Type: image/jpeg
//
// ......
// ------WebKitFormBoundarydt825G9wbuSO99vt--

static vector<string> split(const string& s, const char& c)
{
	string buff{""};
	vector<string> v;

	for(auto n:s)
	{
		if(n != c) buff+=n;
		else if(n == c && buff != "")
		{
			v.push_back(buff);
			buff = "";
		}
	}
	if(buff.length()) v.push_back(buff);

	return v;
}

CURLMethod::CURLMethod() : paramCount(0)
{
}

CURLMethod::~CURLMethod()
{
}

CPost::CPost()
{
	char 	*tmpData = getenv("CONTENT_LENGTH");
	size_t	blocksReadFromSocket = 0;

	MESSAGE_DEBUG("", "", "start");

	if(tmpData == NULL)
	{
		CLog log;
		log.Write(ERROR, "CPost::" + string(__func__) + "[" + to_string(__LINE__) + "]: ERROR: environment variable CONTENT_LENGTH is not set.");
		paramCount = -1;
		return;
	}

	contentLength = atoi(tmpData);

	queryString = (char *)malloc(contentLength + 1);
	if(queryString == NULL)
	{
		CLog log;
		log.Write(ERROR, "CPost::" + string(__func__) + "[" + to_string(__LINE__) + "]: ERROR error in allocating memory (", tmpData, ") bytes");
		throw CException("error in allocating memory");
	}

	blocksReadFromSocket = fread(queryString, contentLength, 1, stdin);
	if(blocksReadFromSocket != static_cast<size_t>(1))
	{
		{
			CLog log;
			ostringstream	ost;
			ost.str("");
			ost << "CPost::" << string(__func__) << "[" << to_string(__LINE__) << "]: ERROR: partial read from socket (waited 1 block with " << contentLength << " bytes, read " << blocksReadFromSocket << " blocks)";
			log.Write(ERROR, ost.str());
		}
	}
	queryString[contentLength] = 0;

	MESSAGE_DEBUG("", "", "finish (size of POSTed data is " + to_string(contentLength) + " bytes)");

	paramCount = -1;
}

char* CPost::Binary_strstr(const char *where, const char *what, unsigned int c)
{
	unsigned int	count = c;
	char*		currPos = (char *)where;
	char*		beginPos = (char *)where;

	if(c == 0) return NULL;

	while((currPos = (char *)memchr(currPos, what[0], count - (currPos - beginPos))) != NULL)
	{
		if((currPos - beginPos + strlen(what)) > c)
			return NULL;
		if(memcmp(currPos, what, strlen(what)) == 0)
		{
			return (char *)currPos;
		}
		if((currPos - beginPos + strlen(what)) <= c)
			currPos = (char *)currPos + 1;
		else
			return NULL;
	}
	return NULL;
}

int CPost::CalculateVars()
{
    char	*tmp, *reminder;

    tmp = reminder = queryString;
    if(tmp == NULL)
    {
		CLog	log;
		log.Write(ERROR, "CPost::" + string(__func__) + "[" + to_string(__LINE__) + "]: ERROR: POST data is empty");
		return -1;
    }

    paramCount = 0;

    if(!isContentMultipart())
    {
		while(true)
		{
			if((tmp = strchr(tmp, '&')) == NULL)
				break;
			paramCount++;
			tmp++;
			reminder = tmp;
		}

		if(strlen(reminder) > 0)
				paramCount++;

		return paramCount;
    }
    // else if(strstr(tmp, "Content-Disposition:") != NULL)
    else if(strstr(tmp, GetBoundaryMarker().c_str()) != NULL)
    {
		char		*p1, *p2;

		p2 = queryString;
		while(1)
		{
			// p1 = Binary_strstr(p2, "Content-Disposition:", contentLength - (p2 - queryString));
			// if((p1 == NULL) || (p1 - queryString >= contentLength)) break;
			p1 = Binary_strstr(p2, GetBoundaryMarker().c_str(), contentLength - (p2 - queryString));
			if((p1 == NULL) || (p1 - queryString >= contentLength)) { paramCount--; break; }
			p2 = p1 + 1;
			paramCount++;
		}

    }
    return(-1);
}

int CPost::ParamCount()
{
    if(paramCount == -1)
		CalculateVars();
    return paramCount;
}

char *CPost::ParamName(int number)
{
    char	*tmp, *reminder;
    bool	isFound = false;
    int		i;
    char	*result = NULL;

    tmp = reminder = queryString;
    if(tmp == NULL)
    {
		CLog	log;
		log.Write(ERROR, "CPost::" + string(__func__) + "[" + to_string(__LINE__) + "]: ERROR: the POST data is empty (CPost::ParamName)");

		return NULL;
    }

    if(!isContentMultipart())
    {
		i = 0;

		while(!isFound)
		{
			if((tmp = strchr(tmp, '&')) == NULL)
			break;
			i++;
			tmp++;
			if(i != number) reminder = tmp; 
			else isFound = true;
		}

		if(!isFound && (strlen(reminder) > 0))
		{
			i++;
			if(i == number) isFound = true;
		}

		if(isFound)
		{
			tmp = strchr(reminder, '=');
			if(!tmp)
				tmp = reminder + strlen(reminder);

			result = (char *)malloc(tmp - reminder + 1);
			if(result == NULL)
			{
	        	MESSAGE_ERROR("", "", "memory allocation error");
				return NULL;
			}

			memset(result, 0, tmp - reminder + 1);
			memcpy(result, reminder, tmp - reminder);
		}
		else
		{
			result = (char *)malloc(1);
			if(result == NULL)
			{
	        	MESSAGE_ERROR("", "", "memory allocation error");
				return NULL;
			}
			memset(result, 0, 1);
		}
    }
    else if(strstr(tmp, "Content-Disposition:") != NULL)
    {
		char		*currPos, *p1, *p2;

		i = 0;
		currPos = queryString;

		while(1)
		{
			p1 = Binary_strstr(currPos, "Content-Disposition:", contentLength - (currPos - queryString));
			if(p1 == NULL) break;
			p1 = Binary_strstr(p1, "\"", contentLength - (p1 - queryString));
			p2 = Binary_strstr(p1 + 1, "\"", contentLength - (p1 - queryString) - 1);
			i++;

			if(i == number)
			{
				result = (char *)malloc(p2 - p1);
				if(result == NULL)
				{
					CLog	log;
					log.Write(ERROR, "CPost::" + string(__func__) + "[" + to_string(__LINE__) + "]: ERROR: memory allocation error");
					return NULL;
				}

				memset(result, 0, p2 - p1);
				memcpy(result, p1 + 1, p2 - p1 - 1);
			}
			currPos = p2;
		}
    }

    return result;
}

bool CPost::isContentMultipart()
{
	string				contentTypeString = getenv("CONTENT_TYPE");
	string::size_type	boundaryStart;

	// --- If CONTENT_TYPE having multipart
	if(contentTypeString.find("multipart") != string::npos)
	{
		if(GetBoundaryMarker().length() > 5) return true;

		if((boundaryStart = contentTypeString.find("boundary=")) != string::npos)
		{
			boundaryMarker = contentTypeString.substr(boundaryStart + strlen("boundary="));

			MESSAGE_DEBUG("", "", "multipart boundary marker: " + boundaryMarker);

			return true;
		}
	}

	return false;
}

string	CPost::GetBoundaryMarker()
{
	return boundaryMarker;
}

bool CPost::isFileName(int number)
{
	bool	result = false;
	char	*tmp;
	int	i = 0;

	if(!isContentMultipart()) { return false; }

	tmp = queryString;
	if(strstr(tmp, "Content-Disposition:") != NULL)
	{
		string		postData, dataVar;

		char		*currPos, *p1, *p2;

		i = 0;

		currPos = queryString;
		while(1)
		{
			p1 = Binary_strstr(currPos, "Content-Disposition:", contentLength - (currPos - queryString));
			if(p1 == NULL) break;
			p2 = Binary_strstr(p1, "\n", contentLength - (p1 - queryString));
			i++;

			if(i == number)
			{
				if(Binary_strstr(p1, "filename=", p2 - p1 - 1) != NULL)
					result = true;
			}

			currPos = p1 + 1;
		}

/*		postData = queryString;

		while(1)
		{
			p1 = postData.find("Content-Disposition:", p2);
			if(p1 == string::npos) break;
			p2 = postData.find("\n", p1);
			i++;

			if(i == number)
			{
				dataVar = postData.substr(p1, p2 - p1 - 1);
				if(dataVar.find("filename=") != string::npos)
					result = true;
			}

		}
*/
	}
	return result;
}

string CPost::GetFileName(int number)
{
	string	result;
	char	*tmp;
	int 	i = 0;

	{
		CLog	log;
		log.Write(DEBUG, "CPost::" + string(__func__) + "(order = " + to_string(number) + ")[" + to_string(__LINE__) + "]: start");
	}

	tmp = queryString;
	result = "";

	if(strstr(tmp, "Content-Disposition:") != NULL)
	{
		string		dataVar;
		char		*currPos, *p1, *p2;
		string::size_type	beginFileName, endFileName;
		bool		isFound = false;

		i = 0;
		currPos = queryString;

		while(!isFound)
		{
			p1 = Binary_strstr(currPos, "Content-Disposition:", contentLength - (currPos - queryString));
			if(p1 == NULL) break;
			p2 = Binary_strstr(p1, "\n", contentLength - (p1 - queryString));
			if(p2 == NULL) break;
			i++;

			if(i == number)
			{
				char		fileName[300];

				memset(fileName, 0, sizeof(fileName));
				if((unsigned int)(p2 - p1 - 1) < sizeof(fileName))
					memcpy(fileName, p1, p2 - p1 - 1);
				else
				{
					CLog	log;

					log.Write(ERROR, "CPost::" + string(__func__) + "[" + to_string(__LINE__) + "]: ERROR: file name too long !");
					throw CException("file name too long");
				}

				dataVar = fileName;
				if((beginFileName = dataVar.find("filename=\"")) == string::npos)
					break;
				beginFileName += 10;
				if((endFileName = dataVar.find("\"", beginFileName)) == string::npos)
					endFileName = dataVar.length();
				else
					endFileName--;

				result = dataVar.substr(beginFileName, endFileName - beginFileName + 1);
				if(result.rfind("\\") != string::npos) result = result.substr(result.rfind("\\") + 1);

				isFound = true;
			}

			currPos = p2;
		}
	}

/*	if(strstr(tmp, "Content-Disposition:") != NULL)
	{
		string		postData, dataVar;
		unsigned int	p1, p2 = 0, beginFileName, endFileName;

		i = 0;
		postData = queryString;

		while(1)
		{
			p1 = postData.find("Content-Disposition:", p2);
			if(p1 == string::npos) break;
			p2 = postData.find("\n", p1);
			i++;

			if(i == number)
			{
				dataVar = postData.substr(p1, p2 - p1 - 1);
				if((beginFileName = dataVar.find("filename=\"")) == string::npos)
					break;
				beginFileName += 10;
				if((endFileName = dataVar.find("\"", beginFileName)) == string::npos)
					endFileName = dataVar.length();
				else
					endFileName--;

				result = dataVar.substr(beginFileName, endFileName - beginFileName + 1);
			}

		}
	}
*/

	{
		CLog	log;
		log.Write(DEBUG, "CPost::" + string(__func__) + "(order = " + to_string(number) + ")[" + to_string(__LINE__) + "]: finish (result = " + result + ")");
	}

	return result;
}

char *CPost::ParamValue(int number)
{
    char	*tmp, *reminder;
    bool	isFound = false;
    int		i;
    char	*result = NULL;

    tmp = reminder = queryString;
    if(tmp == NULL)
    {
		CLog			log;
		ostringstream	ost;

		ost << "CPost::" << string(__func__) << "(" << number << ")[" << to_string(__LINE__) << "]: ERROR: environment variable QUERY_STRING is not set";
		log.Write(ERROR, ost.str());

		return NULL;
    }

    i = 0;

    if(!isContentMultipart())
    {
/*
    	simple HTTP POST handling
    	example:
	    	POST / HTTP/1.1
			content-type:application/x-www-form-urlencoded;charset=utf-8
			host: https://importexport.amazonaws.com
			content-length:207

			Action=GetStatus&SignatureMethod=HmacSHA256&JobId=JOBID&SignatureVersion=2&Version=2014-12-18
*/

		while(!isFound)
		{
			if((tmp = strchr(tmp, '=')) == NULL)
			break;
			i++;
			tmp++;
			reminder = tmp;
			if(i == number)
			isFound = true;
		}

		if(isFound)
		{
				tmp = strchr(reminder, '&');
				if(!tmp)
					tmp = reminder + strlen(reminder);

				result = (char *)malloc(tmp - reminder + 1);
				if(result == NULL)
				{
		        	MESSAGE_ERROR("", "", "memory allocation error");
					return NULL;
				}

				memset(result, 0, tmp - reminder + 1);
				memcpy(result, reminder, tmp - reminder);
		}
		else
		{
			result = (char *)malloc(1);
			if(result == NULL)
			{
	        	MESSAGE_ERROR("", "", "memory allocation error");
				return NULL;
			}
			memset(result, 0, 1);
		}
    }
    else if(strstr(tmp, "Content-Disposition:") != NULL)
    {
/*
    	handling multipart HTTP POST request
    	example:
	    	POST /cgi-bin/avataruploader.cgi HTTP/1.1
			Host: mydomain.ru
			Connection: keep-alive
			Content-Length: 505632
			Accept: application/json, text/javascript, * / *; q=0.01
			Origin: http://mydomain.ru
			X-Requested-With: XMLHttpRequest
			User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/45.0.2454.101 Safari/537.36
			Content-Type: multipart/form-data; boundary=----WebKitFormBoundary651GAAbkidp9Dwrr
			Referer: http://mydomain.ru/edit_profile?rand=9610271653
			Accept-Encoding: gzip, deflate
			Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.6,en;q=0.4
			Cookie: sessid=662085406002941332119817186324381589130730034168489361180812; lng=ru

			------WebKitFormBoundary651GAAbkidp9Dwrr
			Content-Disposition: form-data; name="files[]"; filename="1616285.jpg"
			Content-Type: image/jpeg

			......JFIF.....................

			------WebKitFormBoundary651GAAbkidp9Dwrr--

*/

		char			*currPos, *p1, *p2;
		unsigned int	newLineCharCount;

		i = 0;
		currPos = queryString;

		while(1)
		{
			p1 = Binary_strstr(currPos, "\r\n\r\n", contentLength - (currPos - queryString));
			if(p1 != NULL)
				newLineCharCount = 4;
			else
			{
				p1 = Binary_strstr(currPos, "\n\n", contentLength - (currPos - queryString));
				if(p1 != NULL)
					newLineCharCount = 2;
				else
					break;
			}
			p1 += newLineCharCount;
			p2 = Binary_strstr(p1, GetBoundaryMarker().c_str(), contentLength - (p1 - queryString));

			if(p2 == NULL) p2 = queryString + contentLength - 1; else p2 -= newLineCharCount; // >> 1; // --- removed due to extra 10 13 chars has been found on Windows uploading
			i++;
	/*
	{
		CLog	log;
		ostringstream ost;

		ost << "1) p2=" << (p2 - queryString) << " p1=" << (p1 - queryString) << " p2-p1=" << (p2-p1);
		log.Write(DEBUG, ost.str());
	}
	*/
			if(i == number)
			{
	//			valVar = postData.substr(p1, p2 - p1);

				result = (char *)malloc(p2 - p1 + 1);
				if(result == NULL)
				{
					CLog	log;
					ostringstream	ost;

					ost << "CPost::" << string(__func__) << "(" << number << ")[" << to_string(__LINE__) << "] line 638: ERROR memory allocation error. Need " << p2 - p1 + 1 << " bytes";
					log.Write(ERROR, ost.str());
					return NULL;
				}

				memset(result, 0, p2 - p1 + 1);
	//			memcpy(result, valVar.c_str(), p2 - p1);
				memcpy(result, p1, p2 - p1);
			}

			currPos = p2;
		}
    }
/*    else if(strstr(tmp, "Content-Disposition:") != NULL)
    {
	string		postData, valVar;
	unsigned int	p1, p2 = 0, newLineCharCount;

	i = 0;
	postData = queryString;

	while(1)
	{
		p1 = postData.find("\r\n\r\n", p2);
		if(p1 != string::npos)
			newLineCharCount = 4;
		else
		{
			p1 = postData.find("\n\n", p2);
			if(p1 != string::npos)
				newLineCharCount = 2;
			else
				break;
		}
		p1 += newLineCharCount;
		p2 = postData.find("-----------------------------", p1);

		if(p2 == string::npos) p2 = contentLength; else p2 -= newLineCharCount >> 1;
		i++;

{
	CLog	log;
	ostringstream ost;

	ost << "1) p2=" << p2 << " p1=" << p1 << " p2-p1=" << (p2-p1);
	log.Write(DEBUG, ost.str());
}

		if(i == number)
		{
//			valVar = postData.substr(p1, p2 - p1);

			result = (char *)malloc(p2 - p1 + 1);
			if(result == NULL)
			{
				CLog	log;
				log.Write(ERROR, "memory allocation error");
				return NULL;
			}

			memset(result, 0, p2 - p1 + 1);
//			memcpy(result, valVar.c_str(), p2 - p1);
			memcpy(result, (char *)(queryString + p1), p2 - p1);
		}

	}
    }
*/

    return result;
}

int CPost::ParamValueSize(int number)
{
    char	*tmp, *reminder;
    bool	isFound = false;
    int		i;
    char	*result = NULL;

    tmp = reminder = queryString;
    if(tmp == NULL)
    {
		CLog	log;
		log.Write(ERROR, "int CPost::ParamValueSize(int number): ERROR environment variable QUERY_STRING is not set");

		return -1;
    }

    i = 0;

    if(!isContentMultipart())
    {

	    {
			CLog	log;
			log.Write(DEBUG, "int CPost::ParamValueSize(int number): ");

			return -1;
	    }

		while(!isFound)
		{
			if((tmp = strchr(tmp, '=')) == NULL) break;
			i++;
			tmp++;
			reminder = tmp;
			if(i == number)
			isFound = true;
		}

		if(isFound)
		{
				tmp = strchr(reminder, '&');
				if(!tmp)
					tmp = reminder + strlen(reminder);

				return (tmp - reminder);
		}
		else
		{
			result = (char *)malloc(1);
			if(result == NULL)
			{
			CLog	log;
			log.Write(ERROR, "int CPost::ParamValueSize(int number): ERROR memory allocation error");
			return -1;
			}
			memset(result, 0, 1);
		}
    }
    else if(strstr(tmp, "Content-Disposition:") != NULL)
    {
		char		*currPos, *p1, *p2;
		unsigned int	newLineCharCount;

		i = 0;
		currPos = queryString;

		while(1)
		{
			p1 = Binary_strstr(currPos, "\r\n\r\n", contentLength - (currPos - queryString));
			if(p1 != NULL)
				newLineCharCount = 4;
			else
			{
				p1 = Binary_strstr(currPos, "\n\n", contentLength - (currPos - queryString));
				if(p1 != NULL)
					newLineCharCount = 2;
				else
					break;
			}
			p1 += newLineCharCount;
			p2 = Binary_strstr(p1, GetBoundaryMarker().c_str(), contentLength - (p1 - queryString));

			if(p2 == NULL) p2 = queryString + contentLength - 1; else p2 -= newLineCharCount; // >> 1; // --- removed due to extra 10 13 chars has been found on Windows uploading
			i++;

			if(i == number)
			{
				return (p2 - p1);
			}

			currPos = p2;
		}
    }
    return -1;
}

CPost::~CPost()
{
    if(queryString != NULL)
    {
		free(queryString);
		queryString = NULL;
    }
}

int CGet::CalculateVars()
{
	auto queryString = getenv("QUERY_STRING");

    if(queryString == NULL)
    {
    	MESSAGE_ERROR("", "", "environment variable QUERY_STRING is not set");
    }
    else
    {
    	params = split(queryString, '&');
    	paramCount = params.size();

		return paramCount;
    }

	return -1;
}

int CGet::ParamCount()
{
    if(paramCount == -1) CalculateVars();

    return paramCount;
}

char *CGet::GetParamToken(unsigned int number, unsigned int token_idx)
{
    char	*result = NULL;
    auto    name = ""s;

    if(number && (number <= params.size()))
    {
        auto    key_value = split(params[number - 1], '=');

        if(token_idx < key_value.size())
        {
            name = key_value[token_idx];
        }
        else
        {
            MESSAGE_DEBUG("", "", "HTTP(S) key_value doesn't have either key or value: " + params[number - 1]);
        }
    }
    else
    {
        MESSAGE_ERROR("", "", "requested HTTP(S) param number should be 0 < " + to_string(number) + " <= " + to_string(params.size()));
    }

    if(name.length())
    {
    	result = (char *)malloc(name.length() + 1);
    	if(result == NULL)
        {
            MESSAGE_ERROR("", "", "memory allocation error");

            return NULL;
        }

    	strcpy(result, name.c_str());
    }
    else
    {
    	result = (char *)malloc(1);
    	if(result == NULL)
    	{
        	MESSAGE_ERROR("", "", "memory allocation error");

    	    return NULL;
    	}
    	memset(result, 0, 1);
    }

    return result;
}



CRequest::CRequest() : url(NULL)
{
}

void CRequest::RegisterURLVariables(CVars *v, CFiles *f)
{
    char	*methodType;
    vars = v;
    files = f;

    methodType = getenv("REQUEST_METHOD");

    if(methodType == NULL)
    {
		CLog	log;
        log.Write(ERROR, "CRequest::" + string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: environment variable REQUEST_METHOD is not set");

		return;
    }

    if(strstr(methodType, "GET") != 0)
    {
    	MESSAGE_DEBUG("", "", "HTTP(S) method GET");

		url = new CGet();
    }
    else if(strstr(methodType, "POST") != 0)
    {
    	MESSAGE_DEBUG("", "", "HTTP(S) method POST");

		url = new CPost();
    }
    else if(strstr(methodType, "HEAD") != 0)
    {
    	MESSAGE_DEBUG("", "", "HTTP(S) method HEAD");

		url = new CGet();
    }
    else
    {
    	MESSAGE_ERROR("", "", "Unknown HTTP(S) method(" + methodType + ")");
		throw CException("Unknown HTTP method");
    }

    requestURI = getenv("REQUEST_URI");
    if(requestURI == NULL)
    {
    	MESSAGE_ERROR("", "", "environment variable REQUEST_URI is not set");
		throw CException("environment variable REQUEST_URI is not set");
    }

    for(auto i = 1; i <= (*url).ParamCount(); i++)
    {
		char *name = url->ParamName(i);
		char *value = url->ParamValue(i);

		if(url->isFileName(i))
		{
			MESSAGE_DEBUG("", "", "HTTP POST parameter #" + to_string(i) + " is filename [" + url->GetFileName(i) + "], size [" + to_string(url->ParamValueSize(i)) + "]");

			files->Add(url->GetFileName(i), value, url->ParamValueSize(i));
		}
		else
		{
			string s_name(name);
			string s_value(value);

			s_name = WebString(s_name);
			s_value = WebString(s_value);

			MESSAGE_DEBUG("", "", "HTTP parameter #" + to_string(i) + " [" + s_name + "=" + (s_value) + "]");

			vars->Add(s_name, s_value);

			if(name != NULL) free(name);
			if(name != NULL) free(value);
		}
    }
}

string CRequest::WebString(string str)
{
    string		result(str), result1;
    string::size_type	findPos, findPos1, findPos2, firstPos = 0;
    string		table("0123456789ABCDEF");
    unsigned int	first, second;
    char		resCh;

    findPos = 0;
    while((findPos = result.find("+", findPos)) != string::npos)
    {
		result.replace(findPos++, 1, " ");
    }

    while((findPos = result.find("%", firstPos)) != string::npos)
    {
		char	ch1, ch2;

		result1 += result.substr(firstPos, findPos - firstPos);

		firstPos = findPos + 1;
		ch1 = result[findPos + 1];
		ch2 = result[findPos + 2];

		if(((ch1 >= '0') && (ch1 <= '9')) || ((ch1 >= 'A') && (ch1 <= 'F')))
		{}
		else
		    continue;
		if(((ch2 >= '0') && (ch2 <= '9')) || ((ch2 >= 'A') && (ch2 <= 'F')))
		{}
		else
		    continue;

		if((ch1 >= '0') && (ch1 <= '9'))
		    first = ch1 - '0';
		else
		    first = ch1 - 'A' + 10;

		if((ch2 >= '0') && (ch2 <= '9'))
		    second = ch2 - '0';
		else
		    second = ch2 - 'A' + 10;

		resCh = (first << 4) + second;

		result1 += resCh;
		firstPos += 2;
    }
    result1 += result.substr(firstPos, result.length() - firstPos);

    return result1;
}

CRequest::~CRequest()
{
    if(url != NULL)
        delete(url);
}
