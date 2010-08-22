#include "pastelmathtest.h"

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
			BigInteger(std::string("-0")) !=
			BigInteger(0));
		REPORT(
			BigInteger(std::string("23452345")) /
			BigInteger(10) !=
			BigInteger(std::string("2345234")));
		REPORT(
			BigInteger(std::string("2345234")) /
			BigInteger(10) !=
			BigInteger(std::string("234523")));
		REPORT(
			BigInteger(std::string("234523")) /
			BigInteger(10) !=
			BigInteger(std::string("23452")));
		REPORT(
			BigInteger(std::string("23452")) /
			BigInteger(10) !=
			BigInteger(std::string("2345")));
		REPORT(
			BigInteger(std::string("2345")) /
			BigInteger(10) !=
			BigInteger(std::string("234")));
		REPORT(
			BigInteger(std::string("234")) /
			BigInteger(10) !=
			BigInteger(std::string("23")));
		REPORT(
			BigInteger(std::string("23")) /
			BigInteger(10) !=
			BigInteger(std::string("2")));
		REPORT(
			BigInteger(std::string("2")) /
			BigInteger(10) !=
			BigInteger(0));
		REPORT(
			BigInteger(std::string("21017020")) /
			BigInteger(10) !=
			BigInteger(std::string("2101702")));
		REPORT(-BigInteger(0) !=
			BigInteger(0));

		BigInteger a(std::string("2435325"));
		BigInteger b(std::string("23452345"));

		std::cout << a << std::endl;
		std::cout << b << std::endl;
		std::cout << a + b << std::endl;
		std::cout << b + a << std::endl;
		std::cout << a - b << std::endl;
		std::cout << b - a << std::endl;
		std::cout << a * b << std::endl;
		std::cout << b * a << std::endl;
		std::cout << a / b << std::endl;
		std::cout << b / a << std::endl;
	}

	void testBegin()
	{
		testSimple();
	}

	void testAdd()
	{
		mathTestList().add("BigInteger", testBegin);
	}

	CallFunction run(testAdd);

}
