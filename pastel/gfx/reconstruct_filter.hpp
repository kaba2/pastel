#ifndef PASTEL_RECONSTRUCT_FILTER_HPP
#define PASTEL_RECONSTRUCT_FILTER_HPP

#include "pastel/gfx/reconstruct_filter.h"

#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/pointkdtree_tools.h"
#include "pastel/geometry/overlaps_alignedbox_point.h"

#include "pastel/sys/view_visit.h"

namespace Pastel
{

	namespace Detail_ReconstructFilter
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
				const Point<Real, N>& position,
				const Data& data)
				: position_(position)
				, data_(data)
			{
			}

			Point<Real, N> position_;
			Data data_;
		};

		template <int N, typename Real, typename Data>
		class DataPolicy
		{
		public:
			typedef DataPoint<N, Real, Data> Object;

			const Point<Real, N>& point(
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

		template <int N, typename Real, typename ObjectPolicy, typename Filter>
		class ReconstructFunctor
		{
		public:
			explicit ReconstructFunctor(
				const PointKdTree<Real, N, ObjectPolicy>& kdTree,
				const Filter& filter,
				const Real& filterStretch)
				: kdTree_(kdTree)
				, filter_(filter)
				, filterStretch_(filterStretch)
				, invFilterStretch_(inverse(filterStretch))
			{
			}

			typedef typename ObjectPolicy::Object Data;

			void operator()(
				const Point<integer, N>& position,
				typename Data::Data_& data) const
			{
				if (kdTree_.empty())
				{
					data = 0;
					return;
				}

				typedef PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator
					ConstIterator;

				std::vector<ConstIterator> nearestSet;

				searchNearest(
					kdTree_, 
					Point<real, N>(position) + 0.5,
					DepthFirst_SearchAlgorithm_PointKdTree(),
					Accept_Always(),
					filter_.radius() * filterStretch_,
					0,
					Infinity_NormBijection<Real>(),
					kdTree_.objects(),
					std::back_inserter(nearestSet),
					NullIterator());

				const integer points = nearestSet.size();

				real weightSum = 0;
				typename Data::Data_ valueSum(0);

				for (integer i = 0;i < points;++i)
				{
					//const real weight = filter_.evaluate(nearestSet[i].key() * invFilterStretch_);

					const Vector<real, N> delta = nearestSet[i]->object().position_ - (Point<real, N>(position) + 0.5);
					real weight = 1;
					for (integer k = 0;k < N;++k)
					{
						weight *= filter_.evaluate(delta[k] * invFilterStretch_);
					}

					valueSum += nearestSet[i]->object().data_ * weight;
					weightSum += weight;
				}

				if (weightSum != 0)
				{
					valueSum /= weightSum;
				}

				data = valueSum;
			}

		private:
			const PointKdTree<Real, N, ObjectPolicy>& kdTree_;
			const Filter& filter_;
			const Real& filterStretch_;
			const Real invFilterStretch_;
		};
	}

	template <int N, typename Real, typename Data, typename Filter, typename Output_View>
	void reconstructFilter(
		const std::vector<Point<Real, N> >& positionList,
		const std::vector<Data>& dataList,
		const AlignedBox<Real, N>& region,
		const Filter& filter,
		const PASTEL_NO_DEDUCTION(Real)& filterStretch,
		const View<N, Data, Output_View>& view)
	{
		BOOST_STATIC_ASSERT(N != Dynamic);

		const integer points = positionList.size();

		ENSURE2(points == dataList.size(), points, dataList.size());

		typedef Detail_ReconstructFilter::DataPoint<N, Real, Data> DataPoint;
		typedef Detail_ReconstructFilter::DataPolicy<N, Real, Data> DataPolicy;

		DataPolicy dataPolicy;
		PointKdTree<Real, N, DataPolicy> kdTree(
			ofDimension(N), 16, dataPolicy);

		const Vector<Real, N> scaling = inverse(region.extent()) * Vector<Real, N>(view.extent());

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

		kdTree.refine(SlidingMidpoint2_SplitRule_PointKdTree());

		Detail_ReconstructFilter::ReconstructFunctor<N, Real, DataPolicy, Filter>
			reconstructFunctor(kdTree, filter, filterStretch);

		visitPosition(
			view, reconstructFunctor);
	}

}

#endif
