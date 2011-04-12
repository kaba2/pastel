// Description: Position visitor concept

#ifndef PASTEL_POSITIONVISITOR_CONCEPT_H
#define PASTEL_POSITIONVISITOR_CONCEPT_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	namespace PositionVisitor_Concept
	{

		class PositionVisitor
		{
		public:
			//! The compile-time dimension.
			enum {N = UserDefinedInteger};

			//! Visits the given position.
			/*!
			position:
			Position to visit.

			Returns:
			Whether the visitation should be continued
			to other positions.
			*/
			bool operator()(const Vector<integer, N>& position);
		};

	}

}

#endif
