#include "pastelmathtest.h"

#include "pastel/math/biginteger.h"
#include "pastel/math/integer_tools.h"

namespace
{

	using namespace Pastel;

	typedef BigInteger Integer;

	void testSome()
	{
		REPORT(gcd(Integer(1), Integer(5)) != Integer(1));
		REPORT(gcd(Integer(1), Integer(3455)) != Integer(1));
		REPORT(gcd(Integer(1), Integer(2235)) != Integer(1));
		REPORT(gcd(Integer(1), Integer(2344)) != Integer(1));

		REPORT(gcd(Integer(0), Integer(2344)) != Integer(2344));
		REPORT(gcd(Integer(0), Integer(23455)) != Integer(23455));
		REPORT(gcd(Integer(0), Integer(865)) != Integer(865));
		REPORT(gcd(Integer(0), Integer(5487)) != Integer(5487));

		REPORT(gcd(Integer(13), Integer(7)) != Integer(1));
		REPORT(gcd(Integer(5), Integer(23)) != Integer(1));
		REPORT(gcd(Integer(7), Integer(11)) != Integer(1));
		REPORT(gcd(Integer(3), Integer(2)) != Integer(1));

		REPORT(gcd(Integer(2), Integer(4)) != Integer(2));

		REPORT(gcd(Integer(16), Integer(256)) != Integer(16));
		REPORT(gcd(Integer(128), Integer(256)) != Integer(128));
		REPORT(gcd(Integer(256), Integer(128)) != Integer(128));
	}

	void testBegin()
	{
		testSome();
	}

	void testAdd()
	{
		mathTestList().add("Gcd", testBegin);
	}

	CallFunction run(testAdd);

}
