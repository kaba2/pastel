// Description: Convex hull of a point-set

#ifndef PASTELGEOMETRY_CONVEXHULL_H
#define PASTELGEOMETRY_CONVEXHULL_H

#include "pastel/sys/vector.h"
#include "pastel/sys/math_functions.h"

#include "pastel/geometry/predicates.h"

#include <vector>
#include <algorithm>

namespace Pastel
{

	namespace ConvexHull_
	{

		namespace GrahamsScan
		{

			template <typename Real>
			class ConvexHullPointCompare
			{
			public:
				bool operator()(const Vector<Real, 2>& left,
					const Vector<Real, 2>& right) const
				{
					return lexicographicLess(left, right);
				}
			};

		}

	}

	//! Computes the boundary of the convex hull of a point set.
	/*!
	Time complexity: O(n lg n)
	Exception safety: strong

	The convex hull of a point set is the set of all
	convex combinations	of the point set.

	This function uses the Graham's scan algorithm for
	computing the boundary of the convex hull.

	An optimal convex hull algorithm is output-sensitive
	with complexity O(n log h), where h is the number
	of vertices on the boundary of the convex hull.
	*/
	template <typename Real>
	void convexHullGrahamsScan(
		const std::vector<Vector<Real, 2> >& pointSet,
		std::vector<Vector<Real, 2> >& hullResult)
	{
		// This is the "Graham's scan" algorithm.

		std::vector<Vector<Real, 2> > hull;

		if (pointSet.empty())
		{
			return;
		}

		std::vector<Vector<Real, 2> > orderedSet(pointSet);
		std::sort(orderedSet.begin(), orderedSet.end(),
			ConvexHull_::GrahamsScan::ConvexHullPointCompare<Real>());

		using Container = std::vector<Vector<Real, 2> >;
		typedef typename Container::iterator
			Iterator;
		typedef typename Container::const_iterator
			ConstIterator;
		typedef typename Container::reverse_iterator
			ReverseIterator;
		typedef typename Container::const_reverse_iterator
			ConstReverseIterator;

		// Scan the upper hull.

		ConstIterator from(orderedSet.begin());
		ConstIterator to(orderedSet.end());
		ConstIterator current(from);

		integer points = 0;

		hull.push_back(*current);
		++points;
		++current;
		if (current == to)
		{
			// No more points left,
			// we are done.
			hull.swap(hullResult);

			return;
		}

		// Find a second point
		// from the hull.

		while(points < 2)
		{
			if (*current != hull.back())
			{
				hull.push_back(*current);
				++points;
			}
			++current;
			if (current == to)
			{
				// No more points left,
				// we are done.
				hull.swap(hullResult);

				return;
			}
		}

		// Scan the upper hull.

		while (current != to)
		{
			const Vector<Real, 2>& toPoint = *current;

			if (toPoint != hull.back())
			{
				while (points > 1)
				{
					const Vector<Real, 2>& fromPoint = hull[points - 2];
					const Vector<Real, 2>& middlePoint = hull[points - 1];

					if (signedArea(fromPoint, middlePoint, toPoint) > 0)
					{
						hull.pop_back();
						--points;
					}
					else
					{
						break;
					}
				}

				hull.push_back(toPoint);
				++points;
			}

			++current;
		}

		// Scan the lower hull.

		ConstReverseIterator rFrom(
			orderedSet.rbegin());
		ConstReverseIterator rTo(
			orderedSet.rend());

		++rFrom;

		ConstReverseIterator rCurrent(rFrom);

		while (rCurrent != rTo)
		{
			const Vector<Real, 2>& toPoint = *rCurrent;

			if (toPoint != hull.back())
			{
				while (true)
				{
					const Vector<Real, 2>& fromPoint = hull[points - 2];
					const Vector<Real, 2>& middlePoint = hull[points - 1];

					if (signedArea(fromPoint, middlePoint, toPoint) > 0)
					{
						hull.pop_back();
						--points;
					}
					else
					{
						break;
					}
				}

				hull.push_back(toPoint);
				++points;
			}

			++rCurrent;
		}

		hull.pop_back();

		hull.swap(hullResult);
	}

}

#endif
