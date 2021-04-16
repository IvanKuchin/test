#ifndef __ADMIN__H__
#define __ADMIN__H__

#include <sstream>
#include <stdexcept>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <Magick++.h>
#include <exception>

#include "clog.h"
// #include "cmenu.h"
#include "ccgi.h"
#include "cmail.h"
#include "cmysql.h"
#include "cuser.h"
#include "cexception.h"
#include "cstatistics.h"
#include "utilities_connme.h"
#include "locale.h"

// --- admin definitions
#define	ADMIN_LOG_FILE_NAME							string(LOGDIR) + DOMAIN_NAME + ".admin"

#endif