// Description: Float as a model of the Real concept
// Documentation: real.txt

#ifndef PASTEL_FLOAT_REAL_H
#define PASTEL_FLOAT_REAL_H

#include "pastel/sys/real_concept.h"

namespace Pastel
{

	float inverse(float x);

	float infinity(float*);
	float nan(float*);

	bool zero(float x);
	bool negative(float x);
	bool positive(float x);

	template <typename Integer>
	Integer integerCast(float x);

}

#include "pastel/sys/float_real.hpp"

#endif
