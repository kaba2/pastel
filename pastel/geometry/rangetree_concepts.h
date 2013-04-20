// Description: Concepts for RangeTree

#ifndef PASTELGEOMETRY_RANGETREE_CONCEPTS_H
#define PASTELGEOMETRY_RANGETREE_CONCEPTS_H

#include "pastel/geometry/rangetree.h"

#include "pastel/sys/pointrep_concept.h"

namespace Pastel
{

	namespace RangeTree_Concepts
	{

		class RangeTree_Types
		{
		public:
			//! The type of the contained points.
			typedef PointRep_Concept PointRep;
		};

	}

}

#endif
