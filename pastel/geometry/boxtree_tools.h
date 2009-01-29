#ifndef PASTELGEOMETRY_BOXTREETOOLS_H
#define PASTELGEOMETRY_BOXTREETOOLS_H

#include "pastel/geometry/line.h"

#include "pastel/geometry/boxtree.h"

namespace Pastel
{

	template <int N, typename Real, typename UserData, typename Intersector>
	void intersect(const Line<N, Real>& ray,
		const BoxTree<N, Real, UserData>::Cursor& cursor,
		Intersector intersector);

	template <int N, typename Real, typename UserData, typename Intersector>
	void intersect(const Line<N, Real>& ray,
		const BoxTree<N, Real, UserData>& boxTree,
		Intersector intersector);

}

#include "pastel/geometry/boxtree_tools.hpp"

#endif
