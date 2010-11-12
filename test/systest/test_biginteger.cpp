// Description: Testing for BigInteger
// DocumentationOf: biginteger.h

#include "pastelsystest.h"

#include <pastel/sys/biginteger.h>
#include <pastel/sys/biginteger_tools.h>

#include <iostream>

namespace
{

	using namespace Pastel;

	void testSimple()
	{
		REPORT(
			BigInteger(1234) + BigInteger(5634) !=
			BigInteger(6868));
		REPORT(
			BigInteger(5634) + BigInteger(1234) !=
			BigInteger(6868));
		REPORT(
			BigInteger(1234) - BigInteger(5634) !=
			BigInteger(-4400));
		REPORT(
			BigInteger(5634) - BigInteger(1234) !=
			BigInteger(4400));
		REPORT(
			BigInteger(1234) * BigInteger(5634) !=
			BigInteger(6952356));
		REPORT(
			BigInteger(5634) * BigInteger(1234) !=
			BigInteger(6952356));
		REPORT(
			BigInteger(1234) / BigInteger(5634) !=
			BigInteger(0));
		REPORT(
			BigInteger(5634) / BigInteger(1234) !=
			BigInteger(4));
		REPORT(
			BigInteger(0) + BigInteger(0) !=
			BigInteger(0));
		REPORT(
			BigInteger(1234) - BigInteger(1234) !=
			BigInteger(0));
		REPORT(
			BigInteger(1234) + BigInteger(-1234) !=
			BigInteger(0));
		REPORT(
			BigInteger(-1234) + BigInteger(1234) !=
			BigInteger(0));
		REPORT(
			BigInteger(-1234) + BigInteger(-1234) !=
			BigInteger(-2468));
		REPORT(
			BigInteger(1234) * BigInteger(0) !=
			BigInteger(0));
		REPORT(
			BigInteger(0) * BigInteger(1234) !=
			BigInteger(0));
		REPORT(
			BigInteger(-1234) * BigInteger(0) !=
			BigInteger(0));
		REPORT(
			BigInteger(0) * BigInteger(-1234) !=
			BigInteger(0));
		REPORT(
			BigInteger("-0") !=
			BigInteger(0));
		REPORT(
			BigInteger("23452345") /
			BigInteger(10) !=
			BigInteger("2345234"));
		REPORT(
			BigInteger("2345234") /
			BigInteger(10) !=
			BigInteger("234523"));
		REPORT(
			BigInteger("234523") /
			BigInteger(10) !=
			BigInteger("23452"));
		REPORT(
			BigInteger("23452") /
			BigInteger(10) !=
			BigInteger("2345"));
		REPORT(
			BigInteger("2345") /
			BigInteger(10) !=
			BigInteger("234"));
		REPORT(
			BigInteger("234") /
			BigInteger(10) !=
			BigInteger("23"));
		REPORT(
			BigInteger("23") /
			BigInteger(10) !=
			BigInteger("2"));
		REPORT(
			BigInteger("2") /
			BigInteger(10) !=
			BigInteger(0));
		REPORT(
			BigInteger("21017020") /
			BigInteger(10) !=
			BigInteger("2101702"));
		REPORT(-BigInteger(0) !=
			BigInteger(0));

		BigInteger a("2435325");
		BigInteger b("23452345");
	}

	void testBegin()
	{
		testSimple();
	}

	void testAdd()
	{
		sysTestList().add("BigInteger", testBegin);
	}

	CallFunction run(testAdd);

}
