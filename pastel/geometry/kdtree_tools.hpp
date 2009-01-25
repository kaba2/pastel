#ifndef PASTELGEOMETRY_KDTREETOOLS_HPP
#define PASTELGEOMETRY_KDTREETOOLS_HPP

#include "pastel/geometry/kdtree_tools.h"
#include "pastel/geometry/intersect_line_alignedbox.h"
#include "pastel/geometry/bounding_alignedbox.h"
#include "pastel/geometry/boxarea.h"
#include "pastel/geometry/distance_point_point.h"

#include "pastel/math/linear.h"

#include "pastel/sys/ensure.h"
#include "pastel/sys/syscommon.h"
#include "pastel/sys/vector_tools.h"

#include <algorithm>
#include <vector>

namespace Pastel
{

	inline integer computeKdTreeMaxDepth(integer objects)
	{
		return (real)1.3 * (real)integerLog2(objects + 1) + (real)8.0;
	}

	template <int N, typename Real, typename ObjectPolicy, typename NormFunctor>
	void findNearestArray(const std::vector<typename ObjectPolicy::Object>& pointSet, 
		integer kNearest, 
		NormFunctor distance,
		LinearArray<N, typename ObjectPolicy::Object>& nearestArray)
	{
		typedef KdTree<N, real, ObjectPolicy> Tree;
		typedef typename Tree::ConstObjectIterator ConstTreeIterator;
		typedef typename SmallSet<KeyValue<Real, ConstTreeIterator> > NearestSet;

		Tree tree;
		tree.insert(pointSet.begin(), pointSet.end());

		refineSlidingMidpoint(
			computeKdTreeMaxDepth(tree.objects()), 4, tree);

		const integer points = pointSet.size();
		LinearArray<N, typename ObjectPolicy::Object> result(points, kNearest);

		for (integer i = 0;i < points;++i)
		{
			NearestSet nearestSet;
			nearestSet.reserve(kNearest);
		
			findNearest(tree, pointSet[i], infinity<Real>(), 
				distance, kNearest, nearestSet);
			
			for (integer m = 0;m < kNearest;++m)
			{
				nearestArray(m, i) = *nearestSet[m].value();
			}
		}

		result.swap(nearestArray);
	}

	template <int N, typename Real, typename ObjectPolicy, typename NormFunctor>
	KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		findNearest(
		const KdTree<N, Real, ObjectPolicy>& tree,
		const Point<N, Real>& point,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		NormFunctor normFunctor)
	{
		ENSURE1(maxDistance >= 0, maxDistance);

		typedef KdTree<N, Real, ObjectPolicy> Tree;
		typedef typename Tree::Cursor Cursor;
		typedef typename Tree::ConstObjectIterator ConstObjectIterator;
		typedef KeyValue<Real, ConstObjectIterator> KeyVal;

		SmallSet<KeyVal> result;
		findNearest(tree, point, maxDistance, normFunctor, 1, result);

		if (result.empty())
		{
			return keyValue(infinity<Real>(), tree.end());
		}

		return keyValue(result.front().key(), result.front().value());
	}

	template <int N, typename Real, typename ObjectPolicy>
	KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		findNearest(
		const KdTree<N, Real, ObjectPolicy>& tree,
		const Point<N, Real>& point,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance)
	{
		return Pastel::findNearest(
			tree, point, maxDistance, (Real (*)(const Vector<N, Real>&))norm);
	}

	template <int N, typename Real, typename ObjectPolicy>
	KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		findNearest(
		const KdTree<N, Real, ObjectPolicy>& tree,
		const Point<N, Real>& point)
	{
		return Pastel::findNearest(
			tree, point, infinity<Real>(), (Real (*)(const Vector<N, Real>&))norm);
	}

	namespace Detail
	{

		namespace FindNearest
		{

			template <int N, typename Real, typename ObjectPolicy>
			class StackEntry
			{
			public:
				StackEntry()
					: node_()
					, distance_(0)
				{
				}

				StackEntry(
					const typename KdTree<N, Real, ObjectPolicy>::Cursor& node,
					const Real& distance)
					: node_(node)
					, distance_(distance)
				{
				}

				typename KdTree<N, Real, ObjectPolicy>::Cursor node_;
				Real distance_;
			};
		
		}

	}

