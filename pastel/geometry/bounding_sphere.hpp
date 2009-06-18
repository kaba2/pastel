#ifndef PASTEL_BOUNDING_SPHERE_HPP
#define PASTEL_BOUNDING_SPHERE_HPP

#include "pastel/geometry/bounding_sphere.h"

#include "pastel/sys/tuple_tools.h"

#include "pastel/geometry/sphere.h"
#include "pastel/sys/point.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/vector_tools.h"
#include "pastel/math/matrix_tools.h"
#include "pastel/sys/math_functions.h"

#include "pastel/geometry/bounding_alignedbox.h"
#include "pastel/geometry/overlaps_sphere_point.h"

#include <boost/static_assert.hpp>

namespace Pastel
{

	template <int N, typename Real>
	Sphere<N, Real> boundingSphere(
		const AlignedBox<N, Real>& alignedBox)
	{
		const Vector<N, Real> delta = alignedBox.max() - alignedBox.min();
		return Sphere<N, Real>(
			linear(alignedBox.min(), alignedBox.max(), 0.5),
			std::sqrt(dot(delta)) * 0.5);
	}

	template <int N, typename Real>
	Sphere<N, Real> boundingSphere(
		const Box<N, Real>& box)
	{
		return Sphere<N, Real>(
			box.position(),
			std::sqrt(dot(box.width())));
	}

	template <int N, typename Real>
	Sphere<N, Real> boundingSphere(
		const Point<N, Real>& aPoint)
	{
		return circumscribedSphere(aPoint);
	}

	template <int N, typename Real>
	Sphere<N, Real> boundingSphere(
		const Segment<N, Real>& segment)
	{
		return circumscribedSphere(
			segment.start(), segment.end());
	}

	template <int N, typename Real>
	Sphere<N, Real> boundingSphere(
		const Point<N, Real>& aPoint,
		const Point<N, Real>& bPoint)
	{
		return circumscribedSphere(aPoint, bPoint);
	}

	template <int N, typename Real>
	Sphere<N, Real> boundingSphere(
		const Simplex<N, Real, 0>& simplex)
	{
		return circumscribedSphere(simplex);
	}

	template <int N, typename Real>
	Sphere<N, Real> boundingSphere(
		const Simplex<N, Real, 1>& simplex)
	{
		return circumscribedSphere(simplex);
	}

	namespace BoundingSphere
	{

		template <int SubsetSize, int Elements>
		void firstSubset(Tuple<SubsetSize, integer>& subset)
		{
			BOOST_STATIC_ASSERT(SubsetSize > 0);
			BOOST_STATIC_ASSERT(Elements > 0);
			BOOST_STATIC_ASSERT(SubsetSize <= Elements);

			for (integer i = 0;i < SubsetSize;++i)
			{
				subset[i] = (SubsetSize - 1) - i;
			}
		}

		template <int SubsetSize, int Elements>
		bool nextSubset(Tuple<SubsetSize, integer>& subset)
		{
			BOOST_STATIC_ASSERT(SubsetSize > 0);
			BOOST_STATIC_ASSERT(Elements > 0);
			BOOST_STATIC_ASSERT(SubsetSize <= Elements);

			if (subset[0] + 1 < Elements)
			{
				++subset[0];
			}
			else
			{
				integer j = 1;
				while(j < SubsetSize)
				{
					if (subset[j] + 1 < subset[j - 1])
					{
						break;
					}

					++j;
				}

				if (j == SubsetSize)
				{
					// This is the last subset.
					return false;
				}

				integer value = subset[j] + 1;
				while(j >= 0)
				{
					subset[j] = value;
					++value;
					--j;
				}
			}

			return true;
		}

		template <int K>
		struct DimensionTag {};

		template <int N, typename Real, int M>
		Sphere<N, Real> boundingSphere(
			const Simplex<N, Real, M>& simplex,
			DimensionTag<M>)
		{
			return circumscribedSphere(simplex);
		}

		template <int N, typename Real, int M, int K>
		Sphere<N, Real> boundingSphere(
			const Simplex<N, Real, M>& simplex,
			DimensionTag<K>)
		{
			// Find all the K-subsimplices.

			Simplex<N, Real, K> subSimplex;

			Tuple<K + 1, integer> indexSubset;
			Tuple<K + 1, integer> largestSubset;

			Sphere<N, Real> largestBound;

			firstSubset<K + 1, M + 1>(indexSubset);
			do
			{
				// Copy the subsimplex.
				for (integer i = 0;i < K + 1;++i)
				{
					subSimplex[i] = simplex[indexSubset[i]];
				}

				const Sphere<N, Real> bound(circumscribedSphere(subSimplex));
				if (bound.radius() > largestBound.radius())
				{
					largestBound = bound;
					largestSubset = indexSubset;
				}
			}
			while (nextSubset<K + 1, M + 1>(indexSubset));

			bool boundFound = true;

			integer subsetIndex = K;

			for (integer i = 0;i < M + 1;++i)
			{
				// Those points that were part of the construction
				// of the sphere should not be tested
				// because a simple roundoff error could mess things up.
				if (subsetIndex >= 0 && largestSubset[subsetIndex] == i)
				{
					--subsetIndex;
				}
				else
				{
					if (!overlaps(largestBound, simplex[i]))
					{
						boundFound = false;
						break;
					}
				}
			}

			if (boundFound)
			{
				return largestBound;
			}

			return boundingSphere(simplex, DimensionTag<K + 1>());
		}

	}

