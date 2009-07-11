#ifndef PASTEL_ENDIANCONVERT_HPP
#define PASTEL_ENDIANCONVERT_HPP

#include "pastel/sys/endianconvert.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/syslibrary.h"
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_floating_point.hpp>

#include <boost/static_assert.hpp>
#include "pastel/sys/ensure.h"

namespace Pastel
{

	namespace Detail
	{

		template <integer Size>
		void swapByteOrder(const char* data, char* result)
		{
			BOOST_STATIC_ASSERT(Size > 0);
			ASSERT(data != 0 && result != 0);

			for(integer i = 0; i < Size; ++i)
			{
				result[i] = data[Size - 1 - i];
			}
		}

		template <>
		inline void swapByteOrder<1>(const char* data, char* result)
		{
			ASSERT(data != 0 && result != 0);
		}

		template <>
		inline void swapByteOrder<2>(const char* data, char* result)
		{
			ASSERT(data != 0 && result != 0);

			result[0] = data[1];
			result[1] = data[0];
		}

		template <>
		inline void swapByteOrder<4>(const char* data, char* result)
		{
			ASSERT(data != 0 && result != 0);

			result[0] = data[3];
			result[1] = data[2];
			result[2] = data[1];
			result[3] = data[0];
		}
	}

	inline bool isLittleEndian()
	{
		static const uint8 testTable[2] = {1, 0};
		static const uint16 value = *((uint16 *)testTable);
		static const bool littleEndian = (value == 0x1);

		return littleEndian;
	}

	template <typename Type>
	Type littleEndian(const Type& that)
	{
		BOOST_STATIC_ASSERT(
			boost::is_integral<Type>::value ||
			boost::is_floating_point<Type>::value);

		if (!isLittleEndian())
		{
			Type result = 0;
			Detail::swapByteOrder<sizeof(Type)>(
				(const char*)&that, (char*)&result);
			return result;
		}
		return that;
	}

	template <typename Type>
	Type bigEndian(const Type& that)
	{
		BOOST_STATIC_ASSERT(
			boost::is_integral<Type>::value ||
			boost::is_floating_point<Type>::value);

		if (isLittleEndian())
		{
			Type result = 0;
			Detail::swapByteOrder<sizeof(Type)>(
				(const char*)&that, (char*)&result);
			return result;
		}
		return that;
	}

}

#endif
