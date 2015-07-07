// Description: Testing for endian conversions
// DocumentationOf: endian.h

#include "test/test_init.h"

#include "pastel/sys/endian.h"

TEST_CASE("EndianConvert (Endian)")
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
		REQUIRE(wordResult == 0x0201);

		uint16 uwordResult = littleEndian(uwordData);
		REQUIRE(uwordResult == 0x0201);

		int32 dwordResult = littleEndian(dwordData);
		REQUIRE(dwordResult == 0x04030201);

		uint32 udwordResult = littleEndian(udwordData);
		REQUIRE(udwordResult == 0x04030201);

		int64 qwordResult = littleEndian(qwordData);
		REQUIRE(qwordResult == 0x0807060504030201ll);

		uint64 uqwordResult = littleEndian(uqwordData);
		REQUIRE(uqwordResult == 0x0807060504030201ull);
	}
	{
		int16 wordResult = bigEndian(wordData);
		REQUIRE(wordResult == 0x0102);

		uint16 uwordResult = bigEndian(uwordData);
		REQUIRE(uwordResult == 0x0102);

		int32 dwordResult = bigEndian(dwordData);
		REQUIRE(dwordResult == 0x01020304);

		uint32 udwordResult = bigEndian(udwordData);
		REQUIRE(udwordResult == 0x01020304);

		int64 qwordResult = bigEndian(qwordData);
		REQUIRE(qwordResult == 0x0102030405060708ll);

		uint64 uqwordResult = bigEndian(uqwordData);
		REQUIRE(uqwordResult == 0x0102030405060708ull);
	}
}

