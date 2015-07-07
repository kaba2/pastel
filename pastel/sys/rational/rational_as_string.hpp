#ifndef PASTELSYS_RATIONAL_AS_STRING_HPP
#define PASTELSYS_RATIONAL_AS_STRING_HPP

#include "pastel/sys/rational/rational.h"
#include "pastel/sys/string/digit.h"

namespace Pastel
{

	template <typename Integer>
	std::string Rational<Integer>::asStringRatio() const
	{
		using Pastel::asString;

		std::string text = asString(m());
		if (n() != 1)
		{
			text += "/" + asString(n());
		}
		return text;
	}

	template <typename Integer>
	template <typename... ArgumentSet>
	std::string Rational<Integer>::asString(
		ArgumentSet&&... argumentSet) const
	{
		using Pastel::asString;

		integer base = PASTEL_ARG_S(base, 10);
		integer digits = PASTEL_ARG_S(digits, 3);
		bool showBase = PASTEL_ARG_S(showBase, false);
		bool shortenExact = PASTEL_ARG_S(shortenExact, true);
		Rounding rounding = PASTEL_ARG_S(rounding, Rounding::RoundUp);

		ENSURE_OP(base, >=, 2);
		ENSURE_OP(digits, >=, 0);

		// Handle the degenerate cases.
		switch(classify())
		{
			case NumberType::Infinity:
				return "inf";
			case NumberType::MinusInfinity:
				return "-inf";
			case NumberType::Nan:
				return "nan";
			default:
				// Fall-through
				;
		};

		// This is where the result is constructed.
		std::string text;
		if (isNegative())
		{
			// Print the minus-sign.
			text += "-";
		}		

		// Reduce to a non-negative number.
		Integer m = abs(m_);

		// Compute the integer-part.
		Integer wholes = m / n();

		// Compute the fractional part.
		m -= wholes * n();
	
		// Print the integer-part.
		text += asString(wholes);

		if ((!zero(m) || !shortenExact) && digits > 0)
		{
			// The number has a fractional part.

			// Print the fractional point.
			text += '.';

			// Print the fractional part.
			for (integer i = 0; i < digits && (!zero(m) || !shortenExact); ++i)
			{
				m *= base;

				integer d = (integer)(m / n());
				m -= d * n();

				text += integerAsDigit(d);
			}
		}

		// What we have now is a truncated version
		// of the number.
		if (!zero(m) && rounding == Rounding::RoundUp)
		{
			// Find out one digit more for rounding.
			m *= base;

			integer lastDigit = (integer)(m / n());
			if (lastDigit >= base / 2)
			{
				// Round up.
				bool carry = true;
				for (integer i = text.size() - 1;i >= 0 && carry;--i)
				{
					if (text[i] == '.')
					{
						// Skip the fractional point.
						continue;
					}

					integer digit = digitAsInteger(text[i]);
					++digit;
					carry = (digit == base);
					if (carry)
					{
						digit = 0;
					}

					text[i] = integerAsDigit(digit);
				}

				if (carry)
				{
					text = integerAsDigit(1) + text;
				}
			}
		}

		if (showBase)
		{
			// Print the base.
			text += '_' + asString(base);
		}

		return text;
	}

}

#endif
