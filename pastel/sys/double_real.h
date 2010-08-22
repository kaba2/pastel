// Description: Float as a model of the Real concept
// Documentation: real.txt

#ifndef PASTEL_DOUBLE_REAL_H
#define PASTEL_DOUBLE_REAL_H

#include "pastel/sys/real_concept.h"

namespace Pastel
{

	double inverse(double x);

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
