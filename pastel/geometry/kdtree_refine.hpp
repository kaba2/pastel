#ifndef PASTELGEOMETRY_KDTREE_REFINE_HPP
#define PASTELGEOMETRY_KDTREE_REFINE_HPP

#include "pastel/geometry/kdtree_refine.h"

namespace Pastel
{

	namespace Detail
	{

		template <
			int N, typename Real,
			typename ObjectPolicy>
			void refineMidpoint(
			const typename KdTree<N, Real, ObjectPolicy>::Cursor& cursor,
			integer depth,
			const AlignedBox<N, Real>& bound,
			integer maxDepth,
			integer maxObjects,
			KdTree<N, Real, ObjectPolicy>& tree)
		{
			if (depth >= maxDepth)
			{
				return;
			}

			Real splitPosition = 0;
			integer splitAxis = 0;

			if (cursor.leaf())
			{
				if (cursor.objects() > maxObjects)
				{
					// Split along the longest dimension.

					const Vector<N, Real> extent(bound.max() - bound.min());
					splitAxis = maxIndex(extent);
					splitPosition = linear(bound.min()[splitAxis], bound.max()[splitAxis], 0.5);

					tree.subdivide(cursor, splitPosition, splitAxis);
				}
			}
			else
			{
				splitPosition = cursor.splitPosition();
				splitAxis = cursor.splitAxis();
			}

			// A leaf node might or might not have been turned
			// into an intermediate node.
			if (!cursor.leaf())
			{
				AlignedBox<N, Real> negativeBound(bound);
				negativeBound.max()[splitAxis] = splitPosition;

				refineMidpoint(cursor.negative(), depth + 1,
					negativeBound, maxDepth, maxObjects, tree);

				AlignedBox<N, Real> positiveBound(bound);
				positiveBound.min()[splitAxis] = splitPosition;

				refineMidpoint(cursor.positive(), depth + 1,
					positiveBound, maxDepth, maxObjects, tree);
			}
		}

	}

	template <
		int N,
		typename Real,
		typename ObjectPolicy>
		void refineMidpoint(
		integer maxDepth,
		integer maxObjects,
		KdTree<N, Real, ObjectPolicy>& tree)
	{
		ENSURE1(maxDepth >= 0, maxDepth);
		ENSURE1(maxObjects >= 1, maxObjects);

		Detail::refineMidpoint(tree.root(), 0, tree.bound(), maxDepth, maxObjects, tree);
	}

	namespace Detail
	{

		template <
			int N, typename Real,
			typename ObjectPolicy>
			void refineSlidingMidpoint(
			const typename KdTree<N, Real, ObjectPolicy>::Cursor& cursor,
			integer depth,
			const AlignedBox<N, Real>& bound,
			integer maxDepth, integer maxObjects,
			KdTree<N, Real, ObjectPolicy>& tree)
		{
			typedef KdTree<N, Real, ObjectPolicy> Tree;
			typedef typename Tree::ConstObjectIterator ConstObjectIterator;

			if (depth >= maxDepth)
			{
				return;
			}

			Real splitPosition = 0;
			integer splitAxis = 0;

			if (cursor.leaf())
			{
				if (cursor.objects() > maxObjects)
				{
					// Split along the longest dimension.

					const Vector<N, Real> extent(bound.max() - bound.min());
					splitAxis = maxIndex(extent);
					splitPosition = linear(bound.min()[splitAxis], bound.max()[splitAxis], 0.5);

					// Sliding midpoint

					AlignedBox<1, Real> objectBound;

					ConstObjectIterator iter = cursor.begin();
					ConstObjectIterator iterEnd = cursor.end();
					while(iter != iterEnd)
					{
						const Tuple<2, Real> objectRange = tree.objectPolicy().bound(*iter, splitAxis);

						objectBound = boundingAlignedBox(objectBound,
							AlignedBox<1, Real>(
							Point<1, Real>(objectRange[0]),
							Point<1, Real>(objectRange[1])));

						++iter;
					}

					if (splitPosition < objectBound.min()[0] &&
						objectBound.min()[0] < bound.max()[splitAxis])
					{
						splitPosition = objectBound.min()[0];
					}
					if (splitPosition > objectBound.max()[0] &&
						objectBound.max()[0] > bound.min()[splitAxis])
					{
						splitPosition = objectBound.max()[0];
					}

					/*
					log() << "Subdividing at depth " << depth << " at "
					<< 100 * ((splitPosition - bound.min()[splitAxis]) / extent[splitAxis])
					<< logNewLine;
					*/

					tree.subdivide(cursor, splitPosition, splitAxis);
				}
			}
			else
			{
				splitPosition = cursor.splitPosition();
				splitAxis = cursor.splitAxis();
			}

			// A leaf node might or might not have been turned
			// into an intermediate node.
			if (!cursor.leaf())
			{
				AlignedBox<N, Real> negativeBound(bound);
				negativeBound.max()[splitAxis] = splitPosition;

				refineSlidingMidpoint(cursor.negative(), depth + 1,
					negativeBound, maxDepth, maxObjects, tree);

				AlignedBox<N, Real> positiveBound(bound);
				positiveBound.min()[splitAxis] = splitPosition;

				refineSlidingMidpoint(cursor.positive(), depth + 1,
					positiveBound, maxDepth, maxObjects, tree);
			}
		}

	}

	template <
		int N,
		typename Real,
		typename ObjectPolicy>
		void refineSlidingMidpoint(
		integer maxDepth,
		integer maxObjects,
		KdTree<N, Real, ObjectPolicy>& tree)
	{
		ENSURE1(maxDepth >= 0, maxDepth);
		ENSURE1(maxObjects >= 1, maxObjects);

		// See "Heuristic ray shooting algorithms",
		// The thesis of Vlastimil Havran,
		// "4.5 Automatic termination criteria"

		Detail::refineSlidingMidpoint(tree.root(),
			0, tree.bound(), maxDepth, maxObjects, tree);
	}

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

					for (integer axis = 0;axis < N;++axis)
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
