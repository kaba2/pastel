#include "pastel/sys/biginteger_tools.h"

#include <sstream>
#include <string>
#include <algorithm>

namespace Pastel
{

	PASTELSYS std::ostream& operator<<(
		std::ostream& stream,
		const BigInteger& number)
	{
		if (number.zero())
		{
			stream << '0';
			return stream;
		}

		std::stringstream numberStream;
		BigInteger tempNumber(number);
		while (!tempNumber.zero())
		{
			BigInteger remainder(
				tempNumber % BigInteger(10));

			numberStream << remainder.asNative();

			tempNumber /= BigInteger(10);
		}

		if (number.negative())
		{
			numberStream << '-';
		}

		std::string numberText;
		numberStream >> numberText;

		std::reverse(numberText.begin(),
			numberText.end());

		stream << numberText;

		return stream;
	}

}
