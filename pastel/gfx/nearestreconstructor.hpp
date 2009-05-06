#ifndef PASTELGFX_NEARESTRECONSTRUCTOR_HPP
#define PASTELGFX_NEARESTRECONSTRUCTOR_HPP

#include "pastel/gfx/nearestreconstructor.h"

#include "pastel/geometry/kdtree.h"
#include "pastel/geometry/kdtree_tools.h"
#include "pastel/geometry/overlaps_alignedbox_point.h"

#include "pastel/sys/vector_tools.h"

#include "pastel/sys/view_visit.h"

namespace Pastel
{

	namespace Detail_NearestReconstructor
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
				const KdTree<N, Real, ObjectPolicy>& kdtree)
				: kdtree_(kdtree)
			{
			}

			typedef typename ObjectPolicy::Object Data;

			void operator()(
				const Point<N, integer>& position,
				typename Data::Data_& data) const
			{
				typedef KdTree<N, Real, ObjectPolicy>::ConstObjectIterator
					ConstIterator;

				KeyValue<real, ConstIterator> result =
					searchNearest(kdtree_, Point<N, Real>(position) + 0.5, 
					infinity<Real>(), 0, EuclideanNormBijection<N, Real>());

				if (result.value() != kdtree_.end())
				{
					data = result.value()->data_;
				}
			}

		private:
			const KdTree<N, Real, ObjectPolicy>& kdtree_;
		};
	}

	template <int N, typename Real, typename Data, typename Output_View>
	void reconstructNearest(
		const std::vector<Point<N, Real> >& positionList,
		const std::vector<Data>& dataList,
		const AlignedBox<N, Real>& region,
		const View<N, Data, Output_View>& view)
	{
		BOOST_STATIC_ASSERT(N != Unbounded);

		const integer points = positionList.size();

		ENSURE2(points == dataList.size(), points, dataList.size());

		typedef Detail_NearestReconstructor::DataPoint<N, Real, Data> DataPoint;
		typedef Detail_NearestReconstructor::DataPolicy<N, Real, Data> DataPolicy;

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

		Detail_NearestReconstructor::ReconstructFunctor<N, Real, DataPolicy>
			reconstructFunctor(kdtree);

		visitPosition(
			view, reconstructFunctor);
	}

}

#endif
