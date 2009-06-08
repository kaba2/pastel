#ifndef PASTEL_RECONSTRUCT_NEAREST_HPP
#define PASTEL_RECONSTRUCT_NEAREST_HPP

#include "pastel/gfx/reconstruct_nearest.h"

#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/pointkdtree_tools.h"
#include "pastel/geometry/overlaps_alignedbox_point.h"

#include "pastel/sys/vector_tools.h"

#include "pastel/sys/view_visit.h"

namespace Pastel
{

	namespace Detail_ReconstructNearest
	{

		template <int N, typename Real, typename Data>
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
				const Point<N, Real>& position,
				const Data& data)
				: position_(position)
				, data_(data)
			{
			}

			Point<N, Real> position_;
			Data data_;
		};

		template <int N, typename Real, typename Data>
		class DataPolicy
		{
		public:
			typedef DataPoint<N, Real, Data> Object;
			typedef FalseType ArbitrarySplits;

			const Point<N, Real>& point(
				const DataPoint<N, Real, Data>& dataPoint) const
			{
				return dataPoint.position_;
			}

			Real point(
				const DataPoint<N, Real, Data>& dataPoint,
				integer axis) const
			{
				return dataPoint.position_[axis];
			}
		};

		template <int N, typename Real, typename ObjectPolicy>
		class ReconstructFunctor
		{
		public:
			explicit ReconstructFunctor(
				const PointKdTree<N, Real, ObjectPolicy>& kdTree,
				integer kNearest,
				integer maxRelativeError)
				: kdtree_(kdTree)
				, kNearest_(kNearest)
				, maxRelativeError_(maxRelativeError)
			{
			}

			typedef typename ObjectPolicy::Object Data;

			void operator()(
				const Point<N, integer>& position,
				typename Data::Data_& data) const
			{
				typedef PointKdTree<N, Real, ObjectPolicy>::ConstObjectIterator
					ConstIterator;

				std::vector<ConstIterator> nearestSet;

				searchNearest(kdtree_, Point<N, Real>(position) + 0.5, 
					infinity<Real>(), maxRelativeError_, EuclideanNormBijection<Real>(),
					kNearest_, &nearestSet);

				data = nearestSet.back()->data_;
			}

		private:
			const PointKdTree<N, Real, ObjectPolicy>& kdtree_;
			integer kNearest_;
			Real maxRelativeError_;
		};
	}

	template <int N, typename Real, typename Data, typename Output_View>
	void reconstructNearest(
		const std::vector<Point<N, Real> >& positionList,
		const std::vector<Data>& dataList,
		const AlignedBox<N, Real>& region,
		const View<N, Data, Output_View>& view,
		integer kNearest,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError)
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
		
		const integer points = positionList.size();

		ENSURE1(kNearest > 0, kNearest);
		ENSURE2(kNearest < points, kNearest, points);
		ENSURE2(points == dataList.size(), points, dataList.size());

		typedef Detail_ReconstructNearest::DataPoint<N, Real, Data> DataPoint;
		typedef Detail_ReconstructNearest::DataPolicy<N, Real, Data> DataPolicy;

		DataPolicy dataPolicy;
		PointKdTree<N, Real, DataPolicy> kdTree(N, dataPolicy);

		const Vector<N, Real> scaling = 
			inverse(region.extent()) * Vector<N, Real>(view.extent());

		std::vector<DataPoint> dataPointList;
		for (integer i = 0;i < points;++i)
		{
			if (overlaps(region, positionList[i]))
			{
				dataPointList.push_back(
					DataPoint(asPoint((positionList[i] - region.min()) * scaling), dataList[i]));
			}
		}

		kdTree.insert(dataPointList.begin(), dataPointList.end());

		kdTree.refine(
			computeKdTreeMaxDepth(kdTree.objects()), 16, SlidingMidpoint2_SplitRule());

		Detail_ReconstructNearest::ReconstructFunctor<N, Real, DataPolicy>
			reconstructFunctor(kdTree, kNearest, maxRelativeError);

		visitPosition(
			view, reconstructFunctor);
	}

	template <int N, typename Real, typename Data, typename Output_View>
	void reconstructNearest(
		const std::vector<Point<N, Real> >& positionList,
		const std::vector<Data>& dataList,
		const AlignedBox<N, Real>& region,
		const View<N, Data, Output_View>& view,
		integer kNearest)
	{
		Pastel::reconstructNearest(
			positionList, dataList,
			region, view, kNearest, 0);
	}

	template <int N, typename Real, typename Data, typename Output_View>
	void reconstructNearest(
		const std::vector<Point<N, Real> >& positionList,
		const std::vector<Data>& dataList,
		const AlignedBox<N, Real>& region,
		const View<N, Data, Output_View>& view)
	{
		Pastel::reconstructNearest(
			positionList, dataList,
			region, view, 1, 0);
	}

}

#endif