	template <int N, typename Real, int M>
	Sphere<N, Real> boundingSphere(
		const Simplex<N, Real, M>& simplex)
	{
		// Let m(K) be the boundary K-simplex of
		// the M-simplex S that has the largest
		// minimum volume bounding sphere (if there are
		// many such, choose arbitrarily).
		// Let B(m(K)) be that bounding sphere.
		// In ascending K order, K < M, if some B(m(K)) contains
		// the (points of) simplex S, then B(m(K))
		// is the minimum volume bounding sphere for S.
		// Otherwise, the minimum volume bounding sphere is
		// given by the circumscribed sphere of S.
		//
		// Thus the algorithm is the following:
		//
		// for (integer K = 1;K < N + 1;++K)
		// {
		//     Find the largest circumscribed sphere of a K-subsimplex
		//     If the sphere contains S, return it.
		// }
		//
		// The loop is implemented as a compile time recursion.

		return BoundingSphere::boundingSphere(
			simplex,
			BoundingSphere::DimensionTag<1>());
	}

	template <int N, typename Real>
	Sphere<N, Real> circumscribedSphere(
		const Point<N, Real>& aPoint)
	{
		return Sphere<N, Real>(aPoint, 0);
	}

	template <int N, typename Real>
	Sphere<N, Real> circumscribedSphere(
		const Point<N, Real>& aPoint,
		const Point<N, Real>& bPoint)
	{
		return Sphere<N, Real>(
			linear(aPoint, bPoint, 0.5),
			norm(evaluate(bPoint - aPoint)) * 0.5);
	}

	template <int N, typename Real>
	Sphere<N, Real> circumscribedSphere(
		const Simplex<N, Real, 0>& simplex)
	{
		return Sphere<N, Real>(simplex[0], 0);
	}

	template <int N, typename Real>
	Sphere<N, Real> circumscribedSphere(
		const Simplex<N, Real, 1>& simplex)
	{
		return Sphere<N, Real>(
			linear(simplex[0], simplex[1], 0.5),
			norm(evaluate(simplex[1] - simplex[0])) * 0.5);
	}

