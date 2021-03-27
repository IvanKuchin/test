#include "c_price_spelling.h"


long C_Price_Spelling::GetWhole(double val)
{
    double	intpart;
    long	result;

    modf (val , &intpart);
    result = lround(intpart);

    return result;
}

long C_Price_Spelling::GetFraction(double val)
{
    double	fractpart, intpart;
    long	result;

    fractpart = modf (val , &intpart);
    result = lround(fma(fractpart, 100, 0));

    return result;
}

auto C_Price_Spelling::DeclensionIndex(long number) -> int
{
    MESSAGE_DEBUG("", "", "start");

    auto result = 0;
    
    number = number % 100;
    if(((number % 10) == 0) || (((number % 10) >= 5) && ((number % 10) <= 9)) || ((number >= 5) && (number <=20)))
    {
        result = 0;
    }
    else if((number % 10) == 1)
    {
        result = 1;
    }
    else
    {
        result = 2;
    }
    
    MESSAGE_DEBUG("", "", "finish");

    return result;
}

auto C_Price_Spelling::SpellOrderOfMagnitude(int order_of_magnitude, long number) -> string
{
    MESSAGE_DEBUG("", "", "start");

    auto result = spelling_magnitude[order_of_magnitude][DeclensionIndex(number)];
    
    MESSAGE_DEBUG("", "", "finish");

    return result;
}

auto C_Price_Spelling::SpellHundreds(long number, int gender) -> string
{
    return spelling_hundreds[gender][number - 1];
}

auto C_Price_Spelling::SpellTenths(long number, int gender) -> string
{
    return spelling_tenths[gender][number - 1];
}

auto C_Price_Spelling::Spell11_19(long number, int gender) -> string
{
    MESSAGE_DEBUG("", "", "start (" + to_string(number) + ")");

    MESSAGE_DEBUG("", "", "finish");

    return spelling_11_19[gender][number - 11];
}

auto C_Price_Spelling::SpellDigit(long number, int gender) -> string
{
    return spelling_digit[gender][number - 1];
}


auto C_Price_Spelling::SpellUpToThousand(long number, int gender) -> string
{
    MESSAGE_DEBUG("", "", "start (" + to_string(number) + ", " + to_string(gender) + ")");

    auto    result = ""s;
    auto    hundreds = number / 100;
    auto    tenths = (number % 100) / 10;
    auto    digit = number % 10;
    auto    hundreds_spelled = ""s;
    auto    tenths_spelled = ""s;
    auto    digit_spelled = ""s;

    if(hundreds)                hundreds_spelled = SpellHundreds(hundreds, gender);
    if((tenths == 1) && digit)  tenths_spelled = Spell11_19(tenths * 10 + digit, gender);
    else
    {
        if(tenths)              tenths_spelled = SpellTenths(tenths, gender);
        if(digit)               digit_spelled = SpellDigit(digit, gender);
    }

    if(hundreds_spelled.length())
    {
        if(result.length()) result += " ";
        result = hundreds_spelled;
    }
    if(tenths_spelled.length())
    {
        if(result.length()) result += " ";
        result += tenths_spelled;
    }
    if(digit_spelled.length())
    {
        if(result.length()) result += " ";
        result += digit_spelled;
    }

    MESSAGE_DEBUG("", "", "finish");

    return result;
}

auto C_Price_Spelling::SpellNumber(long number, int gender) -> string
{
    MESSAGE_DEBUG("", "", "start");

    auto            result = ""s;

    if(number <= max)
    {
        auto            temp = number;
        stack<string>   spelling_stack;
        auto            order_of_magnitude = 0;
        
        while(temp)
        {
            auto spelling_part = temp % 1000;
            
            if(spelling_part)
            {
                auto spelled_order_of_magnitude = SpellOrderOfMagnitude(order_of_magnitude, spelling_part);
        
                if(spelled_order_of_magnitude.length()) spelling_stack.push(spelled_order_of_magnitude);
                spelling_stack.push((SpellUpToThousand(spelling_part, gender)));
            }
    
            ++order_of_magnitude;        
            gender = gender_order_of_magnitude[order_of_magnitude];
    
            temp = temp / 1000;
        }
        
        while(!spelling_stack.empty())
        {
            if(result.length()) result += " ";
            result += spelling_stack.top();
            spelling_stack.pop();
        }
    }
    else
    {
        MESSAGE_ERROR("", "", "number(" + to_string(number) + ") is too big for spelling");
    }

    MESSAGE_DEBUG("", "", "finish");

    return result;
}
 
auto C_Price_Spelling::SpellRubles(long numbers) -> string
{
    MESSAGE_DEBUG("", "", "start");

    auto   result = ""s;
    
    result = spelling_dollars[DeclensionIndex(numbers)];
    
    MESSAGE_DEBUG("", "", "finish");

    return result;
}
 
auto C_Price_Spelling::SpellCents(long numbers) -> string
{
    MESSAGE_DEBUG("", "", "start");

    auto   result = ""s;

    result = spelling_cents[DeclensionIndex(numbers)];
    
    MESSAGE_DEBUG("", "", "finish");

    return result;
}
 
auto C_Price_Spelling::Spelling() -> string
{
    MESSAGE_DEBUG("", "", "start");

    MESSAGE_DEBUG("", "", "start (" + to_string(price) + ")");

    auto result = ""s;
    auto whole = GetWhole(price);
    auto fraction = GetFraction(price);
    auto spell_whole = SpellNumber(whole, MALE_GENDER);
    auto spell_fraction = SpellNumber(fraction, FEMALE_GENDER);

    if(spell_whole.length())
        result += spell_whole + " " + SpellRubles(whole);
    if(spell_fraction.length())
    {
        if(result.length()) result += " ";
        result += spell_fraction + " " + SpellCents(fraction);
    }

    MESSAGE_DEBUG("", "", "finish");

    return result;
}
