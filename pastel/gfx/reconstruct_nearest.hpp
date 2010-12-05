#ifndef PASTEL_RECONSTRUCT_NEAREST_HPP
#define PASTEL_RECONSTRUCT_NEAREST_HPP

#include "pastel/gfx/reconstruct_nearest.h"

#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/pointkdtree_tools.h"
#include "pastel/geometry/overlaps_alignedbox_point.h"

#include "pastel/sys/vector_tools.h"
#include "pastel/sys/view_visit.h"
#include "pastel/sys/nulliterator.h"

namespace Pastel
{

	namespace Detail_ReconstructNearest
	{

		template <typename Real, int N, typename Data>
		class DataPoint
		{
		public:
			typedef Data Data_;

			DataPoint()
				: position_()
				, data_()
			{
			}

			DataPoint(
				const Vector<Real, N>& position,
				const Data& data)
				: position_(position)
				, data_(data)
			{
			}

			Vector<Real, N> position_;
			Data data_;
		};

		template <typename Real_, int N, typename Data>
		class DataPolicy
		{
		public:
			typedef Real_ Real;
			typedef DataPoint<Real, N, Data> Point;
			typedef const Real* ConstIterator;
			typedef ConstArray_VectorExpression<Real, N> Expression;

			explicit DataPolicy(
				integer dimension)
				: dimension_(dimension)
			{
				ENSURE(N == Dynamic || 
					dimension == N);
			}

			integer dimension() const
			{
				return (N != Dynamic) ? N : dimension_;
			}

			integer dimension(const Point& point) const
			{
				return (N != Dynamic) ? N : dimension_;
			}

			Expression operator()(const Point& point) const
			{
				return constVectorExpression<N>(
					begin(point), dimension());
			}

			const Real& axis(const Point& point, integer index) const
			{
				return point.position_[index];
			}

			ConstIterator begin(const Point& point) const
			{
				return point.position_.rawBegin();
			}

			ConstIterator end(const Point& point) const
			{
				return point.position_.rawBegin() + dimension();
			}

		private:
			integer dimension_;
		};

		template <typename Real, int N, typename PointPolicy>
		class ReconstructFunctor
		{
		public:
			explicit ReconstructFunctor(
				const PointKdTree<Real, N, PointPolicy>& kdTree,
				integer kNearest,
				integer maxRelativeError)
				: kdtree_(kdTree)
				, kNearest_(kNearest)
				, maxRelativeError_(maxRelativeError)
			{
			}

			typedef typename PointPolicy::Point Data;

			void operator()(
				const Vector<integer, N>& position,
				typename Data::Data_& data) const
			{
				typedef typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator
					ConstIterator;

				std::vector<ConstIterator> nearestSet;

				searchNearest(
					kdtree_, 
					evaluate(Vector<Real, N>(position) + 0.5), 
					kNearest_, 
					std::back_inserter(nearestSet),
					NullIterator(),
					infinity<Real>(), maxRelativeError_);

				data = nearestSet.back()->point().data_;
			}

		private:
			const PointKdTree<Real, N, PointPolicy>& kdtree_;
			integer kNearest_;
			Real maxRelativeError_;
		};
	}

	template <typename Real, int N, typename Data, typename Output_View>
	void reconstructNearest(
		const std::vector<Vector<Real, N> >& positionList,
		const std::vector<Data>& dataList,
		const AlignedBox<Real, N>& region,
		const View<N, Data, Output_View>& view,
		integer kNearest,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError)
	{
		const integer n = region.dimension();
		const integer points = positionList.size();

		ENSURE_OP(kNearest, >, 0);
		ENSURE2(points == dataList.size(), points, dataList.size());

		typedef Detail_ReconstructNearest::DataPoint<Real, N, Data> DataPoint;
		typedef Detail_ReconstructNearest::DataPolicy<Real, N, Data> DataPolicy;

		DataPolicy pointPolicy(n);

		PointKdTree<Real, N, DataPolicy> kdTree(pointPolicy);

		const Vector<Real, N> scaling = 
			inverse(region.extent()) * Vector<Real, N>(view.extent());

		std::vector<DataPoint> dataPointList;
		for (integer i = 0;i < points;++i)
		{
			if (overlaps(region, positionList[i]))
			{
				dataPointList.push_back(
					DataPoint((positionList[i] - region.min()) * scaling, dataList[i]));
			}
		}

		kdTree.insert(
			range(dataPointList.begin(), dataPointList.end()));

		kdTree.refine(SlidingMidpoint2_SplitRule_PointKdTree());

		Detail_ReconstructNearest::ReconstructFunctor<Real, N, DataPolicy>
			reconstructFunctor(kdTree, kNearest, maxRelativeError);

		visitPosition(
			view, reconstructFunctor);
	}

	template <typename Real, int N, typename Data, typename Output_View>
	void reconstructNearest(
		const std::vector<Vector<Real, N> >& positionList,
		const std::vector<Data>& dataList,
		const AlignedBox<Real, N>& region,
		const View<N, Data, Output_View>& view,
		integer kNearest)
	{
		Pastel::reconstructNearest(
			positionList, dataList,
			region, view, kNearest, 0);
	}

	template <typename Real, int N, typename Data, typename Output_View>
	void reconstructNearest(
		const std::vector<Vector<Real, N> >& positionList,
		const std::vector<Data>& dataList,
		const AlignedBox<Real, N>& region,
		const View<N, Data, Output_View>& view)
	{
		Pastel::reconstructNearest(
			positionList, dataList,
			region, view, 1, 0);
	}

}

#endif
