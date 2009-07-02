#include "pastel/geometry/projection.h"

#include "pastel/math/matrix_tools.h"

namespace Pastel
{

	PASTELGEOMETRY void setPerspectiveProjection(
		const AlignedBox2& window,
		const real& zMin,
		const real& zMax,
		Matrix4& matrix)
	{
		/*
		To find the transformation to the
		space [-1, 1]^3 we proceed as follows.

		First, the space has to be sheared in
		the x-direction along z such that the
		ray from the origin to window center
		has zero change in x:

		(xMax + xMin) / 2 + k_x zMin = 0
		=>
		k_x zMin = -(xMax + xMin) / 2
		=>
		k_x = -(xMax + xMin) / (2 zMin)

		Similarly for y-direction:

		k_y = -(yMax + yMin) / (2 zMin)

		These two shears can be combined
		into the same matrix:

			  [1,     0, 0, 0]
		V_1 = [0,     1, 0, 0]
			  [k_x, k_y, 1, 0]
			  [0,     0, 0, 1]

		Next, we want to scale the view window
		to the [-1, 1]^2 range. Let
		s_x = 2 / (xMax - xMin)
		s_y = 2 / (yMax - yMin)
		Then that matrix would be:

			[s_x,   0, 0, 0]
		S = [  0, s_y, 0, 0]
			[  0,   0, 1, 0]
			[  0,   0, 0, 1]

		Combining with the previous matrix we have:

		V_2 = K S = [    s_x,       0, 0, 0]
					[      0,     s_y, 0, 0]
					[k_x s_x, k_y s_y, 1, 0]
					[      0,       0, 0, 1]

		We want the homogeneous screen z, z_h,
		to be related to the world z, z_w by:
		z_h = a / z_w + b
		(since this gives a quantity that can be
		used for hidden surface removal and which
		can be linearly interpolated in screen
		space without perspective correction).

		In this mapping, we want that
		zMin maps to -1 and zMax maps to 1:
		a / zMin + b = -1
		a / zMax + b = 1
		=>
		a (1 / zMin - 1 / zMax) = -2
		=>
		a ((zMax - zMin) / (zMin zMax)) = -2
		=>
		a = (-2 zMin zMax) / (zMax - zMin)
		=>
		b = 1 + 2 zMin / (zMax - zMin)
		  = (zMax + zMin) / (zMax - zMin)

		However, we can't model the z transformation
		with just affine transformations because
		of the division. So first we must multiply
		the points with the z, we get:

		V_3 = [    s_x zMin,            0,      0, 0]
			  [           0,     s_y zMin,      0, 0]
			  [k_x s_x zMin, k_y s_y zMin,      1, 1]
			  [           0,            0,      0, 0]

		Now we can apply the z transform:

		V_4 = [    s_x zMin,            0,      0, 0]
			  [           0,     s_y zMin,      0, 0]
			  [k_x s_x zMin, k_y s_y zMin,      b, 1]
			  [           0,            0,      a, 0]

		Which expands into:

			= [      (2 zMin) / (xMax - xMin),                              0,                              0, 0]
			  [                             0,       (2 zMin) / (yMax - yMin),                              0, 0]
			  [-(xMax + xMin) / (xMax - xMin), -(yMax + yMin) / (yMax - yMin),  (zMax + zMin) / (zMax - zMin), 1]
			  [                             0,                              0, (-2 zMin zMax) / (zMax - zMin), 0]
		*/

		ENSURE1(zMin > 0, zMin);
		ENSURE1(zMax > 0, zMax);
		ENSURE2(zMin < zMax, zMax, zMin);

		const real& xMin = window.min().x();
		const real& xMax = window.max().x();
		const real& yMin = window.min().y();
		const real& yMax = window.max().y();

		const real m11 = (2 * zMin) / (xMax - xMin);
		const real m22 = (2 * zMin) / (yMax - yMin);
		const real m33 = (zMax + zMin) / (zMax - zMin);
		const real m31 = -(xMax + xMin) / (xMax - xMin);
		const real m32 = -(yMax + yMin) / (yMax - yMin);
		const real m43 = -(2 * zMax * zMin) / (zMax - zMin);

		matrix |=
			m11,   0,    0, 0,
			0,   m22,    0, 0,
			m31, m32,  m33, 1,
			0,   0,    m43, 0;
	}

