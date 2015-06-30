// Description: Multi-set concept

#ifndef PASTELSYS_SET_CONCEPT_H
#define PASTELSYS_SET_CONCEPT_H

#include "pastel/sys/concept/concept.h"
#include "pastel/sys/type_traits/remove_cvref.h"

namespace Pastel
{

	//! A multi-set.
	struct Set_Concept
	{
		template <
			typename Type,
			typename Element = typename Type::Element,
			typename State = typename Type::State>
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
				),
				//! A state for iterating incrementally over elements.
				Concept::convertsTo<State>(
					addConst(t).state()),
				//! Returns the element referred to by the state.
				Concept::convertsTo<Element>(
					addConst(t).element(std::declval<State>())),
				//! Advances the state to the next element.
				(addConst(t).next(std::declval<State&>()), 0),
				//! Returns whether there are no elements referred to by the state.
				Concept::convertsTo<bool>(
					addConst(t).empty(std::declval<State>())),
				//! Returns whether the set is empty.
				Concept::convertsTo<bool>(
					addConst(t).empty())
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
	using Set_State = 
		typename RemoveCvRef<Set>::State;
	
	template <typename Set>
	using Set_State_F = 
		Identity_F<Set_State<Set>>;

}

#endif
