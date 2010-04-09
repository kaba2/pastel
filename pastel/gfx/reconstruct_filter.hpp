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

		template <typename Real, int N, typename Data>
		class DataPolicy
		{
		public:
			typedef DataPoint<Real, N, Data> Object;

			const Real* point(const Object& object) const
			{
				return object.position_.rawBegin();
			}

			Real point(const Object& object, integer axis) const
			{
				return object.position_[axis];
			}
		};

		template <typename Real, int N, typename PointPolicy, typename Filter>
		class ReconstructFunctor
		{
		public:
			explicit ReconstructFunctor(
				const PointKdTree<Real, N, PointPolicy>& kdTree,
				const Filter& filter,
				const Real& filterStretch)
				: kdTree_(kdTree)
				, filter_(filter)
				, filterStretch_(filterStretch)
				, invFilterStretch_(inverse(filterStretch))
			{
			}

			typedef typename PointPolicy::Object Data;

			void operator()(
				const Vector<integer, N>& position,
				typename Data::Data_& data) const
			{
				if (kdTree_.empty())
				{
					data = 0;
					return;
				}

				typedef PointKdTree<Real, N, PointPolicy>::ConstObjectIterator
					ConstIterator;

				std::vector<ConstIterator> nearestSet;

				searchNearest(
					kdTree_, 
					evaluate(Vector<real, N>(position) + 0.5),
					kdTree_.objects(),
					std::back_inserter(nearestSet),
					NullIterator(),
					filter_.radius() * filterStretch_, 0,
					Always_AcceptPoint<ConstIterator>(),
					8,
					Maximum_NormBijection<Real>());

				const integer points = nearestSet.size();

				real weightSum = 0;
				typename Data::Data_ valueSum(0);

				for (integer i = 0;i < points;++i)
				{
					//const real weight = filter_.evaluate(nearestSet[i].key() * invFilterStretch_);

					const Vector<real, N> delta = nearestSet[i]->object().position_ - (Vector<real, N>(position) + 0.5);
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
			const PointKdTree<Real, N, PointPolicy>& kdTree_;
			const Filter& filter_;
			const Real& filterStretch_;
			const Real invFilterStretch_;
		};
	}

	template <typename Real, int N, typename Data, typename Filter, typename Output_View>
	void reconstructFilter(
		const std::vector<Vector<Real, N> >& positionList,
		const std::vector<Data>& dataList,
		const AlignedBox<Real, N>& region,
		const Filter& filter,
		const PASTEL_NO_DEDUCTION(Real)& filterStretch,
		const View<N, Data, Output_View>& view)
	{
		BOOST_STATIC_ASSERT(N != Dynamic);

		const integer points = positionList.size();

		ENSURE2(points == dataList.size(), points, dataList.size());

		typedef Detail_ReconstructFilter::DataPoint<Real, N, Data> DataPoint;
		typedef Detail_ReconstructFilter::DataPolicy<Real, N, Data> DataPolicy;

		DataPolicy dataPolicy;
		PointKdTree<Real, N, DataPolicy> kdTree(
			ofDimension(N), false, dataPolicy);

		const Vector<Real, N> scaling = inverse(region.extent()) * Vector<Real, N>(view.extent());

		std::vector<DataPoint> dataPointList;
		for (integer i = 0;i < points;++i)
		{
			if (overlaps(region, positionList[i]))
			{
				dataPointList.push_back(
					DataPoint((positionList[i] - region.min()) * scaling, dataList[i]));
			}
		}

		kdTree.insert(dataPointList.begin(), dataPointList.end());

		kdTree.refine(SlidingMidpoint_SplitRule_PointKdTree());

		Detail_ReconstructFilter::ReconstructFunctor<Real, N, DataPolicy, Filter>
			reconstructFunctor(kdTree, filter, filterStretch);

		visitPosition(
			view, reconstructFunctor);
	}

}

#endif
