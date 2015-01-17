// Description: Point kd-tree concepts

#ifndef PASTELGEOMETRY_POINTKDTREE_CONCEPTS_H
#define PASTELGEOMETRY_POINTKDTREE_CONCEPTS_H

#include "pastel/geometry/pointkdtree/pointkdtree.h"
#include "pastel/geometry/pointkdtree/pointkdtree_fwd.h"

#include "pastel/sys/locator/locator_concept.h"
#include "pastel/sys/real/real_concept.h"

namespace Pastel
{

	namespace PointKdTree_Concepts
	{

		class Settings
		{
		public:
			//! The point abstraction.
			/*!
			Models the Locator concept.
			*/
			using Locator = UserDefinedType;
		};

		template <typename Settings>
		class Customization
		{
		public:
		};

	}	

}

#endif
