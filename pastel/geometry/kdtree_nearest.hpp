#ifndef PASTELGEOMETRY_KDTREE_NEAREST_HPP
#define PASTELGEOMETRY_KDTREE_NEAREST_HPP

#include "pastel/geometry/kdtree_nearest.h"

#include "pastel/sys/smallset.h"

#include <vector>

namespace Pastel
{

	namespace Detail_FindNearest
	{

		template <int N, typename Real, typename ObjectPolicy>
		class NodeEntry
		{
		public:
			NodeEntry()
				: node_()
				, distance_(0)
			{
			}

			NodeEntry(
				const typename KdTree<N, Real, ObjectPolicy>::Cursor& node,
				const Real& distance/*,
				const Vector<N, Real>& distances = Vector<N, Real>(0)*/)
				: node_(node)
				, distance_(distance)
				//, distances_(distances)
			{
			}

			bool operator<(const NodeEntry& that) const
			{
				if (distance_ < that.distance_)
				{
					return true;
				}
				if (that.distance_ < distance_)
				{
					return false;
				}
				
				return node_ < that.node_;
			}

			typename KdTree<N, Real, ObjectPolicy>::Cursor node_;
			Real distance_;
			//Vector<N, Real> distances_;
		};

	}

	template <int N, typename Real, typename ObjectPolicy, typename NormBijection>
	void findNearest(
		const KdTree<N, Real, ObjectPolicy>& tree,
		const Point<N, Real>& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const NormBijection& normBijection,
		integer kNearest,
		SmallSet<KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator> >& result)
	{
		ENSURE1(maxDistance >= 0, maxDistance);

		result.clear();

		if (tree.empty() || kNearest == 0)
		{
			return;
		}

		typedef KdTree<N, Real, ObjectPolicy> Tree;
		typedef typename Tree::Cursor Cursor;
		typedef typename Tree::ConstObjectIterator ConstObjectIterator;
		typedef KeyValue<Real, ConstObjectIterator> KeyVal;

		typedef SmallSet<KeyVal> ResultContainer;
		typedef Detail_FindNearest::NodeEntry<N, Real, ObjectPolicy> NodeEntry;

		const bool kFinite = (kNearest > 0);
		Real cullDistance = maxDistance;

		std::vector<NodeEntry> nodeStack;
		nodeStack.push_back(NodeEntry(tree.root(), 0));

		while(!nodeStack.empty())
		{
			NodeEntry nodeEntry = nodeStack.back();
			nodeStack.pop_back();

			if (nodeEntry.distance_ > cullDistance)
			{
				// The node is beyond cull distance, skip it.
				continue;
			}

			// Find the leaf node that is closest
			// to the search searchPoint in this current branch.
			// Place all the alternative branching nodes into
			// a nodeStack.

			Cursor cursor(nodeEntry.node_);
			while(!cursor.leaf())
			{
				const Real planeDistance =
					normBijection(cursor.splitAxis(),
					searchPoint[cursor.splitAxis()] - cursor.splitPosition());

				Cursor otherBranch;

				if (searchPoint[cursor.splitAxis()] < cursor.splitPosition())
				{
					// The search point is closer to the left branch so follow that.
					otherBranch = cursor.positive();
					cursor = cursor.negative();
				}
				else
				{
					// The search point is closer to the right branch so follow that.
					otherBranch = cursor.negative();
					cursor = cursor.positive();
				}

				// Possibly push the other branch to the nodeStack.
				if (planeDistance <= cullDistance)
				{
					nodeStack.push_back(NodeEntry(otherBranch, planeDistance));
				}
			}

			// We are now in a leaf node.
			// Search through the objects in this node.

			ConstObjectIterator iter = cursor.begin();
			ConstObjectIterator iterEnd = cursor.end();

			while(iter != iterEnd)
			{
				const Real currentDistance =
					normBijection(tree.objectPolicy().point(*iter) - searchPoint);

				// It is essential that this is <= rather
				// than <, because of the possibility
				// of multiple points at same location.
				if (currentDistance <= cullDistance)
				{
					result.insert(
						KeyVal(currentDistance, iter));

					if (kFinite && result.size() > kNearest)
					{
						result.pop_back();

						// Since we now know k neighboring points,
						// we can bound the search radius
						// by the distance to the currently farthest
						// neighbor.

						cullDistance = result.back().key();
					}
				}

				++iter;
			}
		}
	}

