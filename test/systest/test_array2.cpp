#include "pastelsystest.h"

#include "pastel/sys/lineararray.h"

using namespace Pastel;

namespace
{

	void testTrivial()
	{
		LinearArray<2, int> a(1024, 1024);
		LinearArray<2, int> b(a);
		a = b;
		b.clear();
		b = a;
		a.clear();
		a.setExtent(53, 45, 15);
	}

	void testBegin()
	{
		testTrivial();
	}

	void addTest()
	{
		sysTestList().add("LinearArray", testBegin);
	}

	CallFunction run(addTest);

}
