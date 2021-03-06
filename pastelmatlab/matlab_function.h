// Description: Matlab function registration

#ifndef PASTELMATLAB_MATLAB_FUNCTION_H
#define PASTELMATLAB_MATLAB_FUNCTION_H

#include "pastel/sys/callfunction.h"

#include <string>

#include "pastelmatlab/matlab_mex.h"

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

}

#endif
