#ifndef PASTELMATLAB_MATLAB_ARGUMENT_HPP
#define PASTELMATLAB_MATLAB_ARGUMENT_HPP

#include "pastel/matlab/matlab_argument.h"

#include "pastel/sys/ensure.h"
#include "pastel/sys/sequence/copy_n.h"

namespace Pastel
{

	template <typename Type>
	Type* matlabCreateScalar(
		mxArray*& output)
	{
		output = mxCreateNumericMatrix(1, 1,
			typeToMatlabClassId<Type>(), mxREAL);

		return (Type*)mxGetData(output);
	}

	template <typename Type>
	Array<Type> matlabCreateArray(
		const Vector2i& extent,
		mxArray*& output)
	{
		ENSURE(allGreaterEqual(extent, 0));

		output = mxCreateNumericMatrix(extent.y(), extent.x(), 
			typeToMatlabClassId<Type>(), mxREAL);
		
		Type* rawData = (Type*)mxGetData(output);

		Array<Type> result(extent, 
			withAliasing(rawData), 
			StorageOrder::ColumnMajor);

		return result;
	}

	template <typename Type>
	Array<Type> matlabCreateArray(
		integer width, integer height,
		mxArray*& output)
	{
		return matlabCreateArray<Type>(
			Vector2i(width, height), output);
	}

	template <typename Type>
	Type matlabAsScalar(const mxArray* input,
		integer index)
	{
		ENSURE(mxIsNumeric(input));

		integer n = mxGetNumberOfElements(input);

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
				bool reachedHere = true;
				ENSURE(!reachedHere);
			}
			break;
		};

		return result;
	}


	inline std::string matlabAsString(const mxArray* input)
	{
		ENSURE(mxIsChar(input));

		char* text = mxArrayToString(input);
		std::string result(text);
		mxFree(text);

		return result;
	}

	template <typename Type>
	Array<Type> matlabAsArray(
		const mxArray* that)
	{
		ENSURE(mxIsNumeric(that));

		integer width = mxGetN(that);
		integer height = mxGetM(that);

		const integer n = width * height;

		Array<Type> result;

		if (typeToMatlabClassId<Type>() == mxGetClassID(that))
		{
			// No copying is done here. Rather, we aliase
			// the existing data.

			Type* rawData = (Type*)mxGetData(that);

			result = Array<Type>(
				Vector2i(width, height), 
				withAliasing(rawData), 
				StorageOrder::ColumnMajor);
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

			result = Array<Type>(
				Vector2i(width, height),
				0,
				StorageOrder::ColumnMajor);

			matlabGetScalars(that, result.begin());
		}

		return result;
	}

	template <typename Type>
	Array<Type> matlabAsLinearizedArray(
		const mxArray* that)
	{
		ENSURE(mxIsNumeric(that));

		integer n = mxGetNumberOfElements(that);

		Array<Type> result;

		if (typeToMatlabClassId<Type>() == mxGetClassID(that))
		{
			// No copying is done here. Rather, we aliase
			// the existing data.


			Type* rawData = (Type*)mxGetData(that);
			
			result = Array<Type>(
				Vector2i(n, 1), 
				withAliasing(rawData));
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

			result = Array<Type>(Vector2i(n, 1));

			matlabGetScalars(that, result.begin());
		}

		return result;
	}

	template <typename Type, typename Array_Output>
	integer matlabGetArrays(
		const mxArray* cellArray,
		Array_Output report)
	{
		ENSURE(mxIsCell(cellArray));

		integer n = 
			mxGetNumberOfElements(cellArray);

		for (integer i = 0;i < n;++i)
		{

			const mxArray* cell = mxGetCell(cellArray, i);
			report(matlabAsArray<Type>(cell));
		}

		return n;
	}

	template <typename Scalar_Iterator>
	integer matlabGetScalars(
		const mxArray* input,
		Scalar_Iterator output,
		integer offset)
	{
		ENSURE(mxIsNumeric(input));
		ENSURE_OP(offset, >=, 0);

		integer n = 
			mxGetNumberOfElements(input);
		if (offset >= n)
		{
			return n;
		}

		integer m = n - offset;

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

	template <typename Type>
	mxClassID typeToMatlabClassId()
	{
		PASTEL_STATIC_ASSERT(
			std::is_pointer<Type>::value ||
			std::is_integral<Type>::value || 
			std::is_floating_point<Type>::value)

		if (std::is_pointer<Type>::value)
		{
			switch(sizeof(Type))
			{
			case 8:
				return mxUINT64_CLASS;
			case 4:
				return mxUINT32_CLASS;
			};
		}

		// Note: the mxCHAR_CLASS coincides in type with
		// mxINT8_CLASS. Therefore, we leave it out here:
		// the important thing is to support the 
		// numeric types. The mxCHAR_CLASS is handled
		// specially for strings.

		if (std::is_integral<Type>::value)
		{
			if (std::is_signed<Type>::value)
			{
				switch(sizeof(Type))
				{
				case 8:
					return mxINT64_CLASS;
				case 4:
					return mxINT32_CLASS;
				case 2:
					return mxINT16_CLASS;
				case 1:
					return mxINT8_CLASS;
				};
			}
			else
			{
				switch(sizeof(Type))
				{
				case 8:
					return mxUINT64_CLASS;
				case 4:
					return mxUINT32_CLASS;
				case 2:
					return mxUINT16_CLASS;
				case 1:
					return mxUINT8_CLASS;
				};
			}
		}

		if (std::is_floating_point<Type>::value)
		{
			if (std::is_same<float, typename std::remove_cv<Type>::type>::value)
			{
				return mxSINGLE_CLASS;
			}
			if (std::is_same<double, typename std::remove_cv<Type>::type>::value)
			{
				return mxDOUBLE_CLASS;
			}
		}

		bool reachedHere = true;
		ENSURE(!reachedHere);

		return mxUNKNOWN_CLASS;
	}

}

#endif
