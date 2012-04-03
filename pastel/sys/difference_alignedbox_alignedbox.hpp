#ifndef PASTEL_DIFFERENCE_ALIGNEDBOX_ALIGNEDBOX_HPP
#define PASTEL_DIFFERENCE_ALIGNEDBOX_ALIGNEDBOX_HPP

#include "pastel/sys/difference_alignedbox_alignedbox.h"

namespace Pastel
{

	template <typename Real, int N_A, int N_B,
		typename AlignedBox_Reporter>
	integer difference(
		const AlignedBox<Real, N_A>& aBox,
		const AlignedBox<Real, N_B>& bBox,
		AlignedBox_Reporter report)
	{
		PASTEL_STATIC_ASSERT(
			N_A == N_B || 
			N_A == Dynamic || 
			N_B == Dynamic);
		ENSURE_OP(aBox.dimension(), ==, bBox.dimension());

		// We prefer to use a static dimension
		// for the reported boxes.
		enum
		{
			N = (N_A == Dynamic) ? N_B : N_A
		};

		// In case the dimension is static, this gives the compiler a 
		// chance to unroll the loop.
		const integer n = 
			(N == Dynamic) ? aBox.dimension() : N;

		// The idea in this algorithm is to cut out a sub-box from 'aBox'
		// one at a time, so that the cut-out box is given by the
		// intersection of the current box with the outward half-space 
		// induced by a side of 'bBox'. This process guarantees that
		// at most 2n boxes are produced, where n is the dimension.
		// Note that we must be careful to get the side topologies
		// right.

		integer reported = 0;

		if (aBox.empty())
		{
			// Nothing to do since the aBox
			// does not contain any points.
			return reported;
		}

		if (bBox.empty())
		{
			// All of the aBox remains, since the
			// bBox does not contain any points.

			report(aBox);
			++reported;
			
			return reported;
		}

		bool wantToContinue = true;
		AlignedBox<Real, N> cutBox(aBox);
		for (integer i = 0;i < n && wantToContinue;++i)
		{
			if (cutBox.max()[i] <= bBox.min()[i])
			{
				if (cutBox.max()[i] < bBox.min()[i] ||
					cutBox.maxTopology()[i] == Topology::Open ||
					bBox.minTopology()[i] == Topology::Open)
				{
					// The whole box is included in
					// the cut-out box.
					wantToContinue |= report(cutBox);
					++reported;

					// There is nothing more to report.
					return reported;
				}
			}
			
			if (cutBox.min()[i] >= bBox.max()[i])
			{
				if (cutBox.min()[i] > bBox.max()[i] ||
					cutBox.minTopology()[i] == Topology::Open ||
					bBox.maxTopology()[i] == Topology::Open)
				{
					// The whole box is included in
					// the cut-out box.
					wantToContinue |= report(cutBox);
					++reported;

					// There is nothing more to report.
					return reported;
				}
			}

			// The boxes intersect on this axis.
			
			if (cutBox.min()[i] <= bBox.min()[i])
			{
				if (cutBox.min()[i] < bBox.min()[i] ||
					(cutBox.minTopology()[i] == Topology::Closed &&
					bBox.minTopology()[i] == Topology::Open))
				{
					const Topology::Enum previousMaxTopology = 
						cutBox.maxTopology()[i];
					const Real previousMax = 
						cutBox.max()[i];

					// Form the cut-out box,
					cutBox.max()[i] = bBox.min()[i];
					cutBox.maxTopology()[i] = 
						switchTopology(bBox.minTopology()[i]);
					// and report it.
					wantToContinue |= report(cutBox);
					++reported;

					// Form the rest of the box,
					cutBox.min()[i] = bBox.min()[i];
					cutBox.minTopology()[i] = bBox.minTopology()[i];
					cutBox.max()[i] = previousMax;
					cutBox.maxTopology()[i] = previousMaxTopology;
					// and repeat the process with it.
				}
			}

			if (cutBox.max()[i] >= bBox.max()[i])
			{
				if (cutBox.max()[i] > bBox.max()[i] ||
					(cutBox.maxTopology()[i] == Topology::Closed &&
					bBox.maxTopology()[i] == Topology::Open))
				{
					const Topology::Enum previousMinTopology = 
						cutBox.minTopology()[i];
					const Real previousMin = 
						cutBox.min()[i];

					// Form the cut-out box,
					cutBox.min()[i] = bBox.max()[i];
					cutBox.minTopology()[i] = 
						switchTopology(bBox.maxTopology()[i]);
					// and report it.
					wantToContinue |= report(cutBox);
					++reported;

					// Form the rest of the box,
					cutBox.min()[i] = previousMin;
					cutBox.minTopology()[i] = previousMinTopology;
					cutBox.max()[i] = bBox.max()[i];
					cutBox.maxTopology()[i] = 
						bBox.maxTopology()[i];
					// and repeat the process with it.
				}
			}
		}

		return reported;
	}

	template <
		typename Real, int N_A, int N_B,
		typename AlignedBox_Reporter>
	integer symmetricDifference(
		const AlignedBox<Real, N_A>& aBox,
		const AlignedBox<Real, N_B>& bBox,
		AlignedBox_Reporter report)
	{
		integer reported = 0;
		
		reported += difference(aBox, bBox, report);
		reported += difference(bBox, aBox, report);
		
		return reported;
	}

}

#endif
