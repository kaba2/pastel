#ifndef PASTEL_COUNT_ALL_NEIGHBORS_KDTREE_HPP
#define PASTEL_COUNT_ALL_NEIGHBORS_KDTREE_HPP

#include "pastel/geometry/count_all_neighbors_kdtree.h"
#include "pastel/geometry/count_all_neighbors_1d.h"
#include "pastel/geometry/kdtree_tools.h"

#include "pastel/sys/pastelomp.h"
#include "pastel/sys/countingiterator.h"

namespace Pastel
{

	namespace Detail_CountAllNeighborsKdTree
	{

		class Total_CountFunctor
		{
		public:
			explicit Total_CountFunctor(integer& totalCount)
				: totalCount_(totalCount)
			{
			}

			void operator()(integer index, integer count) const
			{
				totalCount_ += count;
			}
		private:
			integer& totalCount_;
		};

		class Vector_CountFunctor
		{
		public:
			explicit Vector_CountFunctor(
				std::vector<integer>& countSet)
				: countSet_(countSet)
			{
			}

			void operator()(integer index, integer count) const
			{
				countSet_[index] = count;
			}

		private:
			std::vector<integer>& countSet_;
		};

	}

	namespace Detail_CountAllNeighborsKdTree
	{

		template <int N, typename Real>
		class PointListPolicy
		{
		public:
			typedef const Point<N, Real>* Object;
			typedef FalseType ArbitrarySplits;

			const Point<N, Real>& point(
				const Object& object) const
			{
				return *object;
			}

			Real point(
				const Object& object, integer axis) const
			{
				return (*object)[axis];
			}
		};

	}

	template <int N, typename Real, typename NormBijection, typename CountFunctor>
	void countAllNeighborsKdTree(
		const std::vector<Point<N, Real> >& pointSet,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection,
		const CountFunctor& countFunctor)
	{
		ENSURE_OP(maxDistance, >=, 0);
		ENSURE_OP(maxRelativeError, >=, 0);

		const integer points = pointSet.size();

		if (kNearest == 0 || points == 0)
		{
			// Nothing to compute.
			return;
		}

		const integer dimension = pointSet.front().size();

		typedef PointKdTree<N, Real, 
			Detail_CountAllNeighborsKdTree::PointListPolicy<N, Real> > Tree;
		typedef typename Tree::ConstObjectIterator ConstTreeIterator;
		typedef ConstCountingIterator<const Point<N, Real>*> SequenceIterator;

		Tree tree(dimension);

		tree.insert(SequenceIterator(&pointSet[0]), 
			SequenceIterator(&pointSet[0] + pointSet.size()));

		tree.refine(
			computeKdTreeMaxDepth(tree.objects()), 16, SlidingMidpoint2_SplitRule());

#pragma omp parallel for
		for (integer i = 0;i < points;++i)
		{
			countFunctor(i, countNearest(tree, pointSet[i], 
				maxDistance, maxRelativeError, normBijection));
		}
	}

	template <int N, typename Real, typename NormBijection, typename CountFunctor>
	void countAllNeighborsKdTree(
		const std::vector<Point<N, Real> >& pointSet,
		const std::vector<PASTEL_NO_DEDUCTION(Real)>& maxDistanceSet,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection,
		const CountFunctor& countFunctor)
	{
		ENSURE_OP(pointSet.size(), ==, maxDistanceSet.size());
		ENSURE_OP(maxRelativeError, >=, 0);

		const integer points = pointSet.size();

		if (pointSet.empty())
		{
			// Nothing to compute.
			return;
		}

		const integer dimension = pointSet.front().size();

		typedef PointKdTree<N, Real, 
			Detail_CountAllNeighborsKdTree::PointListPolicy<N, Real> > Tree;
		typedef typename Tree::ConstObjectIterator ConstTreeIterator;
		typedef Detail_CountAllNeighborsKdTree::CountingIterator<const Point<N, Real>*>
			SequenceIterator;

		Tree tree(dimension);

		tree.insert(SequenceIterator(&pointSet[0]), 
			SequenceIterator(&pointSet[0] + pointSet.size()));

		tree.refine(
			computeKdTreeMaxDepth(tree.objects()), 16, SlidingMidpoint2_SplitRule());

#pragma omp parallel for
		for (integer i = 0;i < points;++i)
		{
			const integer count = 
				countNearest(tree, pointSet[i], 
				maxDistanceSet[i], maxRelativeError, normBijection);

			countFunctor(i, count);
		}
	}

	template <int N, typename Real, typename NormBijection>
	integer countAllNeighborsKdTree(
		const std::vector<Point<N, Real> >& pointSet,
		const std::vector<PASTEL_NO_DEDUCTION(Real)>& maxDistanceSet,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection)
	{
		integer result = 0;
		const Detail_CountAllNeighborsKdTree::Total_CountFunctor 
			countFunctor(result);

		Pastel::countAllNeighborsKdTree(
			pointSet, maxDistanceSet, maxRelativeError,
			normBijection, countFunctor);

		return result;
	}

	template <int N, typename Real, typename NormBijection>
	void countAllNeighborsKdTree(
		const std::vector<Point<N, Real> >& pointSet,
		const std::vector<PASTEL_NO_DEDUCTION(Real)>& maxDistanceSet,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection,
		std::vector<integer>& countSet)
	{
		const integer points = pointSet.size();
		countSet.resize(pointSet.size());

		if (points == 0)
		{
			return;
		}

		const integer dimension = pointSet.front().dimension();

		if (dimension == 1)
		{
			countAllNeighbors1d(pointSet, maxDistanceSet,
				normBijection, countSet);
			return;
		}

		std::fill(countSet.begin(), countSet.end(), 0);

		const Detail_CountAllNeighborsKdTree::Vector_CountFunctor 
			countFunctor(countSet);

		Pastel::countAllNeighborsKdTree(
			pointSet, maxDistanceSet, maxRelativeError,
			normBijection, countFunctor);
	}

}


#endif
