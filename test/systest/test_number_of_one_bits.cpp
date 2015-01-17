// Description: Testing template
// DocumentationOf: number_of_one_bits.h

#include "test_pastelsys.h"
#include <pastel/sys/bit/number_of_one_bits.h>

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
			test();
		}

		void test()
		{
			{
				auto test = [&](uint8 that, integer correct)
				{
					return numberOfOneBits(that) == correct;
				};

				TEST_ENSURE(test(0x00, 0));

				TEST_ENSURE(test(0x01, 1));
				TEST_ENSURE(test(0x02, 1));
				TEST_ENSURE(test(0x04, 1));
				TEST_ENSURE(test(0x08, 1));
				TEST_ENSURE(test(0x10, 1));
				TEST_ENSURE(test(0x20, 1));
				TEST_ENSURE(test(0x40, 1));
				TEST_ENSURE(test(0x80, 1));

				TEST_ENSURE(test(0x02 + 0x01, 2));
				TEST_ENSURE(test(0x04 + 0x01, 2));
				TEST_ENSURE(test(0x08 + 0x01, 2));
				TEST_ENSURE(test(0x10 + 0x01, 2));
				TEST_ENSURE(test(0x20 + 0x01, 2));
				TEST_ENSURE(test(0x40 + 0x01, 2));
				TEST_ENSURE(test(0x80 + 0x01, 2));

				TEST_ENSURE(test(0x04 + 0x02, 2));
				TEST_ENSURE(test(0x08 + 0x02, 2));
				TEST_ENSURE(test(0x10 + 0x02, 2));
				TEST_ENSURE(test(0x20 + 0x02, 2));
				TEST_ENSURE(test(0x40 + 0x02, 2));
				TEST_ENSURE(test(0x80 + 0x02, 2));

				TEST_ENSURE(test(0x08 + 0x04, 2));
				TEST_ENSURE(test(0x10 + 0x04, 2));
				TEST_ENSURE(test(0x20 + 0x04, 2));
				TEST_ENSURE(test(0x40 + 0x04, 2));
				TEST_ENSURE(test(0x80 + 0x04, 2));

				TEST_ENSURE(test(0x10 + 0x08, 2));
				TEST_ENSURE(test(0x20 + 0x08, 2));
				TEST_ENSURE(test(0x40 + 0x08, 2));
				TEST_ENSURE(test(0x80 + 0x08, 2));

				TEST_ENSURE(test(0x20 + 0x10, 2));
				TEST_ENSURE(test(0x40 + 0x10, 2));
				TEST_ENSURE(test(0x80 + 0x10, 2));

				TEST_ENSURE(test(0x40 + 0x20, 2));
				TEST_ENSURE(test(0x80 + 0x20, 2));

				TEST_ENSURE(test(0x80 + 0x40, 2));

				TEST_ENSURE(test(0x37, 5));

				TEST_ENSURE(test(0xFF, 8));

				TEST_ENSURE(test(0xEF, 7));
				TEST_ENSURE(test(0xCF, 6));
				TEST_ENSURE(test(0xBF, 7));
				TEST_ENSURE(test(0x7F, 7));

				TEST_ENSURE(test(0xFE, 7));
				TEST_ENSURE(test(0xFC, 6));
				TEST_ENSURE(test(0xFB, 7));
				TEST_ENSURE(test(0xF7, 7));
			}

			{
				auto test = [&](uint16 that, integer correct)
				{
					return numberOfOneBits(that) == correct;
				};

				TEST_ENSURE(test(0x0000u, 0));

				TEST_ENSURE(test(0x0001u, 1));
				TEST_ENSURE(test(0x0002u, 1));
				TEST_ENSURE(test(0x0004u, 1));
				TEST_ENSURE(test(0x0008u, 1));

				TEST_ENSURE(test(0x0010u, 1));
				TEST_ENSURE(test(0x0020u, 1));
				TEST_ENSURE(test(0x0040u, 1));
				TEST_ENSURE(test(0x0080u, 1));

				TEST_ENSURE(test(0x0100u, 1));
				TEST_ENSURE(test(0x0200u, 1));
				TEST_ENSURE(test(0x0400u, 1));
				TEST_ENSURE(test(0x0800u, 1));

				TEST_ENSURE(test(0x1000u, 1));
				TEST_ENSURE(test(0x2000u, 1));
				TEST_ENSURE(test(0x4000u, 1));
				TEST_ENSURE(test(0x8000u, 1));

				TEST_ENSURE(test(0x0123u, 4));
			}

			{
				auto test = [&](uint32 that, integer correct)
				{
					return numberOfOneBits(that) == correct;
				};

				TEST_ENSURE(test(0x00000000ul, 0));

				TEST_ENSURE(test(0x00000001ul, 1));
				TEST_ENSURE(test(0x00000002ul, 1));
				TEST_ENSURE(test(0x00000004ul, 1));
				TEST_ENSURE(test(0x00000008ul, 1));

				TEST_ENSURE(test(0x00000010ul, 1));
				TEST_ENSURE(test(0x00000020ul, 1));
				TEST_ENSURE(test(0x00000040ul, 1));
				TEST_ENSURE(test(0x00000080ul, 1));

				TEST_ENSURE(test(0x00000100ul, 1));
				TEST_ENSURE(test(0x00000200ul, 1));
				TEST_ENSURE(test(0x00000400ul, 1));
				TEST_ENSURE(test(0x00000800ul, 1));

				TEST_ENSURE(test(0x00001000ul, 1));
				TEST_ENSURE(test(0x00002000ul, 1));
				TEST_ENSURE(test(0x00004000ul, 1));
				TEST_ENSURE(test(0x00008000ul, 1));

				TEST_ENSURE(test(0x00010000ul, 1));
				TEST_ENSURE(test(0x00020000ul, 1));
				TEST_ENSURE(test(0x00040000ul, 1));
				TEST_ENSURE(test(0x00080000ul, 1));

				TEST_ENSURE(test(0x00100000ul, 1));
				TEST_ENSURE(test(0x00200000ul, 1));
				TEST_ENSURE(test(0x00400000ul, 1));
				TEST_ENSURE(test(0x00800000ul, 1));

				TEST_ENSURE(test(0x01000000ul, 1));
				TEST_ENSURE(test(0x02000000ul, 1));
				TEST_ENSURE(test(0x04000000ul, 1));
				TEST_ENSURE(test(0x08000000ul, 1));

				TEST_ENSURE(test(0x10000000ul, 1));
				TEST_ENSURE(test(0x20000000ul, 1));
				TEST_ENSURE(test(0x40000000ul, 1));
				TEST_ENSURE(test(0x80000000ul, 1));

				TEST_ENSURE(test(0x01234567ul, 0 + 1 + 1 + 2 + 1 + 2 + 2 + 3));
			}

			{
				auto test = [&](uint64 that, integer correct)
				{
					return numberOfOneBits(that) == correct;
				};

				TEST_ENSURE(test(0x0000000000000000ull, 0));

				TEST_ENSURE(test(0x0000000000000001ull, 1));
				TEST_ENSURE(test(0x0000000000000002ull, 1));
				TEST_ENSURE(test(0x0000000000000004ull, 1));
				TEST_ENSURE(test(0x0000000000000008ull, 1));

				TEST_ENSURE(test(0x0000000000000010ull, 1));
				TEST_ENSURE(test(0x0000000000000020ull, 1));
				TEST_ENSURE(test(0x0000000000000040ull, 1));
				TEST_ENSURE(test(0x0000000000000080ull, 1));

				TEST_ENSURE(test(0x0000000000000100ull, 1));
				TEST_ENSURE(test(0x0000000000000200ull, 1));
				TEST_ENSURE(test(0x0000000000000400ull, 1));
				TEST_ENSURE(test(0x0000000000000800ull, 1));

				TEST_ENSURE(test(0x0000000000001000ull, 1));
				TEST_ENSURE(test(0x0000000000002000ull, 1));
				TEST_ENSURE(test(0x0000000000004000ull, 1));
				TEST_ENSURE(test(0x0000000000008000ull, 1));

				TEST_ENSURE(test(0x0000000000010000ull, 1));
				TEST_ENSURE(test(0x0000000000020000ull, 1));
				TEST_ENSURE(test(0x0000000000040000ull, 1));
				TEST_ENSURE(test(0x0000000000080000ull, 1));

				TEST_ENSURE(test(0x0000000000100000ull, 1));
				TEST_ENSURE(test(0x0000000000200000ull, 1));
				TEST_ENSURE(test(0x0000000000400000ull, 1));
				TEST_ENSURE(test(0x0000000000800000ull, 1));

				TEST_ENSURE(test(0x0000000001000000ull, 1));
				TEST_ENSURE(test(0x0000000002000000ull, 1));
				TEST_ENSURE(test(0x0000000004000000ull, 1));
				TEST_ENSURE(test(0x0000000008000000ull, 1));

				TEST_ENSURE(test(0x0000000010000000ull, 1));
				TEST_ENSURE(test(0x0000000020000000ull, 1));
				TEST_ENSURE(test(0x0000000040000000ull, 1));
				TEST_ENSURE(test(0x0000000080000000ull, 1));

				TEST_ENSURE(test(0x0000000100000000ull, 1));
				TEST_ENSURE(test(0x0000000200000000ull, 1));
				TEST_ENSURE(test(0x0000000400000000ull, 1));
				TEST_ENSURE(test(0x0000000800000000ull, 1));

				TEST_ENSURE(test(0x0000001000000000ull, 1));
				TEST_ENSURE(test(0x0000002000000000ull, 1));
				TEST_ENSURE(test(0x0000004000000000ull, 1));
				TEST_ENSURE(test(0x0000008000000000ull, 1));

				TEST_ENSURE(test(0x0000010000000000ull, 1));
				TEST_ENSURE(test(0x0000020000000000ull, 1));
				TEST_ENSURE(test(0x0000040000000000ull, 1));
				TEST_ENSURE(test(0x0000080000000000ull, 1));

				TEST_ENSURE(test(0x0000100000000000ull, 1));
				TEST_ENSURE(test(0x0000200000000000ull, 1));
				TEST_ENSURE(test(0x0000400000000000ull, 1));
				TEST_ENSURE(test(0x0000800000000000ull, 1));

				TEST_ENSURE(test(0x0001000000000000ul, 1));
				TEST_ENSURE(test(0x0002000000000000ul, 1));
				TEST_ENSURE(test(0x0004000000000000ul, 1));
				TEST_ENSURE(test(0x0008000000000000ul, 1));

				TEST_ENSURE(test(0x0010000000000000ul, 1));
				TEST_ENSURE(test(0x0020000000000000ul, 1));
				TEST_ENSURE(test(0x0040000000000000ul, 1));
				TEST_ENSURE(test(0x0080000000000000ul, 1));

				TEST_ENSURE(test(0x0100000000000000ul, 1));
				TEST_ENSURE(test(0x0200000000000000ul, 1));
				TEST_ENSURE(test(0x0400000000000000ul, 1));
				TEST_ENSURE(test(0x0800000000000000ul, 1));

				TEST_ENSURE(test(0x1000000000000000ul, 1));
				TEST_ENSURE(test(0x2000000000000000ul, 1));
				TEST_ENSURE(test(0x4000000000000000ul, 1));
				TEST_ENSURE(test(0x8000000000000000ul, 1));

				TEST_ENSURE(test(0x0123456789ABCDEFull, 
					0 + 1 + 1 + 2 + 1 + 2 + 2 + 3 +
					1 + 2 + 2 + 3 + 2 + 3 + 3 + 4));
			}
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("numberOfOneBits", test);
	}

	CallFunction run(addTest);

}
