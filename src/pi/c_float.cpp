#include "c_float.h"

void c_float::Set(string param)
{
	if(param.empty()) Set(0);
	else
	{
		Set(stod_noexcept(param));
	}
}

auto c_float::FixRussianLocale(string param) -> string
{
	locale	loc;

	MESSAGE_DEBUG("", "", "start (" + param + ")")

	if(loc.name().find("ru"))
		if(param.find(".") != string::npos)
			param.replace(param.find("."), 1, ",");

	MESSAGE_DEBUG("", "", "finish (" + param + ")")

	return param;
}

double c_float::RoundWithPrecision(double num, int precision)
{
/*
	double	result;
	long	multiplier;

	multiplier = pow(10, precision);
	result = round(fma(num, multiplier, 0)) / multiplier;

	// MESSAGE_DEBUG("", "", to_string(num) + " -> " + to_string(result));
	return	result;
*/
	return	num;
}

long c_float::GetWhole() const
{
	auto	result				= 0l;
	long	factor				= pow(10, precision);
	auto	fract_part			= GetFraction();
	long	factorized_val		= lround(fma(val, factor, 0));

	result = lround(fma(factorized_val - fract_part, 1/double(factor), 0));


	MESSAGE_DEBUG("", "", "finish (result = " + to_string(result) + ")");

	return result;
}

long c_float::GetFraction() const
{
	long	factor				= pow(10, precision);
/*
	// ---
	// --- if you'll see fraction == 100 then consider use following code
	// --- https://stackoverflow.com/questions/3884232/modf-returns-1-as-the-fractional?answertab=active#tab-top
	// --- 
	double	inf					= copysign(std::numeric_limits<double>::infinity(), val);
	double	theNumberAfter		= nextafter(val, inf);
	double	epsilon				= theNumberAfter - val;
	double	factorized_epsilon	= fma(epsilon, factor/2,0);
	double	rounded_val	= lround(fma(val, factor, factorized_epsilon)) / factor;
*/
/*
	// --- former algorithm
	double	rounded_val			= lround(fma(val, factor, 0)) / factor;
	double	intpart;
	double	fractpart			= modf(rounded_val , &intpart);
	long 	result				= lround(fma(fractpart, factor, 0));
*/
	long 	result				= lround(fma(val, factor, 0)) % factor;

	if(result == factor)
	{
		MESSAGE_ERROR("", "", "fractional part extracting issue from " + to_string(val) + ", fraction part = " + to_string(result) + "");
		result = 0;
	}

	MESSAGE_DEBUG("", "", "finish (result = " + to_string(result) + ")");

	return result;

}

string c_float::GetStringValue() const
{
	long			multiplier = pow(10, precision);
	ostringstream	ost;

	ost.str("");
	ost.precision(precision);
	ost << fixed << (round(val * multiplier)) / multiplier; // --- this trick will print 0.01 from 0.005

	return ost.str();	
}

string c_float::GetFormattedOutput() const
{
	return CutTrailingZeroes(GetStringValue());
}

string c_float::PrintPriceTag() const
{
	auto	result = GetStringValue();

	// --- having spaces between thousands may trigger problems with invoice/act/vat price representation in Excel
	// --- or DB update (UPDATE xxx=123 456.56) will fail sql-query
	// --- for example: 12 000.55 will be presented as 12 in Excel
	{
		auto	frac_separator = result.find_first_not_of("0123456789");

		if(frac_separator == string::npos)
		{
			MESSAGE_ERROR("", "", "fail to find fraction separator in price_tag(" + result + ")");
		}
		else
		{
			// --- insert _spaces_ between thousands
/*
			while(frac_separator > 3)
			{
				frac_separator -= 3;
				result.insert(frac_separator, " ");
			}
*/
		}
	}	

	return result;
}

string c_float::PrintPriceTag_SpaceIfZero() const
{
	// --- replace " " to empty may result to overlap with next line. 
	// --- Empty symbol have 0px height, therefore next line will be printed over current
	// --- for example: subc invoice, subc act in summary after table "Total"-line will override "VAT"-line
	return Get() ? PrintPriceTag() : " ";
}

c_float::operator string() const
{
	return GetFormattedOutput();
}

c_float	c_float::operator+(const c_float &term2)
{
	c_float	result;

	result = RoundWithPrecision(fma(1, term2.Get(), Get()));

	MESSAGE_DEBUG("", "", to_string(Get()) + " + " + to_string(term2.Get()) + " = " + to_string(result.Get()));

	return result;
}

c_float	c_float::operator-(const c_float &term2)
{
	c_float	result;

	result = RoundWithPrecision(fma(-1, term2.Get(), Get()));

	MESSAGE_DEBUG("", "", to_string(Get()) + " - " + to_string(term2.Get()) + " = " + to_string(result.Get()));

	return result;
}

c_float	c_float::operator*(const c_float &term2)
{
	c_float	result;

	result = RoundWithPrecision(fma(Get(), term2.Get(), 0));

	MESSAGE_DEBUG("", "", to_string(Get()) + " * " + to_string(term2.Get()) + " = " + to_string(result.Get()));

	return result;
}

c_float	c_float::operator/(const c_float &term2)
{
	c_float	result;

	result = RoundWithPrecision(fma(Get(), 1 / term2.Get(), 0));

	MESSAGE_DEBUG("", "", to_string(Get()) + " / " + to_string(term2.Get()) + " = " + to_string(result.Get()));

	return result;
}

c_float	c_float::operator/(const long int &term2)
{
	c_float	result;

	result = RoundWithPrecision(Get() / term2);
	MESSAGE_DEBUG("", "", to_string(Get()) + " / " + to_string(term2) + " = " + to_string(result.Get()));

	return result;
}

ostream& operator<<(ostream& os, const c_float &var)
{
	os << string(var);
	return os;
}


double c_float_with_rounding::RoundWithPrecision(double num, int precision)
{

	double	result;
	long	multiplier;

	multiplier = pow(10, precision);
	result = round(fma(num, multiplier, 0)) / multiplier;

	MESSAGE_DEBUG("", "", to_string(num) + " (prec: " + to_string(precision) + ") -> " + to_string(result));
	return	result;

	// return	num;
}

