#ifndef PASTELMATLAB_MATLAB_CLASS_IDS_H
#define PASTELMATLAB_MATLAB_CLASS_IDS_H

#include "pastelmatlab/matlab_mex.h"

namespace Pastel {

	template <mxClassID Id>
	struct MatlabClassIdToType {};

	template <>
	struct MatlabClassIdToType<mxUINT64_CLASS> {
		using type = uint64;
	};

	template <>
	struct MatlabClassIdToType<mxINT64_CLASS> {
		using type = int64;
	};

	template <>
	struct MatlabClassIdToType<mxUINT32_CLASS> {
		using type = uint32;
	};

	template <>
	struct MatlabClassIdToType<mxINT32_CLASS> {
		using type = int32;
	};

	template <>
	struct MatlabClassIdToType<mxUINT16_CLASS> {
		using type = uint16;
	};

	template <>
	struct MatlabClassIdToType<mxINT16_CLASS> {
		using type = int16;
	};

	template <>
	struct MatlabClassIdToType<mxUINT8_CLASS> {
		using type = uint8;
	};

	template <>
	struct MatlabClassIdToType<mxINT8_CLASS> {
		using type = int8;
	};

	template <>
	struct MatlabClassIdToType<mxDOUBLE_CLASS> {
		using type = double;
	};

	template <>
	struct MatlabClassIdToType<mxSINGLE_CLASS> {
		using type = float;
	};

	template <>
	struct MatlabClassIdToType<mxCHAR_CLASS> {
		using type = char;
	};

	template <>
	struct MatlabClassIdToType<mxVOID_CLASS> {
		using type = void;
	};

	template <>
	struct MatlabClassIdToType<mxLOGICAL_CLASS> {
		using type = bool;
	};

	// There are also these class-ids, copied
	// from matlab's matrix.h; add if needed.
	/*
    mxUNKNOWN_CLASS = 0,
    mxCELL_CLASS,
    mxSTRUCT_CLASS,
    mxFUNCTION_CLASS,
    mxOPAQUE_CLASS,
    mxOBJECT_CLASS,
	#if defined(_LP64) || defined(_WIN64)
    	mxINDEX_CLASS = mxUINT64_CLASS
	#else
    	mxINDEX_CLASS = mxUINT32_CLASS
	*/

	//! Returns the Matlab class-id corresponding to 'Type'.
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
