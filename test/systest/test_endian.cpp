#include "pastelsystest.h"
#include "pastel/sys/endianconvert.h"

using namespace Pastel;

namespace
{

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
			REPORT1(wordResult != 0x0201, wordResult);

			uint16 uwordResult = littleEndian(uwordData);
			REPORT1(uwordResult != 0x0201, uwordResult);

			int32 dwordResult = littleEndian(dwordData);
			REPORT1(dwordResult != 0x04030201, dwordResult);

			uint32 udwordResult = littleEndian(udwordData);
			REPORT1(udwordResult != 0x04030201, udwordResult);

			int64 qwordResult = littleEndian(qwordData);
			REPORT1(qwordResult != 0x0807060504030201ll, qwordResult);

			uint64 uqwordResult = littleEndian(uqwordData);
			REPORT1(uqwordResult != 0x0807060504030201ull, uqwordResult);
		}
		{
			int16 wordResult = bigEndian(wordData);
			REPORT1(wordResult != 0x0102, wordResult);

			uint16 uwordResult = bigEndian(uwordData);
			REPORT1(uwordResult != 0x0102, uwordResult);

			int32 dwordResult = bigEndian(dwordData);
			REPORT1(dwordResult != 0x01020304, dwordResult);

			uint32 udwordResult = bigEndian(udwordData);
			REPORT1(udwordResult != 0x01020304, udwordResult);

			int64 qwordResult = bigEndian(qwordData);
			REPORT1(qwordResult != 0x0102030405060708ll, qwordResult);

			uint64 uqwordResult = bigEndian(uqwordData);
			REPORT1(uqwordResult != 0x0102030405060708ull, uqwordResult);
		}
	}

	void testBegin()
	{
		testEndianConvert();
	}

	void testAdd()
	{
		sysTestList().add("Endian", testBegin);
	}

	CallFunction run(testAdd);

}