	template <int N, typename Real, typename ObjectPolicy, typename NormFunctor>
	void findNearest(
		const KdTree<N, Real, ObjectPolicy>& tree,
		const Point<N, Real>& point,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		NormFunctor normFunctor,
		integer count,
		SmallSet<KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator> >& result)
	{
		ENSURE1(maxDistance >= 0, maxDistance);

		result.clear();

		if (tree.empty() || count == 0)
		{
			return;
		}

		const bool onlyNearest = (count > 0);
		Real cullDistance = maxDistance;
		Real cullInfinityDistance = maxDistance;

		typedef KdTree<N, Real, ObjectPolicy> Tree;
		typedef typename Tree::Cursor Cursor;
		typedef typename Tree::ConstObjectIterator ConstObjectIterator;
		typedef KeyValue<Real, ConstObjectIterator> KeyVal;

		typedef SmallSet<KeyVal> ResultContainer;
		typedef Detail::FindNearest::StackEntry<N, Real, ObjectPolicy> StackEntry;

		std::vector<StackEntry> stack;
		stack.push_back(StackEntry(tree.root(), -1));

		while(!stack.empty())
		{
			StackEntry stackEntry = stack.back();
			stack.pop_back();

			if (stackEntry.distance_ > cullInfinityDistance)
			{
				// The node is beyond cull distance, skip it.
				continue;
			}

			Cursor cursor(stackEntry.node_);

			while(!cursor.leaf())
			{
				const Real planeDistance = 
					point[cursor.splitAxis()] - cursor.splitPosition();
				if (planeDistance < 0)
				{
					// First traverse negative, then positive.
					stack.push_back(StackEntry(cursor.positive(), -planeDistance));
					cursor = cursor.negative();
				}
				else
				{
					// First traverse positive, then negative.
					stack.push_back(StackEntry(cursor.negative(), planeDistance));
					cursor = cursor.positive();
				}
			}

			// Iterate through the points in this leaf cursor.

			ConstObjectIterator iter = cursor.begin();
			ConstObjectIterator iterEnd = cursor.end();

			while(iter != iterEnd)
			{
				const Real currentDistance = 
					normFunctor(tree.objectPolicy().bound(*iter).min() - point);

				if (currentDistance < cullDistance)
				{
					result.insert(
						KeyVal(currentDistance, iter));

					if (onlyNearest && result.size() > count)
					{
						result.pop_back();

						// We now know that all 'count'
						// elements of the priority queue
						// are inside the radius given
						// by the farthest nearest point
						// in the list thus far.

						cullDistance = result.back().key();
						cullInfinityDistance = normInfinity(
							tree.objectPolicy().bound(*(result.back().value())).min() - point);
					}
				}

				++iter;
			}
		}
	}

	namespace Detail
	{

		template <
			int N, typename Real,
			typename ObjectPolicy>
		class RecursiveRayTraversal
		{
		private:
			typedef KdTree<N, Real, ObjectPolicy> Tree;
			typedef typename Tree::Cursor Cursor;
			typedef typename Tree::ConstObjectIterator ConstObjectIterator;

			class StackElement
			{
			public:
				StackElement()
					: node_()
					, tMin_(0)
					, tMax_(0)
				{
				}

				StackElement(
					const Cursor& node,
					const Real& tMin,
					const Real& tMax)
					: node_(node)
					, tMin_(tMin)
					, tMax_(tMax)
				{
				}

				Cursor node_;
				Real tMin_;
				Real tMax_;
			};

		public:

			template <typename Intersector>
			ConstObjectIterator work(
				const KdTree<N, Real, ObjectPolicy>& tree,
				const Line<N, Real>& ray,
				Intersector intersector)
			{
				Vector<2, Real> tHit;
				if (!intersect(ray, tree.bound(), tHit) ||
					tHit[1] < 0)
				{
					return tree.end();
				}

				if (tHit[0] < 0)
				{
					tHit[0] = 0;
				}

				// We are doing a front-to-back depth-first traversal.
				// The traversal is
				// done iteratively rather than
				// recursively by using a stack.

				std::vector<StackElement> stack;
				stack.reserve(50);

				Cursor farChild;
				stack.push_back(StackElement(tree.root(), tHit[0], tHit[1]));

				Real tClosest = infinity<Real>();
				ConstObjectIterator iterClosest = tree.end();

				while(!stack.empty())
				{
					StackElement stackElement = stack.back();
					stack.pop_back();
					
					Cursor currentNode = stackElement.node_;
					Real tMin = stackElement.tMin_;
					Real tMax = stackElement.tMax_;
					Point<N, Real> minPosition = ray.position() + tMin * ray.direction();
					Point<N, Real> maxPosition = ray.position() + tMax * ray.direction();

					while(!currentNode.leaf())
					{
						const Real splitPosition = currentNode.splitPosition();
						const integer splitAxis = currentNode.splitAxis();

						if (minPosition[splitAxis] <= splitPosition)
						{
							if (maxPosition[splitAxis] <= splitPosition)
							{
								// Only the negative child needs to be traversed.

								currentNode = currentNode.negative();
								continue;
							}
							// The thesis has a bug here, reading
							//if (stack[exitIndex].position_[splitAxis] == splitPosition)
							// Clearly it should be:
							if (minPosition[splitAxis] == splitPosition)
							{
								// Only the positive child needs to be traversed.

								currentNode = currentNode.positive();
								continue;
							}

							// Both children need to be traversed,
							// first the negative, then the positive.

							farChild = currentNode.positive();
							currentNode = currentNode.negative();
						}
						else
						{
							if (maxPosition[splitAxis] > splitPosition)
							{
								// Only the positive child needs to be traversed.

								currentNode = currentNode.positive();
								continue;
							}

							// Both children need to be traversed,
							// first the positive, then the negative.

							farChild = currentNode.negative();
							currentNode = currentNode.positive();
						}

						// Traversing both children: the farther one
						// is put on the stack.
						// Note if both children are not traversed
						// then this code is skipped by the continues.

						ASSERT(ray.direction()[splitAxis] != 0);

						//const Real t = (splitPosition - ray.position()[splitAxis]) / 
						//	ray.direction()[splitAxis];
						const Real t = (splitPosition - ray.position()[splitAxis]) *
							ray.inverseDirection()[splitAxis];

						ASSERT(t != infinity<Real>());
						ASSERT(t != -infinity<Real>());

						stack.push_back(StackElement(farChild, t, tMax));

						tMax = t;
						maxPosition = ray.position() + tMax * ray.direction();
						maxPosition[splitAxis] = splitPosition;
					}

					// We are now in a leaf node.

					//ASSERT(tMin < tMax);

					ConstObjectIterator iter = currentNode.begin();
					ConstObjectIterator iterEnd = currentNode.end();
					while(iter != iterEnd)
					{
						Real t = 0;
						if (intersector(*iter, ray, t))
						{
							// Only accept intersection if it
							// it is contained in this leaf node
							// and it is closer than the previous 
							// intersection.

							if (t < tClosest &&
								t >= tMin && t <= tMax)
							{
								tClosest = t;
								iterClosest = iter;
							}
						}

						++iter;
					}

					if (iterClosest != tree.end())
					{
						return iterClosest;
					}
				}

				return tree.end();
			}
		};

	}

