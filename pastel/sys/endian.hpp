#ifndef PASTELSYS_ENDIAN_HPP
#define PASTELSYS_ENDIAN_HPP

#include "pastel/sys/endian.h"
#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"

#include <type_traits>

namespace Pastel
{

	namespace Endian_
	{

		template <integer Size>
		void swapByteOrder(const char* data, char* result)
		{
			PASTEL_STATIC_ASSERT(Size > 0);
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
		static PASTEL_CONSTEXPR uint8 testTable[2] = {1, 0};
		static PASTEL_CONSTEXPR uint16 value = *((uint16 *)testTable);
		static PASTEL_CONSTEXPR bool littleEndian = (value == 0x1);

		return littleEndian;
	}

	template <typename Type>
	Type littleEndian(const Type& that)
	{
		PASTEL_STATIC_ASSERT(
			std::is_integral<Type>::value ||
			std::is_floating_point<Type>::value);

		if (!isLittleEndian())
		{
			Type result = 0;
			Endian_::swapByteOrder<sizeof(Type)>(
				(const char*)&that, (char*)&result);
			return result;
		}
		return that;
	}

	template <typename Type>
	Type bigEndian(const Type& that)
	{
		PASTEL_STATIC_ASSERT(
			std::is_integral<Type>::value ||
			std::is_floating_point<Type>::value);

		if (isLittleEndian())
		{
			Type result = 0;
			Endian_::swapByteOrder<sizeof(Type)>(
				(const char*)&that, (char*)&result);
			return result;
		}
		return that;
	}

}

#endif
