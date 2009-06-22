#ifndef PASTEL_COUNT_ALL_NEAREST_NEIGHBORS_KDTREE_HPP
#define PASTEL_COUNT_ALL_NEAREST_NEIGHBORS_KDTREE_HPP

#include "pastel/geometry/count_all_nearest_neighbors_kdtree.h"
#include "pastel/geometry/kdtree_tools.h"

#include "pastel/device/timer.h"

#include "pastel/sys/pastelomp.h"

namespace Pastel
{

	namespace Detail_CountAllNearestNeighborsKdTree
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

	namespace Detail_CountAllNearestNeighborsKdTree
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

		template <typename Type>
		class SequenceIterator
		{
		public:
			SequenceIterator()
				: data_()
			{
			}

			explicit SequenceIterator(const Type& data)
				: data_(data)
			{
			}

			const Type& operator*() const
			{
				return data_;
			}

			SequenceIterator& operator++()
			{
				++data_;
				return *this;
			}

			bool operator==(const SequenceIterator& that) const
			{
				return data_ == that.data_;
			}

			bool operator!=(const SequenceIterator& that) const
			{
				return data_ != that.data_;
			}
		
		private:
			Type data_;
		};

	}

	template <int N, typename Real, typename NormBijection, typename CountFunctor>
	void countAllNearestNeighborsKdTree(
		const std::vector<Point<N, Real> >& pointSet,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection,
		const CountFunctor& countFunctor)
	{
		ENSURE1(maxDistance >= 0, maxDistance);
		ENSURE1(maxRelativeError >= 0, maxRelativeError);

		const integer points = pointSet.size();

		if (kNearest == 0 || points == 0)
		{
			// Nothing to compute.
			return;
		}

		typedef PointKdTree<N, Real, 
			Detail_CountAllNearestNeighborsKdTree::PointListPolicy<N, Real> > Tree;
		typedef typename Tree::ConstObjectIterator ConstTreeIterator;
		typedef Detail_CountAllNearestNeighborsKdTree::SequenceIterator<const Point<N, Real>*>
			SequenceIterator;

		const integer dimension = pointSet.front().size();

		Tree tree(dimension);

		tree.insert(SequenceIterator(&pointSet[0]), 
			SequenceIterator(&pointSet[0] + pointSet.size()));

		tree.refine(
			computeKdTreeMaxDepth(tree.objects()), 16, SlidingMidpoint2_SplitRule());

#pragma omp parallel for
		for (integer i = 0;i < points;++i)
		{
			// The query point itself is in the count,
			// thus we decrement it off.

			countFunctor(i, countNearest(tree, pointSet[i], 
				maxDistance, maxRelativeError, normBijection) - 1);
		}
	}

	template <int N, typename Real, typename NormBijection, typename CountFunctor>
	void countAllNearestNeighborsKdTree(
		const std::vector<Point<N, Real> >& pointSet,
		const std::vector<PASTEL_NO_DEDUCTION(Real)>& maxDistanceSet,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection,
		const CountFunctor& countFunctor)
	{
		ENSURE2(pointSet.size() == maxDistanceSet.size(),
			pointSet.size(), maxDistanceSet.size());
		ENSURE1(maxRelativeError >= 0, maxRelativeError);

		const integer points = pointSet.size();

		if (pointSet.empty())
		{
			// Nothing to compute.
			return;
		}

		typedef PointKdTree<N, Real, 
			Detail_CountAllNearestNeighborsKdTree::PointListPolicy<N, Real> > Tree;
		typedef typename Tree::ConstObjectIterator ConstTreeIterator;
		typedef Detail_CountAllNearestNeighborsKdTree::SequenceIterator<const Point<N, Real>*>
			SequenceIterator;

		const integer dimension = pointSet.front().size();

		Tree tree(dimension);

		tree.insert(SequenceIterator(&pointSet[0]), 
			SequenceIterator(&pointSet[0] + pointSet.size()));

		tree.refine(
			computeKdTreeMaxDepth(tree.objects()), 16, SlidingMidpoint2_SplitRule());

#pragma omp parallel for
		for (integer i = 0;i < points;++i)
		{
			// The query point itself is in the count,
			// thus we decrement it off.

			countFunctor(i, countNearest(tree, pointSet[i], 
				maxDistanceSet[i], maxRelativeError, normBijection) - 1);
		}
	}

	template <int N, typename Real, typename NormBijection>
	integer countAllNearestNeighborsKdTree(
		const std::vector<Point<N, Real> >& pointSet,
		const std::vector<PASTEL_NO_DEDUCTION(Real)>& maxDistanceSet,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection)
	{
		integer result = 0;
		const Detail_CountAllNearestNeighborsKdTree::Total_CountFunctor 
			countFunctor(result);

		Pastel::countAllNearestNeighborsKdTree(
			pointSet, maxDistanceSet, maxRelativeError,
			normBijection, countFunctor);

		return result;
	}

	template <int N, typename Real, typename NormBijection>
	void countAllNearestNeighborsKdTree(
		const std::vector<Point<N, Real> >& pointSet,
		const std::vector<PASTEL_NO_DEDUCTION(Real)>& maxDistanceSet,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection,
		std::vector<integer>& countSet)
	{
		countSet.setSize(pointSet.size());

		const Detail_CountAllNearestNeighborsKdTree::Vector_CountFunctor 
			countFunctor(countSet);

		Pastel::countAllNearestNeighborsKdTree(
			pointSet, maxDistanceSet, maxRelativeError,
			normBijection, countFunctor);

		return result;
	}

}


#endif
