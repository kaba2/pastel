// Description: Mathematical constants
// Documentation: numbers.txt

#ifndef PASTELSYS_CONSTANTS_H
#define PASTELSYS_CONSTANTS_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/string/digit.h"

#include <sstream>
#include <string>
#include <cctype>

namespace Pastel
{

	//! Converts a text string to number.
	template <typename Real>
	requires std::is_floating_point_v<Real>
	Real stringAsReal(const std::string& number)
	{
		std::stringstream stream(number);
		
		Real result = 0;
		stream >> result;

		return result;
	}

	//! Returns pi constant.
	template <typename Real>
	requires std::is_floating_point_v<Real>
	Real constantPi()
	{
		return 3.1415926535897932384626433832795;
	}

	//! Returns neper constant.
	template <typename Real>
	requires std::is_floating_point_v<Real>
	Real constantNeper()
	{
		return 2.7182818284590452353602874713527;
	}

	//! Returns ln(2) constant.
	template <typename Real>
	requires std::is_floating_point_v<Real>
	Real constantLn2()
	{
		return 0.69314718055994530941723212145818;
	}

	//! Returns Euler-Mascheroni constant.
	template <typename Real>
	requires std::is_floating_point_v<Real>
	Real constantEulerMascheroni()
	{
		return 0.57721566490153286060651209008240;
	}

	/*
	template <typename Real>
	Real stringAsReal(
		const std::string& number,
		integer base)
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
			Real unit(Real(1) / base);
			for (integer i = dotIndex + 1;i < length;++i)
			{
				result += unit * digitAsInteger(word.at(i));
				unit /= base;
			}
		}
		{
			Real unit(1);
			for (integer i = dotIndex - 1;i > signIndex;--i)
			{
				result += unit * digitAsInteger(word.at(i));
				unit *= base;
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
	*/

}

#endif
