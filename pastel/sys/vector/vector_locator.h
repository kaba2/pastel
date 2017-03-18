// Description: Vector locator
// Documentation: locators.txt

#ifndef PASTELSYS_VECTOR_LOCATOR_H
#define PASTELSYS_VECTOR_LOCATOR_H

#include "pastel/sys/locator/locator_concept.h"
#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/vector/vector.h"

namespace Pastel
{

	template <
		typename Real,
		integer N,
		Requires<
			Models<Vector<Real, N>, Point_Concept>
		> = 0
	>
	decltype(auto) pointLocator(Vector<Real, N>&& point)
	{
		return Default_Locator<Vector<Real, N>, Real, N>(point.n());
	}

	template <typename Real, integer N>
	IntegerConstant<N> pointN(const Vector<Real, N>* point);


	template <typename Real, int N>
	using Vector_Locator = Default_Locator<Vector<Real, N>, Real, N>;

}

#endif
