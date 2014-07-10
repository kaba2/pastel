// Description: Concepts for the range tree

#ifndef PASTELGEOMETRY_TDTREE_CONCEPTS_H
#define PASTELGEOMETRY_TDTREE_CONCEPTS_H

#include "pastel/geometry/tdtree.h"

#include "pastel/sys/locator_concept.h"

namespace Pastel
{

	namespace TdTree_Concepts
	{

		class Settings
		{
		public:
			//! The locator to abstract points.
			/*!
			The type is a model of the Locator concept.
			*/ 
			using Locator = UserDefinedType;
		};

		template <typename Settings>
		class Customization
		{
		};

	}

}

#endif
