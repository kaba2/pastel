// Description: Testing for X-fast trie
// DocumentationOf: xfasttrie.h

#include "test_pastelsys.h"

#include <pastel/sys/xfasttrie.h>

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
			using Set = XFastTrie_Set<64>;

			Set a;
			a.insert(1);
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("XFastTrie", test);
	}

	CallFunction run(addTest);

}
