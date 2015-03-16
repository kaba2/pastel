// Description: Concept-checking

#ifndef PASTELSYS_CONCEPT_H
#define PASTELSYS_CONCEPT_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/type_traits/and.h"
#include "pastel/sys/type_traits/or.h"

#include <type_traits>

#define PASTEL_CONCEPT_CHECK(Type, Concept) \
	PASTEL_STATIC_ASSERT((Pastel::Models<Type, Concept>::value));

#define PASTEL_CONCEPT_CHECK_DIRECT(Type, Concept) \
	PASTEL_STATIC_ASSERT((Pastel::Models_Directly<Type, Concept>::value))

#define PASTEL_CONCEPT_CHECK_BASE(Type, Concept) \
	PASTEL_STATIC_ASSERT((Pastel::Models_Base<Type, Concept>::value))

#define PASTEL_CONCEPT_REJECT(Type, Concept) \
	PASTEL_STATIC_ASSERT((!Pastel::Models<Type, Concept>::value))

#define PASTEL_CONCEPT_REJECT_DIRECT(Type, Concept) \
	PASTEL_STATIC_ASSERT((!Pastel::Models_Directly<Type, Concept>::value))

#define PASTEL_CONCEPT_REJECT_BASE(Type, Concept) \
	PASTEL_STATIC_ASSERT((!Pastel::Models_Base<Type, Concept>::value))

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

	template <typename... BoolSet>
	using Requires = EnableIf<And<BoolSet...>>;

	template <typename... BoolSet>
	using RequiresSome = EnableIf<Or<BoolSet...>>;

	namespace Concept
	{

		template <
			typename Required,
			typename Type,
			typename = EnableIf<std::is_same<Type, Required>>
			>
		bool hasType(Type&& that);

		template <
			typename Required, 
			typename Type,
			EnableIf<std::is_convertible<Type, Required>> = 0
			>
		bool convertsTo(Type&& that);

		template <typename Required>
		auto holds()
			-> EnableIf<Required>;

		template <
			typename Left,
			typename Right
		>
		auto sameTypes(Left&& left, Right&& right)
			-> EnableIf<std::is_same<Left, Right>>;

	}

}

#endif
