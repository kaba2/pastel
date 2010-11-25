#include "pastel/sys/string_tools.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/log.h"
#include "pastel/sys/constants.h"
#include "pastel/sys/stdext_isnan.h"

#include <algorithm>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <functional>

namespace Pastel
{

	PASTELSYS bool startsWith(const std::string& text, const std::string& prologue)
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

	PASTELSYS std::string lowercase(const std::string& text)
	{
		std::string result(text);
		std::transform(result.begin(), result.end(), 
			result.begin(), std::ptr_fun(&std::tolower));
		return result;
	}

	PASTELSYS std::string uppercase(const std::string& text)
	{
		std::string result(text);
		std::transform(result.begin(), result.end(), 
			result.begin(), std::ptr_fun(&std::toupper));
		return result;
	}

	PASTELSYS std::string repeat(const std::string& that, integer times)
	{
		std::string result;
		result.reserve(that.length() * times);

		for (integer i = 0;i < times;++i)
		{
			result += that;
		}

		return result;
	}

	PASTELSYS std::string ltrim(const std::string& that, 
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

	PASTELSYS std::string rtrim(const std::string& that, 
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

	PASTELSYS std::string trim(const std::string& that, 
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

	PASTELSYS std::string firstWord(const std::string& that)
	{
		const integer begin = that.find_first_not_of(" \t\n");
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

	PASTELSYS std::string removeWhiteSpace(const std::string& that)
	{
		std::string result;
		const integer thatSize = that.size();
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

	PASTELSYS bool separateBlocks(const std::string& that,
		char beginSymbol, char endSymbol,
		std::vector<std::string>& blocks)
	{
		const integer thatSize = that.size();
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

			const integer end =
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

	PASTELSYS void getWords(const std::string& sentence,
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

	PASTELSYS integer stringToInteger(const std::string& text)
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

	PASTELSYS real stringToReal(const std::string& text, integer* indexEnd)
	{
		integer trimmed = 0;
		std::string nText = lowercase(ltrim(text, &trimmed));

		real result = 0;
		integer matchSize = trimmed;
		bool foundMatch = false;
		bool positive = true;

		if (nText == "-")
		{
			positive = false;
			nText = nText.substr(1);
			matchSize += 1;
		}
		else if (nText == "+")
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
			stream << text;
			real number = 0;
			stream >> number;
			if (!stream)
			{
				log() << "stringToReal: could not convert '"
					<< text << "' to a number." << logNewLine;
			}
			else
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

	PASTELSYS std::string integerToString(integer number,
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

			const integer length = text.size();
			if (length < digits)
			{
				const integer toPad = digits - length;
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

	PASTELSYS std::string realToString(real number,
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
		else if (StdExt::isNan(number))
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
