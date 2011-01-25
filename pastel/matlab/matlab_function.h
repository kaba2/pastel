// Description: Matlab function registration

#ifndef PASTEL_MATLAB_FUNCTION_H
#define PASTEL_MATLAB_FUNCTION_H

#include "pastel/sys/callfunction.h"

#include <string>

// This is the Matlab's mex header file.
#include "mex.h"

// Use this macro to define the mex entry point.
#define PASTEL_MATLAB_ENTRY() \
	extern "C" void mexFunction( \
		int outputs, mxArray *outputSet[], \
		int inputs, const mxArray *inputSet[]) \
	{ \
		Pastel::matlabEntry( \
			outputs, outputSet, \
			inputs, inputSet); \
	} \

namespace Pastel
{

	typedef void (MatlabFunction)(
		int outputs, mxArray *outputSet[],
		int inputs, const mxArray *inputSet[]);

	//! Registers a function to be callable from Matlab.
	/*!
	The functions that are registered with this function 
	are made callable via the mex function, with the
	function name as the first argument, followed by
	its actual arguments.
	*/
	void matlabAddFunction(
		const std::string& name,
		MatlabFunction* function);

	//! Call this function at mex entry to redirect to registered functions.
	void matlabEntry(
		int outputs, mxArray *outputSet[],
		int inputs, const mxArray *inputSet[]);

}

#endif