	template <int N, typename Real, typename ObjectPolicy, typename NormBijection>
	void findNearestOld(
		const KdTree<N, Real, ObjectPolicy>& tree,
		const Point<N, Real>& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const NormBijection& normBijection,
		integer kNearest,
		SmallSet<KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator> >& result)
	{
		ENSURE1(maxDistance >= 0, maxDistance);

		result.clear();

		if (tree.empty() || kNearest == 0)
		{
			return;
		}

		typedef KdTree<N, Real, ObjectPolicy> Tree;
		typedef typename Tree::Cursor Cursor;
		typedef typename Tree::ConstObjectIterator ConstObjectIterator;
		typedef KeyValue<Real, ConstObjectIterator> KeyVal;

		typedef SmallSet<KeyVal> ResultContainer;
		typedef Detail_FindNearest::NodeEntry<N, Real, ObjectPolicy> NodeEntry;

		const bool kFinite = (kNearest > 0);
		Real cullDistance = maxDistance;

		typedef std::set<NodeEntry> Stack;
		typedef typename Stack::iterator StackIterator;
		Stack nodeStack;
		nodeStack.insert(NodeEntry(tree.root(), 0));

		while(!nodeStack.empty())
		{
			NodeEntry nodeEntry = *nodeStack.begin();
			nodeStack.erase(nodeStack.begin());

			if (nodeEntry.distance_ > cullDistance)
			{
				// All of the nodes that are to follow
				// are beyond the cull distance, we are done.
				return;
			}

			// Find the leaf node that is closest
			// to the search searchPoint in this current branch.
			// Place all the alternative branching nodes into
			// a nodeStack.

			Cursor cursor(nodeEntry.node_);
			if (!cursor.leaf())
			{
				const Real planeDistance =
					normBijection(cursor.splitAxis(),
					searchPoint[cursor.splitAxis()] - cursor.splitPosition());

				Cursor nearBranch;
				Cursor farBranch;

				if (searchPoint[cursor.splitAxis()] < cursor.splitPosition())
				{
					// The search point is closer to the left branch so follow that.
					nearBranch = cursor.negative();
					farBranch = cursor.positive();
				}
				else
				{
					// The search point is closer to the right branch so follow that.
					nearBranch = cursor.positive();
					farBranch = cursor.negative();
				}
				
				{
					nodeStack.insert(NodeEntry(nearBranch, 
						nodeEntry.distance_, nodeEntry.distances_));
				}

				// Possibly push the other branch to the nodeStack.
				if (planeDistance <= cullDistance)
				{
					Vector<N, Real> distances = nodeEntry.distances_;
					distances[cursor.splitAxis()] = planeDistance;

					nodeStack.insert(NodeEntry(farBranch, 
						sum(distances), distances));
				}
			}
			else
			{
				// We are in a leaf node.
				// Search through the objects in this node.

				ConstObjectIterator iter = cursor.begin();
				ConstObjectIterator iterEnd = cursor.end();

				while(iter != iterEnd)
				{
					const Point<N, Real> objectPoint = 
						tree.objectPolicy().bound(*iter).min();

					const Real currentDistance =
						normBijection(objectPoint - searchPoint);

					// It is essential that this is <= rather
					// than <, because of the possibility
					// of multiple points at same location.
					if (currentDistance <= cullDistance)
					{
						result.insert(
							KeyVal(currentDistance, iter));

						if (kFinite && result.size() > kNearest)
						{
							result.pop_back();

							// Since we now know k neighboring points,
							// we can bound the search radius
							// by the distance to the currently farthest
							// neighbor.

							cullDistance = result.back().key();
						}
					}

					++iter;
				}
			}
		}
	}

	template <int N, typename Real, typename ObjectPolicy, typename NormBijection>
	KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		findNearest(
		const KdTree<N, Real, ObjectPolicy>& tree,
		const Point<N, Real>& point,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const NormBijection& normBijection)
	{
		ENSURE1(maxDistance >= 0, maxDistance);

		typedef KdTree<N, Real, ObjectPolicy> Tree;
		typedef typename Tree::Cursor Cursor;
		typedef typename Tree::ConstObjectIterator ConstObjectIterator;
		typedef KeyValue<Real, ConstObjectIterator> KeyVal;

		SmallSet<KeyVal> result;
		findNearest(tree, point, maxDistance, normBijection, 1, result);

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
		ENSURE1(maxDistance >= 0, maxDistance);

		return Pastel::findNearest(
			tree, point, maxDistance, EuclideanNormBijection<N, Real>());
	}

	template <int N, typename Real, typename ObjectPolicy>
	KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		findNearest(
		const KdTree<N, Real, ObjectPolicy>& tree,
		const Point<N, Real>& point)
	{
		return Pastel::findNearest(
			tree, point, infinity<Real>(), EuclideanNormBijection<N, Real>());
	}

}

#endif
