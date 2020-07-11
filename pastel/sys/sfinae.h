// Description: SFINAE machinery
// Documentation: sfinae_constraints.txt

#ifndef PASTELSYS_SFINAE_H
#define PASTELSYS_SFINAE_H

#include <type_traits>

#include "pastel/sys/type_traits/and.h"
#include "pastel/sys/type_traits/or.h"
#include "pastel/sys/type_traits/not.h"
#include "pastel/sys/type_traits/remove_cvref.h"
#include "pastel/sys/type_traits/identity.h"

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

	template <typename... BoolSet>
	using Requires = EnableIfC<And<BoolSet...>::value>;

	template <typename Type>
	using NoDeduction = Identity<Type>;

}

#endif
