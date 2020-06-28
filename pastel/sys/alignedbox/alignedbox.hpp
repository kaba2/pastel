#ifndef PASTELSYS_ALIGNEDBOX_HPP
#define PASTELSYS_ALIGNEDBOX_HPP

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
