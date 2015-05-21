// Description: A conjunction concept

#ifndef PASTELSYS_REFINES_H
#define PASTELSYS_REFINES_H

#include "pastel/sys/sfinae_macros.h"

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
		// The Refines is itself a trivial
		// concept, which requires nothing.
		// This way a refined concept does 
		// not need to implement requires(), 
		// if it does not add any new requirements.

		template <typename Type>
		void requires(Type&& t);
	};

}

#endif
