// Description: Double as a real number
// Documentation: native_reals.txt

#ifndef PASTEL_DOUBLE_REAL_H
#define PASTEL_DOUBLE_REAL_H

#include "pastel/sys/real_concept.h"

#include <cmath>

namespace Pastel
{

	double inverse(double x);

	// We use std::floor and std::ceil as
	// implementations of those functions
	// in the Real concept.
	using std::floor;
	using std::ceil;

	double infinity(double*);
	double nan(double*);

	bool zero(double x);
	bool negative(double x);
	bool positive(double x);

	template <typename Integer>
	Integer integerCast(double x);

}

#include "pastel/sys/double_real.hpp"

#endif
