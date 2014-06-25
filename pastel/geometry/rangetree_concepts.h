// Description: Concepts for the range tree

#ifndef PASTELGEOMETRY_RANGETREE_CONCEPTS_H
#define PASTELGEOMETRY_RANGETREE_CONCEPTS_H

#include "pastel/geometry/rangetree.h"

#include "pastel/sys/real_concept.h"

namespace Pastel
{

	namespace RangeTree_Concepts
	{

		class Settings
		{
		public:
			using Real = UserDefinedType;
			using Point = UserDefinedType;
		};

		template <typename Settings>
		class Customization
		{
		};

	}

}

#endif
