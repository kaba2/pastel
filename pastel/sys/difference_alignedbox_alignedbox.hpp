#ifndef PASTEL_DIFFERENCE_ALIGNEDBOX_ALIGNEDBOX_HPP
#define PASTEL_DIFFERENCE_ALIGNEDBOX_ALIGNEDBOX_HPP

#include "pastel/sys/difference_alignedbox_alignedbox.h"

namespace Pastel
{

	template <typename Real, int N_A, int N_B,
		typename AlignedBox_Iterator>
	void difference(
		const AlignedBox<Real, N_A>& aBox,
		const AlignedBox<Real, N_B>& bBox,
		AlignedBox_Iterator output)
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

		// In case the dimension is static,
		// this gives the compiler a chance
		// to unroll the loop.
		const integer n = 
			(N == Dynamic) ? aBox.dimension() : N;

		// The idea in this algorithm is to split a sub-box from 'aBox'
		// one at a time, so that the splitted box is given by the
		// intersection of the current box with the outward half-space 
		// induced by a side of 'bBox'. This process guarantees that
		// at most 2n boxes are produced, where n is the dimension.

		AlignedBox<Real, N> cutBox(aBox);

		// First we cut out with the half-spaces
		// induced by the minima.
		for (integer i = 0;i < n;++i)
		{
			if (cutBox.min()[i] < bBox.min()[i])
			{
				const Real previousMax = cutBox.max()[i];

				// Form the cut-out box,
				cutBox.max()[i] = bBox.min()[i];
				// and report it.
				*output = cutBox;

				// Form the rest of the box,
				cutBox.min()[i] = bBox.min()[i];
				cutBox.max()[i] = previousMax;
				// and repeat the process with it.
			}
		}

		// Then we cut out with the half-spaces
		// induced by the maxima.
		for (integer i = 0;i < n;++i)
		{
			if (cutBox.max()[i] > bBox.max()[i])
			{
				const Real previousMin = cutBox.min()[i];

				// Form the cut-out box,
				cutBox.min()[i] = bBox.max()[i];
				// and report it.
				*output = cutBox;

				// Form the rest of the box,
				cutBox.min()[i] = previousMin;
				cutBox.max()[i] = bBox.max()[i];
				// and repeat the process with it.
			}
		}
	}

	template <
		typename Real, int N_A, int N_B,
		typename AlignedBox_Iterator>
	void symmetricDifference(
		const AlignedBox<Real, N_A>& aBox,
		const AlignedBox<Real, N_B>& bBox,
		AlignedBox_Iterator output)
	{
		difference(aBox, bBox, output);
		difference(bBox, aBox, output);
	}

}

#endif
