#ifndef PASTELGFX_FILTERRECONSTRUCTOR_HPP
#define PASTELGFX_FILTERRECONSTRUCTOR_HPP

#include "pastel/gfx/filterreconstructor.h"

#include "pastel/geometry/kdtree.h"
#include "pastel/geometry/kdtree_tools.h"
#include "pastel/geometry/overlaps_alignedbox_point.h"

#include "pastel/sys/view_visit.h"

namespace Pastel
{

	namespace Detail_FilterReconstructor
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
		};

		template <int N, typename Real, typename ObjectPolicy, typename Filter>
		class ReconstructFunctor
		{
		public:
			explicit ReconstructFunctor(
				const KdTree<N, Real, ObjectPolicy>& kdtree,
				const Filter& filter,
				const Real& filterRadius)
				: kdtree_(kdtree)
				, filter_(filter)
				, filterRadius_(filterRadius)
				, filterScaling_(inverse(filterRadius * filter.radius()))
			{
			}

			typedef typename ObjectPolicy::Object Data;

			void operator()(
				const Point<N, integer>& position,
				typename Data::Data_& data) const
			{
				typedef KdTree<N, Real, ObjectPolicy>::ConstObjectIterator
					ConstIterator;

				SmallSet<KeyValue<real, ConstIterator> > pointSet;

				findNearest(kdtree_, Point<N, real>(position) + 0.5,
					filter_.radius() * filterRadius_,
					(Real (*)(const Vector<N, Real>&))norm,
					-1,
					pointSet);

				const integer points = pointSet.size();

				real weightSum = 0;
				typename Data::Data_ valueSum(0);

				for (integer i = 0;i < points;++i)
				{
					const real weight = filter_.evaluate(pointSet[i].key() * filterScaling_);

					valueSum += pointSet[i].value()->data_ * weight;
					weightSum += weight;
				}

				if (weightSum != 0)
				{
					valueSum /= weightSum;
				}

				data = valueSum;
			}

		private:
			const KdTree<N, Real, ObjectPolicy>& kdtree_;
			const Filter& filter_;
			const Real& filterRadius_;
			const Real filterScaling_;
		};
	}

	template <int N, typename Real, typename Data, typename Filter, typename Output_View>
	void reconstructFilter(
		const std::vector<Point<N, Real> >& positionList,
		const std::vector<Data>& dataList,
		const AlignedBox<N, Real>& region,
		const Filter& filter,
		const PASTEL_NO_DEDUCTION(Real)& filterRadius,
		const View<N, Data, Output_View>& view)
	{
		const integer points = positionList.size();

		ENSURE2(points == dataList.size(), points, dataList.size());

		typedef Detail_FilterReconstructor::DataPoint<N, Real, Data> DataPoint;
		typedef Detail_FilterReconstructor::DataPolicy<N, Real, Data> DataPolicy;

		DataPolicy dataPolicy;
		KdTree<N, Real, DataPolicy> kdtree(dataPolicy);

		const Vector<N, Real> scaling = inverse(region.extent()) * Vector<N, Real>(view.extent());

		std::vector<DataPoint> dataPointList;
		for (integer i = 0;i < points;++i)
		{
			if (overlaps(region, positionList[i]))
			{
				dataPointList.push_back(
					DataPoint(asPoint((positionList[i] - region.min()) * scaling), dataList[i]));
			}
		}

		kdtree.insert(dataPointList.begin(), dataPointList.end());

		refineSlidingMidpoint(computeKdTreeMaxDepth(kdtree.objects()), 4, kdtree);

		Detail_FilterReconstructor::ReconstructFunctor<N, Real, DataPolicy, Filter>
			reconstructFunctor(kdtree, filter, filterRadius);

		visitPosition(
			view, reconstructFunctor);
	}

}

#endif
