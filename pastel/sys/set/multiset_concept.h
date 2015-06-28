#ifndef PASTELSYS_MULTISET_CONCEPT_H
#define PASTELSYS_MULTISET_CONCEPT_H

#include "pastel/sys/concept/concept.h"
#include "pastel/sys/type_traits/remove_cvref.h"

namespace Pastel
{

	//! A multi-set.
	struct MultiSet_Concept
	{
		template <
			typename Type,
			typename Element = typename Type::Element>
		auto requires(Type&& t) -> decltype
		(
			conceptCheck(
				//! Returns the number of elements in the multi-set.
				Concept::convertsTo<integer>(addConst(t).n()),
				//! The type of the elements in the multi-set.
				Concept::exists<Element>(),
				//! Calls the given function for each element in the multi-set.
				/*!
				The visitor function returns whether to continue iterating
				over elements. The implementation of forEach must respect
				this request.

				returns:
				Whether the for-each was uninterrupted by the user.
				*/
				Concept::convertsTo<bool>(
					addConst(t).forEach(Concept::function<bool, Element>)
				)
			)
		);
	};

	template <typename MultiSet>
	using MultiSet_Element = 
		typename RemoveCvRef<MultiSet>::Element;
	
	template <typename MultiSet>
	using MultiSet_Element_F = 
		Identity_F<MultiSet_Element<MultiSet>>;

}

#endif