	template <int N, typename Real, int M>
	Sphere<N, Real> circumscribedSphere(
		const Simplex<N, Real, M>& simplex)
	{
		// Let p be the (m+1)-tuple of vertices
		// of an m-simplex.
		// The problem is to solve c in R^n and r in R
		// given the following requirements:
		//
		// * For all i e [0, m]: ||p[i] - c|| = r
		// * Minimize r.
		//
		// Or equivalently:
		// 
		// { dot(p_0 - c) = r^2
		// { dot(p_i - c) = dot(p_0 - c), for i in [1, m]
		// 
		// And we wish to minimize r^2.
		// 
		// So we set up a minimization problem using Lagrange multipliers:
		// 
		// e(c, u_1, ..., u_m) = 
		// dot(p_0 - c) + sum[i = 1..m] u_i (dot(p_i - c) - dot(p_0 - c))
		// 
		// Now the unconstrained minimum of e corresponds to the constrained 
		// minimum of r. Next we shall use calculus of variations to find the 
		// minimum while avoiding component notation.
		// 
		// Taking the variations of e w.r.t. u_i when equated with zero gives n 
		// equations which recover the constraint equations. That's trivial so we 
		// leave that out.
		// 
		// The (first) variation of e w.r.t. c equated to zero gives:
		// 
		// de(c, u_1, ..., u_m; h)
		// = 2 dot(p_0 - c, h) + 
		// sum[i = 1..m] u_i (2 dot(p_i - c, h) - 2 dot(p_0 - c, h))
		// = 2 dot(p_0 - c, h) + 
		// 2 sum[i = 1..m] u_i dot(p_i - p_0, h)
		// = 0
		// 
		// which holds for any variation vector h.
		// 
		// =>
		// dot(c - p_0, h) = sum[i = 1..m] u_i dot(p_i - p_0, h)
		//
		// So plug in the standard basis e_i to h:
		// 
		// dot(c - p_0, e_i) = sum[i = 1..m] u_i dot(p_i - p_0, e_i), i in [1, n]
		//  =>
		//  c - p_0 = sum[i = 1..m] u_i (p_i - p_0)
		// =>
		// c = p_0 + sum[i = 1..m] u_i (p_i - p_0)
		//
		// Which tells us that the center point lies on
		// the affine hyperplane set by the points p_i.
		// This gives us the idea to represent c by
		// barycentric coordinates w.r.t. to points p_i.
		//
		// ||p_i - c|| = r
		// <=>
		// dot(p_i - c) = r^2
		// <=>
		// dot((p_i - p_0) - (c - p_0)) = r^2
		// <=>
		// dot(d_i - (c - p_0)) = r^2
		//
		// where d_i = p_i - p_0
		//
		// Eliminate r^2 from all but the first equation:
		// <=>
		// { dot(d_0 - (c - p_0)) = r^2
		// { dot(d_i - (c - p_0)) - dot(d_0 - (c - p_0)) = 0
		// <=>
		// { dot(c - p_0) = r^2
		// { dot(d_i - (c - p_0)) - dot(c - p_0) = 0
		//
		// dot(d_i - (c - p_0)) - dot(c - p_0) = 0
		// <=>
		// dot(d_i) - 2 dot(d_i, c - p_0) + dot(c - p_0) -
		// dot(c - p_0) = 0
		// <=>
		// dot(d_i) - 2 dot(d_i, c - p_0) = 0
		// <=>
		// dot(d_i, c - p_0) = (1/2) dot(d_i)
		//
		// Give c via barycentric coordinates w.r.t. p_i:
		// <=>
		// dot(d_i, sum[j = 0..m](u_j p_i) - p_0) = (1/2) dot(d_i)
		// <=>
		// dot(d_i, sum[j = 0..m](u_j (p_j - p_0))) = (1/2) dot(d_i)
		// <=>
		// dot(d_i, sum[j = 1..m](u_j d_j)) = (1/2) dot(d_i)
		// <=>
		// sum[j = 1..m](dot(d_i, d_j) u_j) = (1/2) dot(d_i)
		//
		// This can be given by matrices as:
		// D^T D u = b
		// with
		// D = [d_1, ..., d_m]
		// b = 0.5 * [dot(d_1, d_1), ... dot(d_m, d_m)]^T
		//
		// D^T D is an m x m matrix, b is an m-vector.
		// D^T D is invertible iff D has full rank, that is,
		// the d_i are linearly independent.

		Matrix<M, N, Real> d;
		Vector<M, Real> b;

		for (integer i = 0;i < M;++i)
		{
			const Vector<N, Real> delta = 
				simplex[i + 1] - simplex[0];

			d[i] = delta;
			b[i] = dot(delta) * 0.5;
		}

		Matrix<M, M, Real> ddt;

		for (integer i = 0;i < M;++i)
		{
			for (integer j = 0;j < M;++j)
			{
				ddt(i, j) = dot(d[i], d[j]);
			}
		}

		// D^T D u = b
		// <=>
		// u^T D^T D = b^T

		const Vector<M, Real> u = 
			solveLinear(ddt, b);

		const Vector<N, Real> translation = 
			u * d;

		return Sphere<N, Real>(
			simplex[0] + translation,
			norm(translation));
	}

	template <int N, typename Real, typename InputIterator, typename PositionFunctor>
	Sphere<N, Real> boundingSphere(
		const InputIterator& from,
		const InputIterator& to,
		const PositionFunctor& positionFunctor)
	{
		// This does not give the minimum volume
		// bounding sphere, but it does give something.

		if (from == to)
		{
			return Sphere<N, Real>();
		}

		Point<N, Real> midPoint(0);

		integer points = 0;

		// Find out the midpoint.

		InputIterator iter = from;
		while(iter != to)
		{
			midPoint += asVector(positionFunctor(*iter));
			++iter;
			++points;
		}

		asVector(midPoint) /= points;

		// Compute the maximum distance from the midpoint.

		Real maxDistance2 = 0;

		iter = from;
		while(iter != to)
		{
			const Real currentDistance2 =
				dot(positionFunctor(*iter) - midPoint);
			if (currentDistance2 > maxDistance2)
			{
				maxDistance2 = currentDistance2;
			}

			++iter;
		}

		return Sphere<N, Real>(
			midPoint,
			std::sqrt(maxDistance2));
	}

	namespace Detail_BoundingSphere
	{

		template <int N, typename Real>
		class PositionFunctor
		{
		public:
			const Point<N, Real>& operator()(const Point<N, Real>& position) const
			{
				return position;
			}
		};

	}

	template <int N, typename Real, typename InputIterator>
	Sphere<N, Real> boundingSphere(
		const InputIterator& from,
		const InputIterator& to)
	{
		Detail_BoundingSphere::PositionFunctor<N, Real> positionFunctor;
		return Pastel::boundingSphere<N, Real>(from, to, positionFunctor);
	}

}

#endif
