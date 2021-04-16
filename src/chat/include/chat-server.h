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
#ifndef __CHAT__SERVER__H__
#define __CHAT__SERVER__H__

#if defined(_WIN32) && defined(EXTERNAL_POLL)
#define WINVER 0x0600
#define _WIN32_WINNT 0x0600
#define poll(fdArray, fds, timeout)  WSAPoll((LPWSAPOLLFD)(fdArray), (ULONG)(fds), (INT)(timeout))
#endif

#include "lws_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <Magick++.h>

#include "libwebsockets.h"

#include "chat-server-const.h"
#include "chat-server-presencecache.h"
#include "chat-server-singlemessage.h"

#include "cexception.h"
#include "cuser.h"
#include "cmysql.h"
#include "localy.h"


#ifdef _WIN32
#include <io.h>
#include "gettimeofday.h"
#else
#include <syslog.h>
#include <sys/time.h>
#include <unistd.h>
#endif

using namespace std;

extern int close_testing;
extern int max_poll_elements;

#ifdef EXTERNAL_POLL
extern struct lws_pollfd *pollfds;
extern int *fd_lookup;
extern int count_pollfds;
#endif
extern volatile int force_exit;
extern struct lws_context *context;
extern const char *resource_path;
#if defined(LWS_OPENSSL_SUPPORT) && defined(LWS_HAVE_SSL_CTX_set1_param)
extern char crl_path[1024];    /* Flawfinder: ignore */
#endif

extern void test_server_lock(int care);
extern void test_server_unlock(int care);

#ifndef __func__
#define __func__ __FUNCTION__
#endif

struct per_session_data__http {
	lws_filefd_type fd;
	char post_string[256];    /* Flawfinder: ignore */
#ifdef LWS_WITH_CGI
	struct lws_cgi_args args;
#endif
#if defined(LWS_WITH_CGI) || !defined(LWS_NO_CLIENT)
	int reason_bf;
#endif
	unsigned int client_finished:1;
};

/*
 * one of these is auto-created for each connection and a pointer to the
 * appropriate instance is passed to the callback in the user parameter
 *
 * for this example protocol we use it to individualize the count for each
 * connection.
 */

struct per_session_data__message {
	struct lws 		*wsi;
	CUser			*user;
	CSingleMessage	*messageList[CHAT_MAX_MESSAGE_QUEUE];
	char			sessID[1024];    /* Flawfinder: ignore */
	int				ringBufferHead;
	int				ringBufferTail;

	// --- packet reassembly
	char			*packetReassemble;
	int				packetSize;
	unsigned int	packetAllocatedSize;

	// --- connected list
	struct 			per_session_data__message	*nextConnection;

	// --- status purpose only
	struct timeval	tv_established;
	char 			ip[270];    /* Flawfinder: ignore */
	char 			user_agent[512];    /* Flawfinder: ignore */

	// --- close flag (can be used only from admin console, CNX rate-limiting or "stuck" connections [without user assigned])
	// --- flag checking in LWS_CALLBACK_SERVER_WRITEABLE and in case=true connection must be closed
	bool			closeFlag;
};

struct per_session_data__dumb_increment {
	int number;
};

struct per_session_data__lws_mirror {
	struct lws *wsi;
	int ringbuffer_tail;
};

struct per_session_data__echogen {
	size_t total;
	size_t total_rx;
	int fd;
	int fragsize;
	int wr;
};

struct per_session_data__lws_status {
	struct per_session_data__lws_status *list;
	// int last;
	struct timeval tv_established;
	char ip[270];    /* Flawfinder: ignore */
	char user_agent[512];    /* Flawfinder: ignore */
	// const char *pos;
	// int len;
};

extern bool CloseAllTextMessageConnections();

extern int
callback_lws_dumb_increment(struct lws *wsi, enum lws_callback_reasons reason,
			void *user, void *in, size_t len);
extern int
callback_lws_message(struct lws *wsi, enum lws_callback_reasons reason,
			void *user, void *in, size_t len);
extern int
callback_lws_status(struct lws *wsi, enum lws_callback_reasons reason,
		    void *user, void *in, size_t len);


extern void
dump_handshake_info(struct lws *wsi);


#endif
