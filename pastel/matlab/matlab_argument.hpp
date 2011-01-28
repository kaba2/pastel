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

	template <typename Type>
	Type asScalar(const mxArray* input)
	{
		ENSURE(mxIsNumeric(input));

		Type result = 0;
		switch(mxGetClassID(input))
		{
		case mxSINGLE_CLASS:
			result = *((real32*)mxGetData(input));
			break;
		case mxDOUBLE_CLASS:
			result = *((real64*)mxGetData(input));
			break;
		case mxINT8_CLASS:
			result = *((int8*)mxGetData(input));
			break;
		case mxUINT8_CLASS:
			result = *((uint8*)mxGetData(input));
			break;
		case mxINT16_CLASS:
			result = *((int16*)mxGetData(input));
			break;
		case mxUINT16_CLASS:
			result = *((uint16*)mxGetData(input));
			break;
		case mxINT32_CLASS:
			result = *((int32*)mxGetData(input));
			break;
		case mxUINT32_CLASS:
			result = *((uint32*)mxGetData(input));
			break;
		case mxINT64_CLASS:
			result = *((int64*)mxGetData(input));
			break;
		case mxUINT64_CLASS:
			result = *((uint64*)mxGetData(input));
			break;
		default:
			// This should not be possible, since
			// the above covers all numeric types.
			ENSURE(false);
			break;
		};

		return result;
	}

	inline std::string asString(const mxArray* input)
	{
		ENSURE(mxIsChar(input));

		char* text = mxArrayToString(input);
		std::string result(text);
		mxFree(text);

		return result;
	}

	template <typename Type>
	inline boost::shared_ptr<Array<Type> > asArray(
		const mxArray* that)
	{
		ENSURE(mxIsNumeric(that));
		ENSURE_OP(typeToMatlabClassId<Type>(), ==, mxGetClassID(that));

		const integer width = mxGetN(that);
		const integer height = mxGetM(that);

		Type* rawData = (Type*)mxGetData(that);
		
		// No copying is done here. Rather, we aliase
		// the existing data.
		return boost::shared_ptr<Array<Type> >(
			new Array<Type>(width, height, withAliasing(rawData), 
			StorageOrder::ColumnMajor));
	}

	template <typename Type, typename ArrayPtr_Iterator>
	integer getArrays(
		const mxArray* cellArray,
		ArrayPtr_Iterator output)
	{
		ENSURE(mxIsCell(cellArray));

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

	template <typename Scalar_Iterator>
	integer getScalars(
		const mxArray* input,
		Scalar_Iterator output)
	{
		ENSURE(mxIsNumeric(input));

		const integer n = 
			mxGetNumberOfElements(input);

		switch(mxGetClassID(input))
		{
		case mxSINGLE_CLASS:
			StdExt::copy_n((real32*)mxGetData(input), n, output);
			break;
		case mxDOUBLE_CLASS:
			StdExt::copy_n((real64*)mxGetData(input), n, output);
			break;
		case mxINT8_CLASS:
			StdExt::copy_n((int8*)mxGetData(input), n, output);
			break;
		case mxUINT8_CLASS:
			StdExt::copy_n((uint8*)mxGetData(input), n, output);
			break;
		case mxINT16_CLASS:
			StdExt::copy_n((int16*)mxGetData(input), n, output);
			break;
		case mxUINT16_CLASS:
			StdExt::copy_n((uint16*)mxGetData(input), n, output);
			break;
		case mxINT32_CLASS:
			StdExt::copy_n((int32*)mxGetData(input), n, output);
			break;
		case mxUINT32_CLASS:
			StdExt::copy_n((uint32*)mxGetData(input), n, output);
			break;
		case mxINT64_CLASS:
			StdExt::copy_n((int64*)mxGetData(input), n, output);
			break;
		case mxUINT64_CLASS:
			StdExt::copy_n((uint64*)mxGetData(input), n, output);
			break;
		default:
			// This should not be possible, since
			// the above covers all numeric types.
			ENSURE(false);
			break;
		}
		
		return n;
	}

	namespace Matlab_Detail
	{

#define PASTEL_TYPE_TO_MATLAB_CLASSID(Type, Id) \
		template <> \
		class TypeToMatlabClassId<Type> \
		{ \
		public: \
			mxClassID operator()() const \
			{ \
				return Id; \
			} \
		}

		template <typename Type>
		class TypeToMatlabClassId
		{
		public:
			mxClassID operator()(const mxArray* that) const
			{
				return mxUNKNOWN_CLASS;
			}
		};

		// Note: the mxCHAR_CLASS coincides in type with
		// mxINT8_CLASS. Therefore, we leave it out here:
		// the important thing is to support the 
		// numeric types. The mxCHAR_CLASS is handled
		// specially for strings.

        PASTEL_TYPE_TO_MATLAB_CLASSID(void, mxVOID_CLASS);
        PASTEL_TYPE_TO_MATLAB_CLASSID(real64, mxDOUBLE_CLASS);
        PASTEL_TYPE_TO_MATLAB_CLASSID(real32, mxSINGLE_CLASS);
        PASTEL_TYPE_TO_MATLAB_CLASSID(int8, mxINT8_CLASS);
        PASTEL_TYPE_TO_MATLAB_CLASSID(uint8, mxUINT8_CLASS);
        PASTEL_TYPE_TO_MATLAB_CLASSID(int16, mxINT16_CLASS);
        PASTEL_TYPE_TO_MATLAB_CLASSID(uint16, mxUINT16_CLASS);
        PASTEL_TYPE_TO_MATLAB_CLASSID(int32, mxINT32_CLASS);
        PASTEL_TYPE_TO_MATLAB_CLASSID(uint32, mxUINT32_CLASS);
        PASTEL_TYPE_TO_MATLAB_CLASSID(int64, mxINT64_CLASS);
        PASTEL_TYPE_TO_MATLAB_CLASSID(uint64, mxUINT64_CLASS);

	}

	template <typename Type>
	mxClassID typeToMatlabClassId()
	{
		Matlab_Detail::TypeToMatlabClassId<Type> f;
		return f();
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
