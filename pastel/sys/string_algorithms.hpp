#ifndef PASTELSYS_STRING_ALGORITHMS_HPP
#define PASTELSYS_STRING_ALGORITHMS_HPP

#include "pastel/sys/string_algorithms.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/log.h"
#include "pastel/sys/constants.h"

#include <algorithm>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <functional>

namespace Pastel
{

	inline bool startsWith(const std::string& text, const std::string& prologue)
	{
		if (prologue.size() > text.size())
		{
			return false;
		}
		
		for (integer i = 0;i < prologue.size();++i)
		{
			if (text[i] != prologue[i])
			{
				return false;
			}
		}
		
		return true;
	}

	inline std::string& makeLowercase(std::string& text)
	{
		// See the makeUppercase() function for some notes.
		for (integer i = 0;i < text.size();++i)
		{
			text[i] = std::tolower(text[i]);
		}

		return text;
	}

	inline std::string lowercase(const std::string& text)
	{
		std::string result(text);
		return makeLowercase(result);
	}

	inline std::string& makeUppercase(std::string& text)
	{
		// See the makeUppercase() function for some notes.
		for (integer i = 0;i < text.size();++i)
		{
			text[i] = std::toupper(text[i]);
		}

		// I'd use the following, but it doesn't compile in GCC.
		// The problem may be in the definition of the
		// toupper() function (e.g. a macro).

		//std::transform(result.begin(), result.end(), 
		//	result.begin(), std::ptr_fun(&std::toupper));

		return text;
	}

	inline std::string uppercase(const std::string& text)
	{
		std::string result(text);
		return makeUppercase(result);
	}

	inline std::string repeat(const std::string& that, integer times)
	{
		std::string result;
		result.reserve(that.length() * times);

		for (integer i = 0;i < times;++i)
		{
			result += that;
		}

		return result;
	}

	inline std::string ltrim(const std::string& that, 
		integer *indexBegin)
	{
		std::string result;

		integer begin = that.find_first_not_of(" \t\n");
		if (begin == std::string::npos)
		{
			// Everything is trimmed away.
			begin = that.size();
		}
		else
		{
			result = that.substr(begin);
		}

		if (indexBegin)
		{
			*indexBegin = begin;
		}

		return result;
	}

	inline std::string rtrim(const std::string& that, 
		integer *indexEnd)
	{
		std::string result;

		integer end = that.find_last_not_of(" \t\n");
		if (end == std::string::npos)
		{
			// Nothing is trimmed away.
			result = that;
			end = that.size();
		}
		else
		{
			result = that.substr(0, end);
		}

		if (indexEnd)
		{
			*indexEnd = end;
		}

		return result;
	}

	inline std::string trim(const std::string& that, 
		integer* indexBegin, integer* indexEnd)
	{
		std::string result;

		integer begin = that.find_first_not_of(" \t\n");
		integer end = that.size();
		if (begin == std::string::npos)
		{
			// Everything is trimmed away.
			begin = that.size();
		}
		else
		{
			end = that.find_last_not_of(" \t\n");
			if (end == std::string::npos)
			{
				end = that.size();
			}

			result = that.substr(begin, (end - begin) + 1);
		}

		if (indexBegin)
		{
			*indexBegin = begin;
		}
		if (indexEnd)
		{
			*indexEnd = end;
		}

		return result;
	}

	inline std::string firstWord(const std::string& that)
	{
		integer begin = that.find_first_not_of(" \t\n");
		integer end = that.find_first_of(" \t\n", begin);
		if (end == std::string::npos)
		{
			end = that.size();
		}

		std::string result;


		if (begin != std::string::npos &&
			end != std::string::npos)
		{
			result = that.substr(begin, end - begin);

		}

		return result;
	}

	inline std::string removeWhiteSpace(const std::string& that)
	{
		std::string result;
		integer thatSize = that.size();
		for (integer i = 0;i < thatSize;++i)
		{

			if (that[i] != ' ' &&
				that[i] != '\t' &&
				that[i] != '\n')
			{
				result.push_back(that[i]);
			}
		}
		return result;
	}

	inline bool separateBlocks(const std::string& that,
		char beginSymbol, char endSymbol,
		std::vector<std::string>& blocks)
	{
		integer thatSize = that.size();
		integer position = 0;
		while (position < thatSize)
		{
			integer begin =
				that.find_first_of(beginSymbol, position);
			if (begin == std::string::npos ||
				begin + 1 == thatSize)
			{
				break;
			}
			++begin;

			integer end =
				that.find_first_of(endSymbol, begin);
			if (end == std::string::npos)
			{
				return false;
			}

			blocks.push_back(that.substr(begin, end - begin));

			position = end + 1;
		}

		return true;
	}


	inline void getWords(const std::string& sentence,
		std::vector<std::string>& words)
	{
		std::stringstream stream;
		stream << sentence;

		words.clear();

		std::string word;
		while (true)
		{
			stream >> word;
			if (!stream)
			{
				break;
			}
			words.push_back(word);
		}
	}

	inline integer stringToInteger(const std::string& text)
	{
		std::stringstream stream;
		stream << text;
		integer number = 0;
		stream >> number;
		if (!stream)
		{
			log() << "stringToInteger: could not convert '"
				<< text << "' to a number." << logNewLine;
		}

		return number;
	}

	inline real stringToReal(const std::string& text, integer* indexEnd)
	{
		integer trimmed = 0;
		std::string nText = lowercase(ltrim(text, &trimmed));

		real result = nan<real>();
		integer matchSize = trimmed;
		bool foundMatch = false;
		bool positive = true;

		if (startsWith(nText, "-"))
		{
			positive = false;
			nText = nText.substr(1);
			matchSize += 1;
		}
		else if (startsWith(nText, "+"))
		{
			nText = nText.substr(1);
			matchSize += 1;
		}
				
		if (startsWith(nText, "nan"))
		{
			matchSize += 3;
			result = nan<real>();
			foundMatch = true;
		}
		else if (startsWith(nText, "inf"))
		{
			matchSize += 3;
			result = infinity<real>();
			foundMatch = true;
		}
		else
		{
			std::stringstream stream;
			stream << nText;
			real number = 0;
			stream >> number;
			if (stream)
			{
				result = number;
				matchSize = stream.tellg();
				foundMatch = true;
			}
		}

		if (!foundMatch)
		{
			matchSize = 0;
		}

		if (indexEnd)
		{
			*indexEnd = matchSize;
		}

		if (!positive)
		{
			return -result;
		}

		return result;
	}

	inline std::string integerToString(integer number,
		integer digits, char padding)
	{
		std::stringstream stream;
		stream << number;

		std::string text;
		stream >> text;

		if (!stream)
		{
			log() << "integerToString: could not convert to a string"
				<< logNewLine;
		}

		std::string padText;
		if (digits > 0)
		{
			// Pad or trim.

			integer length = text.size();
			if (length < digits)
			{
				integer toPad = digits - length;
				for (integer i = 0;i < toPad;++i)
				{
					padText += padding;
				}
			}
			else if (length > digits)
			{
				text = text.substr(length - digits);
			}
		}

		return padText + text;
	}

	inline std::string realToString(real number,
		integer digits)
	{
		if (number == infinity<real>())
		{
			return std::string("inf");
		}
		else if (number == -infinity<real>())
		{
			return std::string("-inf");
		}
		else if (isNan(number))
		{
			return std::string("nan");
		}

		std::stringstream stream;
		if (digits > 0)
		{
			stream << std::setprecision(digits);
		}
		stream << number;

		std::string text;
		stream >> text;

		return text;
	}

}

#endif
