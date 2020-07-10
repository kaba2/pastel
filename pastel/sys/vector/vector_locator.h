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
		int N
	>
	requires Point_Concept_<Vector<Real, N>>
	decltype(auto) pointLocator(Vector<Real, N>&& point)
	{
		return Default_Locator<Vector<Real, N>, Real, N>(point.n());
	}

	template <typename Real, int N>
	IntegerConstant<N> pointN(const Vector<Real, N>* point);


	template <typename Real, int N>
	using Vector_Locator = Default_Locator<Vector<Real, N>, Real, N>;

	using Vector1_Locator = Vector_Locator<dreal, 1>;
	using Vector2_Locator = Vector_Locator<dreal, 2>;
	using Vector3_Locator = Vector_Locator<dreal, 3>;
	using Vector4_Locator = Vector_Locator<dreal, 4>;

}

#endif
