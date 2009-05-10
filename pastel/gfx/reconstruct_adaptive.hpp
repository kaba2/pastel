#ifndef PASTELGFX_RECONSTRUCT_ADAPTIVE_HPP
#define PASTELGFX_RECONSTRUCT_ADAPTIVE_HPP

#include "pastel/gfx/reconstruct_adaptive.h"

#include "pastel/geometry/kdtree.h"
#include "pastel/geometry/kdtree_tools.h"
#include "pastel/geometry/overlaps_alignedbox_point.h"

#include "pastel/dsp/box_filter.h"

#include "pastel/sys/vector_tools.h"
#include "pastel/sys/view_visit.h"

namespace Pastel
{

	namespace Detail_ReconstructAdaptive
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
			typedef TrueType UseBounds;

			AlignedBox<N, Real> bound(
				const DataPoint<N, Real, Data>& dataPoint) const
			{
				return AlignedBox<N, Real>(dataPoint.position_);
			}

			Tuple<2, Real> bound(
				const DataPoint<N, Real, Data>& dataPoint, integer index) const
			{
				return Tuple<2, Real>(dataPoint.position_[index]);
			}

			const Point<N, Real>& point(
				const DataPoint<N, Real, Data>& dataPoint) const
			{
				return dataPoint.position_;
			}
		};

		template <int N, typename Real, typename ObjectPolicy>
		class ReconstructFunctor
		{
		public:
			explicit ReconstructFunctor(
				const KdTree<N, Real, ObjectPolicy>& kdTree,
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
				typedef KdTree<N, Real, ObjectPolicy>::ConstObjectIterator
					ConstIterator;

				std::vector<ConstIterator> nearestSet;
				std::vector<Real> distanceSet;

				EuclideanNormBijection<N, Real> normBijection;

				searchNearest(kdtree_, Point<N, Real>(position) + 0.5, 
					infinity<Real>(), maxRelativeError_, normBijection,
					kNearest_ + 1, &nearestSet, &distanceSet);

				Real weightSum = 0;
				Data::Data_ dataSum(0);
				FilterPtr filter = triangleFilter();
				const Real scaling = filter->radius() / distanceSet.back();
				for (integer i = 0;i < kNearest_;++i)
				{
					const Real t = normBijection.toNorm(distanceSet[i] * scaling);
					const Real weight = filter->evaluate(t);

					weightSum += weight;
					dataSum += weight * nearestSet[i]->data_;
				}

				if (weightSum > 0)
				{
					data = dataSum / weightSum;
				}
				else
				{
					data = 0;
				}
			}

		private:
			const KdTree<N, Real, ObjectPolicy>& kdtree_;
			integer kNearest_;
			Real maxRelativeError_;
		};
	}

	template <int N, typename Real, typename Data, typename Output_View>
	void reconstructAdaptive(
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

		typedef Detail_ReconstructAdaptive::DataPoint<N, Real, Data> DataPoint;
		typedef Detail_ReconstructAdaptive::DataPolicy<N, Real, Data> DataPolicy;

		DataPolicy dataPolicy;
		KdTree<N, Real, DataPolicy> kdTree(N, dataPolicy);

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
			computeKdTreeMaxDepth(kdTree.objects()), 4, SlidingMidpointRule());

		Detail_ReconstructAdaptive::ReconstructFunctor<N, Real, DataPolicy>
			reconstructFunctor(kdTree, kNearest, maxRelativeError);

		visitPosition(
			view, reconstructFunctor);
	}

}

#endif
