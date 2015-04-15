// Description: Concept-checking

#ifndef PASTELSYS_CONCEPT_H
#define PASTELSYS_CONCEPT_H

#include "pastel/sys/sfinae_macros.h"
#include "pastel/sys/concept/refines.h"
#include "pastel/sys/concept/models.h"
#include "pastel/sys/type_traits/and.h"
#include "pastel/sys/type_traits/or.h"

#include <type_traits>

#define PASTEL_CONCEPT_CHECK(Type, Concept) \
	static_assert(Pastel::Models<Type, Concept>::value, #Type " is not a model of " #Concept ".")

#define PASTEL_CONCEPT_CHECK_DIRECT(Type, Concept) \
	static_assert(Pastel::Models_Directly<Type, Concept>::value, #Type " is not a direct model of " #Concept ".")

#define PASTEL_CONCEPT_CHECK_BASE(Type, Concept) \
	static_assert(Pastel::Models_Base<Type, Concept>::value, #Type " is not a base model of " #Concept ".")

#define PASTEL_CONCEPT_REJECT(Type, Concept) \
	static_assert(!Pastel::Models<Type, Concept>::value, #Type " is a model of " #Concept ", although it should not be.")

#define PASTEL_CONCEPT_REJECT_DIRECT(Type, Concept) \
	static_assert(!Pastel::Models_Directly<Type, Concept>::value, #Type " is a direct model of " #Concept ", although it should not be.")

#define PASTEL_CONCEPT_REJECT_BASE(Type, Concept) \
	static_assert(!Pastel::Models_Base<Type, Concept>::value, #Type " is a base model of " #Concept ", although it should not be.")

namespace Pastel
{

	//! List expressions in a concept-check.
	/*!
	This function exists to avoid comma-separated
	expressions from being interpreted as being
	chained by the comma-operator.
	*/
	template <typename... TypeSet>
	void conceptCheck(TypeSet&&... that);

	// A bug in Visual Studio 2015 CTP6 requires to
	// use EnableIfC rather than EnableIf.
	template <typename... BoolSet>
	using Requires = EnableIfC<And<BoolSet...>::value>;

	template <typename... BoolSet>
	using RequiresSome = EnableIfC<Or<BoolSet...>::value>;

	namespace Concept
	{

		//! Checks whether 'that' has type Required.
		template <
			typename Required,
			typename Type,
			typename = EnableIf<std::is_same<Type, Required>>
			>
		bool hasType(Type&& that);

		//! Checks whether Type is convertible to Required.
		template <
			typename Required, 
			typename Type,
			EnableIf<std::is_convertible<Type, Required>> = 0
			>
		bool convertsTo(Type&& that);

		template <typename Type, typename Concept>
		auto models()
			-> EnableIf<Models<Type, Concept>>;

		//! Checks whether a bool-meta-function is true.
		template <typename Required>
		auto holds()
			-> EnableIf<Required>;

		//! Checks whether the types of 'left' and 'right' agree.
		template <
			typename Left,
			typename Right
		>
		auto sameTypes(Left&& left, Right&& right)
			-> EnableIf<std::is_same<Left, Right>>;

		//! Checks whether Type is a valid type-expression.
		template <typename Type>
		bool exists();

	}

}

#endif
