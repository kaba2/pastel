#ifndef PASTELGEOMETRY_KDTREE_NEAREST_HPP
#define PASTELGEOMETRY_KDTREE_NEAREST_HPP

#include "pastel/geometry/kdtree_nearest.h"
#include "pastel/geometry/closest_alignedbox_point.h"

#include "pastel/sys/smallset.h"
#include "pastel/sys/mytypes.h"

#include <vector>

namespace Pastel
{

	// Depth-first approximate search

	namespace Detail_FindNearest
	{

		template <int N, typename Real, typename ObjectPolicy>
		class NodeEntry
		{
		public:
			NodeEntry(
				const typename KdTree<N, Real, ObjectPolicy>::Cursor& node,
				const Real& distance)
				: node_(node)
				, distance_(distance)
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
		};

	}

	template <int N, typename Real, typename ObjectPolicy, typename NormBijection>
	void findNearest(
		const KdTree<N, Real, ObjectPolicy>& tree,
		const Point<N, Real>& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection,
		integer kNearest,
		SmallSet<KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator> >& result)
	{
		ENSURE1(maxDistance >= 0, maxDistance);
		ENSURE1(maxRelativeError >= 0, maxRelativeError);
		ENSURE1(kNearest > 0, kNearest);

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
		const Real errorScaling = 
			normBijection.scalingFactor(1 + maxRelativeError);

		std::vector<NodeEntry> nodeStack;
		nodeStack.push_back(NodeEntry(tree.root(), 
			normBijection(closest(tree.bound(), searchPoint) - searchPoint)));

		Real cullDistance = maxDistance;

		while(!nodeStack.empty())
		{
			NodeEntry nodeEntry = nodeStack.back();
			nodeStack.pop_back();

			if (nodeEntry.distance_ * errorScaling > cullDistance)
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
				const integer splitAxis = cursor.splitAxis();
				
				const Real searchPosition =
					searchPoint[splitAxis];
				
				const Real splitPosition = 
					cursor.splitPosition();

				Cursor nearBranch;
				Cursor farBranch;

				if (searchPosition < splitPosition)
				{
					// The search point is closer to the left branch so follow that.
					farBranch = cursor.positive();
					nearBranch = cursor.negative();
				}
				else
				{
					// The search point is closer to the right branch so follow that.
					farBranch = cursor.negative();
					nearBranch = cursor.positive();
				}

				// Try to cull the farther node off based on the distance 
				// of the search point to the splitting plane.

				const Real planeDistance =
					normBijection(splitAxis,
					searchPosition - splitPosition);

				if (planeDistance * errorScaling <= cullDistance)
				{
					// Try to cull the farther node off based on the distance 
					// of the search point to the farther child node.

					Real oldAxisDistance = 0;
					if (searchPosition < cursor.min())
					{
						oldAxisDistance = 
							normBijection(splitAxis, cursor.min() - searchPosition);
					}
					else if (searchPosition > cursor.max())
					{
						oldAxisDistance = 
							normBijection(splitAxis, searchPosition - cursor.max());
					}

					const Real childDistance = 
						normBijection.replaceAxisDistance(
						nodeEntry.distance_, oldAxisDistance,
						planeDistance);
					
					if (childDistance * errorScaling <= cullDistance)
					{
						// No culling could be done, visit the farther node
						// later.
						nodeStack.push_back(NodeEntry(farBranch, childDistance));
					}
				}

				// Follow downwards the tree with the nearer node.
				cursor = nearBranch;
			}

			// We are now in a leaf node.
			// Search through the objects in this node.

			ConstObjectIterator iter = cursor.begin();
			ConstObjectIterator iterEnd = cursor.end();

			while(iter != iterEnd)
			{
				const Real currentDistance =
					normBijection(tree.objectPolicy().point(*iter) - searchPoint,
					cullDistance);

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

	// Best-first search.
	// All non-leaf nodes to priority queue.

	namespace Detail_FindNearestBestFirst
	{

		template <int N, typename Real, typename ObjectPolicy>
		class NodeEntry
		{
		public:
			NodeEntry(
				const typename KdTree<N, Real, ObjectPolicy>::Cursor& node,
				const Real& distance,
				const TemporaryVector<N, Real>& distances)
				: node_(node)
				, distance_(distance)
				, distances_(distances)
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
			Vector<N, Real> distances_;
		};

	}

	template <int N, typename Real, typename ObjectPolicy, typename NormBijection>
	void findNearestBestFirst(
		const KdTree<N, Real, ObjectPolicy>& tree,
		const Point<N, Real>& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const NormBijection& normBijection,
		integer kNearest,
		SmallSet<KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator> >& result)
	{
		ENSURE2(tree.dimension() == searchPoint.size(), 
			tree.dimension(), searchPoint.size());
		ENSURE1(maxDistance >= 0, maxDistance);
		ENSURE1(kNearest > 0, kNearest);

		result.clear();

		if (tree.empty() || kNearest == 0)
		{
			return;
		}

		const integer dimension = tree.dimension();

		typedef KdTree<N, Real, ObjectPolicy> Tree;
		typedef typename Tree::Cursor Cursor;
		typedef typename Tree::ConstObjectIterator ConstObjectIterator;
		typedef KeyValue<Real, ConstObjectIterator> KeyVal;

		typedef SmallSet<KeyVal> ResultContainer;
		typedef Detail_FindNearestBestFirst::NodeEntry<N, Real, ObjectPolicy> NodeEntry;

		const bool kFinite = (kNearest > 0);
		Real cullDistance = maxDistance;

		typedef std::set<NodeEntry> Stack;
		typedef typename Stack::iterator StackIterator;
		
		Vector<N, Real> boundTranslation = 
			closest(tree.bound(), searchPoint) - searchPoint;
		ASSERT(boundTranslation.size() == dimension);
		
		for (integer i = 0;i < dimension;++i)
		{
			boundTranslation[i] = normBijection(i, boundTranslation[i]);
		}
		
		Stack nodeStack;
		nodeStack.insert(NodeEntry(tree.root(), 
			sum(boundTranslation), boundTranslation));

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
					Vector<N, Real> distances = nodeEntry.distances_.asTemporary();
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
	}

	// Best-first search.
	// Only the other non-leaf node to the priority queue,
	// depth first.

	template <int N, typename Real, typename ObjectPolicy, typename NormBijection>
	void findNearestBestFirst3(
		const KdTree<N, Real, ObjectPolicy>& tree,
		const Point<N, Real>& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const NormBijection& normBijection,
		integer kNearest,
		SmallSet<KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator> >& result)
	{
		ENSURE2(tree.dimension() == searchPoint.size(), 
			tree.dimension(), searchPoint.size());
		ENSURE1(maxDistance >= 0, maxDistance);
		ENSURE1(kNearest > 0, kNearest);

		result.clear();

		if (tree.empty() || kNearest == 0)
		{
			return;
		}

		const integer dimension = tree.dimension();

		typedef KdTree<N, Real, ObjectPolicy> Tree;
		typedef typename Tree::Cursor Cursor;
		typedef typename Tree::ConstObjectIterator ConstObjectIterator;
		typedef KeyValue<Real, ConstObjectIterator> KeyVal;

		typedef SmallSet<KeyVal> ResultContainer;
		typedef Detail_FindNearestBestFirst::NodeEntry<N, Real, ObjectPolicy> NodeEntry;

		const bool kFinite = (kNearest > 0);
		Real cullDistance = maxDistance;

		typedef std::set<NodeEntry> Stack;
		typedef typename Stack::iterator StackIterator;
		
		Vector<N, Real> boundTranslation = 
			closest(tree.bound(), searchPoint) - searchPoint;
		ASSERT(boundTranslation.size() == dimension);
		
		for (integer i = 0;i < dimension;++i)
		{
			boundTranslation[i] = normBijection(i, boundTranslation[i]);
		}
		
		Stack nodeStack;
		nodeStack.insert(NodeEntry(tree.root(), 
			sum(boundTranslation), boundTranslation));

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
			while(!cursor.leaf())
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

				// Possibly push the other branch to the nodeStack.
				if (planeDistance <= cullDistance)
				{
					Vector<N, Real> distances = nodeEntry.distances_.asTemporary();
					distances[cursor.splitAxis()] = planeDistance;

					nodeStack.insert(NodeEntry(farBranch, 
						sum(distances), distances));
				}
				
				cursor = nearBranch;
			}

			// We are in a leaf node.
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
	void findNearestBestFirst2(
		const KdTree<N, Real, ObjectPolicy>& tree,
		const Point<N, Real>& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const NormBijection& normBijection,
		integer kNearest,
		SmallSet<KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator> >& result)
	{
		ENSURE2(tree.dimension() == searchPoint.size(), 
			tree.dimension(), searchPoint.size());
		ENSURE1(maxDistance >= 0, maxDistance);
		ENSURE1(kNearest > 0, kNearest);

		result.clear();

		if (tree.empty() || kNearest == 0)
		{
			return;
		}

		const integer dimension = tree.dimension();

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
			while(!cursor.leaf())
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

				// Possibly push the other branch to the nodeStack.
				if (planeDistance <= cullDistance)
				{
					nodeStack.insert(NodeEntry(farBranch, planeDistance));
				}

				cursor = nearBranch;
			}

			// We are in a leaf node.
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
	KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		findNearest(
		const KdTree<N, Real, ObjectPolicy>& tree,
		const Point<N, Real>& point,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection)
	{
		ENSURE1(maxDistance >= 0, maxDistance);
		ENSURE1(maxRelativeError >= 0, maxRelativeError);

		typedef KdTree<N, Real, ObjectPolicy> Tree;
		typedef typename Tree::Cursor Cursor;
		typedef typename Tree::ConstObjectIterator ConstObjectIterator;
		typedef KeyValue<Real, ConstObjectIterator> KeyVal;

		SmallSet<KeyVal> result;
		findNearest(tree, point, maxDistance, maxRelativeError,
			normBijection, 1, result);

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
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError)
	{
		ENSURE1(maxDistance >= 0, maxDistance);
		ENSURE1(maxRelativeError >= 0, maxRelativeError);

		return Pastel::findNearest(
			tree, point, maxDistance, maxRelativeError,
			EuclideanNormBijection<N, Real>());
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
			tree, point, maxDistance, 0,
			EuclideanNormBijection<N, Real>());
	}

	template <int N, typename Real, typename ObjectPolicy>
	KeyValue<Real, typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator>
		findNearest(
		const KdTree<N, Real, ObjectPolicy>& tree,
		const Point<N, Real>& point)
	{
		return Pastel::findNearest(
			tree, point, infinity<Real>(), 0,
			EuclideanNormBijection<N, Real>());
	}

}

#endif
