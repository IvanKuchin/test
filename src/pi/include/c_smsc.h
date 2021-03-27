#ifndef __C_SMSC__H__
#define __C_SMSC__H__
#include <curl/curl.h>
#include <string.h>
#include <libintl.h>

#include "utilities.h"	// --- split and CMysql
#include "cmysql.h"
#include "c_config.h"
#include "clog.h"
#include "localy.h"

using namespace std;

#define	SMSC_SENDER_NAME							GetDomain()					// --- check sender name in smsc.ru control panel
#define	SMSC_EXPIRATION								120
#define	SMSC_CHARSET								"utf-8"s

#define SMSC_DELIVERY_MODE							SMSC_NORMAL_DELIVERY
#define SMSC_NORMAL_DELIVERY						0
#define SMSC_FLASH_DELIVERY							1

class c_smsc
{
private:
	char					__SMSC_HTTPS = 1;// использовать протокол HTTPS
	char					__SMSC_POST = 0;// использовать метод POST

	// Константы для отправки SMS по SMTP 
	string			 		__SMTP_FROM = "api@smsc.ru";        // e-mail адрес отправителя 
	string			 		__SMTP_SERVER = "send.smsc.ru";    // адрес smtp сервера 
	string			 		__SMTP_LOGIN = "";            // логин для smtp сервера 
	string			 		__SMTP_PASSWORD = "";            // пароль для smtp сервера 

	CMysql					*db = nullptr;

	void _print_debug(string str);
	string _smsc_send_cmd(string cmd, string arg, string files);
	string	TrackSMS(string phones, string sms_id, string sms_cost, string sms_quantity, string sms_text, string current_balance);

public:
			c_smsc(CMysql *param) : db(param)	{};
	void	SetDB(CMysql *param)				{ db = param; };
	string	urlencode(string str);
	string	urldecode(string str);

	// Функция отправки SMS
	//
	// обязательные параметры:
	//
	// phones - список телефонов через запятую или точку с запятой
	// message - отправляемое сообщение
	//
	// необязательные параметры:
	//
	// translit - переводить или нет в транслит (1,2 или 0)
	// time - необходимое время доставки в виде строки (DDMMYYhhmm, h1-h2, 0ts, +m)
	// id - идентификатор сообщения. Представляет собой 32-битное число в диапазоне от 1 до 2147483647.
	// format - формат сообщения (0 - обычное sms, 1 - flash-sms, 2 - wap-push, 3 - hlr, 4 - bin, 5 - bin-hex, 6 - ping-sms, 7 - mms, 8 - mail, 9 - call, 10 - viber)
	// sender - имя отправителя (Sender ID).
	// query - строка дополнительных параметров, добавляемая в URL-запрос ("valid=01:00&maxsms=3&tz=2")
	// files - массив путей к файлам для отправки mms или e-mail сообщений
	//
	// возвращает <id>, <количество sms>, <стоимость>, <баланс> в случае успешной отправки
	// либо <id>, -<код ошибки> в случае ошибки

	string send_sms (string phones, string message, int translit, string time, int id, int format, string sender, string query, string files);

	// SMTP версия метода отправки SMS 
	// void send_sms_mail(string phones, string mes, int translit, string time, int id, int format, string sender);

	// Получение стоимости SMS 
	// 
	// обязательные параметры: 
	// 
	// phones - список телефонов через запятую или точку с запятой 
	// message - отправляемое сообщение 
	// 
	// необязательные параметры: 
	// 
	// translit - переводить или нет в транслит 
	// format - формат сообщения (0 - обычное sms, 1 - flash-sms, 2 - wap-push, 3 - hlr, 4 - bin, 5 - bin-hex, 6 - ping-sms, 7 - mms, 8 - mail, 9 - call) 
	// sender - имя отправителя (Sender ID) 
	// query - строка дополнительных параметров, добавляемая в URL-запрос ("list=79999999999:Ваш пароль: 123\n78888888888:Ваш пароль: 456") 
	// 
	// возвращает <стоимость>, <количество sms> либо 0, -<код ошибки> в случае ошибки 

	string get_sms_cost(string phones, string mes, int translit, int format, string sender, string query);

	// Проверка статуса отправленного SMS или HLR-запроса 
	//
	// id - ID cообщения или список ID через запятую
	// phone - номер телефона или список номеров через запятую
	// all - вернуть все данные отправленного SMS, включая текст сообщения (0,1 или 2)
	//
	//
	// для одиночного SMS-сообщения:
	// <статус>, <время изменения>, <код ошибки доставки>
	//
	// для HLR-запроса:
	// <статус>, <время изменения>, <код ошибки sms>, <код IMSI SIM-карты>, <номер сервис-центра>, <код страны регистрации>, <код оператора>,
	// <название страны регистрации>, <название оператора>, <название роуминговой страны>, <название роумингового оператора>
	//
	// при all = 1 дополнительно возвращаются элементы:
	// <время отправки>, <номер телефона>, <стоимость>, <sender id>, <название статуса>, <текст сообщения>
	//
	// при all = 2 дополнительно возвращаются элементы <страна>, <оператор> и <регион>
	//
	// если all = 0, то для каждого сообщения или HLR-запроса дополнительно возвращается <ID сообщения> и <номер телефона>
	//
	// если all = 1 или all = 2, то в ответ добавляется <ID сообщения>
	//
	// либо 0, -<код ошибки> в случае ошибки

	string get_status(string id, string phone, int all);

	// Получение баланса 
	// 
	// без параметров 
	// 
	// возвращает баланс в виде строки или пустую строку в случае ошибки 

	string get_balance(void);
};

#endif

