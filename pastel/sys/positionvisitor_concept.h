#ifndef PASTEL_POSITIONVISITOR_CONCEPT_H
#define PASTEL_POSITIONVISITOR_CONCEPT_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	class PositionVisitor_Concept
	{
	public:
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

#endif
