#ifndef PASTELGEOMETRY_KDTREE_REFINE_HPP
#define PASTELGEOMETRY_KDTREE_REFINE_HPP

#include "pastel/geometry/kdtree_refine.h"

namespace Pastel
{

	class MidpointRule
	{
	public:
		template <
			int N, typename Real,
			typename ObjectPolicy>
			std::pair<Real, integer> operator()(
			const Point<N, Real>& minBound,
			const Point<N, Real>& maxBound,
			const ObjectPolicy& objectPolicy,
			const typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator& objectBegin,
			const typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator& objectEnd) const
		{
			// Split along the longest dimension.

			const integer splitAxis = maxIndex(maxBound - minBound);
			const Real splitPosition = linear(minBound[splitAxis], 
				maxBound[splitAxis], 0.5);

			return std::make_pair(splitPosition, splitAxis);
		}
	};

	class SlidingMidpointRule
	{
	public:
		template <
			int N, typename Real,
			typename ObjectPolicy>
			std::pair<Real, integer> operator()(
			const Point<N, Real>& minBound,
			const Point<N, Real>& maxBound,
			const ObjectPolicy& objectPolicy,
			const typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator& objectBegin,
			const typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator& objectEnd) const
		{
			typedef typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator 
				ConstObjectIterator;

			// Split along the longest dimension.

			const integer splitAxis = maxIndex(maxBound - minBound);
			Real splitPosition = linear(minBound[splitAxis], 
				maxBound[splitAxis], 0.5);

			// Sliding midpoint

			AlignedBox<1, Real> objectBound;

			ConstObjectIterator iter = objectBegin;
			ConstObjectIterator iterEnd = objectEnd;
			while(iter != iterEnd)
			{
				const Tuple<2, Real> objectRange = objectPolicy.bound(*iter, splitAxis);

				objectBound = boundingAlignedBox(objectBound,
					AlignedBox<1, Real>(
					Point<1, Real>(objectRange[0]),
					Point<1, Real>(objectRange[1])));

				++iter;
			}

			if (splitPosition < objectBound.min()[0] &&
				objectBound.min()[0] < maxBound[splitAxis])
			{
				splitPosition = objectBound.min()[0];
			}
			if (splitPosition > objectBound.max()[0] &&
				objectBound.max()[0] > minBound[splitAxis])
			{
				splitPosition = objectBound.max()[0];
			}

			return std::make_pair(splitPosition, splitAxis);
		}
	};

	class SlidingMidpointRule2
	{
	public:
		template <
			int N, typename Real,
			typename ObjectPolicy>
			std::pair<Real, integer> operator()(
			const Point<N, Real>& minBound,
			const Point<N, Real>& maxBound,
			const ObjectPolicy& objectPolicy,
			const typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator& objectBegin,
			const typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator& objectEnd) const
		{
			typedef typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator 
				ConstObjectIterator;

			// Find object spread.

			const integer dimension = bound.dimension();

			AlignedBox<N, Real> objectBound(dimension);

			ConstObjectIterator iter = objectBegin;
			ConstObjectIterator iterEnd = objectEnd;
			while(iter != iterEnd)
			{
				objectBound = boundingAlignedBox(
					objectBound,
					objectPolicy.bound(*iter));

				++iter;
			}

			// Find the longest dimension.

			const Vector<N, Real> extent = maxBound - minBound;

			const integer maxExtentAxis = maxIndex(extent);
			const Real maxExtent = extent[maxExtentAxis];

			integer maxLegalSpreadAxis = 0;
			Real maxLegalSpread = 0;

			const Vector<N, Real> spread = objectBound.extent();

			for (integer i = 0;i < dimension;++i)
			{
				if (extent[i] >= 0.8 * maxExtent)
				{
					if (spread[i] >= maxLegalSpread)
					{
						maxLegalSpreadAxis = i;
						maxLegalSpread = spread[i];
					}
				}
			}

			const integer splitAxis = maxLegalSpreadAxis;

			Real splitPosition = linear(minBound[splitAxis], 
				maxBound[splitAxis], 0.5);

			if (splitPosition < objectBound.min()[splitAxis])
			{
				splitPosition = objectBound.min()[splitAxis];
			}
			if (splitPosition > objectBound.max()[splitAxis])
			{
				splitPosition = objectBound.max()[splitAxis];
			}

			if (splitPosition < minBound[splitAxis])
			{
				splitPosition = minBound[splitAxis];
			}
			if (splitPosition > maxBound[splitAxis])
			{
				splitPosition = maxBound[splitAxis];
			}

			return std::make_pair(splitPosition, splitAxis);
		}
	};

	namespace Detail
	{

		template <int N, typename Real, typename ObjectPolicy>
		class BoundPoint
		{
		private:
			typedef KdTree<N, Real, ObjectPolicy> Tree;
			typedef typename Tree::ConstObjectIterator ConstObjectIterator;

		public:
			BoundPoint(
				const Real& position,
				bool start,
				const ConstObjectIterator& object)
				: position_(position)
				, start_(start)
				, object_(object)
			{
			}

			bool operator<(const BoundPoint& that) const
			{
				if (position_ < that.position_)
				{
					return true;
				}

				if (position_ > that.position_)
				{
					return false;
				}

				if (!start_ && that.start_)
				{
					// Ending points before starting points.
					return true;
				}

				if (start_ && !that.start_)
				{
					return false;
				}

				return object_ < that.object_;
			}

