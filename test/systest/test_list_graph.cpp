// Description: Testing for List_Graph
// DocumentationOf: list_graph.h

#include "pastelsystest.h"

#include "pastel/sys/list_graph.h"

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
		}

	};

	void test()
	{
		Test test;
		test.run();		
	}

	void addTests()
	{
		testRunner().add("List_Graph", test);
	}

	CallFunction run(addTests);

}
