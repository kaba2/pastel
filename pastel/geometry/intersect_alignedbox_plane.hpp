#ifndef PASTEL_INTERSECT_ALIGNEDBOX_PLANE_HPP
#define PASTEL_INTERSECT_ALIGNEDBOX_PLANE_HPP

#include "pastel/geometry/intersect_alignedbox_plane.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	bool intersect(
		const AlignedBox<N, Real>& box,
		const Plane<N, Real>& plane, 
		integer& clipDimension,
		PASTEL_NO_DEDUCTION(Real)& minBoxMax,
		PASTEL_NO_DEDUCTION(Real)& maxBoxMin)
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

		// Map the points so that the box
		// maps to [0, 1]^n.

		const Vector<N, Real> boxExtent =
			box.extent();

		const Point<N, Real> planePosition(
			(plane.position() - box.min()) / boxExtent);

		Vector<N, Real> planeNormal =
			plane.normal() * boxExtent;

		// The signed distance of the plane from (0, 0, ..., 0) is
		// <0 - q, n>
		// = -<q, n>

		const Real planeDistance = -dot(asVector(planePosition), planeNormal);

		// Find maximal and minimal signed distances
		// of the vertices in V along the normal vector.

		Real minDistance = planeDistance;
		Real maxDistance = planeDistance;
		for (integer i = 0;i < dimension;++i)
		{
			// dt = <p + e_i - q, n> - <p - q, n>
			// = <e_i, n>
			// = n_i

			const Real& deltaAxisDistance = planeNormal[i];
			if (deltaAxisDistance < 0)
			{
				minDistance += deltaAxisDistance;
			}
			else
			{
				maxDistance += deltaAxisDistance;
			}
		}

		if (minDistance >= 0 ||
			maxDistance <= 0)
		{
			// The plane does not intersect the box.
			return false;
		}
		
		integer k = 0;
		integer kFits = 0;
		for (integer i = 0;i < dimension;++i)
		{
			const Real& deltaAxisDistance = std::abs(planeNormal[i]);
			if (minDistance > -deltaAxisDistance &&
				maxDistance < deltaAxisDistance)
			{
				++kFits;
				k = i;
			}
		}

		if (kFits == 0)
		{
			// The plane intersects the box
			// but not in the way required
			// by the condition.

			return false;
		}

		/*
		if (kFits > 1)
		{
			// The plane does intersect the box,
			// but in a way that does not affect
			// the end-result.

			clipDimension = k;
			minBoxMax = box.max()[k];
			maxBoxMin = box.min()[k];
			
			return true;
		}
		*/

		// Exactly one k fits the bill.
		// That's our clipping dimension.
		// The plane intersects the box
		// in the way given in the condition.

		// Now find out the extents of the
		// sub-boxes.

		// Note that the distances we have measured
		// thus far are from the plane along
		// the plane normal. They can't be
		// used to give bounds for the sub-boxes.

		// This correction factor allows us to 
		// measure the distance along e_k,
		// rather than along plane normal.
		const Real axisCorrection = -inverse(planeNormal[k]);
		planeNormal *= axisCorrection;

		// Find the minimum and maximum distances along e_k
		// between intersection(V, H) and P.

		// The distance to the
		// plane along e_k is given by:
		// <p + te_k - q, n> = 0
		// => t<e_k, n> + <p - q, n> = 0
		// => t = <q - p, n> / <e_k, n>
		// => t = <q - p, n> / n_k
		// We start from p = (0, 0, ..., 0), so
		// t = <q, n> / n_k
		//   = -<q, n> / (-n_k)

		Real maxAxisDistance = planeDistance * axisCorrection;
		Real minAxisDistance = planeDistance * axisCorrection;

		for (integer i = 0;i < k;++i)
		{
			// dt = <q - (p + e_i), n / n_k> - <q - p, n / n_k>
			// = <-e_i, n / n_k>
			// = n_i / (-n_k)

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

		clipDimension = k;
		maxBoxMin = box.min()[k] + minAxisDistance * boxExtent[k];
		minBoxMax = box.min()[k] + maxAxisDistance * boxExtent[k];
		
		return true;
	}


}

#endif
