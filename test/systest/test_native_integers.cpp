// Description: Testing for native integers
// DocumentationOf: native_integer.h

#include "test_pastelsys.h"

#include <pastel/sys/math/number_tests.h>
#include <pastel/sys/integer/integer_mean.h>

using namespace Pastel;

namespace
{

	class Test
		: public TestSuite
	{
	public:
		Test()
			: TestSuite(&testReport())
		{
		}

		virtual void run()
		{
			testInfinity();

			//testMany<bool>();
			testMany<int8>();
			testMany<int16>();
			testMany<int32>();
			testMany<int64>();

			testMany<uint8>();
			testMany<uint16>();
			testMany<uint32>();
			testMany<uint64>();

			testMany<schar>();
			testMany<char>();
			testMany<short>();
			testMany<int>();
			testMany<long>();
			testMany<longlong>();

			testMany<uchar>();
			testMany<ushort>();
			testMany<uint>();
			testMany<ulong>();
			testMany<ulonglong>();

			testMany<integer>();
			testMany<uinteger>();
		}

		template <typename Type>
		void testMany()
		{
			PASTEL_CONCEPT_CHECK(Type, Finite_Integer_Concept);

			testIntegerMean<Type>();
			testComparison<Type>();
			testEvenOdd<Type>();
			testIsPowerOfTwo<Type>();
		}

		template <typename Type>
		void testComparison()
		{
			TEST_ENSURE(negative((Type)-1) || std::is_unsigned<Type>::value);
			TEST_ENSURE(!negative((Type)0));
			TEST_ENSURE(positive((Type)1));
			TEST_ENSURE(!positive((Type)0));
		}

		void testInfinity()
		{
			TEST_ENSURE(infinity<uint8>() == 0xFFu);
			TEST_ENSURE(infinity<uint16>() == 0xFFFFu);
			TEST_ENSURE(infinity<uint32>() == 0xFFFFFFFFul);
			TEST_ENSURE(infinity<uint64>() == 0xFFFFFFFFFFFFFFFFull);

			TEST_ENSURE(infinity<int8>() == 0x7Fu);
			TEST_ENSURE(infinity<int16>() == 0x7FFFu);
			TEST_ENSURE(infinity<int32>() == 0x7FFFFFFFul);
			TEST_ENSURE(infinity<int64>() == 0x7FFFFFFFFFFFFFFFull);
		}

		template <typename Type>
		void testIntegerMean()
		{
			Type min = std::is_unsigned<Type>::value ? 
				-16 : 0;

			for (Type i = min; i < 16; ++i)
			{
				for (Type j = min; j < 16;++j)
				{
					TEST_ENSURE_OP(integerMean(i, j), ==, (i + j) / 2);
				}				
			}

			TEST_ENSURE_OP(integerMean(infinity<Type>(), infinity<Type>()), ==, infinity<Type>());
			if (std::is_signed<Type>::value)
			{
				TEST_ENSURE_OP(integerMean(-infinity<Type>(), -infinity<Type>()), ==, -infinity<Type>());
			}
		}

		template <typename Type>
		void testEvenOdd()
		{
			for (integer i = 1;i < 100;i += 2)
			{
				TEST_ENSURE(odd((Type)i));
			}

			for (integer i = 0;i < 100;i += 2)
			{
				TEST_ENSURE(even((Type)i));
			}
		}

		template <typename Type>
		void testIsPowerOfTwo()
		{
			auto f = [](integer t)
			{
				return isPowerOfTwo((Type)t);
			};

			TEST_ENSURE(f(1));
			TEST_ENSURE(f(2));
			TEST_ENSURE(!f(3));
			TEST_ENSURE(f(4));
			TEST_ENSURE(!f(5));
			TEST_ENSURE(!f(6));
			TEST_ENSURE(!f(7));
			TEST_ENSURE(f(8));
			TEST_ENSURE(!f(9));
			TEST_ENSURE(!f(10));
			TEST_ENSURE(!f(11));
			TEST_ENSURE(!f(12));
			TEST_ENSURE(!f(13));
			TEST_ENSURE(!f(14));
			TEST_ENSURE(!f(15));
			TEST_ENSURE(f(16));
			TEST_ENSURE(!f(17));
			TEST_ENSURE(!f(18));
			TEST_ENSURE(!f(19));
			TEST_ENSURE(!f(20));
			TEST_ENSURE(!f(21));
			TEST_ENSURE(!f(22));
			TEST_ENSURE(!f(23));
			TEST_ENSURE(!f(24));
			TEST_ENSURE(!f(25));
			TEST_ENSURE(!f(26));
			TEST_ENSURE(!f(27));
			TEST_ENSURE(!f(28));
			TEST_ENSURE(!f(29));
			TEST_ENSURE(!f(30));
			TEST_ENSURE(!f(31));
			TEST_ENSURE(f(32));
			TEST_ENSURE(f(64));
			TEST_ENSURE(!f(127));
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("native_integer", test);
	}

	CallFunction run(addTest);

}
