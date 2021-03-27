/*
 * libwebsockets-test-server - libwebsockets test implementation
 *
 * Copyright (C) 2010-2016 Andy Green <andy@warmcat.com>
 *
 * This file is made available under the Creative Commons CC0 1.0
 * Universal Public Domain Dedication.
 *
 * The person who associated a work with this deed has dedicated
 * the work to the public domain by waiving all of his or her rights
 * to the work worldwide under copyright law, including all related
 * and neighboring rights, to the extent allowed by law. You can copy,
 * modify, distribute and perform the work, even for commercial purposes,
 * all without asking permission.
 *
 * The test apps are intended to be adapted for use in your code, which
 * may be proprietary.  So unlike the library itself, they are licensed
 * Public Domain.
 */

#include <pthread.h>
#include "chat-server.h"
#include "utilities.h"

CPresenceCache				presenceCache;
CMysql						db;
// --- by default MySQL connection expire in 8 hours idle. To avoid SQL connection drop
// --- dbConnectionLifetime will reset connection each hour
double						dbConnectionInitTimestamp = GetSecondsSinceY2k(); 

int close_testing;
int max_poll_elements;
int debug_level = 7;

#ifdef EXTERNAL_POLL
struct lws_pollfd *pollfds;
int *fd_lookup;
int count_pollfds;
#endif
volatile int force_exit = 0;
struct lws_context *context;

#if defined(LWS_USE_POLARSSL)
#else
#if defined(LWS_USE_MBEDTLS)
#else
#if defined(LWS_OPENSSL_SUPPORT) && defined(LWS_HAVE_SSL_CTX_set1_param)
char crl_path[1024] = "";
#endif
#endif
#endif

/*
 * This mutex lock protects code that changes or relies on wsi list outside of
 * the service thread.  The service thread will acquire it when changing the
 * wsi list and other threads should acquire it while dereferencing wsis or
 * calling apis like lws_callback_on_writable_all_protocol() which
 * use the wsi list and wsis from a different thread context.
 */
pthread_mutex_t lock_established_conns;

/* http server gets cert and private key for SSL-encryption from this path */
// #define LOCAL_RESOURCE_PATH LIBWEBSOCKET_DATADIR"/libwebsockets-test-server"
#define LOCAL_RESOURCE_PATH "/tmp"
const char *resource_path = LOCAL_RESOURCE_PATH;

/*
 * multithreaded version - protect wsi lifecycle changes in the library
 * these are called from protocol 0 callbacks
 */

void test_server_lock(int care)
{
	if (care)
		pthread_mutex_lock(&lock_established_conns);
}
void test_server_unlock(int care)
{
	if (care)
		pthread_mutex_unlock(&lock_established_conns);
}

/*
 * This demo server shows how to use libwebsockets for one or more
 * websocket protocols in the same server
 *
 * It defines the following websocket protocols:
 *
 *  dumb-increment-protocol:  once the socket is opened, an incrementing
 *				ascii string is sent down it every 50ms.
 *				If you send "reset\n" on the websocket, then
 *				the incrementing number is reset to 0.
 *
 *  lws-mirror-protocol: copies any received packet to every connection also
 *				using this protocol, including the sender
 */

enum demo_protocols {
	/* always first */
	// PROTOCOL_HTTP = 0,

	PROTOCOL_DUMB_INCREMENT,
	STATUS_MESSAGE,
	PROTOCOL_MESSAGE,
	/* always last */
	DEMO_PROTOCOL_COUNT
};

/* list of supported protocols and callbacks */

static struct lws_protocols protocols[] = {
	/* first protocol must always be HTTP handler */

