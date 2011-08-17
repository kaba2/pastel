// Description: Testing for maximum bipartite matching
// DocumentationOf: maximum_bipartite_matching.h

#include "pastelsystest.h"

#include "pastel/sys/maximum_bipartite_matching.h"
#include "pastel/sys/tuple.h"

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
			Integer2 edgeSet[] =
			{
				Integer2(0, 0)
			};
			
			maximumBipartiteMatching(
				range(edgeSet));
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
