// Documentation: math.txt

#ifndef PASTEL_NATIVEREAL_H
#define PASTEL_NATIVEREAL_H

namespace Pastel
{

	// Constants

	double infinity(double*);
	double nan(double*);

	template <typename Integer>
	Integer integerCast(double x);

	// Optimization functions

	double inverse(double x);
	bool zero(double x);
	bool negative(double x);
	bool positive(double x);

	// Constants

	float infinity(float*);
	float nan(float*);

	template <typename Integer>
	Integer integerCast(float x);

	// Optimization functions

	float inverse(float x);
	bool zero(float x);
	bool negative(float x);
	bool positive(float x);

}

#include "pastel/sys/nativereal.hpp"

#endif
