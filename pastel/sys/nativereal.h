#ifndef PASTELSYS_NATIVEREAL_H
#define PASTELSYS_NATIVEREAL_H

namespace Pastel
{

	// Constants

	double infinity(double*);
	double nan(double*);

	template <typename Integer>
	Integer integerCast(double x);

	// Optimization functions

	void multiplyByPowerOf2(double& x, double power);
	void divideByPowerOf2(double& x, double power);
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

	void multiplyByPowerOf2(float& x, float power);
	void divideByPowerOf2(float& x, float power);
	float inverse(float x);
	bool zero(float x);
	bool negative(float x);
	bool positive(float x);

}

#include "pastel/sys/nativereal.hpp"

#endif
