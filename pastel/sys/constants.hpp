#ifndef PASTELSYS_CONSTANTS_HPP
#define PASTELSYS_CONSTANTS_HPP

#include "pastel/sys/constants.h"
#include "pastel/sys/ensure.h"

#include <cctype>

namespace Pastel
{

	template <typename Real>
	Real constant(const std::string& number)
	{
		ENSURE(!number.empty());

		std::size_t first = number.find_first_not_of(" ");

		ENSURE(first != std::string::npos);

		std::size_t last = number.find_first_of(" ", first);

		if (last == std::string::npos)
		{
			last = number.size();
		}

		// Examples of the allowed input:

		// "012345"
		// "-012345"
		// "+012345"
		// "012345."
		// "-012345."
		// "+012345."
		// " 012345.012345"
		// "-012345.012345"
		// "+012345.012345"
		// " .012345"
		// "-.012345"
		// "+.012345"

		std::string word(number.substr(first, last - first));

		ENSURE(!word.empty());

		integer length = word.size();
		integer signIndex = word.find_first_of("+-");
		integer dotIndex = word.find_first_of(".");

		if (signIndex == std::string::npos)
		{
			signIndex = -1;
		}

		if (dotIndex == std::string::npos)
		{
			dotIndex = length;
		}

		Real result(0);

		{
			Real unit(Real(1) / 10);
			for (integer i = dotIndex + 1;i < length;++i)
			{
				ENSURE(isdigit(word.at(i)));

				// PORTABILITY: relying on ascii
				// FIX

				result += unit * (word.at(i) - '0');
				unit /= 10;
			}
		}
		{
			Real unit(1);
			for (integer i = dotIndex - 1;i > signIndex;--i)
			{
				ENSURE(isdigit(word.at(i)));

				// PORTABILITY: relying on ascii
				// FIX

				result += unit * (word.at(i) - '0');
				unit *= 10;
			}
		}

		if (signIndex != -1)
		{
			if (word.at(signIndex) == '-')
			{
				result = -result;
			}
		}

		return result;
	}

	template <typename Real>
	Real constantPi()
	{
		static PASTEL_CONSTEXPR Real Pi(
			constant<Real>("3.1415926535897932384626433832795"));

		return Pi;
	}

	template <typename Real>
	Real constantNeper()
	{
		static PASTEL_CONSTEXPR Real Neper(
			constant<Real>("2.7182818284590452353602874713527"));

		return Neper;
	}

	template <typename Real>
	Real constantLn2()
	{
		static PASTEL_CONSTEXPR Real Ln2(
			constant<Real>("0.69314718055994530941723212145818"));

		return Ln2;
	}

	template <typename Real>
	Real constantEulerMascheroni()
	{
		static PASTEL_CONSTEXPR Real EulerMascheroni(
			constant<Real>("0.57721566490153286060651209008240"));

		return EulerMascheroni;
	}

}

#endif
