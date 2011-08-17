// Description: Testing for maximum bipartite matching
// DocumentationOf: maximum_bipartite_matching.h

#include "pastelsystest.h"

#include "pastel/sys/maximum_bipartite_matching.h"

using namespace Pastel;
using namespace std;

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
			test();
		}

		void test()
		{
			KeyValue<integer, integer> edgeSet[] =
			{
				keyValue(0, 0);				
			};
			
			maximumBipartiteMatching(
				range(edgeSet), KeyValue_Pair_ConceptMap());
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("maximumBipartiteMatching", test);
	}

	CallFunction run(addTest);

}