	PASTELGEOMETRY void setOrthogonalProjection(
		const AlignedBox2& window,
		const real& zMin,
		const real& zMax,
		Matrix4& matrix)
	{
		ENSURE2(zMin < zMax, zMax, zMin);

		const real& xMin = window.min().x();
		const real& xMax = window.max().x();
		const real& yMin = window.min().y();
		const real& yMax = window.max().y();

		const real m11 = 2 / (xMax - xMin);
		const real m22 = 2 / (yMax - yMin);
		const real m33 = 2 / (zMax - zMin);
		const real m41 = -(xMax + xMin) / (xMax - xMin);
		const real m42 = -(yMax + yMin) / (yMax - yMin);
		const real m43 = -(zMax + zMin) / (zMax - zMin);

		matrix |=
			m11,   0,    0, 0,
			0,   m22,    0, 0,
			0,     0,  m33, 0,
			m41, m42,  m43, 1;
	}

	PASTELGEOMETRY Matrix3 projectiveTransformation(
		const Tuple<4, Point2>& from,
		const Tuple<4, Point2>& to)
	{
		/*
		Using homogeneous coordinates, a projective 2d transformation is
		given by:

		[u, v, 1] [A D G] = [x, y, w]
				  [B E H]
				  [C F 1]

		That is:

		x = uA + vB + C
		y = uD + vE + F
		w = uG + vH + 1

		The projected points are then given by:

		x' = x / w = (uA + vB + C) / (uG + vH + 1)
		y' = y / w = (uD + vE + F) / (uG + vH + 1)

		If he have four transformation point pairs, then
		we can deduce the 8 variables in the transformation.
		We get 4 linear equations of the form:

		x' = (uA + vB + C) / (uG + vH + 1)
		<=>
		uA + vB + C - u x' G - v x' H = x'

		And 4 linear equations of the form:

		y' = (uD + vE + F) / (uG + vH + 1)
		<=>
		uD + vE + F - u y' G - v y' H = y'

		These can be arranged in a matrix form as:

		[u0, v0, 1,  0,  0, 0, -u0 x'0, -v0 x'0] [A] = [x'0]
		[u1, v1, 1,  0,  0, 0, -u1 x'1, -v1 x'1] [B] = [x'1]
		[u2, v2, 1,  0,  0, 0, -u2 x'2, -v2 x'2] [C] = [x'2]
		[u3, v3, 1,  0,  0, 0, -u3 x'3, -v3 x'3] [D] = [x'3]
		[ 0,  0, 0, u0, v0, 1, -u0 y'0, -v0 y'0] [E] = [y'0]
		[ 0,  0, 0, u1, v1, 1, -u1 y'1, -v1 y'1] [F] = [y'1]
		[ 0,  0, 0, u2, v2, 1, -u2 y'2, -v2 y'2] [G] = [y'2]
		[ 0,  0, 0, u3, v3, 1, -u3 y'3, -v3 y'3] [H] = [y'3]

		Which can be solved using standard methods.
		*/

		Matrix<8, 8, real> matrix;
		for (integer i = 0;i < 4;++i)
		{
			matrix(0, i) = from[i][0];
			matrix(1, i) = from[i][1];
			matrix(2, i) = 1;
			matrix(3, i) = 0;
			matrix(4, i) = 0;
			matrix(5, i) = 0;
			matrix(6, i) = -from[i][0] * to[i][0];
			matrix(7, i) = -from[i][1] * to[i][0];
		}

		for (integer i = 0;i < 4;++i)
		{
			matrix(0, 4 + i) = 0;
			matrix(1, 4 + i) = 0;
			matrix(2, 4 + i) = 0;
			matrix(3, 4 + i) = from[i][0];
			matrix(4, 4 + i) = from[i][1];
			matrix(5, 4 + i) = 1;
			matrix(6, 4 + i) = -from[i][0] * to[i][1];
			matrix(7, 4 + i) = -from[i][1] * to[i][1];
		}

		Vector<8, real> target;
		target[0] = to[0][0];
		target[1] = to[1][0];
		target[2] = to[2][0];
		target[3] = to[3][0];
		target[4] = to[0][1];
		target[5] = to[1][1];
		target[6] = to[2][1];
		target[7] = to[3][1];

		const Vector<8, real> solution = solveLinear(matrix, target);

		return Matrix3(
			solution[0], solution[3], solution[6],
			solution[1], solution[4], solution[7],
			solution[2], solution[5], 1);
	}

}
