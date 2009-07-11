/*!
\file
\brief Functions for sampling uniformly from different regions.
*/

#ifndef PASTEL_UNIFORM_SAMPLING_H
#define PASTEL_UNIFORM_SAMPLING_H

#include "pastel/sys/vector.h"
#include "pastel/sys/point.h"

#include <boost/utility/enable_if.hpp>

namespace Pastel
{

	//! Uniformly samples a point from an annulus
	/*!
	Preconditions:
	'uv' is in [0, 1]^N.
	minRadius >= 0
	maxRadius >= 0
	minRadius < maxRadius

	The annulus is considered as the set:
	{x | minRadius <= norm(x) <= minRadius}

	If 'uv' are random variables uniformly distributed
	on the square, then the returned points are random
	variables uniformly distributed on the annulus.
	*/

	template <int N, typename Real>
	typename boost::enable_if_c<N == 1, Vector<N, Real> >::type
		uniformlySampleAnnulus(
		const Vector<N, Real>& uv,
		const PASTEL_NO_DEDUCTION(Real)& minRadius,
		const PASTEL_NO_DEDUCTION(Real)& maxRadius);

	template <int N, typename Real>
	typename boost::enable_if_c<N == 2, Vector<N, Real> >::type
		uniformlySampleAnnulus(
		const Vector<N, Real>& uv,
		const PASTEL_NO_DEDUCTION(Real)& minRadius,
		const PASTEL_NO_DEDUCTION(Real)& maxRadius);

	template <int N, typename Real>
	typename boost::enable_if_c<N == 3, Vector<N, Real> >::type
		uniformlySampleAnnulus(
		const Vector<N, Real>& uv,
		const PASTEL_NO_DEDUCTION(Real)& minRadius,
		const PASTEL_NO_DEDUCTION(Real)& maxRadius);

	//! Uniformly samples a point from a simplex.
	/*!
	Preconditions:
	'uv' is in [0, 1]^N.

	The simplex is considered as the set
	{x | sum(x) <= 1, allGreaterEqual(x, 0)}

	If 'uv' are random variables uniformly distributed
	on the cube, then the returned points are random
	variables uniformly distributed on the simplex.
	*/

	template <int N, typename Real>
	typename boost::enable_if_c<N == 1, Vector<N, Real> >::type
		uniformlySampleSimplex(
		const Vector<N, Real>& uv);

	template <int N, typename Real>
	typename boost::enable_if_c<N == 2, Vector<N, Real> >::type
		uniformlySampleSimplex(
		const Vector<N, Real>& uv);

	template <int N, typename Real>
	typename boost::enable_if_c<(N >= 3 || N == Dynamic), Vector<N, Real> >::type
		uniformlySampleSimplex(
		const Vector<N, Real>& uv);

	//! Uniformly samples a point from a ball.
	/*!
	Preconditions:
	'uv' is in [0, 1]^N.

	Returns:
	A point from the set {x in R^N | norm(x) <= 1}.

	If 'uv' is a uniformly distributed random variable on the cube,
	so is the returned value on the ball.
	*/

	template <int N, typename Real>
	typename boost::enable_if_c<N == 1, Vector<N, Real> >::type
		uniformlySampleBall(
		const Vector<N, Real>& uv);

	template <int N, typename Real>
	typename boost::enable_if_c<N == 2, Vector<N, Real> >::type
		uniformlySampleBall(
		const Vector<N, Real>& uv);

	template <int N, typename Real>
	typename boost::enable_if_c<N == 3, Vector<N, Real> >::type
		uniformlySampleBall(
		const Vector<N, Real>& uv);

	// TODO: provide functions for 4d and up?

	//! Uniformly samples a point from a sphere.
	/*!
	Preconditions:
	'uv' is in [0, 1]^N

	Returns:
	A point from the set {x in R^(N + 1) | norm(x) = 1}.

	If 'uv' is a uniformly distributed random variable on the cube,
	so is the returned value on the sphere.
	*/

	template <int N, typename Real>
	typename boost::enable_if_c<N == 1, Vector<PASTEL_ADD_N(N, 1), Real> >::type
		uniformlySampleSphere(
		const Vector<N, Real>& uv);

	template <int N, typename Real>
	typename boost::enable_if_c<N == 2, Vector<PASTEL_ADD_N(N, 1), Real> >::type
		uniformlySampleSphere(
		const Vector<N, Real>& uv);

	// TODO: provide functions for 4d and up?

	//! Uniformly samples a point from a hemisphere.
	/*!
	Preconditions:
	'uv' is in [0, 1]^N.

	Returns:
	A point from the set {x in R^(N + 1) | norm(x) <= 1, x[N] >= 0}.

	If 'uv' is a uniformly distributed random variable on the cube,
	so is the returned value on the hemisphere.
	*/

	template <int N, typename Real>
	typename boost::enable_if_c<N == 1, Vector<PASTEL_ADD_N(N, 1), Real> >::type
		uniformlySampleHemisphere(
		const Vector<N, Real>& uv);

	template <int N, typename Real>
	typename boost::enable_if_c<N == 2, Vector<PASTEL_ADD_N(N, 1), Real> >::type
		uniformlySampleHemisphere(
		const Vector<N, Real>& uv);

	// TODO: provide functions for 4d and up?

	//! Samples a cosine-weighted point from a hemisphere.
	/*!
	Preconditions:
	'uv' is in [0, 1]^N.

	The hemisphere is considered as the set
	{x in R^(N+1) | norm(x) = 1, x[N] >= 0}

	If 'uv' are random variables uniformly distributed
	on the square, then the returned points are random
	variables cosine-distributed on the hemisphere.
	*/

	template <int N, typename Real>
	Vector<PASTEL_ADD_N(N, 1), Real> cosineSampleHemisphere(
		const Vector<N, Real>& uv);

}

#include "pastel/math/uniform_sampling.hpp"

#endif
