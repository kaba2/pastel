#ifndef PASTEL_ALIGNEDBOX_HPP
#define PASTEL_ALIGNEDBOX_HPP

#include "pastel/sys/alignedbox.h"

namespace Pastel
{

	template <typename Real, int N>
	void swap(AlignedBox<Real, N>& left,
		AlignedBox<Real, N>& right)
	{
		left.swap(right);
	}

}

#endif
