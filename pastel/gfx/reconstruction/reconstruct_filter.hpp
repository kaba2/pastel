#ifndef PASTELGFX_RECONSTRUCT_FILTER_HPP
#define PASTELGFX_RECONSTRUCT_FILTER_HPP

#include "pastel/gfx/reconstruct_filter.h"

#include "pastel/geometry/pointkdtree/pointkdtree.h"
#include "pastel/geometry/overlap/overlaps_alignedbox_point.h"

#include "pastel/sys/view/view_visit.h"
#include "pastel/sys/output.h"

namespace Pastel
{

	namespace ReconstructFilter_
	{

		template <typename Real, integer N, typename Data>
		class DataPoint
		{
		public:
			using Data_ = Data;

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

		template <typename Real_, integer N_, typename Data>
		class DataPolicy
		{
		public:
			using Real = Real_;
			static constexpr integer N = N_;
			using Point = DataPoint<Real, N, Data>;

			explicit DataPolicy(
				integer dimension)
				: dimension_(dimension)
			{
				ENSURE(N == Dynamic || 
					dimension == N);
			}

			integer n() const
			{
				return (N != Dynamic) ? N : dimension_;
			}

			const Real& operator()(
				const Point& point, integer index) const
			{
				return point.position_[index];
			}

		private:
			integer dimension_;
		};

		template <
			typename Tree,
			typename Filter>
		class ReconstructFunctor
		{
		public:
			using Locator = typename Tree::Locator:
			using Real = typename Locator::Real;
			static constexpr integer N = Locator::N;

			explicit ReconstructFunctor(
				const Tree& kdTree,
				const Filter& filter,
				const Real& filterStretch)
				: kdTree_(kdTree)
				, filter_(filter)
				, filterStretch_(filterStretch)
				, invFilterStretch_(inverse(filterStretch))
			{
			}

			using Data = typename Locator::Point;

			void operator()(
				const Vector<integer, N>& position,
				typename Data::Data_& data) const
			{
				if (kdTree_.empty())
				{
					data = 0;
					return;
				}

				typedef typename Tree::Point_ConstIterator
					ConstIterator;

				std::vector<ConstIterator> nearestSet;
				auto nearestOutput = [&](
					Real distance, ConstIterator point)
				{
					nearestSet.push_back(point);
				};

				searchNearest(
					kdTree_, 
					evaluate(Vector<real, N>(position) + 0.5),
					PASTEL_TAG(nearestOutput), nearestOutput,
					Maximum_NormBijection<Real>(),
					PASTEL_TAG(kNearest), kdTree_.points(),
					PASTEL_TAG(maxDistance2), filter_.radius() * filterStretch_);

				integer points = nearestSet.size();

				real weightSum = 0;
				typename Data::Data_ valueSum(0);

				for (integer i = 0;i < points;++i)
				{

					//const real weight = filter_.evaluate(nearestSet[i].key() * invFilterStretch_);

					Vector<real, N> delta = 
						nearestSet[i]->point().position_ - 
						(Vector<real, N>(position) + 0.5);
					real weight = 1;
					for (integer k = 0;k < N;++k)
					{

						weight *= filter_.evaluate(delta[k] * invFilterStretch_);
					}

					valueSum += nearestSet[i]->point().data_ * weight;
					weightSum += weight;
				}

				if (weightSum != 0)
				{
					valueSum /= weightSum;
				}

				data = valueSum;
			}

		private:
			const Tree& kdTree_;
			const Filter& filter_;
			const Real& filterStretch_;
			Real invFilterStretch_;
		};
	}

	template <typename Real, integer N, typename Data, typename Filter, typename Output_View>
	void reconstructFilter(
		const std::vector<Vector<Real, N> >& positionList,
		const std::vector<Data>& dataList,
		const AlignedBox<Real, N>& region,
		const Filter& filter,
		const NoDeduction<Real>& filterStretch,
		const View<N, Data, Output_View>& view)
	{
		integer n = region.n();
		integer points = positionList.size();

		ENSURE2(points == dataList.size(), points, dataList.size());

		using DataPoint = ReconstructFilter_::DataPoint<Real, N, Data>;
		using DataPolicy = ReconstructFilter_::DataPolicy<Real, N, Data>;

		DataPolicy locator(n);

		using Tree = PointKdTree<PointKdTree_Settings<DataPolicy>>;

		Tree kdTree(locator);


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

		kdTree.insertRange(
			range(dataPointList.begin(), dataPointList.end()));

		kdTree.refine(SlidingMidpoint_SplitRule());

		ReconstructFilter_::ReconstructFunctor<Tree, Filter>
			reconstructFunctor(kdTree, filter, filterStretch);

		visitPosition(
			view, reconstructFunctor);
	}

}

#endif
