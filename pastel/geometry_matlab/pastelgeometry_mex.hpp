#ifndef PASTEL_PASTELGEOMETRY_MEX_HPP
#define PASTEL_PASTELGEOMETRY_MEX_HPP

#include "pastel/geometry_matlab/pastelgeometry_mex.h"

#include <pastel/sys/ensure.h>
#include <pastel/sys/pastelomp.h>
#include <pastel/sys/stdext_copy_n.h>

namespace Pastel
{

	template <typename Integer_OutputIterator>
	void getIntegers(const mxArray* input,
					 Integer_OutputIterator output)
	{
		getReals(input, output);
	}

	template <typename Real_OutputIterator>
	void getReals(const mxArray* input,
				  Real_OutputIterator output)
	{
		const integer n = mxGetNumberOfElements(input);
		StdExt::copy_n(mxGetPr(input), n, output);
	}

	inline integer getInteger(const mxArray* input)
	{
		return *mxGetPr(input);
	}

	inline real getReal(const mxArray* input)
	{
		return *mxGetPr(input);
	}

	inline std::string getString(const mxArray* input)
	{
		char* text = mxArrayToString(input);
		std::string result(text);
		mxFree(text);
		return result;
	}

	inline void setNumberOfThreads(integer threads)
	{
		ENSURE_OP(threads, >, 0);
#if PASTEL_ENABLE_OMP != 0
		omp_set_num_threads(threads);
#endif
	}

}

#endif
