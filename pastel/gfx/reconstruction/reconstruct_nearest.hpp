#ifndef PASTELGFX_RECONSTRUCT_NEAREST_HPP
#define PASTELGFX_RECONSTRUCT_NEAREST_HPP

#include "pastel/gfx/reconstruct_nearest.h"

#include "pastel/geometry/pointkdtree/pointkdtree.h"
#include "pastel/geometry/overlap/overlaps_alignedbox_point.h"

#include "pastel/sys/vector/vector_tools.h"
#include "pastel/sys/view/view_visit.h"
#include "pastel/sys/output.h"

namespace Pastel
{

	namespace ReconstructNearest_
	{

		template <typename Real, int N, typename Data>
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

		template <
			typename Real_, int N_, typename Data>
		class DataPolicy
		{
		public:
			using Real = Real_;
			static constexpr int N = N_;
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

			const Real& operator(
				const Point& point, integer index) const
			{
				return point.position_[index];
			}

		private:
			integer dimension_;
		};

		template <typename Tree>
		class ReconstructFunctor
		{
		public:
			using Locator = typename Tree::Locator;
			using Real = typename Locator::Real;
			static constexpr int N = Locator::N;

			explicit ReconstructFunctor(
				const Tree& kdTree,
				integer kNearest,
				integer maxRelativeError)
				: kdtree_(kdTree)
				, kNearest_(kNearest)
				, maxRelativeError_(maxRelativeError)
			{
			}

			using Data = typename Locator::Point;

			void operator()(
				const Vector<integer, N>& position,
				typename Data::Data_& data) const
			{
				typedef typename Tree::Point_ConstIterator
					ConstIterator;

				std::vector<ConstIterator> nearestSet;
				auto report = [&](
					const auto& distance, 
					ConstIterator point)
				{
					nearestSet.push_back(point);
				};

				searchNearest(
					kdtree_, 
					evaluate(Vector<Real, N>(position) + 0.5), 
					report,
					PASTEL_TAG(kNearest), kNearest_,
					PASTEL_TAG(maxRelativeError), maxRelativeError_);

				data = nearestSet.back()->point().data_;
			}

		private:
			const Tree& kdtree_;
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
		const NoDeduction<Real>& maxRelativeError)
	{
		integer n = region.n();
		integer points = positionList.size();

		ENSURE_OP(kNearest, >, 0);
		ENSURE2(points == dataList.size(), points, dataList.size());

		using DataPoint = ReconstructNearest_::DataPoint<Real, N, Data>;
		using DataPolicy = ReconstructNearest_::DataPolicy<Real, N, Data>;

		DataPolicy locator(n);

		using Tree = PointKdTree<PointKdTree_Settings<DataPolicy>>

		Tree kdTree(locator);

		Vector<Real, N> scaling =
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

		kdTree.insertSet(dataPointList);
		kdTree.refine(SlidingMidpoint2_SplitRule());

		ReconstructNearest_::ReconstructFunctor<Tree>
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
