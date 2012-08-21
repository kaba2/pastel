// Description: Testing for endian conversions
// DocumentationOf: endian.h

#include "test_pastelsys.h"
#include "pastel/sys/endian.h"

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
			testEndianConvert();
		}

		void testEndianConvert()
		{
			uint8 data[] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8};

			int16& wordData = (int16&)data[0];
			uint16& uwordData = (uint16&)data[0];
			int32& dwordData = (int32&)data[0];
			uint32& udwordData = (uint32&)data[0];
			int64& qwordData = (int64&)data[0];
			uint64& uqwordData = (uint64&)data[0];

			{
				int16 wordResult = littleEndian(wordData);
				TEST_ENSURE_OP(wordResult, ==, 0x0201);

				uint16 uwordResult = littleEndian(uwordData);
				TEST_ENSURE_OP(uwordResult, ==, 0x0201);

				int32 dwordResult = littleEndian(dwordData);
				TEST_ENSURE_OP(dwordResult, ==, 0x04030201);

				uint32 udwordResult = littleEndian(udwordData);
				TEST_ENSURE_OP(udwordResult, ==, 0x04030201);

				int64 qwordResult = littleEndian(qwordData);
				TEST_ENSURE_OP(qwordResult, ==, 0x0807060504030201ll);

				uint64 uqwordResult = littleEndian(uqwordData);
				TEST_ENSURE_OP(uqwordResult, ==, 0x0807060504030201ull);
			}
			{
				int16 wordResult = bigEndian(wordData);
				TEST_ENSURE_OP(wordResult, ==, 0x0102);

				uint16 uwordResult = bigEndian(uwordData);
				TEST_ENSURE_OP(uwordResult, ==, 0x0102);

				int32 dwordResult = bigEndian(dwordData);
				TEST_ENSURE_OP(dwordResult, ==, 0x01020304);

				uint32 udwordResult = bigEndian(udwordData);
				TEST_ENSURE_OP(udwordResult, ==, 0x01020304);

				int64 qwordResult = bigEndian(qwordData);
				TEST_ENSURE_OP(qwordResult, ==, 0x0102030405060708ll);

				uint64 uqwordResult = bigEndian(uqwordData);
				TEST_ENSURE_OP(uqwordResult, ==, 0x0102030405060708ull);
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
		testRunner().add("Endian", test);
	}

	CallFunction run(addTest);

}
