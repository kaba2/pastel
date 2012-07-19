// Description: Testing for Adjacency_Graph
// DocumentationOf: adjacency_graph.h

#include "pastelsystest.h"

#include "pastel/sys/adjacency_graph.h"

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
		testRunner().add("Adjacency_Graph", test);
	}

	CallFunction run(addTests);

}
