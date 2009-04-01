#ifndef PASTELGEOMETRY_BOUNDING_SPHERE_HPP
#define PASTELGEOMETRY_BOUNDING_SPHERE_HPP

#include "pastel/geometry/bounding_sphere.h"

#include "pastel/sys/tuple_tools.h"

#include "pastel/geometry/sphere.h"
#include "pastel/sys/point.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/vector_tools.h"
#include "pastel/math/matrix_tools.h"
#include "pastel/math/linear.h"

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
		// Let p be the (M+1)-tuple of vertices
		// of an M-simplex.
		// Let the circumscribed sphere S of
		// the simplex be
		// S = (c, r)
		// with c the center
		// and r the radius.
		//
		// The problem is to solve c and r
		// given the following requirements:
		//
		// * For all i e [0, M]: ||p[i] - c|| = r
		// * Minimize r.
		//
		// ||p[i] - c|| = r
		// <=>
		// dot(p[i] - c) = r^2
		// <=>
		// dot((p[i] - p[0]) - (c - p[0])) = r^2
		// <=>
		// dot(d[i] - (c - p[0])) = r^2
		//
		// where d[i] = p[i] - p[0]
		//
		// Eliminate r^2 from all but the first equation:
		// <=>
		// { dot(d[0] - (c - p[0])) = r^2
		// { dot(d[i] - (c - p[0])) - dot(d[0] - (c - p[0])) = 0
		// <=>
		// { dot(c - p[0]) = r^2
		// { dot(d[i] - (c - p[0])) - dot(c - p[0]) = 0
		//
		// dot(d[i] - (c - p[0])) - dot(c - p[0]) = 0
		// <=>
		// dot(d[i]) - 2 dot(d[i], c - p[0]) + dot(c - p[0]) -
		// dot(c - p[0]) = 0
		// <=>
		// dot(d[i]) - 2 dot(d[i], c - p[0]) = 0
		// <=>
		// dot(d[i], c - p[0]) = (1/2) dot(d[i])
		//
		// Give c via barycentric coordinates w.r.t. p[i]:
		// <=>
		// dot(d[i], sum[j = 1..m](u[j] p[i]) - p[0]) = (1/2) dot(d[i])
		// <=>
		// dot(d[i], sum[j = 1..m](u[j] (p[j] - p[0]))) = (1/2) dot(d[i])
		// <=>
		// dot(d[i], sum[j = 1..m](u[j] d[j])) = (1/2) dot(d[i])
		// <=>
		// sum[j = 1..m](dot(d[i], d[j]) u_j) = (1/2) dot(d[i])
		//
		// This can be given by matrices as:
		// D^T D u = b
		// with
		// D = [d[1], ..., d[M]]
		// b = 0.5 * [dot(d[1], d[1]), ... dot(d[M], d[M])]^T
		//
		// D^T D is an MxM matrix, b is an M-vector.
		// D^T D is invertible iff D has full rank, that is,
		// the d[i] are linearly independent.
		// D^T D is obviously symmetric.

		Matrix<M, N, Real> d;
		Vector<M, Real> b;

		for (integer i = 0;i < M;++i)
		{
			const Vector<N, Real> delta(simplex[i + 1] - simplex[0]);

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

		const Vector<M, Real> u(
			solveLinearSystem(ddt, b));

		const Vector<N, Real> translation(u * d);

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
