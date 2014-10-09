#ifndef PASTELSYS_POINTSET_POP_H
#define PASTELSYS_POINTSET_POP_H

#include "pastel/sys/pointset_concept.h"
#include "pastel/sys/pointset_input.h"

namespace Pastel
{

	template <typename PointSet>
	auto pointSetPop(PointSet& pointSet)
	{
		pointSetInput(pointSet).pop();
	}

}

#endif
