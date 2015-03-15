// Description: Check direct requirements of a concept

#ifndef PASTELSYS_MODELS_DIRECTLY_H
#define PASTELSYS_MODELS_DIRECTLY_H

namespace Pastel
{

	//! Checks direct requirements of a concept.
	/*!
	The requirements of base-concepts will not 
	be checked.
	*/
	template <
		typename Type, 
		typename Concept>
	struct Models_Directly;

}

#include "pastel/sys/concept/models_directly.hpp"

#endif
