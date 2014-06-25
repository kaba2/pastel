// Description: Testing for RangeTree
// DocumentationOf: rangetree.h

#include "test_pastelgeometry.h"

#include "pastel/geometry/rangetree.h"

#include "pastel/sys/random_uniform.h"
#include "pastel/sys/locators.h"
#include "pastel/sys/inputs.h"
#include "pastel/sys/iterators.h"

#include <boost/range/adaptor/indirected.hpp>

using namespace Pastel;

namespace
{

	class Settings
	{
	public:
		using Real = real;
		using Point = integer;
	};

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
			RangeTree<Settings> tree;
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("RangeTree", test);
	}

	CallFunction run(addTest);

}
