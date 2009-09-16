#ifndef PASTEL_ANTIPODAL_HPP
#define PASTEL_ANTIPODAL_HPP

#include "pastel/geometry/antipodal.h"

#include "pastel/geometry/distance_point_point.h"
#include "pastel/geometry/convexhull.h"
#include "pastel/geometry/predicates.h"

#include "pastel/sys/math_functions.h"

namespace Pastel
{

	template <typename Real>
	Integer2 antipodal(
		const std::vector<Vector<Real, 2> >& pointSet,
		const Vector<Real, 2>& direction)
	{
		const Vector<Real, 2> normal = -cross(direction);

		Integer2 result;
		Real tMin = infinity<Real>();
		Real tMax = -infinity<Real>();

		const integer points = pointSet.size();
		for (integer i = 0;i < points;++i)
		{
			const Vector<Real, 2> delta = pointSet[i] - pointSet[0];
			const Real t = dot(delta, normal);
			if (t < tMin)
			{
				tMin = t;
				result[0] = i;
			}
			if (t > tMax)
			{
				tMax = t;
				result[1] = i;
			}
		}

		return result;
	}

	template <typename Real>
	Integer2 antipodal(
		const std::vector<Vector<Real, 2> >& pointSet)
	{
		std::vector<Vector<Real, 2> > hull;

		convexHullGrahamsScan(pointSet, hull);

		return convexPolygonAntipodal(hull);
	}

	template <int N, typename Real>
	Integer2 antipodalBruteForce(
		const std::vector<Vector<Real, N> >& pointSet)
	{
		Real maxDistance = 0;
		integer kMax = 0;
		integer iMax = 0;

		const integer points = pointSet.size();
		for (integer i = 0;i < points;++i)
		{
			for (integer k = i + 1;k < points;++k)
			{
				const Real currentDistance = distance2(pointSet[i], pointSet[k]);
				if (currentDistance >= maxDistance)
				{
					maxDistance = currentDistance;
					iMax = i;
					kMax = k;
				}
			}
		}

		return Integer2(iMax, kMax);
	}

	namespace Detail
	{

		namespace ConvexDiameter
		{

			template <typename Real>
			class DiameterFunctor
			{
			public:
				DiameterFunctor(const std::vector<Vector<Real, 2> >& pointSet)
					: pointSet_(&pointSet)
					, maxDistance_(0)
					, aMax_(0)
					, bMax_(0)
				{
				}

				void operator()(integer a, integer b)
				{
					const Real currentDistance = distance2((*pointSet_)[a], (*pointSet_)[b]);
					if (currentDistance >= maxDistance_)
					{
						maxDistance_ = currentDistance;
						aMax_ = a;
						bMax_ = b;
					}
				}

				Integer2 antipodal() const
				{
					return Integer2(aMax_, bMax_);
				}

				Real maxDistance() const
				{
					return maxDistance_;
				}

			private:
				const std::vector<Vector<Real, 2> >* pointSet_;
				Real maxDistance_;
				integer aMax_;
				integer bMax_;
			};

		}

	}

	template <typename Real>
	Integer2 convexPolygonAntipodal(
		const std::vector<Vector<Real, 2> >& pointSet)
	{
		Detail::ConvexDiameter::DiameterFunctor<Real> diameterFunctor(pointSet);

		reportConvexPolygonAntipodal(pointSet, diameterFunctor);

		return diameterFunctor.antipodal();
	}

	template <typename Real, typename ReportFunctor>
	void reportConvexPolygonAntipodal(
		const std::vector<Vector<Real, 2> >& pointSet,
		ReportFunctor& reportFunctor)
	{
		// This algorithm is by
		// Preparata and Shamos,
		// using the rotating calipers technique.

		const integer points = pointSet.size();

		if (points == 0)
		{
			return;
		}

		if (points == 1)
		{
			return;
		}

		if (points == 2)
		{
			reportFunctor(0, 1);
			return;
		}

		const integer p0 = points - 1;
		integer p = 0;
		integer pNext = p;
		++pNext;
		if (pNext == points)
		{
			pNext = 0;
		}
		integer q = pNext;
		integer qNext = q;
		++qNext;
		if (qNext == points)
		{
			qNext = 0;
		}

		while(mabs(signedArea(pointSet[p], pointSet[pNext], pointSet[qNext])) >
			mabs(signedArea(pointSet[p], pointSet[pNext], pointSet[q])))
		{
			q = qNext;
			++qNext;
			if (qNext == points)
			{
				qNext = 0;
			}
			const integer q0 = q;
			while(q != p0)
			{
				p = pNext;
				++pNext;
				if (pNext == points)
				{
					pNext = 0;
				}
				reportFunctor(p, q);
				while(mabs(signedArea(pointSet[p], pointSet[pNext], pointSet[qNext])) >
					mabs(signedArea(pointSet[p], pointSet[pNext], pointSet[q])))
				{
					q = qNext;
					++qNext;
					if (qNext == points)
					{
						qNext = 0;
					}
					if (p == q0 && q == p0)
					{
						return;
					}
					else
					{
						reportFunctor(p, q);
					}
				}
				if (mabs(signedArea(pointSet[p], pointSet[pNext], pointSet[qNext])) ==
					mabs(signedArea(pointSet[p], pointSet[pNext], pointSet[q])))
				{
					if (p == q0 && q == p0)
					{
						reportFunctor(pNext, q);
					}
					else
					{
						reportFunctor(p, qNext);
					}
				}
			}
		}
	}

}

#endif
