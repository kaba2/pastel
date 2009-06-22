#include "pastel/sys/string_tools.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/log.h"

#include <sstream>
#include <iomanip>

namespace Pastel
{

	PASTELSYS std::string trim(const std::string& that)
	{
		const integer begin = that.find_first_not_of(" \t\n");
		const integer end = that.find_last_not_of(" \t\n");

		std::string result;

		if (begin != std::string::npos &&
			end != std::string::npos)
		{
			result = that.substr(begin, (end - begin) + 1);

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

	PASTELSYS real stringToReal(const std::string& text)
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

		return number;
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
