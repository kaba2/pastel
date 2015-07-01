// Description: Concept-checking

#ifndef PASTELSYS_CONCEPT_H
#define PASTELSYS_CONCEPT_H

#include "pastel/sys/sfinae.h"
#include "pastel/sys/concept/refines.h"
#include "pastel/sys/concept/models.h"

#include <type_traits>

// For std::declval, std::forward, and std::move.
#include <utility>

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

	namespace Concept
	{

		//! Checks whether 'that' has type Required.
		/*!
		Visual Studio 2015 RC has a bug in that the Requires
		cannot be placed in the trailing return-type.
		*/
		template <
			typename Required,
			typename Type,
			Requires<std::is_same<Type, Required>> = 0>
		bool hasType(Type&& that);

		//! Checks whether Type is convertible to Required.
		/*!
		Visual Studio 2015 RC has a bug in that the Requires
		cannot be placed in the trailing return-type.
		*/
		template <typename Required>
		bool convertsTo(NoDeduction<Required> that);

		template <
			typename Type, 
			typename Concept>
		auto models()
			-> Requires<Models<Type, Concept>>;

		//! Checks whether a bool-meta-function is true.
		template <typename Required>
		auto holds()
			-> Requires<Required>;

		//! Checks whether the types of 'left' and 'right' agree.
		template <
			typename Left,
			typename Right>
		auto sameTypes(Left&& left, Right&& right)
			-> Requires<std::is_same<Left, Right>>;

		//! Checks whether Type is a valid type-expression.
		template <typename Type>
		bool exists();

		//! An archetype for functions.
		template <typename Return, typename... ArgumentSet>
		Return function(ArgumentSet&&... argumentSet);

	}

}

#endif
