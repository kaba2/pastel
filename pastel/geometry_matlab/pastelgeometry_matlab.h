// Description: pastelgeometry_matlab() function registration
// Documentation: pastelgeometry_matlab_cpp.txt

#ifndef PASTEL_PASTELGEOMETRY_MATLAB_H
#define PASTEL_PASTELGEOMETRY_MATLAB_H

#include "pastel/geometry_matlab/pastelgeometry_mex.h"

namespace Pastel
{

	//! Registers a function to be callable from Matlab.
	/*!
	The functions that are registered with this function 
	are made callable via the pastelgeometry_matlab() mex function
	(whose entry point is located in pastelgeometry_matlab.cpp).
	*/
	void geometryMatlabAddFunction(
		const std::string& name,
		MatlabFunction* function);

}

#endif
