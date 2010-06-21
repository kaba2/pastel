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
			inverse(that.matrix()));

		const AffineTransformation<Real, N> result(
			transformInverse,
			evaluate(-that.translation()) * transformInverse);

		return result;
	}

	template <typename Real, int N>
	AffineTransformation<Real, N> orthogonalInverse(
		const AffineTransformation<Real, N>& that)
	{
		const Matrix<Real, N, N> transformInverse(
			transpose(that.matrix()));

		const AffineTransformation<Real, N> result(
			transformInverse,
			evaluate(-that.translation()) * transformInverse);

		return result;
	}

	// 2D

	template <typename Real>
	AffineTransformation<Real, 2> xShear2(
		const PASTEL_NO_DEDUCTION(Real)& yShear)
	{
		const AffineTransformation<Real, 2> result(
			Matrix<Real, 2, 2>(
			1, yShear,
			0,      1),
			Vector<Real, 2>(0));

		return result;
	}

	template <typename Real>
	AffineTransformation<Real, 2> yShear2(
		const PASTEL_NO_DEDUCTION(Real)& xShear)
	{
		const AffineTransformation<Real, 2> result(
			Matrix<Real, 2, 2>(
			1,      0,
			xShear, 1),
			Vector<Real, 2>(0));

		return result;
	}

	template <typename Real>
	AffineTransformation<Real, 2> rotation2(
		const PASTEL_NO_DEDUCTION(Real)& angle)
	{
		const Real c(std::cos(angle));
		const Real s(std::sin(angle));

		// Rotation from x to y

		const AffineTransformation<Real, 2> result(
			Matrix<Real, 2, 2>(
			c, s,
			-s, c),
			Vector<Real, 2>(0));

		return result;
	}

	template <typename Real>
	AffineTransformation<Real, 2> translation2(
		const Vector<Real, 2>& translation)
	{
		const AffineTransformation<Real, 2> result(
			Matrix<Real, 2, 2>(
			1, 0,
			0, 1),
			translation);

		return result;
	}

	template <typename Real, int N>
	AffineTransformation<Real, N> lsAffine(
		const std::vector<Vector<Real, N> >& from,
		const std::vector<Vector<Real, N> >& to)
	{
		PASTEL_STATIC_ASSERT(N > 0);
		return Pastel::lsAffine(
			N, from, to);
	}

	template <typename Real, int N>
	AffineTransformation<Real, N> lsAffine(
		integer dimension,
		const std::vector<Vector<Real, N> >& from,
		const std::vector<Vector<Real, N> >& to)
	{
		ENSURE2(from.size() == to.size(), from.size(), to.size());

		// Let P = [p_1, ..., p_n] in R^{m x n}
		// and R = [r_1, ..., r_n] in R^{m x n}
		// represent sets of points in R^m.
		//
		// f : R^m -> R^m : f(x) = Ax + b
		//
		// The affine transformation that maps P to R, while
		// minimizing least-squares error is given by:
		//
		// A = (PR^T)(PP^T)^-1
		// b = r' - Ap'
		//
		// where 
		// p' = (1 / n) sum_{i = 1}^n p_i
		// r' = (1 / n) sum_{i = 1}^n r_i

		const integer points = from.size();

		if (points == 0)
		{
			return AffineTransformation<Real, N>(dimension);
		}

		if (points == 1)
		{
			return AffineTransformation<Real, N>(
				dimension,
				identityMatrix<Real, N, N>(dimension, dimension),
				to.front() - from.front());
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

		// A = (PR^T)(PP^T)^-1
		// =>
		// A^T = (PP^T)^-1 (RP^T)

		// Compute RP^T and PP^T.

		Matrix<Real, N, N> rpt(dimension, dimension);
		Matrix<Real, N, N> ppt(dimension, dimension);
		for (integer i = 0;i < points;++i)
		{
			ppt += outerProduct(from[i] - fromCentroid);
			rpt += outerProduct(
				from[i] - fromCentroid, 
				to[i] - toCentroid);
		}

		Matrix<Real, N, N> at = inverse(ppt) * rpt;

		return AffineTransformation<Real, N>(
			std::move(at), toCentroid - fromCentroid * at);
	}

	template <typename Real, int N>
	AffineTransformation<Real, N> toAffine(
		const ConformalAffine2D<Real, N>& that)
	{
		PASTEL_STATIC_ASSERT(N == 2 || N == Dynamic);

		const Real& scaling = that.scaling();
		const Real& ccwRotation = that.rotation();

		Matrix<Real, N, N> transform(2, 2);
		transform |= 
			scaling * std::cos(ccwRotation), scaling * std::sin(ccwRotation),
			-scaling * std::sin(ccwRotation), scaling * std::cos(ccwRotation);

		return AffineTransformation<Real, 2>(transform,	that.translation());
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
		const AffineTransformation<Real, 3> result(
			Matrix<Real, 3, 3>(
			1, yShear, zShear,
			0, 1,      0,
			0, 0,      1),
			Vector<Real, 3>(0));

		return result;
	}

	template <typename Real>
	AffineTransformation<Real, 3> yShear3(
		const PASTEL_NO_DEDUCTION(Real)& xShear,
		const PASTEL_NO_DEDUCTION(Real)& zShear)
	{
		const AffineTransformation<Real, 3> result(
			Matrix<Real, 3, 3>(
			1,      0, 0,
			xShear, 1, zShear,
			0,      0, 1),
			Vector<Real, 3>(0));

		return result;
	}

	template <typename Real>
	AffineTransformation<Real, 3> zShear3(
		const PASTEL_NO_DEDUCTION(Real)& xShear,
		const PASTEL_NO_DEDUCTION(Real)& yShear)
	{
		const AffineTransformation<Real, 3> result(
			Matrix<Real, 3, 3>(
			1,      0,      0,
			0,      1,      0,
			xShear, yShear, 1),
			Vector<Real, 3>(0));

		return result;
	}

	template <typename Real>
	AffineTransformation<Real, 3> xRotation3(
		const PASTEL_NO_DEDUCTION(Real)& angle)
	{
		// Rotation from y to z

		const Real c(std::cos(angle));
		const Real s(std::sin(angle));

		const AffineTransformation<Real, 3> result(
			Matrix<Real, 3, 3>(
			1,  0, 0,
			0,  c, s,
			0, -s, c),
			Vector<Real, 3>(0));

		return result;
	}

	template <typename Real>
	AffineTransformation<Real, 3> yRotation3(
		const PASTEL_NO_DEDUCTION(Real)& angle)
	{
		// Rotation from z to x

		const Real c(std::cos(angle));
		const Real s(std::sin(angle));

		const AffineTransformation<Real, 3> result(
			Matrix<Real, 3, 3>(
			c, 0,-s,
			0, 1, 0,
			s, 0, c),
			Vector<Real, 3>(0));

		return result;
	}

	template <typename Real>
	AffineTransformation<Real, 3> zRotation3(
		const PASTEL_NO_DEDUCTION(Real)& angle)
	{
		// Rotation from x to y

		const Real c(std::cos(angle));
		const Real s(std::sin(angle));

		AffineTransformation<Real, 3> result(
			Matrix<Real, 3, 3>(
			c, s, 0,
			-s, c, 0,
			0, 0, 1),
			Vector<Real, 3>(0, 0, 0));

		return result;
	}

	template <typename Real>
	AffineTransformation<Real, 3> scaling3(
		const Vector<Real, 3>& scaling)
	{
		AffineTransformation<Real, 3> result(
			identityMatrix<Real, 3, 3>());

		for (integer i = 0;i < 3;++i)
		{
			result.matrix()(i, i) = scaling[i];
		}

		return result;
	}

	template <typename Real>
	AffineTransformation<Real, 3> translation3(
		const Vector<Real, 3>& translation)
	{
		const AffineTransformation<Real, 3> result(
			identityMatrix<Real, 3, 3>(),
			translation);

		return result;
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
