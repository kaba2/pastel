#ifndef PASTEL_POISSONDISKPATTERN_HPP
#define PASTEL_POISSONDISKPATTERN_HPP

#include "pastel/geometry/poissondiskpattern.h"

#include "pastel/sys/vector_tools.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/array.h"
#include "pastel/sys/rectangle_tools.h"
#include "pastel/sys/subview.h"
#include "pastel/sys/arrayview.h"

#include "pastel/sys/random_vector.h"

#include "pastel/geometry/overlaps_alignedbox_point.h"

#include <vector>

namespace Pastel
{

	namespace Detail_PoissonDiskPattern
	{

		template <int N, typename Real>
		class Visitor
		{
		public:
			explicit Visitor(
				const Vector<Real, N>& newPoint,
				const Real& minDistance2,
				bool& validNewPoint)
				: newPoint_(newPoint)
				, minDistance2_(minDistance2)
				, validNewPoint_(validNewPoint)
			{
			}

			void operator()(const Vector<Real, N>& point) const
			{
				if (validNewPoint_ && point.x() != infinity<Real>())
				{
					const Vector<Real, N> delta = newPoint_ - point;
					if (dot(delta) < minDistance2_)
					{
						validNewPoint_ = false;
					}
				}
			}

			const Vector<Real, N> newPoint_;
			const Real minDistance2_;
			bool& validNewPoint_;
		};

	}

	template <int N, typename Real, typename ReportFunctor, typename ConstSeedIterator>
	void poissonDiskPattern(
		const AlignedBox<Real, N>& window,
		const PASTEL_NO_DEDUCTION(Real)& minDistance,
		ReportFunctor& reportFunctor,
		const ConstSeedIterator& seedSetBegin,
		const ConstSeedIterator& seedSetEnd,
		integer maxRejections)
	{
		// This is the algorithm from the paper:
		// "Fast poisson disk sampling in arbitrary dimensions",
		// Robert Bridson, Siggraph 2007.

		// Assume we work in R^n.
		// We wish to construct a grid
		// with cubic voxels with such
		// dimensions that every voxel can
		// contain at most one point of
		// the poisson disk pattern.
		// If the voxel edge length is given by h,
		// then the diagonal length is given by
		// sqrt(n * h^2) = h * sqrt(n).
		//
		// If the minimum distance is r,
		// then we want to have:
		// h * sqrt(n) <= r
		// =>
		// h <= r / sqrt(n)
		//
		// The number of voxels in the e_i axis is then
		// given by:
		// voxels_i = ceil(width_i / (r / sqrt(n)))
		// = ceil(width_i * (sqrt(n) / r))

		const Real minDistance2 = minDistance * minDistance;
		const Real invDiagonal = std::sqrt((Real)N / minDistance2);

		const Vector<Real, N> windowDelta = window.max() - window.min();
		const Vector<integer, N> extent =
			ceil(windowDelta * invDiagonal);

		const Vector<Real, N> invVoxelDelta = Vector<Real, N>(extent) / windowDelta;

		//const Vector<integer, N> voxels = ceil(windowDelta * invDiagonal);

		Array<Vector<Real, N>, N> grid(extent + 1, Vector<Real, N>(infinity<Real>()));
		const Rectangle<N> gridWindow(Vector<integer, N>(0), extent);

		std::vector<Vector<integer, N> > activeSet;

		if (seedSetBegin == seedSetEnd)
		{
			const Vector<Real, N> seedPoint(window.at(randomVector<N, Real>()));
			const Vector<integer, N> seedGridPosition(
				floor((seedPoint - window.min()) * invVoxelDelta));

			activeSet.push_back(seedGridPosition);
			grid(seedGridPosition) = seedPoint;
		}
		else
		{
			ConstSeedIterator iter = seedSetBegin;
			while(iter != seedSetEnd)
			{
				const Vector<Real, N> seedPoint(*iter);
				const Vector<integer, N> seedGridPosition(
					floor((seedPoint - window.min()) * invVoxelDelta));

				activeSet.push_back(seedGridPosition);
				grid(seedGridPosition) = seedPoint;

				++iter;
			}
		}

		while(!activeSet.empty())
		{
			const integer randomIndex = randomInteger() % activeSet.size();
			std::swap(activeSet[randomIndex], activeSet[activeSet.size() - 1]);

			const Vector<integer, N> coordinates = activeSet.back();
			activeSet.pop_back();

			const Vector<Real, N> activePoint = grid(coordinates);

			bool foundNewPoint = false;
			do
			{
				foundNewPoint = false;

				for (integer i = 0;i < maxRejections && !foundNewPoint;++i)
				{
					// Generate a new point from the annulus
					// around the active point.

					const Vector<Real, N> newPoint =
						activePoint +
						randomVectorAnnulus<N, Real>(
						minDistance, 2 * minDistance);

					// See if the new point is inside the region.

					if (!overlaps(window, newPoint))
					{
						continue;
					}

					// Search the neighbourhood to see
					// if the new point fits or not.

					const Vector<integer, N> gridPosition(
						(newPoint - window.min()) * invVoxelDelta);

					const AlignedBox<Real, N> neighborhood(
						newPoint - minDistance,
						newPoint + minDistance);

					const Rectangle<N> searchWindow(
						Vector<integer, N>((neighborhood.min() - window.min()) * invVoxelDelta),
						Vector<integer, N>((neighborhood.max() - window.min()) * invVoxelDelta) + 1);

					/*
					const Rectangle<N> searchWindow(
						gridPosition - 2,
						gridPosition + 3);
					*/

					Rectangle<N> clippedSearchWindow;
					bool validNewPoint = true;

					if (intersect(searchWindow, gridWindow, clippedSearchWindow))
					{
						Detail_PoissonDiskPattern::Visitor<N, Real> visitor(
							newPoint, minDistance2, validNewPoint);
						visit(subView(arrayView(grid), clippedSearchWindow), visitor);
					}

					if (validNewPoint)
					{
						// The point can be added as a new point
						// in the pattern.

						reportFunctor(newPoint);
						activeSet.push_back(gridPosition);
						grid(gridPosition) = newPoint;
						foundNewPoint = true;
					}
				}
			}
			while(foundNewPoint);
		}
	}

	template <int N, typename Real, typename ReportFunctor>
	void poissonDiskPattern(
		const AlignedBox<Real, N>& window,
		const PASTEL_NO_DEDUCTION(Real)& minDistance,
		ReportFunctor& reportFunctor,
		integer maxRejections)
	{
		std::vector<Vector<Real, N> > seedSet;
		Pastel::poissonDiskPattern(
			window, minDistance, reportFunctor,
			seedSet.begin(), seedSet.end(), maxRejections);
	}

}

#endif