	template <
		int N, 
		typename Real, 
		typename ObjectPolicy,
		typename Intersector>
		typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator
		recursiveRayTraversal(
		const KdTree<N, Real, ObjectPolicy>& tree,
		const Line<N, Real>& ray,
		Intersector intersector)
	{
		static Detail::RecursiveRayTraversal<N, Real, ObjectPolicy> rayTraversal;
		return rayTraversal.work(tree, ray, intersector);
	}

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
		Detail::refineSurfaceAreaHeuristic(
			tree.root(), 0, tree.bound(), maxDepth, maxObjects, 0, tree);
	}

	namespace Detail
	{

		template <int N, typename Real, typename ObjectPolicy>
		integer depth(const KdTree<N, Real, ObjectPolicy>& tree,
			const typename KdTree<N, Real, ObjectPolicy>::Cursor& cursor,
			integer currentDepth)
		{
			if (cursor.leaf())
			{
				return currentDepth;
			}

			return std::max(
				depth(tree, cursor.positive(), currentDepth + 1), 
				depth(tree, cursor.negative(), currentDepth + 1));
		}

	}

	template <int N, typename Real, typename ObjectPolicy>
	integer depth(const KdTree<N, Real, ObjectPolicy>& tree)
	{
		return Detail::depth(tree, tree.root(), 0);
	}

	namespace Detail
	{

		template <int N, typename Real, typename ObjectPolicy>
		bool check(const KdTree<N, Real, ObjectPolicy>& tree,
			const typename KdTree<N, Real, ObjectPolicy>::Cursor& cursor,
			const AlignedBox<N, Real>& bound)
		{
			if (cursor.leaf())
			{
				if (cursor.objects() == 0)
				{
					if (REPORT(cursor.begin() != tree.end() ||
						cursor.end() != tree.end()))
					{
						return false;
					}
				}
				else
				{
					if (REPORT(cursor.begin() == cursor.end()))
					{
						return false;
					}
					if (REPORT(std::distance(cursor.begin(), cursor.end()) != cursor.objects()))
					{
						return false;
					}
				}
			}
			else
			{
				if (REPORT(cursor.splitPosition() < bound.min()[cursor.splitAxis()]))
				{
					return false;
				}

				if (REPORT(cursor.splitPosition() > bound.max()[cursor.splitAxis()]))
				{
					return false;
				}

				AlignedBox<N, Real> positiveBound(bound);
				positiveBound.min()[cursor.splitAxis()] = cursor.splitPosition();

				if (!check(tree, cursor.positive(), positiveBound))
				{
					return false;
				}

				AlignedBox<N, Real> negativeBound(bound);
				negativeBound.max()[cursor.splitAxis()] = cursor.splitPosition();

				if (!check(tree, cursor.negative(), negativeBound))
				{
					return false;
				}
			}

			return true;
		}

	}

	template <int N, typename Real, typename ObjectPolicy>
	bool check(const KdTree<N, Real, ObjectPolicy>& tree)
	{
		return Detail::check(tree, tree.root(), tree.bound());
	}

}

#endif
