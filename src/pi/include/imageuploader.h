#ifndef __IMAGEUPLOADER__H__
#define __IMAGEUPLOADER__H__

#include <sstream>
#include <stdexcept>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <fstream>
#include <Magick++.h>
#include <sys/resource.h>

#include "localy.h"
#include "ccgi.h"
#include "cvars.h" 
#include "clog.h"
#include "cmysql.h"
#include "cexception.h"
#include "cactivator.h"
#include "cuser.h"
#include "cmail.h"
#include "cstatistics.h"
#include "cvideoconverter.h"
#include "utilities_common.h"

#define	MESSAGE_HAVENO_MEDIA			1
#define	MESSAGE_HAVING_VIDEO 			2
#define	MESSAGE_HAVING_IMAGE 			3
#define	MESSAGE_HAVING_YOUTUBE_VIDEO 	4

#define	FILETYPE_UNDEFINED				1
#define	FILETYPE_VIDEO		 			2
#define	FILETYPE_IMAGE		 			3
#define	FILETYPE_YOUTUBE_VIDEO 			4

#endif
