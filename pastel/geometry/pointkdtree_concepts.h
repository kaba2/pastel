// Description: Point kd-tree concepts

#ifndef PASTELGEOMETRY_POINTKDTREE_CONCEPTS_H
#define PASTELGEOMETRY_POINTKDTREE_CONCEPTS_H

#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/pointkdtree_fwd.h"

#include "pastel/sys/pointpolicy_concept.h"
#include "pastel/sys/real_concept.h"

namespace Pastel
{

	namespace PointKdTree_Concepts
	{

		class Settings
		{
		public:
			//! The type of the point coordinates.
			/*!
			Models the Real concept.
			*/
			using Real = UserDefinedType;

			//! The dimension of the tree.
			/*!
			Preconditions:
			N > 0 || N == Dynamic

			The Dynamic case denotes a dimension which 
			is decided at run-time.
			*/
			static PASTEL_CONSTEXPR integer N = UserDefinedInteger;

			//! The point-abstraction.
			/*!
			Models the PointPolicy concept.
			*/
			using PointPolicy = UserDefinedType;
		};

		template <typename Settings>
		class Customization
		{
		public:
		};

	}	

}

#endif
