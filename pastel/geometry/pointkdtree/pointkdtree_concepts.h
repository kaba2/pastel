// Description: Point kd-tree concepts

#ifndef PASTELGEOMETRY_POINTKDTREE_CONCEPTS_H
#define PASTELGEOMETRY_POINTKDTREE_CONCEPTS_H

#include "pastel/geometry/pointkdtree/pointkdtree_fwd.h"

#include "pastel/sys/locator/locator_concept.h"
#include "pastel/sys/real/real_concept.h"

namespace Pastel
{

	namespace PointKdTree_Concepts
	{

		template <typename Settings>
		class Customization
		{
		public:
		};

	}

	template <typename T>
	concept PointKdTree_Settings_Concept_ = requires {
		typename T::Locator;
		requires Locator_Concept_<typename T::Locator>;
	};

}

#endif
