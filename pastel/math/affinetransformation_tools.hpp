#ifndef PASTEL_AFFINETRANSFORMATION_TOOLS_HPP
#define PASTEL_AFFINETRANSFORMATION_TOOLS_HPP

#include "pastel/math/affinetransformation_tools.h"

#include "pastel/math/matrix_tools.h"
#include "pastel/sys/point_tools.h"
#include "pastel/sys/vector_tools.h"
#include "pastel/sys/math_functions.h"

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	template <int N, typename Real>
	AffineTransformation<N, Real> inverse(
		const AffineTransformation<N, Real>& that)
	{
		const Matrix<N, N, Real> transformInverse(
			inverse(that.transformation()));

		return AffineTransformation<N, Real>(
			transformInverse,
			evaluate(-that.translation()) * transformInverse);
	}

	template <int N, typename Real>
	AffineTransformation<N, Real> orthogonalInverse(
		const AffineTransformation<N, Real>& that)
	{
		const Matrix<N, N, Real> transformInverse(
			transpose(that.transformation()));

		return AffineTransformation<N, Real>(
			transformInverse,
			evaluate(-that.translation()) * transformInverse);
	}

	// 2D

	template <typename Real>
	AffineTransformation<2, Real> xShear2(
		const PASTEL_NO_DEDUCTION(Real)& yShear)
	{
		return AffineTransformation<2, Real>(
			Matrix<2, 2, Real>(
			1, yShear,
			0,      1),
			Vector<2, Real>(0));
	}

	template <typename Real>
	AffineTransformation<2, Real> yShear2(
		const PASTEL_NO_DEDUCTION(Real)& xShear)
	{
		return AffineTransformation<2, Real>(
			Matrix<2, 2, Real>(
			1,      0,
			xShear, 1),
			Vector<2, Real>(0));
	}

	template <typename Real>
	AffineTransformation<2, Real> rotation2(
		const PASTEL_NO_DEDUCTION(Real)& angle)
	{
		const Real c(std::cos(angle));
		const Real s(std::sin(angle));

		// Rotation from x to y

		return AffineTransformation<2, Real>(
			Matrix<2, 2, Real>(
			c, s,
			-s, c),
			Vector<2, Real>(0));
	}

	template <typename Real>
	AffineTransformation<2, Real> translation2(
		const Vector<2, Real>& translation)
	{
		return AffineTransformation<2, Real>(
			Matrix<2, 2, Real>(
			1, 0,
			0, 1),
			translation);
	}

	template <int N, typename Real>
	AffineTransformation<N, Real> leastSquaresAffineTransformation(
		const std::vector<Point<N, Real> >& from,
		const std::vector<Point<N, Real> >& to)
	{
		BOOST_STATIC_ASSERT(N > 0);
		return Pastel::leastSquaresAffineTransformation(
			N, from, to);
	}

	template <int N, typename Real>
	AffineTransformation<N, Real> leastSquaresAffineTransformation(
		integer dimension,
		const std::vector<Point<N, Real> >& from,
		const std::vector<Point<N, Real> >& to)
	{
		ENSURE2(from.size() == to.size(), from.size(), to.size());

		const integer points = from.size();

		if (points == 0)
		{
			return AffineTransformation<N, Real>(dimension);
		}

		// Compute centroids

		Point<N, Real> fromCentroid(ofDimension(dimension));
		Point<N, Real> toCentroid(ofDimension(dimension));

		for (integer i = 0;i < points;++i)
		{
			fromCentroid += asVector(from[i]);
			toCentroid += asVector(to[i]);
		}

		asVector(fromCentroid) /= points;
		asVector(toCentroid) /= points;

		Matrix<N, N, Real> c(dimension, dimension);
		for (integer i = 0;i < points;++i)
		{
			const Vector<N, Real> v = from[i] - fromCentroid;
			for (integer k = 0;k < dimension;++k)
			{
				c[k] += v * v[k];
			}
		}

		Matrix<N, N, Real> d(dimension, dimension);
		for (integer i = 0;i < points;++i)
		{
			const Vector<N, Real> v = to[i] - toCentroid;
			const Vector<N, Real> w = from[i] - fromCentroid;
			for (integer k = 0;k < dimension;++k)
			{
				d[k] += v * w[k];
			}
		}

		const Matrix<N, N, Real> a = inverse(c) * d;

		return AffineTransformation<2, Real>(
			a, toCentroid - fromCentroid * a);
	}

	template <typename Real>
	Tuple<4, Real> similarityTransformation(
		const std::vector<Point<2, Real> >& from,
		const std::vector<Point<2, Real> >& to)
	{
		ENSURE2(from.size() == to.size(), from.size(), to.size());
		typedef std::vector<Point<2, Real> >::const_iterator InputIterator;

		InputIterator fromIter = from.begin();
		InputIterator fromEnd = from.end();
		InputIterator toIter = to.begin();
		InputIterator toEnd = to.end();

		Vector<2, Real> sumFrom;
		Vector<2, Real> sumTo;
		Real sumSquareFrom = 0;
		Real dotSum = 0;
		Real crossDotSum = 0;

		integer points = 0;

		while(fromIter != fromEnd)
		{
			const Vector<2, Real>& fromVector = asVector(*fromIter);
			const Vector<2, Real>& toVector = asVector(*toIter);

			sumFrom += fromVector;
			sumTo += toVector;

			sumSquareFrom += dot(fromVector);
			dotSum += dot(fromVector, toVector);
			crossDotSum += dot(cross(fromVector), toVector);

			++fromIter;
			++toIter;
			++points;
		}

		const Real det = points * sumSquareFrom - dot(sumFrom);
		const Real invDet = inverse(det);

		const Vector<2, Real> translation(
			(sumSquareFrom * sumTo[0] - sumFrom[0] * dotSum + sumFrom[1] * crossDotSum) * invDet,
			(sumSquareFrom * sumTo[1] - sumFrom[1] * dotSum - sumFrom[0] * crossDotSum) * invDet);

		// scaledCos = scale * cos(angle)

		const Real scaledCos =
			(-sumFrom[0] * sumTo[0] - sumFrom[1] * sumTo[1] + points * dotSum) * invDet;

		// scaledSin = scale * sin(angle)

		const Real scaledSin =
			(sumFrom[1] * sumTo[0] - sumFrom[0] * sumTo[1] + points * crossDotSum) * invDet;

		// scaledCos^2 + scaledSin^2 = scale * cos^2(angle) + scale * sin^2(angle)
		// = scale * (cos^2(angle) + sin^2(angle)) = scale

		const Real scale = std::sqrt(scaledCos * scaledCos + scaledSin * scaledSin);

		// atan(scaledSin / scaledCos) = atan((scale * sin(angle)) / (scale * cos(angle)))
		// = atan(tan(angle)) = angle

		const Real angle = positiveRadians<Real>(std::atan2(scaledSin, scaledCos));

		return Tuple<4, Real>(scale, angle, translation[0], translation[1]);
	}

	template <typename Real>
	Tuple<4, Real> similarityTransformation(
		const Point<2, Real>& aFrom, const Point<2, Real>& bFrom,
		const Point<2, Real>& aTo, const Point<2, Real>& bTo)
	{
		const Vector<2, Real> fromDelta = bFrom - aFrom;
		const Vector<2, Real> toDelta = bTo - aTo;

		const Real fromNorm = norm(fromDelta);
		const Real toNorm = norm(toDelta);

		const Real scaling = toNorm / fromNorm;

		const Real ccwRotation = ccwAngle(
			fromDelta, toDelta,
			fromNorm, toNorm);

		const AffineTransformation<2, Real> scaleRotate =
			Pastel::similarityTransformation(
			scaling, ccwRotation, Vector<2, Real>(0));

		const Vector<2, Real> translation = aTo - aFrom * scaleRotate;

		return Tuple<4, Real>(scaling, ccwRotation, translation[0], translation[1]);
	}

	template <typename Real>
	AffineTransformation<2, Real> similarityTransformation(
		const PASTEL_NO_DEDUCTION(Real)& scaling,
		const PASTEL_NO_DEDUCTION(Real)& ccwRotation,
		const Vector<2, Real>& translation)
	{
		return AffineTransformation<2, Real>(
			Matrix<2, 2, Real>(
			scaling * std::cos(ccwRotation), scaling * std::sin(ccwRotation),
			-scaling * std::sin(ccwRotation), scaling * std::cos(ccwRotation)),
			translation);
	}

	template <typename Real>
	AffineTransformation<2, Real> similarityTransformation(
		const Tuple<4, Real>& parameter)
	{
		return Pastel::similarityTransformation(parameter[0], parameter[1],
			Vector<2, Real>(parameter[2], parameter[3]));
	}

	template <typename Real>
	bool affineTransform(
		const Point<2, Real>& aFrom, const Point<2, Real>& bFrom,
		const Point<2, Real>& cFrom,
		const Point<2, Real>& aTo, const Point<2, Real>& bTo,
		const Point<2, Real>& cTo,
		AffineTransformation<2, Real>& matrix)
	{
		const Vector<2, Real> bFromDelta(
			bFrom - aFrom);
		const Vector<2, Real> cFromDelta(
			cFrom - aFrom);

		const AffineTransformation<2, Real> fromMatrix(
			Matrix<2, 2, Real>(
			bFromDelta,
			cFromDelta),
			asVector(aFrom));

		const AffineTransformation<2, Real> invFromMatrix(
			inverse(fromMatrix));

		const Vector<2, Real> bToDelta(
			bTo - aTo);
		const Vector<2, Real> cToDelta(
			cTo - aTo);

		const AffineTransformation<2, Real> toMatrix(
			Matrix<2, 2, Real>(
			bToDelta,
			cToDelta),
			asVector(aTo));

		matrix = invFromMatrix * toMatrix;

		return true;
	}

	template <typename Real>
	bool affineTransform(
		const Point<3, Real>& aFrom, const Point<3, Real>& bFrom,
		const Point<3, Real>& cFrom,
		const Point<3, Real>& aTo, const Point<3, Real>& bTo,
		const Point<3, Real>& cTo,
		AffineTransformation<3, Real>& matrix)
	{
		const Vector<3, Real> bFromDelta(
			bFrom - aFrom);
		const Vector<3, Real> cFromDelta(
			cFrom - aFrom);
		const Vector<3, Real> normalFrom(
			cross(bFromDelta, cFromDelta));

		const AffineTransformation<3, Real> fromMatrix(
			Matrix<3, 3, Real>(
			bFromDelta,
			cFromDelta,
			normalFrom),
			asVector(aFrom));

		const AffineTransformation<3, Real> invFromMatrix(
			inverse(fromMatrix));

		const Vector<3, Real> bToDelta(
			bTo - aTo);
		const Vector<3, Real> cToDelta(
			cTo - aTo);
		const Vector<3, Real> normalTo(
			cross(bToDelta, cToDelta));

		const AffineTransformation<3, Real> toMatrix(
			Matrix<3, 3, Real>(
			bToDelta,
			cToDelta,
			normalTo),
			asVector(aTo));

		matrix = invFromMatrix * toMatrix;

		return true;
	}

	// 3D

	template <typename Real>
	AffineTransformation<3, Real> xShear3(
		const PASTEL_NO_DEDUCTION(Real)& yShear,
		const PASTEL_NO_DEDUCTION(Real)& zShear)
	{
		return AffineTransformation<3, Real>(
			Matrix<3, 3, Real>(
			1, yShear, zShear,
			0, 1,      0,
			0, 0,      1),
			Vector<3, Real>(0));
	}

	template <typename Real>
	AffineTransformation<3, Real> yShear3(
		const PASTEL_NO_DEDUCTION(Real)& xShear,
		const PASTEL_NO_DEDUCTION(Real)& zShear)
	{
		return AffineTransformation<3, Real>(
			Matrix<3, 3, Real>(
			1,      0, 0,
			xShear, 1, zShear,
			0,      0, 1),
			Vector<3, Real>(0));
	}

	template <typename Real>
	AffineTransformation<3, Real> zShear3(
		const PASTEL_NO_DEDUCTION(Real)& xShear,
		const PASTEL_NO_DEDUCTION(Real)& yShear)
	{
		return AffineTransformation<3, Real>(
			Matrix<3, 3, Real>(
			1,      0,      0,
			0,      1,      0,
			xShear, yShear, 1),
			Vector<3, Real>(0));
	}

	template <typename Real>
	AffineTransformation<3, Real> xRotation3(
		const PASTEL_NO_DEDUCTION(Real)& angle)
	{
		// Rotation from y to z

		const Real c(std::cos(angle));
		const Real s(std::sin(angle));

		return AffineTransformation<3, Real>(
			Matrix<3, 3, Real>(
			1,  0, 0,
			0,  c, s,
			0, -s, c),
			Vector<3, Real>(0));
	}

	template <typename Real>
	AffineTransformation<3, Real> yRotation3(
		const PASTEL_NO_DEDUCTION(Real)& angle)
	{
		// Rotation from z to x

		const Real c(std::cos(angle));
		const Real s(std::sin(angle));

		return AffineTransformation<3, Real>(
			Matrix<3, 3, Real>(
			c, 0,-s,
			0, 1, 0,
			s, 0, c),
			Vector<3, Real>(0));
	}

	template <typename Real>
	AffineTransformation<3, Real> zRotation3(
		const PASTEL_NO_DEDUCTION(Real)& angle)
	{
		// Rotation from x to y

		const Real c(std::cos(angle));
		const Real s(std::sin(angle));

		return AffineTransformation<3, Real>(
			Matrix<3, 3, Real>(
			c, s, 0,
			-s, c, 0,
			0, 0, 1),
			Vector<3, Real>(0, 0, 0));
	}

	template <typename Real>
	AffineTransformation<3, Real> translation3(
		const Vector<3, Real>& translation)
	{
		return AffineTransformation<3, Real>(
			identityMatrix<3, 3, Real>(),
			translation);
	}

	template <typename Real>
	bool affineTransform(
		const Point<3, Real>& aFrom, const Point<3, Real>& bFrom,
		const Point<3, Real>& cFrom, const Point<3, Real>& dFrom,
		const Point<3, Real>& aTo, const Point<3, Real>& bTo,
		const Point<3, Real>& cTo, const Point<3, Real>& dTo,
		AffineTransformation<3, Real>& matrix)
	{
		const Vector<3, Real> bFromDelta(
			bFrom - aFrom);
		const Vector<3, Real> cFromDelta(
			cFrom - aFrom);
		const Vector<3, Real> dFromDelta(
			dFrom - aFrom);

		const AffineTransformation<3, Real> fromMatrix(
			Matrix<3, 3, Real>(
			bFromDelta,
			cFromDelta,
			dFromDelta),
			asVector(aFrom));

		const AffineTransformation<3, Real> invFromMatrix(
			inverse(fromMatrix));

		const Vector<3, Real> bToDelta(
			bTo - aTo);
		const Vector<3, Real> cToDelta(
			cTo - aTo);
		const Vector<3, Real> dToDelta(
			dTo - aTo);

		const AffineTransformation<3, Real> toMatrix(
			Matrix<3, 3, Real>(
			bToDelta,
			cToDelta,
			dToDelta),
			asVector(aTo));

		matrix = invFromMatrix * toMatrix;

		return true;
	}

}

#endif