			Real position_;
			bool start_;
			ConstObjectIterator object_;
		};

		template <
			int N, typename Real,
			typename ObjectPolicy>
			void refineSurfaceAreaHeuristic(
			const typename KdTree<N, Real, ObjectPolicy>::Cursor& cursor,
			integer depth,
			const AlignedBox<N, Real>& bound,
			integer maxDepth,
			integer maxObjects,
			integer badRefines,
			KdTree<N, Real, ObjectPolicy>& tree)
		{
			typedef KdTree<N, Real, ObjectPolicy> Tree;
			typedef typename Tree::ConstObjectIterator ConstObjectIterator;

			if (depth >= maxDepth)
			{
				return;
			}

			const Real costToTraverse = 1;
			const Real costToIntersect = 80;
			const Real emptyScale = 0.85;

			if (cursor.leaf())
			{
				// Surface area heuristic

				// This implementation is essentially from the book
				// "Physically based rendering - From theory to implementation"

				// For the surface heuristic area also refer
				// "Heuristic ray shooting algorithms"

				// Start from the axis which
				// has the maximum extent.

				if (cursor.objects() > maxObjects)
				{
					const Real nodeArea = area(bound);
					const Real invNodeArea = inverse(nodeArea);

					const Real leafCost = cursor.objects() * costToIntersect;

					bool foundSplit = false;
					Real minPosition = 0;
					Real minCost = infinity<Real>();
					integer minAxis = 0;

					const integer n = tree.dimension();

					for (integer axis = 0;axis < n;++axis)
					{
						std::vector<BoundPoint<N, Real, ObjectPolicy> > pointList;
						const integer boundPoints = 2 * cursor.objects();
						pointList.reserve(boundPoints);

						ConstObjectIterator iter = cursor.begin();
						ConstObjectIterator iterEnd = cursor.end();
						while(iter != iterEnd)
						{
							const Tuple<2, Real> objectBound = tree.objectPolicy().bound(*iter, axis);

							pointList.push_back(BoundPoint<N, Real, ObjectPolicy>(objectBound[0], true, iter));
							pointList.push_back(BoundPoint<N, Real, ObjectPolicy>(objectBound[1], false, iter));

							++iter;
						}

						std::sort(pointList.begin(), pointList.end());

						AlignedBox<N, Real> positiveBound(bound);
						AlignedBox<N, Real> negativeBound(bound);

						integer negativeObjects = 0;
						integer positiveObjects = cursor.objects();
						for (integer i = 0;i < boundPoints;++i)
						{
							if (!pointList[i].start_)
							{
								--positiveObjects;
							}

							const real position = pointList[i].position_;

							if (position >= bound.min()[axis] &&
								position < bound.max()[axis])
							{
								positiveBound.min()[axis] = position;
								negativeBound.max()[axis] = position;

								// Note that the probabilities
								// of intersecting the negative and
								// the positive child node do not
								// need to sum to one (and they don't).

								const Real negativeProbability =
									area(negativeBound) * invNodeArea;
								const Real positiveProbability =
									area(positiveBound) * invNodeArea;

								Real scale = 1;
								if (negativeObjects == 0 || positiveObjects == 0)
								{
									scale = emptyScale;
								}

								const Real totalCost =
									costToTraverse +
									scale * (negativeProbability * negativeObjects +
									positiveProbability * positiveObjects) * costToIntersect;

								if (totalCost < minCost)
								{
									minCost = totalCost;
									minPosition = position;
									minAxis = axis;
									foundSplit = true;
								}
							}

							if (pointList[i].start_)
							{
								++negativeObjects;
							}
						}
					}

					if (foundSplit)
					{
						if (minCost > leafCost)
						{
							++badRefines;
							if (badRefines == 3)
							{
								return;
							}
						}

						tree.subdivide(cursor, minPosition, minAxis);
					}
				}
			}

			// A leaf node might have been turned
			// into an intermediate node and thus this can't
			// be an else branch.
			if (!cursor.leaf())
			{
				const Real splitPosition = cursor.splitPosition();
				const integer splitAxis = cursor.splitAxis();

				AlignedBox<N, Real> negativeBound(bound);
				negativeBound.max()[splitAxis] = splitPosition;

				refineSurfaceAreaHeuristic(cursor.negative(), depth + 1,
					negativeBound, maxDepth, maxObjects, badRefines, tree);

				AlignedBox<N, Real> positiveBound(bound);
				positiveBound.min()[splitAxis] = splitPosition;

				refineSurfaceAreaHeuristic(cursor.positive(), depth + 1,
					positiveBound, maxDepth, maxObjects, badRefines, tree);
			}
		}

	}

	template <
		int N,
		typename Real,
		typename ObjectPolicy>
		void refineSurfaceAreaHeuristic(
		integer maxDepth,
		integer maxObjects,
		KdTree<N, Real, ObjectPolicy>& tree)
	{
		ENSURE1(maxDepth >= 0, maxDepth);
		ENSURE1(maxObjects >= 1, maxObjects);

		Detail::refineSurfaceAreaHeuristic(
			tree.root(), 0, tree.bound(), maxDepth, maxObjects, 0, tree);
	}

}

#endif
