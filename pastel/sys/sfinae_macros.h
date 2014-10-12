// Description: SFINAE machinery
// Documentation: environment.txt

#ifndef PASTELSYS_SFINAE_MACROS_H
#define PASTELSYS_SFINAE_MACROS_H

#include <type_traits>

namespace Pastel
{

	template <
		typename Condition,
		typename Return = void>
	using EnableIf = 
		typename std::enable_if<Condition::value, Return>::type;

	template <
		bool Condition,
		typename Return = void>
	using EnableIfC = 
		typename std::enable_if<Condition, Return>::type;

	template <
		typename Condition,
		typename Return = void>
	using DisableIf = 
		typename std::enable_if<!Condition::value, Return>::type;

	template <
		bool Condition,
		typename Return = void>
	using DisableIfC = 
		typename std::enable_if<!Condition, Return>::type;

}


#endif
