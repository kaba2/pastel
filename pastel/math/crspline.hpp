#ifndef PASTEL_CRSPLINE_HPP
#define PASTEL_CRSPLINE_HPP

#include "pastel/math/crspline.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector_concept.h"

#include "pastel/math/hermite.h"

namespace Pastel
{

	template <typename PointType,
		typename Real>
		PointType crSpline(
		const PointType& previousPoint,
		const PointType& startPoint,
		const PointType& endPoint,
		const PointType& nextPoint,
		const Real& time)
	{
		return cubicHermite(
			startPoint,
			(endPoint - previousPoint) * 0.5,
			endPoint,
			(nextPoint - startPoint) * 0.5,
			time);
	}

	template <typename Vector_Iterator>
	typename boost::iterator_value<Vector_Iterator>::type crSpline(
		const RandomAccessRange<Vector_Iterator>& pointSet,
		const typename Vector_Scalar<typename boost::iterator_value<Vector_Iterator>::type>::type& time)
	{
	}

}

#endif
