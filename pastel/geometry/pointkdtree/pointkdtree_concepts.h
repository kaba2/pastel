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

		template <typename Settings>
		class Customization
		{
		public:
		};

	}

	struct PointKdTree_Settings_Concept
	{
		template <typename Type>
		auto requires_(Type&& t) -> decltype
		(
			conceptCheck(
				Concept::exists<typename Type::Locator>(),
				Concept::models<typename Type::Locator, Locator_Concept>()
			)
		);
	};

}

#endif
