#ifndef PASTEL_MAXIMUM_CLIQUE_ALIGNEDBOX_H
#define PASTEL_MAXIMUM_CLIQUE_ALIGNEDBOX_H

#include "pastel/geometry/alignedbox.h"

#include <vector>

namespace Pastel
{
	
	template <typename Real>
	AlignedBox<Real, 2> maximumClique(
		const std::vector<AlignedBox<Real, 2> >& boxSet);

}

#include "pastel/geometry/maximum_clique_alignedbox.hpp"

#endif
