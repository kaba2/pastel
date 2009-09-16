#ifndef PASTEL_AFFINETRANSFORMATION_TOOLS_HPP
#define PASTEL_AFFINETRANSFORMATION_TOOLS_HPP

#include "pastel/math/affinetransformation_tools.h"
#include "pastel/math/matrix_tools.h"

#include "pastel/sys/vector_tools.h"
#include "pastel/sys/math_functions.h"
#include "pastel/sys/mytypes.h"

namespace Pastel
{

	template <typename Real, int N>
	AffineTransformation<Real, N> inverse(
		const AffineTransformation<Real, N>& that)
	{
		const Matrix<Real, N, N> transformInverse(
			inverse(that.transformation()));

		return AffineTransformation<Real, N>(
			transformInverse,
			evaluate(-that.translation()) * transformInverse);
	}

	template <typename Real, int N>
	AffineTransformation<Real, N> orthogonalInverse(
		const AffineTransformation<Real, N>& that)
	{
		const Matrix<Real, N, N> transformInverse(
			transpose(that.transformation()));

		return AffineTransformation<Real, N>(
			transformInverse,
			evaluate(-that.translation()) * transformInverse);
	}

	// 2D

	template <typename Real>
	AffineTransformation<Real, 2> xShear2(
		const PASTEL_NO_DEDUCTION(Real)& yShear)
	{
		return AffineTransformation<Real, 2>(
			Matrix<Real, 2, 2>(
			1, yShear,
			0,      1),
			Vector<Real, 2>(0));
	}

	template <typename Real>
	AffineTransformation<Real, 2> yShear2(
		const PASTEL_NO_DEDUCTION(Real)& xShear)
	{
		return AffineTransformation<Real, 2>(
			Matrix<Real, 2, 2>(
			1,      0,
			xShear, 1),
			Vector<Real, 2>(0));
	}

	template <typename Real>
	AffineTransformation<Real, 2> rotation2(
		const PASTEL_NO_DEDUCTION(Real)& angle)
	{
		const Real c(std::cos(angle));
		const Real s(std::sin(angle));

		// Rotation from x to y

		return AffineTransformation<Real, 2>(
			Matrix<Real, 2, 2>(
			c, s,
			-s, c),
			Vector<Real, 2>(0));
	}

	template <typename Real>
	AffineTransformation<Real, 2> translation2(
		const Vector<Real, 2>& translation)
	{
		return AffineTransformation<Real, 2>(
			Matrix<Real, 2, 2>(
			1, 0,
			0, 1),
			translation);
	}

	template <typename Real, int N>
	AffineTransformation<Real, N> leastSquaresAffineTransformation(
		const std::vector<Vector<Real, N> >& from,
		const std::vector<Vector<Real, N> >& to)
	{
		BOOST_STATIC_ASSERT(N > 0);
		return Pastel::leastSquaresAffineTransformation(
			N, from, to);
	}

	template <typename Real, int N>
	AffineTransformation<Real, N> leastSquaresAffineTransformation(
		integer dimension,
		const std::vector<Vector<Real, N> >& from,
		const std::vector<Vector<Real, N> >& to)
	{
		ENSURE2(from.size() == to.size(), from.size(), to.size());

		const integer points = from.size();

		if (points == 0)
		{
			return AffineTransformation<Real, N>(dimension);
		}

		// Compute centroids

		Vector<Real, N> fromCentroid(ofDimension(dimension));
		Vector<Real, N> toCentroid(ofDimension(dimension));

		for (integer i = 0;i < points;++i)
		{
			fromCentroid += from[i];
			toCentroid += to[i];
		}

		fromCentroid /= points;
		toCentroid /= points;

		Matrix<Real, N, N> c(dimension, dimension);
		for (integer i = 0;i < points;++i)
		{
			const Vector<Real, N> v = from[i] - fromCentroid;
			for (integer k = 0;k < dimension;++k)
			{
				c[k] += v * v[k];
			}
		}

		Matrix<Real, N, N> d(dimension, dimension);
		for (integer i = 0;i < points;++i)
		{
			const Vector<Real, N> v = to[i] - toCentroid;
			const Vector<Real, N> w = from[i] - fromCentroid;
			for (integer k = 0;k < dimension;++k)
			{
				d[k] += v * w[k];
			}
		}

		const Matrix<Real, N, N> a = inverse(c) * d;

		return AffineTransformation<Real, 2>(
			a, toCentroid - fromCentroid * a);
	}

