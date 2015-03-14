// Description: Concept-checking

#ifndef PASTELSYS_CONCEPT_H
#define PASTELSYS_CONCEPT_H

#include "pastel/sys/mytypes.h"

#include <type_traits>

#define PASTEL_CONCEPT_CHECK(Type, Concept) \
	PASTEL_STATIC_ASSERT((Models<Type, Concept>::value))

#define PASTEL_CONCEPT_CHECK_DIRECT(Type, Concept) \
	PASTEL_STATIC_ASSERT((Models_Directly<Type, Concept>::value))

#define PASTEL_CONCEPT_CHECK_BASE(Type, Concept) \
	PASTEL_STATIC_ASSERT((Models_Base<Type, Concept>::value))

#define PASTEL_CONCEPT_REJECT(Type, Concept) \
	PASTEL_STATIC_ASSERT((!Models<Type, Concept>::value))

#define PASTEL_CONCEPT_REJECT_DIRECT(Type, Concept) \
	PASTEL_STATIC_ASSERT((!Models_Directly<Type, Concept>::value))

#define PASTEL_CONCEPT_REJECT_BASE(Type, Concept) \
	PASTEL_STATIC_ASSERT((!Models_Base<Type, Concept>::value))

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

		template <
			typename Required,
			typename Type>
		auto hasType(Type that) -> decltype
		(
			std::is_same<Type, Required>::value
		);

		template <
			typename Required, 
			typename Type>
		auto convertsTo(Type that) -> decltype
		(
			std::is_convertible<Type, Required>::value
		);

		template <
			typename Left,
			typename Right>
		auto sameTypes(Left left, Right right) -> decltype
		(
			std::is_same<Left, Right>::value
		);

		template <
			typename Type,
			typename Concept>
		void printMostRefined()
		{
			using A = typename MostRefinedConcept<Type, Concept>::type;
			A::errorToPrintMostRefined();
		}

	}

}

#endif
