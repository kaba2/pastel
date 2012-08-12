#ifndef PASTEL_UNDERLINED_H
#define PASTEL_UNDERLINED_H

#include <string>

namespace Pastel
{

	class Underlined
		: public std::string
	{
	public:
		template <typename That>
		explicit Underlined(That&& that, char symbol_ = '-')
			: std::string(std::forward<That>(that))
			, symbol(symbol_)
		{
		}

		char symbol;
	};

}

#include <ostream>

namespace Pastel
{

	inline std::ostream& operator<<(
		std::ostream& stream, 
		const Underlined& that)
	{
		stream << (const std::string&)that << std::endl;
		for (integer i = 0;i < that.size();++i)
		{
			stream << that.symbol;
		}

		return stream;
	}

}

#endif