	template <typename Real>
	Tuple<Real, 4> similarityTransformation(
		const std::vector<Vector<Real, 2> >& from,
		const std::vector<Vector<Real, 2> >& to)
	{
		ENSURE2(from.size() == to.size(), from.size(), to.size());
		typedef std::vector<Vector<Real, 2> >::const_iterator InputIterator;

		InputIterator fromIter = from.begin();
		InputIterator fromEnd = from.end();
		InputIterator toIter = to.begin();
		InputIterator toEnd = to.end();

		Vector<Real, 2> sumFrom;
		Vector<Real, 2> sumTo;
		Real sumSquareFrom = 0;
		Real dotSum = 0;
		Real crossDotSum = 0;

		integer points = 0;

		while(fromIter != fromEnd)
		{
			const Vector<Real, 2>& fromVector = *fromIter;
			const Vector<Real, 2>& toVector = *toIter;

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

		const Vector<Real, 2> translation(
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

		return Tuple<Real, 4>(scale, angle, translation[0], translation[1]);
	}

	template <typename Real>
	Tuple<Real, 4> similarityTransformation(
		const Vector<Real, 2>& aFrom, const Vector<Real, 2>& bFrom,
		const Vector<Real, 2>& aTo, const Vector<Real, 2>& bTo)
	{
		const Vector<Real, 2> fromDelta = bFrom - aFrom;
		const Vector<Real, 2> toDelta = bTo - aTo;

		const Real fromNorm = norm(fromDelta);
		const Real toNorm = norm(toDelta);

		const Real scaling = toNorm / fromNorm;

		const Real ccwRotation = ccwAngle(
			fromDelta, toDelta,
			fromNorm, toNorm);

		const AffineTransformation<Real, 2> scaleRotate =
			Pastel::similarityTransformation(
			scaling, ccwRotation, Vector<Real, 2>(0));

		const Vector<Real, 2> translation = aTo - transformPoint(aFrom, scaleRotate);

		return Tuple<Real, 4>(scaling, ccwRotation, translation[0], translation[1]);
	}

	template <typename Real>
	AffineTransformation<Real, 2> similarityTransformation(
		const PASTEL_NO_DEDUCTION(Real)& scaling,
		const PASTEL_NO_DEDUCTION(Real)& ccwRotation,
		const Vector<Real, 2>& translation)
	{
		return AffineTransformation<Real, 2>(
			Matrix<Real, 2, 2>(
			scaling * std::cos(ccwRotation), scaling * std::sin(ccwRotation),
			-scaling * std::sin(ccwRotation), scaling * std::cos(ccwRotation)),
			translation);
	}

	template <typename Real>
	AffineTransformation<Real, 2> similarityTransformation(
		const Tuple<Real, 4>& parameter)
	{
		return Pastel::similarityTransformation(parameter[0], parameter[1],
			Vector<Real, 2>(parameter[2], parameter[3]));
	}

	template <typename Real>
	bool affineTransform(
		const Vector<Real, 2>& aFrom, const Vector<Real, 2>& bFrom,
		const Vector<Real, 2>& cFrom,
		const Vector<Real, 2>& aTo, const Vector<Real, 2>& bTo,
		const Vector<Real, 2>& cTo,
		AffineTransformation<Real, 2>& matrix)
	{
		const Vector<Real, 2> bFromDelta(
			bFrom - aFrom);
		const Vector<Real, 2> cFromDelta(
			cFrom - aFrom);

		const AffineTransformation<Real, 2> fromMatrix(
			Matrix<Real, 2, 2>(
			bFromDelta,
			cFromDelta),
			aFrom);

		const AffineTransformation<Real, 2> invFromMatrix(
			inverse(fromMatrix));

		const Vector<Real, 2> bToDelta(
			bTo - aTo);
		const Vector<Real, 2> cToDelta(
			cTo - aTo);

		const AffineTransformation<Real, 2> toMatrix(
			Matrix<Real, 2, 2>(
			bToDelta,
			cToDelta),
			aTo);

		matrix = invFromMatrix * toMatrix;

		return true;
	}

	template <typename Real>
	bool affineTransform(
		const Vector<Real, 3>& aFrom, const Vector<Real, 3>& bFrom,
		const Vector<Real, 3>& cFrom,
		const Vector<Real, 3>& aTo, const Vector<Real, 3>& bTo,
		const Vector<Real, 3>& cTo,
		AffineTransformation<Real, 3>& matrix)
	{
		const Vector<Real, 3> bFromDelta(
			bFrom - aFrom);
		const Vector<Real, 3> cFromDelta(
			cFrom - aFrom);
		const Vector<Real, 3> normalFrom(
			cross(bFromDelta, cFromDelta));

		const AffineTransformation<Real, 3> fromMatrix(
			Matrix<Real, 3, 3>(
			bFromDelta,
			cFromDelta,
			normalFrom),
			aFrom);

		const AffineTransformation<Real, 3> invFromMatrix(
			inverse(fromMatrix));

		const Vector<Real, 3> bToDelta(
			bTo - aTo);
		const Vector<Real, 3> cToDelta(
			cTo - aTo);
		const Vector<Real, 3> normalTo(
			cross(bToDelta, cToDelta));

		const AffineTransformation<Real, 3> toMatrix(
			Matrix<Real, 3, 3>(
			bToDelta,
			cToDelta,
			normalTo),
			aTo);

		matrix = invFromMatrix * toMatrix;

		return true;
	}

	// 3D

	template <typename Real>
	AffineTransformation<Real, 3> xShear3(
		const PASTEL_NO_DEDUCTION(Real)& yShear,
		const PASTEL_NO_DEDUCTION(Real)& zShear)
	{
		return AffineTransformation<Real, 3>(
			Matrix<Real, 3, 3>(
			1, yShear, zShear,
			0, 1,      0,
			0, 0,      1),
			Vector<Real, 3>(0));
	}

	template <typename Real>
	AffineTransformation<Real, 3> yShear3(
		const PASTEL_NO_DEDUCTION(Real)& xShear,
		const PASTEL_NO_DEDUCTION(Real)& zShear)
	{
		return AffineTransformation<Real, 3>(
			Matrix<Real, 3, 3>(
			1,      0, 0,
			xShear, 1, zShear,
			0,      0, 1),
			Vector<Real, 3>(0));
	}

	template <typename Real>
	AffineTransformation<Real, 3> zShear3(
		const PASTEL_NO_DEDUCTION(Real)& xShear,
		const PASTEL_NO_DEDUCTION(Real)& yShear)
	{
		return AffineTransformation<Real, 3>(
			Matrix<Real, 3, 3>(
			1,      0,      0,
			0,      1,      0,
			xShear, yShear, 1),
			Vector<Real, 3>(0));
	}

	template <typename Real>
	AffineTransformation<Real, 3> xRotation3(
		const PASTEL_NO_DEDUCTION(Real)& angle)
	{
		// Rotation from y to z

		const Real c(std::cos(angle));
		const Real s(std::sin(angle));

		return AffineTransformation<Real, 3>(
			Matrix<Real, 3, 3>(
			1,  0, 0,
			0,  c, s,
			0, -s, c),
			Vector<Real, 3>(0));
	}

	template <typename Real>
	AffineTransformation<Real, 3> yRotation3(
		const PASTEL_NO_DEDUCTION(Real)& angle)
	{
		// Rotation from z to x

		const Real c(std::cos(angle));
		const Real s(std::sin(angle));

		return AffineTransformation<Real, 3>(
			Matrix<Real, 3, 3>(
			c, 0,-s,
			0, 1, 0,
			s, 0, c),
			Vector<Real, 3>(0));
	}

	template <typename Real>
	AffineTransformation<Real, 3> zRotation3(
		const PASTEL_NO_DEDUCTION(Real)& angle)
	{
		// Rotation from x to y

		const Real c(std::cos(angle));
		const Real s(std::sin(angle));

		return AffineTransformation<Real, 3>(
			Matrix<Real, 3, 3>(
			c, s, 0,
			-s, c, 0,
			0, 0, 1),
			Vector<Real, 3>(0, 0, 0));
	}

	template <typename Real>
	AffineTransformation<Real, 3> translation3(
		const Vector<Real, 3>& translation)
	{
		return AffineTransformation<Real, 3>(
			identityMatrix<Real, 3, 3>(),
			translation);
	}

	template <typename Real>
	bool affineTransform(
		const Vector<Real, 3>& aFrom, const Vector<Real, 3>& bFrom,
		const Vector<Real, 3>& cFrom, const Vector<Real, 3>& dFrom,
		const Vector<Real, 3>& aTo, const Vector<Real, 3>& bTo,
		const Vector<Real, 3>& cTo, const Vector<Real, 3>& dTo,
		AffineTransformation<Real, 3>& matrix)
	{
		const Vector<Real, 3> bFromDelta(
			bFrom - aFrom);
		const Vector<Real, 3> cFromDelta(
			cFrom - aFrom);
		const Vector<Real, 3> dFromDelta(
			dFrom - aFrom);

		const AffineTransformation<Real, 3> fromMatrix(
			Matrix<Real, 3, 3>(
			bFromDelta,
			cFromDelta,
			dFromDelta),
			aFrom);

		const AffineTransformation<Real, 3> invFromMatrix(
			inverse(fromMatrix));

		const Vector<Real, 3> bToDelta(
			bTo - aTo);
		const Vector<Real, 3> cToDelta(
			cTo - aTo);
		const Vector<Real, 3> dToDelta(
			dTo - aTo);

		const AffineTransformation<Real, 3> toMatrix(
			Matrix<Real, 3, 3>(
			bToDelta,
			cToDelta,
			dToDelta),
			aTo);

		matrix = invFromMatrix * toMatrix;

		return true;
	}

}

#endif
