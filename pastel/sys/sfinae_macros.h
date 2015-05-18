// Description: SFINAE machinery
// Documentation: sfinae_overloading.txt

#ifndef PASTELSYS_SFINAE_MACROS_H
#define PASTELSYS_SFINAE_MACROS_H

#include <type_traits>

namespace Pastel
{

	template <
		bool Condition,
		typename Return = int>
	using EnableIfC = 
		typename std::enable_if<Condition, Return>::type;

	template <
		typename Condition,
		typename Return = int>
	using EnableIf = 
		EnableIfC<Condition::value, Return>;

	template <
		typename Condition,
		typename Return = int>
	using DisableIf = 
		EnableIfC<!Condition::value, Return>;

	template <
		bool Condition,
		typename Return = int>
	using DisableIfC = 
		EnableIfC<!Condition, Return>;

	template <typename Return>
	using Disable = 
		DisableIfC<true, Return>;

}


#endif
