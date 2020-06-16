// Description: A conjunction concept

#ifndef PASTELSYS_REFINES_H
#define PASTELSYS_REFINES_H

#include <concepts/concepts.hpp>

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
		template <typename Type>
		void requires_(Type&&);
	};

}

#endif
