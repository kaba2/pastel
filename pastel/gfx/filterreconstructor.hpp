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

		template <int N, typename Real, typename ObjectPolicy, typename Filter>
		class ReconstructFunctor
		{
		public:
			explicit ReconstructFunctor(
				const KdTree<N, Real, ObjectPolicy>& kdtree,
				const Filter& filter,
				const Real& filterStretch)
				: kdtree_(kdtree)
				, filter_(filter)
				, filterStretch_(filterStretch)
				, invFilterStretch_(inverse(filterStretch))
			{
			}

			typedef typename ObjectPolicy::Object Data;

			void operator()(
				const Point<N, integer>& position,
				typename Data::Data_& data) const
			{
				if (kdtree_.empty())
				{
					data = 0;
					return;
				}

				typedef KdTree<N, Real, ObjectPolicy>::ConstObjectIterator
					ConstIterator;

				SmallSet<KeyValue<real, ConstIterator> > pointSet;

				searchNearest(kdtree_, Point<N, real>(position) + 0.5,
					filter_.radius() * filterStretch_,
					0,
					InfinityNormBijection<N, Real>(),
					kdtree_.objects() - 1,
					pointSet);

				const integer points = pointSet.size();

				real weightSum = 0;
				typename Data::Data_ valueSum(0);

				for (integer i = 0;i < points;++i)
				{
					//const real weight = filter_.evaluate(pointSet[i].key() * invFilterStretch_);

					const Vector<N, real> delta = pointSet[i].value()->position_ - (Point<N, real>(position) + 0.5);
					real weight = 1;
					for (integer k = 0;k < N;++k)
					{
						weight *= filter_.evaluate(delta[k] * invFilterStretch_);
					}

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
			const Real& filterStretch_;
			const Real invFilterStretch_;
		};
	}

	template <int N, typename Real, typename Data, typename Filter, typename Output_View>
	void reconstructFilter(
		const std::vector<Point<N, Real> >& positionList,
		const std::vector<Data>& dataList,
		const AlignedBox<N, Real>& region,
		const Filter& filter,
		const PASTEL_NO_DEDUCTION(Real)& filterStretch,
		const View<N, Data, Output_View>& view)
	{
		BOOST_STATIC_ASSERT(N != Unbounded);

		const integer points = positionList.size();

		ENSURE2(points == dataList.size(), points, dataList.size());

		typedef Detail_FilterReconstructor::DataPoint<N, Real, Data> DataPoint;
		typedef Detail_FilterReconstructor::DataPolicy<N, Real, Data> DataPolicy;

		DataPolicy dataPolicy;
		KdTree<N, Real, DataPolicy> kdtree(N, dataPolicy);

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

		kdtree.refine(
			computeKdTreeMaxDepth(kdtree.objects()), 4, SlidingMidpointRule());

		Detail_FilterReconstructor::ReconstructFunctor<N, Real, DataPolicy, Filter>
			reconstructFunctor(kdtree, filter, filterStretch);

		visitPosition(
			view, reconstructFunctor);
	}

}

#endif
