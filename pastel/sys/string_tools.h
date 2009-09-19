// Description: String algorithms
// Documentation: basic_algorithms.txt

#ifndef PASTEL_STRING_TOOLS_H
#define PASTEL_STRING_TOOLS_H

#include "pastel/sys/syslibrary.h"
#include "pastel/sys/mytypes.h"

#include <string>
#include <vector>

namespace Pastel
{

	//! Concatenates a string with itself 'times' times.
	/*!
	Preconditions:
	times >= 0
	*/
	PASTELSYS std::string repeat(const std::string& that, integer times);

	//! Removes white-space from the ends.
	PASTELSYS std::string trim(const std::string& that);

	//! Returns the first word of the string.
	PASTELSYS std::string firstWord(const std::string& that);

	//! Removes all white-space from the string.
	PASTELSYS std::string removeWhiteSpace(const std::string& that);

	//! Collects begin- and end-symbol separated words.
	PASTELSYS bool separateBlocks(const std::string& that,
		char beginSymbol, char endSymbol,
		std::vector<std::string>& blocks);

	//! Collects the white-space separated words.
	/*!
	Time complexity: linear
	Exception safety: strong
	*/
	PASTELSYS void getWords(const std::string& sentence,
		std::vector<std::string>& words);

	//! Converts a string to an integer.
	/*!
	Time complexity: linear
	Exception safety: strong
	*/
	PASTELSYS integer stringToInteger(const std::string& text);

	//! Converts a string to a real number.
	/*!
	Time complexity: linear
	Exception safety: strong
	*/
	PASTELSYS real stringToReal(const std::string& text);

	//! Converts an integer to a string.
	/*!
	number:
	The integer to convert to a string.

	digits:
	The number of characters to use for the string.
	Negative values represent infinity.

	padding:
	The character to use for padding the string
	to the required length.

	Time complexity: linear
	Exception safety: strong

	If the integer takes less digits than required,
	the result is padded from left with 'padding' characters
	to the required length.

	If the integer is longer than allowed digits, the
	digits from the left end are thrown away.
	*/
	PASTELSYS std::string integerToString(integer number,
		integer digits = -1, char padding = '0');

	//! Converts a real number to a string.
	PASTELSYS std::string realToString(real number, integer digits = -1);

}

#include "pastel/sys/string_tools.hpp"

#endif
