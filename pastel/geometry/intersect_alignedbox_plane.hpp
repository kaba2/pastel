#ifndef PASTEL_INTERSECT_ALIGNEDBOX_PLANE_HPP
#define PASTEL_INTERSECT_ALIGNEDBOX_PLANE_HPP

#include "pastel/geometry/intersect_alignedbox_plane.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	AlignedBox<1, Real> intersect(
		const AlignedBox<N, Real>& box,
		const Plane<N, Real>& plane, 
		integer k)
	{
		// Let B = [0, 1]^n subset R^n and call it a box.
		// Denote the vertices of this box
		// by V = {v_1, ..., v_(2^n)} = {0, 1}^n.
		// Let a plane P be given by a normal n and a 
		// position q on the plane.
		// Denote the half-space induced by the plane P by
		// H = {x in R^n : dot(x - q, n) >= 0}.
		// Assume that the plane P intersects the box B
		// such that
		//
		// exists k: for all i: (v_i[k] = 0 <=> v_i in H)
		//
		// The k in the above is called the clipping dimension.
		//
		// Problem:
		// Find a bounding axis-aligned box for
		// the intersection of H and B.
		//
		// Solution:
		// If (0, 0, ..., 0) is in H,
		// then that point is the minimum
		// point of the solution box.
		// It then remains to find out the
		// maximum point. Further, because
		// of the way the plane subdivides the box,
		// it suffices to find the k:th coordinate
		// of the maximum point (other dimensions
		// can't change).
		// The (signed) distance t from a
		// given point p along the standard
		// basis axis e_k is given by
		// solving:
		//
		// <p + t e_k - q, n) = 0
		// =>
		// <p - q, n> + t <e_k, n> = 0
		// =>
		// t = <q - p, n> / <e_k, n>
		//
		// If one moves from a point p
		// along a standard basis axis e_i,
		// then the distance t is increased by:
		//
		// dt = (<q - p - e_i, n> - <q - p, n>) / <e_k, n>
		// = -<e_i, n> / <e_k, n>
		// = -n[i] / n[k]
		//
		// Thus our strategy is the following:
		// Let k be the clipping dimension.
		// Start from 0, and try to move to
		// higher distances, trying each e_i, i != k,
		// sequantially. For each e_i, compute
		// dt, and if it is positive, move
		// towards that direction and
		// update the distance.
		// When you have considered all dimensions,
		// you are done.
		// 
		// If on the other hand 
		// (0, 0, ..., 0) is not in H,
		// then (1, 1, ..., 1) must be the
		// maximum point of the result, and
		// then we only need to find the k:th
		// component of the minimum point of
		// the result. The strategy is again
		// the same, but now we move towards
		// decreasing distances.
		//
		// Now assume B is an arbitrary box.
		// We transform all points such that
		// the box becomes [0, 1]^n.
		// First the points are translated
		// by box.min(). This does not
		// affect the normal of the plane,
		// just its position.
		// Then the points are transformed
		// by a linear transformation
		// M = diag[1 / w_1, 1 / w_2, ..., 1 / w_n].
		// where w_i is the extent of the box
		// in the i:th standard basis axis.
		// The effect of this is that the normal 
		// is transformed by inverse-transpose,
		// which is simply given by
		// M^-T = diag[w_1, w_2, ..., w_n].
		// When the result of this transformed
		// problem is found, one must remember
		// to map it back to the original box.

		// We will be a bit more general here.
		// We find both points at the same time,
		// reporting the maximum of the minimum sub-box,
		// and the minimum of the maximum sub-box.

		const integer dimension = box.dimension();
		
		ENSURE2(dimension == plane.dimension(),
			dimension, plane.dimension());
		ENSURE2(k >= 0 && k < dimension, k, dimension);

		// Map the points so that the box
		// maps to [0, 1]^n.

		const Vector<N, Real> boxExtent =
			box.extent();

		const Point<N, Real> planePosition(
			(plane.position() - box.min()) / boxExtent);

		Vector<N, Real> planeNormal =
			plane.normal() * boxExtent;
		planeNormal /= -planeNormal[k];

		// Find the minimum and maximum distances along e_k
		// between intersection(V, H) and P.

		Real maxAxisDistance = -dot(asVector(planePosition), planeNormal);
		Real minAxisDistance = maxAxisDistance;

		for (integer i = 0;i < k;++i)
		{
			const Real& deltaAxisDistance = planeNormal[i];

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
			const Real& deltaAxisDistance = planeNormal[i];

			if (deltaAxisDistance < 0)
			{
				minAxisDistance += deltaAxisDistance;
			}
			else
			{
				maxAxisDistance += deltaAxisDistance;
			}
		}

		// Map the result back to the original
		// problem domain.

		const AlignedBox<1, Real> result(
			box.min()[k] + minAxisDistance * boxExtent[k],
			box.min()[k] + maxAxisDistance * boxExtent[k]);
		
		return result;
	}


}

#endif