	// {
	// 	"http-only",		/* name */
	// 	callback_http,		/* callback */
	// 	sizeof (struct per_session_data__http),	/* per_session_data_size */
	// 	0,			/* max frame size / rx buffer */
	// },
	{
		"dumb-increment-protocol",
		callback_lws_dumb_increment,
		sizeof(struct per_session_data__dumb_increment),
		1024,
		1,			// --- protocol version, ignored by lws and connme
		nullptr		// --- user data, ignored by lws and connme
	},
	{
		"status-protocol",
		callback_lws_status,
		sizeof(struct per_session_data__lws_status),
		1024,
		1,			// --- protocol version, ignored by lws and connme
		nullptr		// --- user data, ignored by lws and connme
	},
	{
		"text-message-protocol",
		callback_lws_message,
		sizeof(struct per_session_data__message),
		1024,
		1,			// --- protocol version, ignored by lws and connme
		nullptr		// --- user data, ignored by lws and connme
	},
	{ NULL, NULL, 0, 0, 1, nullptr } /* terminator */
};

void *thread_dumb_increment(void *threadid)
{
	while (!force_exit) {
		/*
		 * this lock means wsi in the active list cannot
		 * disappear underneath us, because the code to add and remove
		 * them is protected by the same lock
		 */
		pthread_mutex_lock(&lock_established_conns);
		lws_callback_on_writable_all_protocol(context,
				&protocols[PROTOCOL_DUMB_INCREMENT]);
		pthread_mutex_unlock(&lock_established_conns);
		usleep(100000);
	}

	pthread_exit(NULL);
}

/*
 * this is just an example of parsing handshake headers, you don't need this
 * in your code unless you will filter allowing connections by the header
 * content
 */
void
dump_handshake_info(struct lws *wsi)
{
	int n = 0, len;
	char buf[256];
	const unsigned char *c;

	do {
		c = lws_token_to_string((lws_token_indexes)n);
		if (!c) {
			n++;
			continue;
		}

		len = lws_hdr_total_length(wsi, (lws_token_indexes)n);
		if (!len || len > (signed int) sizeof(buf) - 1) {
			n++;
			continue;
		}

		lws_hdr_copy(wsi, buf, sizeof buf, (lws_token_indexes)n);
		buf[sizeof(buf) - 1] = '\0';

		fprintf(stderr, "\t%s %s (len %d)\n", (char *)c, buf, len);
		n++;
	} while (c);
}


void *thread_service(void *threadid)
{
	{
		CLog			log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << "[" << __LINE__ << "]:" << __func__ << ": begin (threadid = " << (int)(long)threadid << ")";
		log.Write(DEBUG, ost.str());
	}

	while (lws_service_tsi(context, 50, (int)(long)threadid) >= 0 && !force_exit);

	{
		CLog			log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << "[" << __LINE__ << "]:" << __func__ << ": exit (threadid = " << (int)(long)threadid << ")";
		log.Write(DEBUG, ost.str());
	}

	pthread_exit(NULL);
}

void sighandler(int sig)
{
	{
		CLog			log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << "[" << __LINE__ << "]:" << __func__ << ": begin (signal = " << sig << ")";
		log.Write(DEBUG, ost.str());
	}

	force_exit = 1;

	lws_cancel_service(context);
	CloseAllTextMessageConnections();

	{
		CLog			log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << "[" << __LINE__ << "]:" << __func__ << ": end";
		log.Write(DEBUG, ost.str());
	}
}

static const struct lws_extension exts[] = {
	{
		"permessage-deflate",
		lws_extension_callback_pm_deflate,
		"permessage-deflate; client_no_context_takeover; client_max_window_bits"
	},
	{
		"deflate-frame",
		lws_extension_callback_pm_deflate,
		"deflate_frame"
	},

	{ NULL, NULL, NULL /* terminator */ }
};

static struct option options[] = {
	{ "help",	no_argument,		NULL, 'h' },
	{ "debug",	required_argument,	NULL, 'd' },
	{ "port",	required_argument,	NULL, 'p' },
	{ "ssl",	no_argument,		NULL, 's' },
	{ "allow-non-ssl",	no_argument,	NULL, 'a' },
	{ "interface",  required_argument,	NULL, 'i' },
	{ "closetest",  no_argument,		NULL, 'c' },
	{ "libev",  no_argument,		NULL, 'e' },
	{ "threads",  required_argument,	NULL, 'j' },
#ifndef LWS_NO_DAEMONIZE
	{ "daemonize", 	no_argument,		NULL, 'D' },
#endif
	{ "resource_path", required_argument,	NULL, 'r' },
	{ NULL, 0, 0, 0 }
};

