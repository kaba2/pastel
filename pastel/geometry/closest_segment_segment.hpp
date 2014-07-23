#ifndef PASTELGEOMETRY_CLOSEST_SEGMENT_SEGMENT_HPP
#define PASTELGEOMETRY_CLOSEST_SEGMENT_SEGMENT_HPP

#include "pastel/geometry/closest_segment_segment.h"
#include "pastel/geometry/closest_line_line.h"
#include "pastel/geometry/closest_line_point.h"

namespace Pastel
{

	template <typename Real, int N>
	Tuple<Real, 2> closest(
		const Segment<Real, N>& aSegment,
		const Segment<Real, N>& bSegment)
	{
		ENSURE_OP(aSegment.n(), ==, bSegment.n());

		// The following ideas are from the
		// article "Distance between Lines and Segments
		// with their Closest Vector of Approach"
		// by Dan Sunday.

		// Parametrize the line segment 'aSegment' by:
		// p0(u) = p0 + u d0
		// and the line segment 'bSegment' by:
		// p1(v) = p1 + v d1
		//
		// We wish to find (u, v) in [0, 1]^2 such that
		// w(u, v) = |p1(v) - p0(u)|
		// is minimized.
		// This is equivalent to minimizing
		// w^2(u, v) = dot(p1(v) - p0(u))
		// in [0, 1]^2.
		//
		// w^2 is an increasing quadratic in u and v.
		// Let (tu, tv) be the global minimum of w^2.
		// If (tu, tv) is in [0, 1]^2, then we have
		// found the closest points on the segments.
		// Otherwise, the minimum is on the
		// boundary (edges) of [0, 1]^2. We need
		// not test all of the edges: at most
		// two of them need to be tested
		// (the ones 'visible' to (tu, tv)).

		// The regions in the uv-plane
		// are divided as follows:
		//
		// 0 1 2
		// 3 x 4
		// 5 6 7
		//
		// where x = [0, 1]^2
		//
		// Each region will be handled separately.

		// w^2(u, v) = dot((p1 - p0) + d1 * v - d0 * u)
		// = dot(p1 - p0) + 2 v dot(p1 - p0, d1) - 2 u dot(p1 - p0, d0) + v^2 dot(d1) - 2 u v dot(d0, d1) + u^2 dot(d0)
		// d/du w^2(u, v) = -2 dot(p1 - p0, d0) - 2 v dot(d0, d1) + 2 u dot(d0)
		// d/dv w^2(u, v) = 2 dot(p1 - p0, d1) + 2 v dot(d1) - 2 u dot(d0, d1)

		Vector<Real, N> aDelta = 
			aSegment.end() - aSegment.start();
		Vector<Real, N> bDelta = 
			bSegment.end() - bSegment.start();

		Real aNorm2 = dot(aDelta);
		Real bNorm2 = dot(bDelta);

		const Vector<Real, N> aUnitDelta = 
			aDelta / std::sqrt(aNorm2);
		const Line<Real, N> aLine(aSegment.start(), aUnitDelta);

		const Vector<Real, N> bUnitDelta = 
			bDelta / std::sqrt(aNorm2);
		const Line<Real, N> bLine(bSegment.start(), bUnitDelta);

		// Handle degenerate cases.

		// EPSILON
		if (aNorm2 == 0)
		{
			if (bNorm2 == 0)
			{
				return Tuple<Real, 2>(0, 0);
			}
			else
			{
				return closest(bLine, aSegment.start());
			}
		}
		else
		{
			if (bNorm2 == 0)
			{
				return closest(aLine, bSegment.start());
			}
		}

		// Non-degenerate case.

		// Find global minimum of w^2.

		const Tuple<Real, 2> t = closest(aLine, bLine);

		const Real& u = t[0];
		const Real& v = t[1];

		bool uLess0 = (u < 0);
		bool uGreater1 = (u > 1);
		bool vLess0 = (v < 0);
		bool vGreater1 = (v > 1);

		if (uLess0)
		{
			if (vLess0)
			{
				// Region 5!
				// => Minimum at u = 0 or v = 0

				const Real u0vCandidate =
					clamp(-dot(startDelta, bDelta) / bNorm2, 0, 1);
				const Real u0vDistance2 = dot((bSegment.start() - aSegment.start()) +

					bDelta * u1vCandidate);

				Real v0uCandidate =
					clamp(dot(startDelta, aDelta) / aNorm2, 0, 1);
				Real v0uDistance2 = dot((bSegment.start() - aSegment.start()) -

					aDelta * v0uCandidate);

				if (u0vDistance2 < v0uDistance2)
				{
					return Tuple<Real, 2>(0, u0vCandidate);
				}

				return Tuple<Real, 2>(v0uCandidate, 0);
			}
			else
			{
				if (vGreater1)
				{
					// Region 0!
					// => Minimum at u = 0 or v = 1

					Real u0vCandidate =
						clamp(-dot(startDelta, bDelta) / bNorm2, 0, 1);
					Real u0vDistance2 =

						dot((bSegment.start() - aSegment.start()) + bDelta * u1vCandidate);

					Real v1uCandidate =
						clamp((dot(startDelta, aDelta) +
						dot(aDelta, bDelta)) / aNorm2, 0, 1);
					Real v1uDistance2 =

						dot((bSegment.end() - aSegment.start()) - aDelta * v1uCandidate);

					if (u0vDistance2 < v1uDistance2)
					{
						return Tuple<Real, 2>(0, u0vCandidate);
					}

					return Tuple<Real, 2>(v1uCandidate, 1);
				}
				else
				{
					// Region 3!
					// => Minimum at u = 0
					// d/dv w^2(0, v) = 2 dot(p1 - p0, d1) + 2 v dot(d1) = 0
					// => v = -dot(p1 - p0, d1) / dot(d1)

					return Tuple<Real, 2>(
						clamp(-dot(bSegment.start() - aSegment.start(), bDelta) / bNorm2, 0, 1), 0);
				}
			}
		}
		else
		{
			if (vLess0)
			{
				if (uGreater1)
				{
					// Region 7!
					// => Minimum at u = 1 or v = 0

					Real u1vCandidate =
						clamp((dot(aDelta, bDelta) -
						dot(startDelta, bDelta)) / bNorm2, 0, 1);
					Real u1vDistance2 =
						dot((bSegment.start() - aSegment.end()) +

						bDelta * u1vCandidate);

					Real v0uCandidate =
						clamp(dot(startDelta, aDelta) / aNorm2, 0, 1);
					Real v0uDistance2 =
						dot((bSegment.start() - aSegment.start()) -

						aDelta * v0uCandidate);

					if (u1vDistance2 < v0uDistance2)
					{
						return Tuple<Real, 2>(1, u1vCandidate);
					}

					return Tuple<Real, 2>(v0uCandidate, 0);
				}
				else
				{
					// Region 6!
					// => Minimum at v = 0.
					// d/du w^2(u, 0) = -2dot(p1 - p0, d0) + 2u dot(d0) = 0
					// => u = dot(p1 - p0, d0) / dot(d0)

					return Tuple<Real, 2>(
						clamp(dot(bSegment.start() - aSegment.start(), aDelta) / aNorm2, 0, 1), 0);
				}
			}
			else
			{
				if (uGreater1)
				{
					if (vGreater1)
					{
						// Region 2!
						// => Minimum at u = 1 or v = 1

						Real u1vCandidate =
							clamp((dot(aDelta, bDelta) -
							dot(startDelta, bDelta)) / bNorm2, 0, 1);
						Real u1vDistance2 =

							dot((bSegment.start() - aSegment.end()) + bDelta * u1vCandidate);

						Real v1uCandidate =
							clamp((dot(startDelta, aDelta) +
							dot(aDelta, bDelta)) / aNorm2, 0, 1);
						Real v1uDistance2 =

							dot((bSegment.end() - aSegment.start()) - aDelta * v1uCandidate);

						if (u1vDistance2 < v1uDistance2)
						{
							return Tuple<Real, 2>(1, u1vCandidate);
						}

						return Tuple<Real, 2>(v1uCandidate, 1);
					}
					else
					{
						// Region 4!
						// => Minimum at u = 1
						// d/dv w^2(1, v) = 2 dot(p1 - p0, d1) + 2 v dot(d1) - 2 dot(d0, d1) = 0
						// => v = [dot(d0, d1) - dot(p1 - p0, d1)] / dot(d1)

						return Tuple<Real, 2>(
							clamp((dot(aDelta, bDelta) -
							dot(bSegment.start() - aSegment.start(), bDelta)) / bNorm2, 0, 1), 0);
					}
				}
				else
				{
					if (vGreater1)
					{
						// Region 1
						// => Minimum at v = 1
						// d/du w^2(u, 1) = -2 dot(p1 - p0, d0) - 2 dot(d0, d1) + 2 u dot(d0) = 0
						// => u = [dot(p1 - p0, d0) + dot(d0, d1)] / dot(d0)

						return Tuple<Real, 2>(
							clamp((dot(bSegment.start() - aSegment.start(), aDelta) +
							dot(aDelta, bDelta)) / aNorm2, 0, 1), 0);
					}
					//else
					//{
						// Region x
						// Will be handled at the end of the function.
					//}
				}
			}
		}

		// Region x

		return t;
	}

}

#endif
