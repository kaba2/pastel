// Description: A conjunction concept

#ifndef PASTELSYS_REFINES_H
#define PASTELSYS_REFINES_H

namespace Pastel
{

	template <
		typename Type, 
		typename... ConceptSet>
	struct Models;

	//! Refined concept
	/*!
	The Refines-class has two roles.
	First, it is a concept, which requires
	all the requirements in the given set of
	concepts. Second, it is used to denote 
	concept-refinement when used as a base-class.
	*/
	template <typename... ConceptSet>
	struct Refines 
	{	
		// Refines is also a concept;
		// it requires all the requirements
		// of its concepts. This way a
		// refined concept does not need to
		// implement requires(), if it does
		// not add any new requirements.

		template <typename Type>
		auto requires(Type&& t) -> decltype(
			conceptCheck(
				Models<Type, ConceptSet...>::value
			));
	};

}

#endif
