// Description: Position visitor concept

#ifndef PASTELSYS_POSITIONVISITOR_CONCEPT_H
#define PASTELSYS_POSITIONVISITOR_CONCEPT_H

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
			static PASTEL_CONSTEXPR int N = UserDefinedInteger;

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
