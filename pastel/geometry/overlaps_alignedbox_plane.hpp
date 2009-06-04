#ifndef PASTEL_ALIGNEDBOXPLANE_HPP
#define PASTEL_ALIGNEDBOXPLANE_HPP

#include "pastel/geometry/overlaps_alignedbox_plane.h"

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/alignedbox.h"
#include "pastel/sys/point.h"
#include "pastel/geometry/plane.h"

namespace Pastel
{

	template <int N, typename Real>
		bool overlaps(
			const AlignedBox<N, Real>& alignedBox,
			const Plane<N, Real>& plane)
	{
		// Notation
		// --------
		//
		// Let B = {x in R^n : b_min <= x < b_max} and call it a box.
		// Denote the vertices of this box
		// by V = {v_1, ..., v_(2^n)}.
		// Let a plane P be given by a normal n and a 
		// position q on the plane:
		// P = {x in R^n: <x - q, n> = 0}
		// Let e_i denote the i:th standard basis axis.
		//
		// Problem
		// -------
		//
		// Find out if B and P intersect. 
		//
		// Solution
		// --------
		//
		// B and P intersect
		// <=>
		// exists v_min, v_max in V: dot(v_min - q, n) dot(v_max - q, n) <= 0
		//
		// The distance t from a point p to the plane is 
		// solved by:
		//
		// <p + tn - q, n> = 0
		// =>
		// <p - q, n> + t<n, n> = 0
		// =>
		// t = <q - p, n> / <n, n>
		//
		// Because we are only interested in the sign
		// of this value, we shall denote:
		//
		// d(p) = <p - q, n>
		//
		// Find out 
		// d_min = min {d(p) : p in V}
		// d_max = max {d(p) : p in V}
		//
		// This is easy to do incrementally by 
		// starting from b_min,
		// and then moving along standard basis axes to
		// minimize (or maximize) d(p):
		//
		// p(b_min) = <b_min - q, n>
		//
		// delta_d(p, h) 
		// = d(p + h) - d(p) 
		// = <(p + h) - q, n> - <p - q, n>
		// = <h, n>
		//
		// Particularly:
		// 
		// delta(p, w_i e_i)
		// = <w_i e_i, n>
		// = w_i n_i
		//
		// Notice: you can find d_min and d_max in parallel,
		// since if delta_d is negative, then d_min should follow
		// the direction (and d_max not). Otherwise d_max
		// should follow the direction (and d_min not).
		//
		// Thus determining d_min and d_max takes O(d) time.
		//
		// The plane overlaps the box if and only if
		// d_min <= 0 and d_max >= 0.

		PENSURE(alignedBox.dimension() == plane.dimension());

		const integer dimension = alignedBox.dimension();

		// Find maximal and minimal signed distances
		// of the vertices in V along the normal vector
		// of the plane.

		Real minDistance = 
			dot(alignedBox.min() - plane.position(), plane.normal());
		Real maxDistance = minDistance;
		for (integer i = 0;i < dimension;++i)
		{
			const Real deltaAxisDistance = 
				alignedBox.extent(i) * plane.normal()[i];
			if (deltaAxisDistance < 0)
			{
				minDistance += deltaAxisDistance;
			}
			else
			{
				maxDistance += deltaAxisDistance;
			}
		}

		return (minDistance <= 0 && maxDistance >= 0);
	}

}

#endif
