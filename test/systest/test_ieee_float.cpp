// Description: Testing for IEEE floating-point numbers
// DocumentationOf: ieee_float.h

#include "test_pastelsys.h"

#include "pastel/sys/bit/bitmask.h"
#include "pastel/sys/real/ieee_float.h"
#include "pastel/sys/logging.h"

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
			test<float>();
			test<double>();
		}

		template <typename Type>
		void test()
		{
			auto t = [](bool negative, integer exponent, uint64 mantissa, Type correct)
			{
				ScientificNotation s{ negative, exponent, mantissa };

				return (asIeeeFloat<Type>(s) == correct) &&
					(asIeeeFloat<Type>(asScientific(correct)) == correct);
			};

			uint64 oneHalf = singleBitMask<uint64>(SizeInBits<uint64>::value - 1);

			TEST_ENSURE(t(false, 0, 0, 1));
			TEST_ENSURE(t(false, 1, 0, 2));
			TEST_ENSURE(t(false, 2, 0, 4));
			TEST_ENSURE(t(false, 3, 0, 8));
			TEST_ENSURE(t(false, 4, 0, 16));

			TEST_ENSURE(t(false, -1, 0, (Type)1 / 2));
			TEST_ENSURE(t(false, -2, 0, (Type)1 / 4));
			TEST_ENSURE(t(false, -3, 0, (Type)1 / 8));
			TEST_ENSURE(t(false, -4, 0, (Type)1 / 16));

			TEST_ENSURE(t(true, 0, 0, -1));
			TEST_ENSURE(t(true, 1, 0, -2));
			TEST_ENSURE(t(true, 2, 0, -4));
			TEST_ENSURE(t(true, 3, 0, -8));
			TEST_ENSURE(t(true, 4, 0, -16));

			TEST_ENSURE(t(true, -1, 0, (Type)-1 / 2));
			TEST_ENSURE(t(true, -2, 0, (Type)-1 / 4));
			TEST_ENSURE(t(true, -3, 0, (Type)-1 / 8));
			TEST_ENSURE(t(true, -4, 0, (Type)-1 / 16));

			TEST_ENSURE(t(false, 0, oneHalf, (Type)1 * 1.5));
			TEST_ENSURE(t(false, 1, oneHalf, (Type)2 * 1.5));
			TEST_ENSURE(t(false, 2, oneHalf, (Type)4 * 1.5));
			TEST_ENSURE(t(false, 3, oneHalf, (Type)8 * 1.5));
			TEST_ENSURE(t(false, 4, oneHalf, (Type)16 * 1.5));

			TEST_ENSURE(t(false, -1, oneHalf, (Type)1.5 / 2));
			TEST_ENSURE(t(false, -2, oneHalf, (Type)1.5 / 4));
			TEST_ENSURE(t(false, -3, oneHalf, (Type)1.5 / 8));
			TEST_ENSURE(t(false, -4, oneHalf, (Type)1.5 / 16));

			TEST_ENSURE(t(true, 0, oneHalf, (Type)-1 * 1.5));
			TEST_ENSURE(t(true, 1, oneHalf, (Type)-2 * 1.5));
			TEST_ENSURE(t(true, 2, oneHalf, (Type)-4 * 1.5));
			TEST_ENSURE(t(true, 3, oneHalf, (Type)-8 * 1.5));
			TEST_ENSURE(t(true, 4, oneHalf, (Type)-16 * 1.5));

			TEST_ENSURE(t(true, -1, oneHalf, (Type)-1.5 / 2));
			TEST_ENSURE(t(true, -2, oneHalf, (Type)-1.5 / 4));
			TEST_ENSURE(t(true, -3, oneHalf, (Type)-1.5 / 8));
			TEST_ENSURE(t(true, -4, oneHalf, (Type)-1.5 / 16));

			TEST_ENSURE(t(false, 1000000, 0, infinity<Type>()));
			TEST_ENSURE(t(true, 1000000, 0, -infinity<Type>()));
			
			TEST_ENSURE(t(false, -1000000, 0, 0));
			TEST_ENSURE(t(true, -1000000, 0, 0));

			TEST_ENSURE(t(false, 1000000, oneHalf, infinity<Type>()));
			TEST_ENSURE(t(true, 1000000, oneHalf, -infinity<Type>()));
			
			TEST_ENSURE(t(false, -1000000, oneHalf, 0));
			TEST_ENSURE(t(true, -1000000, oneHalf, 0));
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("ieee_float", test);
	}

	CallFunction run(addTest);

}
