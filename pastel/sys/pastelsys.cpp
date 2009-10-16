#include "pastel/sys/mytypes.h"
#include "pastel/sys/environment.h"
#include "pastel/sys/syslibrary.h"

// These headers are here so that they
// get dll-exported.
#include "pastel/sys/possiblyemptymember.h"

#include <boost/static_assert.hpp>

#include <climits>

namespace Pastel
{

	namespace
	{

		void testDataSizes()
		{
			using namespace std;

			// Check that:
			// char is 8 bits
			// short is 16 bits
			// int is 32 bits

			BOOST_STATIC_ASSERT(
				CHAR_MIN == (-127 - 1) && CHAR_MAX == 127);
			BOOST_STATIC_ASSERT(
				UCHAR_MAX == 0xFF);

			BOOST_STATIC_ASSERT(
				SHRT_MIN == (-32767 - 1) && SHRT_MAX == 32767);
			BOOST_STATIC_ASSERT(
				USHRT_MAX == 0xFFFF);

			BOOST_STATIC_ASSERT(
				INT_MIN == (-2147483647 - 1) && INT_MAX == 2147483647);
			BOOST_STATIC_ASSERT(
				UINT_MAX == 0xFFFFFFFF);

			BOOST_STATIC_ASSERT(
				sizeof(pointer_integer) == sizeof(void*));

			BOOST_STATIC_ASSERT(
				sizeof(uint16) == 2 * sizeof(uint8));
			BOOST_STATIC_ASSERT(
				sizeof(uint32) == 4 * sizeof(uint8));
			BOOST_STATIC_ASSERT(
				sizeof(uint64) == 8 * sizeof(uint8));

			BOOST_STATIC_ASSERT(
				sizeof(real32) == 4 * sizeof(uint8));
			BOOST_STATIC_ASSERT(
				sizeof(real64) == 8 * sizeof(uint8));

			BOOST_STATIC_ASSERT(
				sizeof(int8) == sizeof(uint8));
			BOOST_STATIC_ASSERT(
				sizeof(int16) == sizeof(uint16));
			BOOST_STATIC_ASSERT(
				sizeof(int32) == sizeof(uint32));
			BOOST_STATIC_ASSERT(
				sizeof(int64) == sizeof(uint64));
		}

	}

}
