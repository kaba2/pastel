#ifndef PASTEL_BOXTREETOOLS_HPP
#define PASTEL_BOXTREETOOLS_HPP

#include "pastel/geometry/boxtree_tools.h"

namespace Pastel
{

	template <int N, typename Real, typename UserData, typename Intersector>
	void intersect(const Line<N, Real>& ray,
		const BoxTree<N, Real, UserData>::Cursor& cursor,
		Intersector intersector)
	{
		if (cursor.positiveLeaf())
		{
			intersector(ray, cursor.positiveData());
		}
		else
		{
			if (overlaps(cursor.positiveBox()))
			{
				intersect(ray, cursor.positive(), intersector);
			}
		}
		if (cursor.negativeLeaf())
		{
			intersector(ray, cursor.negativeData());
		}
		else
		{
			if (overlaps(cursor.negativeBox()))
			{
				intersect(ray, cursor.negative(), intersector);
			}
		}
	}

	template <int N, typename Real, typename UserData, typename Intersector>
	void intersect(const Line<N, Real>& ray,
		const BoxTree<N, Real, UserData>& boxTree,
		Intersector intersector)
	{
		if (boxTree.empty())
		{
			return;
		}

		if (!overlaps(ray, boxTree.bound()))
		{
			return;
		}

		intersect(ray, boxTree.root(), intersector);
	}

}

#endif
