// Description: SFINAE machinery
// Documentation: sfinae_overloading.txt

#ifndef PASTELSYS_SFINAE_H
#define PASTELSYS_SFINAE_H

#include <type_traits>

#include "pastel/sys/type_traits/and.h"
#include "pastel/sys/type_traits/or.h"

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

	// A bug in Visual Studio 2015 CTP6 requires to
	// use RequiresC rather than Requires.
	template <typename... BoolSet>
	using Requires = EnableIfC<And<BoolSet...>::value>;

	template <bool Bool>
	using RequiresC = EnableIfC<Bool>;

	template <typename... BoolSet>
	using RequiresSome = EnableIfC<Or<BoolSet...>::value>;

}


#endif
