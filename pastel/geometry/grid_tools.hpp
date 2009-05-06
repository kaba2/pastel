#ifndef PASTELGEOMETRY_GRIDTOOLS_HPP
#define PASTELGEOMETRY_GRIDTOOLS_HPP

#include "pastel/geometry/grid_tools.h"

#include "pastel/sys/cursorview.h"
#include "pastel/sys/view_visit.h"

namespace Pastel
{

	template <int N, typename Real, typename Object, typename VisitorFunctor>
	void visit(
		const Grid<N, Real, Object>& grid,
		const Rectangle<N>& region,
		VisitorFunctor& visitor)
	{
		visit(cursorView(grid.nodeAt(region.min()), region.extent()), visitor);
	}

	namespace Detail_SearchNearest
	{

		template <typename Grid, typename Metric>
		class SearchNearest
		{
		public:
			enum
			{
				N = Grid::N_
			};
			typedef typename Grid::Real_ Real;
			typedef typename Grid::Cursor Cursor;
			typedef typename Grid::ConstObjectIterator ConstObjectIterator;

			SearchNearest(
				ConstObjectIterator& minIter,
				Real& minDistance,
				Metric metric,
				const Point<N, Real>& position)
				: minIter_(minIter)
				, minDistance_(minDistance)
				, metric_(metric)
				, position_(position)
			{
			}


			void operator()(const Cursor& cursor)
			{
				ConstObjectIterator iter = cursor.begin();
				const ConstObjectIterator iterEnd = cursor.end();
				while(iter != iterEnd)
				{
					const Real currentDistance = metric_(position, *iter);
					if (currentDistance < minDistance)
					{
						minIter = iter;
						minDistance = currentDistance;
					}
					++iterEnd;
				}
			}

		private:
			ConstObjectIterator& minIter_;
			Real& minDistance_;
			Metric metric_;
		};
	}

	template <int N, typename Real, typename Object, typename PositionFunctor, typename Metric>
	std::pair<typename Grid<N, Real, Object>::ConstObjectIterator, Real>
		searchNearest(
		const Grid<N, Real, Object>& grid,
		const Point<N, Real>& point,
		const PASTEL_NO_DEDUCTION(Real)& radius,
		const PositionFunctor& positionFunctor,
		const Metric& metric)
	{
		typedef Grid<N, Real, Object> MyGrid;
		typedef typename MyGrid::ConstObjectIterator ConstObjectIterator;

		const AlignedBox<N, Real> bound(point - radius, point + radius);

		const Rectangle<N> region = grid.nodesAt(bound);

		ConstObjectIterator minIter = grid.end();
		Real minDistance = infinity<Real>();

		NearestFunctor<MyGrid, Metric> nearestFunctor(
			minIter, minDistance, metric, point);

		visit(grid, region, nearestFunctor);

		if (minDistance > radius)
		{
			return std::make_pair(grid.end(), infinity<Real>());
		}

		return std::make_pair(minIter, minDistance);
	}

}

#endif
