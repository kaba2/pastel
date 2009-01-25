#include "pastelgeometrytest.h"

#include "pastel/geometry/boxcollider.h"

using namespace Pastel;

namespace
{

	void test()
	{
	}

	void testBegin()
	{
		test();
	}

	void testAdd()
	{
		geometryTestList().add("BoxCollider", testBegin);
	}

	CallFunction run(testAdd);

}
