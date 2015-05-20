// Description: A number in scientific notation

#ifndef PASTELSYS_SCIENTIFIC_NOTATION_H
#define PASTELSYS_SCIENTIFIC_NOTATION_H

#include "pastel/sys/real/scientific_notation_fwd.h"

#include "pastel/sys/mytypes.h"

#include "pastel/sys/integer/native_integer.h"

#include <string>

namespace Pastel
{

	inline bool operator==(
		const ScientificNotation& left,
		const ScientificNotation& right)
	{
		return 
			left.negative == right.negative &&
			left.exponent == right.exponent &&
			left.mantissa == right.mantissa;
	}

	inline bool operator!=(
		const ScientificNotation& left,
		const ScientificNotation& right)
	{
		return !(left == right);
	}

	inline std::string asString(const ScientificNotation& scientific)
	{
		return std::string("(") + 
			(scientific.negative ? "-" : "+") + 
			", " +
			asString(scientific.mantissa) +
			", " +
			asString(scientific.exponent) +
			")";

		// return 
		// 	std::string(scientific.negative ? "-" : "") + 
		// 	"1." + 
		// 	Pastel::asString(scientific.mantissa) +
		// 	"e" +
		// 	Pastel::asString(scientific.exponent);
	}

}

#endif
