// Description: String algorithms
// Documentation: string_algorithms.txt

#ifndef PASTELSYS_STRING_ALGORITHMS_H
#define PASTELSYS_STRING_ALGORITHMS_H

#include "pastel/sys/mytypes.h"

#include <string>
#include <vector>

namespace Pastel
{

	//! Returns whether the given string begins with another string.
	bool startsWith(const std::string& text, const std::string& prologue);

	//! Converts alphabets to lowercase in-place.
	std::string& makeLowercase(std::string& text);

	//! Converts alphabets to lowercase.
	std::string lowercase(const std::string& text);
	
	//! Converts alphabets to uppercase in-place.
	std::string& makeUppercase(std::string& text);

	//! Converts alphabets to uppercase.
	std::string uppercase(const std::string& text);

	//! Concatenates a string with itself 'times' times.
	/*!
	Preconditions:
	times >= 0
	*/
	std::string repeat(const std::string& that, integer times);

	//! Removes white-space from the beginning.
	std::string ltrim(const std::string& that, 
		integer* indexBegin = 0);

	//! Removes white-space from the end.
	std::string rtrim(const std::string& that, 
		integer* indexEnd = 0);

	//! Removes white-space from the ends.
	std::string trim(const std::string& that, 
		integer* indexBegin = 0, integer* indexEnd = 0);

	//! Returns the first word of the string.
	std::string firstWord(const std::string& that);

	//! Removes all white-space from the string.
	std::string removeWhiteSpace(const std::string& that);

	//! Collects begin- and end-symbol separated words.
	bool separateBlocks(const std::string& that,
		char beginSymbol, char endSymbol,
		std::vector<std::string>& blocks);

	//! Collects the white-space separated words.
	/*!
	Time complexity: linear
	Exception safety: strong
	*/
	void getWords(const std::string& sentence,
		std::vector<std::string>& words);

	//! Converts a string to an integer.
	/*!
	Time complexity: linear
	Exception safety: strong
	*/
	integer stringToInteger(const std::string& text);

	//! Converts a string to a dreal number.
	/*!
	Time complexity: linear
	Exception safety: strong

	Special meanings:
	inf, +inf: (dreal)Infinity()
	-inf: -(dreal)Infinity()
	nan, +nan, -nan: (dreal)Nan()	
	*/
	dreal stringToReal(const std::string& text, 
		integer* indexEnd = 0);

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
	std::string integerToString(integer number,
		integer digits = -1, char padding = '0');

	//! Converts a dreal number to a string.
	std::string realToString(dreal number, integer digits = -1);

}

#include "pastel/sys/string/string_algorithms.hpp"

#endif
