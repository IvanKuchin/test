#ifndef __C_PRICE_SPELLING__H__
#define __C_PRICE_SPELLING__H__

#include <string>
#include <cmath>
#include <stack>
#include <vector>

#include "clog.h"

using namespace std;

#define MALE_GENDER             0
#define FEMALE_GENDER           1
#define PLURAL_MALE_GENDER      2
#define PLURAL_FEMALE_GENDER    3


class C_Price_Spelling
{
    private:
        vector<vector<string>>  spelling_magnitude = {  {"", "", ""}, 
                                                        {"тысяч", "тысяча", "тысячи"},
                                                        {"миллионов", "миллион", "миллиона"},
                                                        {"миллиардов", "миллиард", "миллиарда"},
                                                        {"трилионов", "трилион", "трилиона"},
                                                    };
        vector<string>  spelling_cents =            {"копеек", "копейка", "копейки"};
        vector<string>  spelling_dollars =          {"рублей", "рубль", "рубля"};
        vector<vector<string>>  spelling_hundreds = {
                                                         {"сто", "двести", "триста", "четыреста", "пятьсот", "шестьсот", "семьсот", "восемьсот", "девятьсот"},
                                                         {"сто", "двести", "триста", "четыреста", "пятьсот", "шестьсот", "семьсот", "восемьсот", "девятьсот"},
                                                         {"ста", "двухсот", "трехсот", "четырёхсот", "пятисот", "шестисот", "семисот", "восьмисот", "девятисот"},
                                                         {"ста", "двухсот", "трехсот", "четырёхсот", "пятисот", "шестисот", "семисот", "восьмисот", "девятисот"},
                                                    };  
        vector<vector<string>>  spelling_tenths =   {
                                                        {"десять", "двадцать", "тридцать", "сорок", "пятьдесят", "шестьдесят", "семьдесят", "восемьдесят", "девяносто"},
                                                        {"десять", "двадцать", "тридцать", "сорок", "пятьдесят", "шестьдесят", "семьдесят", "восемьдесят", "девяносто"},
                                                        {"десяти", "двадцати", "тридцати", "сорока", "пятьдесяти", "шестьдесяти", "семьдесяти", "восемьдесяти", "девяноста"},
                                                        {"десяти", "двадцати", "тридцати", "сорока", "пятьдесяти", "шестьдесяти", "семьдесяти", "восемьдесяти", "девяноста"},
                                                    };
        vector<vector<string>>  spelling_11_19 =    {
                                                        {"одиннадцать", "двенадцать", "тринадцать", "четырнадцать", "пятнадцать", "шестнадцать", "семнадцать", "восемнадцать", "девятнадцать"},
                                                        {"одиннадцать", "двенадцать", "тринадцать", "четырнадцать", "пятнадцать", "шестнадцать", "семнадцать", "восемнадцать", "девятнадцать"},
                                                        {"одиннадцати", "двенадцати", "тринадцати", "четырнадцати", "пятнадцати", "шестнадцати", "семнадцати", "восемнадцати", "девятнадцати"},
                                                        {"одиннадцати", "двенадцати", "тринадцати", "четырнадцати", "пятнадцати", "шестнадцати", "семнадцати", "восемнадцати", "девятнадцати"},
                                                    };
        vector<vector<string>>  spelling_digit =    {
                                                        {"один", "два", "три", "четыре", "пять", "шесть", "семь", "восемь", "девять"},
                                                        {"одна", "две", "три", "четыре", "пять", "шесть", "семь", "восемь", "девять"},
                                                        {"одного", "двух", "трёх", "четырёх", "пяти", "шести", "семи", "восеми", "девяти"},
                                                        {"одной", "двух", "трёх", "четырёх", "пяти", "шести", "семи", "восеми", "девяти"},
                                                    };
        vector<int>     gender_order_of_magnitude = {
                                                        MALE_GENDER, // --- index[0] - doesn't matter
                                                        FEMALE_GENDER, // --- thousand is female gender
                                                        MALE_GENDER, // --- million is male gender
                                                        MALE_GENDER, // --- milliard is male gender
                                                        MALE_GENDER, // --- trillion is male gender
                                                    };
        double price = 0;
        const long max = 1000000000000000 - 1;

    public:
    			C_Price_Spelling() {};
    			C_Price_Spelling(double param) : price(param) {};
        auto	Set(double param) {price = param; };
        auto	SpellNumber(long, int gender) -> string;
        auto	SpellRubles(long) -> string;
        auto	SpellCents(long) -> string;
        auto	Spelling() -> string;
        auto	GetWhole(double param) -> long;
        auto	GetFraction(double param) -> long;
        auto	DeclensionIndex(long number) -> int;
        auto	SpellOrderOfMagnitude(int order_of_magnitude, long number) -> string;
        auto	SpellUpToThousand(long, int gender) -> string;
        auto	SpellHundreds(long, int gender) -> string;
        auto	Spell11_19(long, int gender) -> string;
        auto	SpellTenths(long, int gender) -> string;
        auto	SpellDigit(long, int gender) -> string;
};

#endif 