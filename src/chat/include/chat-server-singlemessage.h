#ifndef __CHAT__SERVER__SINGLE__MESSAGE__H__
#define __CHAT__SERVER__SINGLE__MESSAGE__H__

#include <string>
#include "clog.h"

using namespace std;

class CSingleMessage {

private:
	string		requestType;
	string		id, requestID, toID, fromID, toType, fromType;
	string		message, messageType, messageStatus;
	string		eventTimestamp;
	double		secondsSince2000;

public:
					CSingleMessage() = default;
	// --- By default assign operator copy all member. 
	// --- It must be override in case of pointers or "deep copying"
	// CSingleMessage&	operator=(const CSingleMessage& src);

	string			GetID() const					{ return id; };
	void			SetID(string param)				{ id = param; };
	string			GetRequestType() const			{ return requestType; };
	void			SetRequestType(string param)	{ requestType = param; }
	string			GetMessage() const				{ return message; };
	void			SetMessage(string param)		{ message = param; };
	string			GetMessageType() const			{ return messageType; };
	void			SetMessageType(string param)	{ messageType = param; };
	string			GetMessageStatus() const		{ return messageStatus; };
	void			SetMessageStatus(string param)	{ messageStatus = param; };
	string			GetRequestID() const			{ return requestID; };
	void			SetRequestID(string param)		{ requestID = param; };
	string			GetToID() const					{ return toID; };
	void			SetToID(string param)			{ toID = param; };
	string			GetFromID() const				{ return fromID; };
	void			SetFromID(string param)			{ fromID = param; };
	string			GetToType() const				{ return toType; };
	void			SetToType(string param)			{ toType = param; };
	string			GetFromType() const				{ return fromType; };
	void			SetFromType(string param)		{ fromType = param; };
	string			GetEventTimestamp() const		{ return eventTimestamp; };
	void			SetEventTimestamp(string param)	{ eventTimestamp = param; };
	double			GetSecondsSince2000() const		{ return secondsSince2000; };
	void			SetSecondsSince2000(double param) { secondsSince2000 = param; };
		
		
					~CSingleMessage();
};

#endif
