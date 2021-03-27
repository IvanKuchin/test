#ifndef __C_FLOAT__H__
#define __C_FLOAT__H__

#include <string>
#include <cstdlib>
#include <math.h>		// --- pow() in RoundWithPrecision
#include <ios>			// --- getloc()

#include "clog.h"
#include "utilities.h"

using namespace std;

// extern auto			CutTrailingZeroes(string number) -> string;

class c_float
{
	protected:
		double				val = 0;
		int					precision = 2;

		virtual auto		RoundWithPrecision(double num, int precision) -> double;
		auto				RoundWithPrecision(double num)			{ return RoundWithPrecision(num, precision); };
		auto 				FixRussianLocale(string param) -> string;
		string				GetStringValue() const;
		string				GetFormattedOutput() const;

	public:
					c_float()								: c_float(0, 2)			{};
					c_float(string param)					: c_float(param, 2)		{};
					c_float(double param)					: c_float(param, 2)		{};
					c_float(string param, int prec_param)	: c_float(stod_noexcept(param), prec_param) { };
					c_float(double param, int prec_param)	: precision(prec_param)	{ val = RoundWithPrecision(param, precision); };

		void		Set(double param) 						{ val = RoundWithPrecision(param, precision); };
		void		Set(string param);

		double		Get()	const 							{ return val; };
		double		GetPrecision()	const					{ return precision; };

		long		GetWhole()		const;
		long		GetFraction()	const;

		string		PrintPriceTag() const;
		string		PrintPriceTag_SpaceIfZero() const;

					operator double() 						{ return Get(); };
					operator string() const;

		c_float		operator+(const c_float&);
		c_float		operator-(const c_float&);
		c_float		operator*(const c_float&);
		c_float		operator/(const c_float&);
		c_float		operator/(const long int&);
		// c_float&	operator=(const c_float &);
};

ostream&	operator<<(ostream& os, const c_float &);

class c_float_with_rounding : public c_float
{
	protected:
		 auto		RoundWithPrecision(double num, int precision) -> double;

	public:
					c_float_with_rounding()								: c_float_with_rounding(0, 2)		{};
					c_float_with_rounding(string param)					: c_float_with_rounding(param, 2)	{};
					c_float_with_rounding(double param)					: c_float_with_rounding(param, 2)	{};
					c_float_with_rounding(string param, int prec_param) : c_float_with_rounding(stod_noexcept(param), prec_param)	{};
					c_float_with_rounding(double param, int prec_param) : c_float(param, prec_param)		{val = RoundWithPrecision(param, precision);};
					c_float_with_rounding(const c_float &param)			: c_float_with_rounding(param.Get(), param.GetPrecision()) {};
};


#endif
