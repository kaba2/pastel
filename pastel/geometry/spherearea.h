#ifndef PASTELGEOMETRY_SPHEREAREA_H
#define PASTELGEOMETRY_SPHEREAREA_H

#include "pastel/geometry/sphere.h"

#include <boost/utility/enable_if.hpp>

namespace Pastel
{

	template <typename Real>
	Real area(const Sphere<2, Real>& sphere);

	template <typename Real>
	Real area(const Sphere<3, Real>& sphere);

	template <int N, typename Real>
	typename boost::enable_if_c<(N & 1) == 0 && (N > 3), Real>::type
		area(const Sphere<N, Real>& sphere);

	template <int N, typename Real>
	typename boost::enable_if_c<(N & 1) != 0 && (N > 3), Real>::type
		area(const Sphere<N, Real>& sphere);

}

#include "pastel/geometry/spherearea.hpp"

#endif
