#ifndef __CHAT__SERVER__CONST__H__
#define __CHAT__SERVER__CONST__H__

// --- chat definitions
#define	CHAT_MAX_MESSAGE_QUEUE						512 - 1 // --- (-1) required to get all "1" in binary representation
#define	CHAT_LOG_FILE_NAME							string(LOGDIR) + DOMAIN_NAME + ".chat"
#define	CHAT_MAX_PACKET_SIZE						4096
#define CHAT_PRESENCE_CACHE_LIFETIME				60
#define	CHAT_MESSAGES_PER_PAGE						20
#define	CHAT_MESSAGES_REQUEST_PER_SEC				3
#define	CHAT_DB_CONNECTION_RESET_TIMEOUT			3600 * 2  // --- timeout in sec, resetting db connection, otherwise MySQL drop connection (show variables like "wait_timeout")
#define CHAT_IMAGE_NUMBER_OF_FOLDERS				512
#define	CHAT_IMAGE_MAX_WIDTH					 	640
#define	CHAT_IMAGE_MAX_HEIGHT						480
#define CHAT_CERTIFICATE							"/etc/ssl/certs/"s + DOMAIN_NAME + ".crt"
#define CHAT_PRIVATE_KEY							"/etc/ssl/private/"s + DOMAIN_NAME + ".key"

#define	CHAT_IMAGE_DIRECTORY						string(IMAGE_DIRECTORY) + string("chat")
#define	CHAT_IMAGE_NUMBER_OF_FOLDERS				512

#endif
