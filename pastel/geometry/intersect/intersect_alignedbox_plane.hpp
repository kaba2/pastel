#ifndef PASTELGEOMETRY_INTERSECT_ALIGNEDBOX_PLANE_HPP
#define PASTELGEOMETRY_INTERSECT_ALIGNEDBOX_PLANE_HPP

#include "pastel/geometry/intersect/intersect_alignedbox_plane.h"
#include "pastel/geometry/overlap/overlaps_alignedbox_plane.h"

#include "pastel/geometry/nearest_main_axis.h"

#include "pastel/sys/vector/vector_tools.h"

namespace Pastel
{

	template <typename Real, int N>
	bool intersect(
		const AlignedBox<Real, N>& box,
		const Plane<Real, N>& plane, 
		integer& clipDimension,
		NoDeduction<Real>& minBoxMax,
		NoDeduction<Real>& maxBoxMin)
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
		// Denote the half-space induced by the plane P by
		// H = {x in R^n : <x - q, n> >= 0}.
		// Let e_k be the standard basis vector which forms 
		// minimal angle with n.
		//
		// Problem
		// -------
		//
		// On axis e_k, find the projection intervals of
		// B1 = intersection(H, B) and 
		// B2 = intersection(!H, B).
		//
		// Solution
		// --------
		//
		// First check if the plane and the box overlap.
		// If they do not, we are done. Otherwise continue.
		//
		// The basis axis e_k closest to n in angle is given
		// by picking the direction with maximum absolute projected
		// length:
		//
		// |<e_k, n>| = |n_k|
		//		
		// Determining k thus takes O(d) time.
		//
		// The distance t = dk(p) from a point p 
		// to the plane along e_k is solved by:
		//
		// <p + te_k - q, n> = 0
		// =>
		// <p - q, n> + t<e_k, n> = 0
		// =>
		// t = -<p - q, n> / <e_k, n>
		// =>
		// t = <p - q, n> / (-n_k)
		//
		// (note n_k != 0 because it is the component
		// with maximum absolute value and n != 0)
		//
		// Thus, let
		// dk(p) = <p - q, n> / (-n_k)
		//
		// Find out
		// dk_min = min {dk(p) : p in V, p[k] = 0}
		// dk_max = max {dk(p) : p in V, p[k] = 0}
		//
		// These can be solved incrementally:
		//
		// dk(b_min) 
		// = <b_min - q, n> / (-n_k)
		//
		// delta_dk(p, h) 
		// = dk(p + h) - dk(p)
		// = <(p + h) - q, n> / (-n_k) - <p - q, n> / (-n_k)
		// = <h, n> / (-n_k)
		//
		// Particularly:
		//
		// delta_dk(p, w_i e_i)
		// = <w_i e_i, n> / (-n_k)
		// = (w_i n_i) / (-n_k)
		// 
		// To find out dk_min (dk_max), start from 
		// b_min, and move towards decreasing (increasing)
		// values of dk sequantially along standard basis axes. 
		// Note that it is efficient to find
		// dk_min and dk_max in parallel: if dk_min
		// moves towards a direction, then dk_max
		// does not, and vice versa.
		//
		// The found distances might result in values that are
		// outside the box. In this case we simply clamp the results
		// to the range [0, box.extent()[k]].
		//
		// The values dk_min and dk_max are found in O(d) time.
		// Thus the whole algorithm is O(d) in time. 
		// The additional storage requirements are O(1).

		integer dimension = box.n();
		
		ENSURE_OP(dimension, ==, plane.n());

		if (!overlaps(box, plane))
		{
			return false;
		}

		// Choose a standard basis axis e_k
		// which has minimal angle with the plane normal.
		
		integer k = nearestMainAxis(plane.normal());

		// Otherwise we need to calculate the 
		// projected ranges of the sub-pieces
		// [box.min()[k], minBoxMax] and
		// [maxBoxMin, box.max()[k]].

		// Find the minimum and maximum distances along e_k 
		// between {v in V : v[k] = 0} and P.

		Real axisScale = -inverse(plane.normal()[k]);

		Real minAxisDistance =
			dot(box.min() - plane.position(), plane.normal()) * axisScale;
		Real maxAxisDistance = minAxisDistance;

		for (integer i = 0;i < k;++i)
		{
			Real deltaAxisDistance =
				box.extent(i) * plane.normal()[i] * axisScale;

			if (deltaAxisDistance < 0)
			{
				minAxisDistance += deltaAxisDistance;
			}
			else
			{
				maxAxisDistance += deltaAxisDistance;
			}
		}
		// Jump over i = k.
		for (integer i = k + 1;i < dimension;++i)
		{
			Real deltaAxisDistance =
				box.extent(i) * plane.normal()[i] * axisScale;

			if (deltaAxisDistance < 0)
			{
				minAxisDistance += deltaAxisDistance;
			}
			else
			{
				maxAxisDistance += deltaAxisDistance;
			}
		}

		clipDimension = k;
		maxBoxMin = box.min()[k] + 
			clamp(minAxisDistance, 0, box.extent(k));
		minBoxMax = box.min()[k] + 
			clamp(maxAxisDistance, 0, box.extent(k));
		
		return true;
	}

}

#endif
