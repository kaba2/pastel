#ifndef PASTEL_MATLAB_ARGUMENT_HPP
#define PASTEL_MATLAB_ARGUMENT_HPP

#include "pastel/matlab/matlab_argument.h"

#include "pastel/sys/ensure.h"
#include "pastel/sys/pastelomp.h"
#include "pastel/sys/copy_n.h"

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

namespace Pastel
{

	template <typename Type>
	Type* createScalar(
		mxArray*& output)
	{
		const mwSize size[] = {1, 1};

		output = mxCreateNumericArray(
			2, size, 
			typeToMatlabClassId<Type>(),
			mxREAL);

		return (Type*)mxGetData(output);
	}

	template <typename Type>
	std::unique_ptr<Array<Type> > createArray(
		const Vector2i& extent,
		mxArray*& output)
	{
		ENSURE(allGreaterEqual(extent, 0));

		typedef std::unique_ptr<Array<Type> > Result;

		const mwSize size[] = {extent.y(), extent.x()};

		output = mxCreateNumericArray(2, size, 
			typeToMatlabClassId<Type>(), mxREAL);
		
		Type* rawData = (Type*)mxGetData(output);

		return Result(
			new Array<Type>(extent, 
			withAliasing(rawData), 
			StorageOrder::ColumnMajor));
	}

	template <typename Type>
	std::unique_ptr<Array<Type> > createArray(
		integer width, integer height,
		mxArray*& output)
	{
		return createArray<Type>(
			Vector2i(width, height), output);
	}

	template <typename Type>
	Type asScalar(const mxArray* input,
		integer index)
	{
		ENSURE(mxIsNumeric(input));

		const integer n = mxGetNumberOfElements(input);

		ENSURE_OP(index, >=, 0);
		ENSURE_OP(index, <, n);

		Type result = 0;
		switch(mxGetClassID(input))
		{
		case mxSINGLE_CLASS:
			result = *((real32*)mxGetData(input) + index);
			break;
		case mxDOUBLE_CLASS:
			result = *((real64*)mxGetData(input) + index);
			break;
		case mxINT8_CLASS:
			result = *((int8*)mxGetData(input) + index);
			break;
		case mxUINT8_CLASS:
			result = *((uint8*)mxGetData(input) + index);
			break;
		case mxINT16_CLASS:
			result = *((int16*)mxGetData(input) + index);
			break;
		case mxUINT16_CLASS:
			result = *((uint16*)mxGetData(input) + index);
			break;
		case mxINT32_CLASS:
			result = *((int32*)mxGetData(input) + index);
			break;
		case mxUINT32_CLASS:
			result = *((uint32*)mxGetData(input) + index);
			break;
		case mxINT64_CLASS:
			result = *((int64*)mxGetData(input) + index);
			break;
		case mxUINT64_CLASS:
			result = *((uint64*)mxGetData(input) + index);
			break;
		default:
			// This should not be possible, since
			// the above covers all numeric types.
			{
				const bool reachedHere = true;
				ENSURE(!reachedHere);
			}
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
	inline std::unique_ptr<Array<Type> > asArray(
		const mxArray* that)
	{
		ENSURE(mxIsNumeric(that));

		const integer width = mxGetN(that);
		const integer height = mxGetM(that);
		const integer n = width * height;

		typedef std::unique_ptr<Array<Type> > Result;
		
		Result result;

		if (typeToMatlabClassId<Type>() == mxGetClassID(that))
		{
			// No copying is done here. Rather, we aliase
			// the existing data.

			Type* rawData = (Type*)mxGetData(that);
			
			result = Result(
				new Array<Type>(Vector2i(width, height), withAliasing(rawData), 
				StorageOrder::ColumnMajor));
		}
		else
		{
			// Copy the data into an array of the required type.

			if (n >= (1 << 14))
			{
				log() << "Warning: Copying a large amount of data "
					<< "because of type mismatch. Using a matching type, " 
					<< "if possible, avoids any copying." 
					<< logNewLine;
			}

			result = Result(
				new Array<Type>(Vector2i(width, height),
				StorageOrder::ColumnMajor));

			getScalars(that, result->begin());
		}

		return result;
	}

	template <typename Type>
	std::unique_ptr<Array<Type> > asLinearizedArray(
		const mxArray* that)
	{
		ENSURE(mxIsNumeric(that));

		const integer n = mxGetNumberOfElements(that);

		typedef std::unique_ptr<Array<Type> > Result;
		
		Result result;

		if (typeToMatlabClassId<Type>() == mxGetClassID(that))
		{
			// No copying is done here. Rather, we aliase
			// the existing data.

			Type* rawData = (Type*)mxGetData(that);
			
			result = Result(
				new Array<Type>(Vector2i(n, 1), withAliasing(rawData)));
		}
		else
		{
			// Copy the data into an array of the required type.

			if (n >= (1 << 14))
			{
				log() << "Warning: Copying a large amount of data "
					<< "because of type mismatch. Using a matching type, " 
					<< "if possible, avoids any copying." 
					<< logNewLine;
			}

			result = Result(
				new Array<Type>(Vector2i(n, 1)));

			getScalars(that, result->begin());
		}

		return result;
	}

	template <typename Type, typename ArrayPtr_Iterator>
	integer getArrays(
		const mxArray* cellArray,
		ArrayPtr_Iterator output)
	{
		ENSURE(mxIsCell(cellArray));

		const integer n = 
			mxGetNumberOfElements(cellArray);

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
		Scalar_Iterator output,
		integer offset)
	{
		ENSURE(mxIsNumeric(input));
		ENSURE_OP(offset, >=, 0);

		const integer n = 
			mxGetNumberOfElements(input);
		if (offset >= n)
		{
			return n;
		}

		const integer m = n - offset;

		switch(mxGetClassID(input))
		{
		case mxSINGLE_CLASS:
			copy_n((real32*)mxGetData(input) + offset, m, output);
			break;
		case mxDOUBLE_CLASS:
			copy_n((real64*)mxGetData(input) + offset, m, output);
			break;
		case mxINT8_CLASS:
			copy_n((int8*)mxGetData(input) + offset, m, output);
			break;
		case mxUINT8_CLASS:
			copy_n((uint8*)mxGetData(input) + offset, m, output);
			break;
		case mxINT16_CLASS:
			copy_n((int16*)mxGetData(input) + offset, m, output);
			break;
		case mxUINT16_CLASS:
			copy_n((uint16*)mxGetData(input) + offset, m, output);
			break;
		case mxINT32_CLASS:
			copy_n((int32*)mxGetData(input) + offset, m, output);
			break;
		case mxUINT32_CLASS:
			copy_n((uint32*)mxGetData(input) + offset, m, output);
			break;
		case mxINT64_CLASS:
			copy_n((int64*)mxGetData(input) + offset, m, output);
			break;
		case mxUINT64_CLASS:
			copy_n((uint64*)mxGetData(input) + offset, m, output);
			break;
		default:
			// This should not be possible, since
			// the above covers all numeric types.
			ENSURE(false);
			break;
		}
		
		return n;
	}

	namespace Matlab_
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
			mxClassID operator()() const
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
		Matlab_::TypeToMatlabClassId<Type> f;
		return f();
	}

}

#endif
