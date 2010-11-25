// Description: Float as a real number
// Documentation: native_reals.txt

#ifndef PASTEL_FLOAT_REAL_H
#define PASTEL_FLOAT_REAL_H

#include "pastel/sys/real_concept.h"

#include <cmath>

namespace Pastel
{

	float inverse(float x);

	// We use std::floor and std::ceil as
	// implementations of those functions
	// in the Real concept.
	using std::floor;
	using std::ceil;

	bool zero(float x);
	bool negative(float x);
	bool positive(float x);

	template <typename Integer>
	Integer integerCast(float x);

}

#include "pastel/sys/float_real.hpp"

#endif
