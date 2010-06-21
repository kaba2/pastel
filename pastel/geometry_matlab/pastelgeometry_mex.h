// Description: Functions to ease interfacing with Matlab
// Documentation: pastelgeometry_matlab_cpp.txt

#ifndef PASTEL_PASTELGEOMETRY_MEX_H
#define PASTEL_PASTELGEOMETRY_MEX_H

#include "mex.h"

#include "pastel/sys/mytypes.h"

#include <pastel/sys/array.h>
#include <pastel/sys/callfunction.h>

#include <boost/type_traits/is_same.hpp>

namespace Pastel
{

	typedef void (MatlabFunction)(
		int outputs, mxArray *outputSet[],
		int inputs, const mxArray *inputSet[]);

	namespace Detail
	{

		enum
		{
			RealIsDouble = boost::is_same<real, double>::value
		};
		PASTEL_STATIC_ASSERT(RealIsDouble);

	}

	template <typename Integer_OutputIterator>
	void getIntegers(const mxArray* input,
					 Integer_OutputIterator output);

	template <typename Real_OutputIterator>
	void getReals(const mxArray* input,
				  Real_OutputIterator output);

	integer getInteger(const mxArray* input);

	real getReal(const mxArray* input);

	std::string getString(const mxArray* input);

	void setNumberOfThreads(integer threads);

}

#include "pastel/geometry_matlab/pastelgeometry_mex.hpp"

#endif
