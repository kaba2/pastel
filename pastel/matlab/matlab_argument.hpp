#ifndef PASTEL_MATLAB_ARGUMENT_HPP
#define PASTEL_MATLAB_ARGUMENT_HPP

#include "pastel/matlab/matlab_argument.h"

#include <pastel/sys/ensure.h>
#include <pastel/sys/pastelomp.h>
#include <pastel/sys/stdext_copy_n.h>

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

namespace Pastel
{

	inline integer asInteger(const mxArray* input)
	{
		return (integer)asReal(input);
	}

	inline real asReal(const mxArray* input)
	{
		return *mxGetPr(input);
	}

	inline std::string asString(const mxArray* input)
	{
		char* text = mxArrayToString(input);
		std::string result(text);
		mxFree(text);

		return result;
	}

	template <typename Type>
	inline boost::shared_ptr<Array<Type> > asArray(
		const mxArray* that)
	{
		// It is intentional to assign the width
		// and height the wrong way. The reason
		// is that Matlab uses column-major storage
		// while we use row-major storage.
		const integer width = mxGetM(that);
		const integer height = mxGetN(that);

		Type* rawData = (Type*)mxGetData(that);
		
		// No copying is done here. Rather, we aliase
		// the existing data.
		return boost::shared_ptr<Array<Type> >(
			new Array<Type>(width, height, withAliasing(rawData)));
	}

	template <typename Type, typename ArrayPtr_Iterator>
	integer getArrays(
		const mxArray* cellArray,
		ArrayPtr_Iterator output)
	{
		const integer n = 
			mxGetNumberOfElements(that);

		for (integer i = 0;i < n;++i)
		{
			const mxArray* cell = mxGetCell(cellArray, i);
			*output = asArray<Type>(cell);
			++output;
		}

		return n;
	}

	template <typename Integer_Iterator>
	integer getIntegers(
		const mxArray* input,
		Integer_Iterator output)
	{
		return getReals(input, output);
	}

	template <typename Real_Iterator>
	integer getReals(
		const mxArray* input,
		Real_Iterator output)
	{
		const integer n = 
			mxGetNumberOfElements(input);

		StdExt::copy_n(mxGetPr(input), n, output);
		
		return n;
	}

	namespace Matlab_Detail
	{

		// For things to work, the 'real' type must be 'double'.
		// If you get an error from this check, change the 'real'
		// type in 'mytypes.h'.

		enum
		{
			RealIsDouble = boost::is_same<real, double>::value
		};
		BOOST_STATIC_ASSERT(RealIsDouble);

	}

}

#endif
