// Description: SFINAE machinery
// Documentation: sfinae_constraints.txt

#ifndef PASTELSYS_SFINAE_H
#define PASTELSYS_SFINAE_H

#include <type_traits>

#include "pastel/sys/type_traits/and.h"
#include "pastel/sys/type_traits/or.h"
#include "pastel/sys/type_traits/not.h"
#include "pastel/sys/type_traits/remove_cvref.h"

#define PASTEL_REMOVE_BRACKETS(x) typename Pastel::Deduction_::RemoveBrackets<void (x)>::Type

namespace Pastel
{

	template <typename T>
	concept Arithmetic_Concept = std::is_arithmetic_v<RemoveCvRef<T>>;

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

}

namespace Pastel
{

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

}

namespace Pastel
{

	template <typename Return>
	using Disable = 
		DisableIfC<true, Return>;

}

namespace Pastel
{

	// A bug in Visual Studio 2015 CTP6 requires to
	// use RequiresC rather than Requires.
	template <typename... BoolSet>
	using Requires = EnableIfC<And<BoolSet...>::value>;

	template <bool Bool>
	using RequiresC = EnableIfC<Bool>;

	template <typename... BoolSet>
	using RequiresSome = EnableIfC<Or<BoolSet...>::value>;

}

#include "pastel/sys/type_traits/identity.h"

namespace Pastel
{

	template <typename Type>
	using NoDeduction = Identity<Type>;

}

#endif