int main(int argc, char **argv)
{
	struct lws_context_creation_info info;
	char interface_name[128] = "";
	const char *iface = NULL;
	pthread_t pthread_dumb, pthread_service[32];
	// char cert_path[1024];
	// char key_path[1024];
 	unsigned int threads = 1;
	int use_ssl = 1;
	void *retval;
	int opts = 0;
	int n = 0;
	char	__certificate_path[128];
	char	__private_key_path[128];
#ifndef _WIN32
	int syslog_options = LOG_PID | LOG_PERROR;
#endif
#ifndef LWS_NO_DAEMONIZE
 	int daemonize = 0;
#endif

    signal(SIGSEGV, crash_handler); 

	/*
	 * take care to zero down the info struct, he contains random garbage
	 * from the stack otherwise
	 */
	memset(&info, 0, sizeof info);
	info.port = 7681;

	pthread_mutex_init(&lock_established_conns, NULL);

	while (n >= 0) {
		n = getopt_long(argc, argv, "eci:hsap:d:Dr:j:", options, NULL);
		if (n < 0)
			continue;
		switch (n) {
		case 'j':
			threads = atoi(optarg);
			if (threads > ARRAY_SIZE(pthread_service)) {
				lwsl_err("Max threads %d\n",
					 ARRAY_SIZE(pthread_service));
				return 1;
			}
			break;
		case 'e':
			opts |= LWS_SERVER_OPTION_LIBEV;
			break;
#ifndef LWS_NO_DAEMONIZE
		case 'D':
			daemonize = 1;

			#ifndef _WIN32
			syslog_options &= ~LOG_PERROR;
			#endif
			break;
#endif
		case 'd':
			debug_level = atoi(optarg);
			break;
		case 's':
			use_ssl = 1;
			break;
			opts |= LWS_SERVER_OPTION_ALLOW_NON_SSL_ON_SSL_PORT;
			break;
		case 'p':
			info.port = atoi(optarg);
			break;
		case 'i':
			strncpy(interface_name, optarg, sizeof interface_name);
			interface_name[(sizeof interface_name) - 1] = '\0';
			iface = interface_name;
			break;
		case 'c':
			close_testing = 1;
			fprintf(stderr, " Close testing mode -- closes on "
					   "client after 50 dumb increments"
					   "and suppresses lws_mirror spam\n");
			break;
		case 'r':
			resource_path = optarg;
			printf("Setting resource path to \"%s\"\n", resource_path);
			break;
		case 'h':
			fprintf(stderr, "Usage: test-server "
					"[--port=<p>] [--ssl] "
					"[-d <log bitfield>] "
					"[--resource_path <path>]\n");
			exit(1);
		}
	}

#if !defined(LWS_NO_DAEMONIZE) && !defined(WIN32)
	/*
	 * normally lock path would be /var/lock/lwsts or similar, to
	 * simplify getting started without having to take care about
	 * permissions or running as root, set to /tmp/.lwsts-lock
	 */
	if (daemonize && lws_daemonize("/tmp/.lwsts-lock")) {
		fprintf(stderr, "Failed to daemonize\n");
		return 1;
	}
#endif


	if(db.Connect() < 0)
	{
		CLog	log(CHAT_LOG_FILE_NAME);

		log.Write(ERROR, string("[") + to_string(__LINE__) + string("]:") + __func__ + ": ERROR: Can not connect to mysql database");
		return(1);
	}

	presenceCache.SetDB(&db);

	signal(SIGINT, sighandler);

#ifndef IMAGEMAGICK_DISABLE
    Magick::InitializeMagick(NULL);
#endif

#ifndef _WIN32
	/* we will only try to log things according to our debug_level */
	setlogmask(LOG_UPTO (LOG_DEBUG));
	openlog("lwsts", syslog_options, LOG_DAEMON);
#endif

	/* tell the library what debug level to emit and to send it to syslog */
	lws_set_log_level(debug_level, lwsl_emit_syslog);
	lwsl_notice("libwebsockets test server pthreads - license LGPL2.1+SLE\n");
	lwsl_notice("(C) Copyright 2010-2016 Andy Green <andy@warmcat.com>\n");

#ifndef LWD_NO_DAEMONIZE
	if(daemonize) lwsl_notice("daemonize process");
#endif

	printf("Using resource path \"%s\"\n", resource_path);
#ifdef EXTERNAL_POLL
	max_poll_elements = getdtablesize();
	pollfds = malloc(max_poll_elements * sizeof (struct lws_pollfd));
	fd_lookup = malloc(max_poll_elements * sizeof (int));
	if (pollfds == NULL || fd_lookup == NULL) {
		lwsl_err("Out of memory pollfds=%d\n", max_poll_elements);
		return -1;
	}
#endif

	info.iface = iface;
	info.protocols = protocols;
	info.extensions = exts;

	info.ssl_cert_filepath = NULL;
	info.ssl_private_key_filepath = NULL;

/*
	if (use_ssl) {
		if (strlen(resource_path) > sizeof(cert_path) - 32) {
			lwsl_err("resource path too long\n");
			return -1;
		}
		sprintf(cert_path, "%s/libwebsockets-test-server.pem",
			resource_path);
		if (strlen(resource_path) > sizeof(key_path) - 32) {
			lwsl_err("resource path too long\n");
			return -1;
		}
		sprintf(key_path, "%s/libwebsockets-test-server.key.pem",
			resource_path);

		info.ssl_cert_filepath = cert_path;
		info.ssl_private_key_filepath = key_path;
	}
*/

	if(use_ssl)
	{
		if(!isFileExists(CHAT_CERTIFICATE))
		{
			MESSAGE_ERROR("chat-server", "", CHAT_CERTIFICATE + " file doesn't exist");
			return -1;
		}
		if(!isFileExists(CHAT_PRIVATE_KEY))
		{
			MESSAGE_ERROR("chat-server", "", CHAT_PRIVATE_KEY + " file doesn't exist");
			return -1;
		}

		opts |= LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;

		strcpy(__certificate_path, (CHAT_CERTIFICATE).c_str());
		strcpy(__private_key_path, (CHAT_PRIVATE_KEY).c_str());

		info.ssl_cert_filepath = __certificate_path;
		info.ssl_private_key_filepath = __private_key_path;
	}

	info.gid = -1;
	info.uid = -1;
	info.options = opts;
	info.count_threads = threads;
	info.extensions = exts;
	info.max_http_header_pool = 4;

	context = lws_create_context(&info);
	if (context == NULL) {
		lwsl_err("libwebsocket init failed\n");
		return -1;
	}

	/* start the dumb increment thread */

	n = pthread_create(&pthread_dumb, NULL, thread_dumb_increment, 0);
	if (n) {
		lwsl_err("Unable to create dumb thread\n");
		goto done;
	}

	/*
	 * notice the actual number of threads may be capped by the library,
	 * so use lws_get_count_threads() to get the actual amount of threads
	 * initialized.
	 */

	for (n = 0; n < lws_get_count_threads(context); n++)
		if (pthread_create(&pthread_service[n], NULL, thread_service,
				   (void *)(long)n))
			lwsl_err("Failed to start service thread\n");
		else
			lwsl_notice("pthread_create: %d created\n", n);

	/* wait for all the service threads to exit */

	while ((--n) >= 0)
		pthread_join(pthread_service[n], &retval);

	/* wait for pthread_dumb to exit */
	pthread_join(pthread_dumb, &retval);

done:
	lws_context_destroy(context);
	pthread_mutex_destroy(&lock_established_conns);

	lwsl_notice("libwebsockets-test-server exited cleanly\n");

#ifndef _WIN32
	closelog();
#endif

	return 0;
}
