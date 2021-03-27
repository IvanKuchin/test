#ifndef __HELPDESK__H__
#define __HELPDESK__H__

#include <sstream>
#include <sys/time.h>
#include <string.h>

#include "ccgi.h"
#include "cmysql.h"
#include "cuser.h"
#include "cmail.h"
#include "cstatistics.h"
#include "utilities_common.h"
#include "utilities.h"
#include "c_smsc.h"
#include "localy.h"

// --- Helpdesk part
#define	HELPDESK_USER_ROLE							"helpdesk"s
#define	HELPDESK_STATE_NEW							"new"s
#define	HELPDESK_STATE_ASSIGNED						"assigned"s
#define	HELPDESK_STATE_MONITORING					"monitoring"s
#define	HELPDESK_STATE_SOLUTION_PROVIDED			"solution_provided"s
#define	HELPDESK_STATE_CUSTOMER_PENDING				"customer_pending"s
#define	HELPDESK_STATE_COMPANY_PENDING				"company_pending"s
#define	HELPDESK_STATE_SEVERITY_CHANGED				"severity_changed"s
#define	HELPDESK_STATE_CLOSE_PENDING				"close_pending"s
#define	HELPDESK_STATE_CLOSED						"closed"s

#define HELPDESK_TICKET_CLOSE_PENDING_TIMEOUT		2			// --- in days
#define HELPDESK_TICKET_CUSTOMER_PENDING_TIMEOUT	5			// --- in days

#endif
