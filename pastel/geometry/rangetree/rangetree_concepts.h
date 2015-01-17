// Description: Concepts for the range tree

#ifndef PASTELGEOMETRY_RANGETREE_CONCEPTS_H
#define PASTELGEOMETRY_RANGETREE_CONCEPTS_H

#include "pastel/geometry/rangetree/rangetree.h"
#include "pastel/sys/real/real_concept.h"
#include "pastel/sys/algebra/less_concept.h"

namespace Pastel
{

	namespace RangeTree_Concepts
	{

		class MultiLess
		{
		public:
			//! Returns whether left <_i right.
			/*!
			Each <_i is a strict weak order; binding i 
			to a specific value provides a strict weak order,
			i.e., a model of the Less concept.
			*/
			bool operator()(
				const UserDefinedType& left,
				const UserDefinedType& right,
				integer i) const;
		};

		class Settings
		{
		public:
			//! The type of the points.
			using Point = UserDefinedType;

			//! The type by which to sort points.
			/*!
			The type is a model of the MultiLess
			concept.
			*/ 
			using MultiLess = UserDefinedType;
		};

		template <typename Settings>
		class Customization
		{
		};

	}

}

#endif
