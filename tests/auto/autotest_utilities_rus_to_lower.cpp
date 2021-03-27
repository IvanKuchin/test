#include <string>
#include <iostream>
#include "ccgi.h"
#include "cmysql.h"
#include "cuser.h"
#include "cstatistics.h"
#include "utilities.h"
#include "utilities_common.h"
#include "localy.h"

using namespace std;

bool Test1()
{
	auto			result = true;
	auto			rus_alphabet = "Аа Бб Вв Гг Дд Ее Ёё Жж Зз Ии Йй Кк Лл Мм Нн Оо Пп Рр Сс Тт Уу Фф Хх Цц Чч Шш Щщ Ъъ Ыы Ьь Ээ Юю Яя"s;
	auto			rus_alphabet_upper = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"s;
	auto			rus_alphabet_lower = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя"s;

	setlocale(LC_ALL, "en_US.utf8");

	if(toLower(rus_alphabet_upper) == rus_alphabet_lower) {}
	else
	{
		result = false;
		cout << "failed on toLower us locale" << endl;
	}

	return result;
}

bool Test2()
{
	auto			result = true;
	auto			rus_alphabet = "Аа Бб Вв Гг Дд Ее Ёё Жж Зз Ии Йй Кк Лл Мм Нн Оо Пп Рр Сс Тт Уу Фф Хх Цц Чч Шш Щщ Ъъ Ыы Ьь Ээ Юю Яя"s;
	auto			rus_alphabet_upper = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"s;
	auto			rus_alphabet_lower = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя"s;

	setlocale(LC_ALL, "ru_RU.utf8");

	if(toLower(rus_alphabet_upper) == rus_alphabet_lower) {}
	else
	{
		result = false;
		cout << "failed on toLower ru locale" << endl;
	}

	return result;
}

int main(void)
{
	CStatistics		appStat;  // --- CStatistics must be firts statement to measure end2end param's
	CCgi			indexPage(EXTERNAL_TEMPLATE);
	CUser			user;
	CMysql			db;
	auto			testing_success = true;

	{ MESSAGE_DEBUG("", "", __FILE__); }
	signal(SIGSEGV, crash_handler);

	if(!Test1())
	{
		testing_success = false;
		cout << "Test1 failed" << endl;
	}

	if(testing_success)
		cout << "Test passed" << endl;

	return(0);
}

