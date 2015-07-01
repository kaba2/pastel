// Description: Set concept

#ifndef PASTELSYS_SET_CONCEPT_H
#define PASTELSYS_SET_CONCEPT_H

#include "pastel/sys/concept/concept.h"
#include "pastel/sys/type_traits/remove_cvref.h"

namespace Pastel
{

	//! Set
	/*!
	To be precise, the set-concept represents a multi-set;
	elements can occur multiple times. Here we use the term 
	'set' a bit loosely, for brevity.
	*/
	struct Set_Concept
	{
		template <
			typename Type,
			//! The type of the elements in the set.
			typename Element = typename Type::Element,
			//! The state of iteration in the sequence.
			typename Index = typename Type::Index
		>
		auto requires(Type&& t) -> decltype
		(
			conceptCheck(
				//! Returns the number of elements in the multi-set.
				Concept::convertsTo<integer>(addConst(t).n()),
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
				),
				//! Returns whether the set is empty.
				Concept::convertsTo<bool>(
					addConst(t).empty()),
				//! Returns a state for iterating over elements.
				Concept::convertsTo<Index>(
					addConst(t).index()),
				//! Returns the element referred to by the state.
				Concept::convertsTo<Element>(
					addConst(t).element(std::declval<Index>())),
				//! Advances the state to the next element.
				(addConst(t).next(std::declval<Index&>()), 0),
				//! Returns whether there are no elements referred to by the state.
				Concept::convertsTo<bool>(
					addConst(t).empty(std::declval<Index>()))
			)
		);
	};

	template <typename Set>
	using Set_Element = 
		typename RemoveCvRef<Set>::Element;
	
	template <typename Set>
	using Set_Element_F = 
		Identity_F<Set_Element<Set>>;

	template <typename Set>
	using Set_Index = 
		typename RemoveCvRef<Set>::Index;
	
	template <typename Set>
	using Set_Index_F = 
		Identity_F<Set_Index<Set>>;

}

#endif
