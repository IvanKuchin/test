#ifndef __CHAT_SERVER_PROTOCOL_MESSAGE__H__
#define __CHAT_SERVER_PROTOCOL_MESSAGE__H__

#include <unordered_map>
#include <list>
#include <memory>
#include <cstddef>
#include <fstream>
#include <Magick++.h>

#include "ccgi.h"
#include "chat-server.h"
#include "clog.h"
#include "cmysql.h"
#include "utilities_connme.h"
#include "chat-server-jsonparser.h"
#include "chat-server-presencecache.h"
#include "chat-server-requestratelimiter.h"

using namespace std;

#define	MESSAGE_TYPE_TEXT	"text"
#define MESSAGE_TYPE_IMAGE	"image"

#define	KEEP_BR				false
#define	REMOVE_BR			true	
 
#endif 