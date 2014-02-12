// Description: Testing for c-fast trie
// DocumentationOf: cfasttrie.h

#include "test_pastelsys.h"

#include <pastel/sys/cfasttrie.h>

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
			using Set = CFastTrie_Set<64>;
			using Key = Set::Key;
			using Value = Set::Value;

			Set a;

			{
				a.insert(1);
				TEST_ENSURE(checkInvariants(a));
				a.insert(9);
				TEST_ENSURE(checkInvariants(a));
				a.insert(5);
				TEST_ENSURE(checkInvariants(a));
				a.insert(4);
				TEST_ENSURE(checkInvariants(a));
				a.insert(6);
				TEST_ENSURE(checkInvariants(a));
				a.insert(3);
				TEST_ENSURE(checkInvariants(a));
				a.insert(7);
				TEST_ENSURE(checkInvariants(a));
				a.insert(8);
				TEST_ENSURE(checkInvariants(a));
				a.insert(2);
				TEST_ENSURE(checkInvariants(a));
				a.insert(0);
				TEST_ENSURE(checkInvariants(a));
				a.insert(9);
				TEST_ENSURE(checkInvariants(a));
				a.insert(10);
				TEST_ENSURE(checkInvariants(a));
				a.insert(15);
				TEST_ENSURE(checkInvariants(a));
				a.insert(16);
				TEST_ENSURE(checkInvariants(a));
				for (auto&& element : a)
				{
					std::cout << element.cKey().word(0) << " : "
						<< element.chain()->first.word(0) << std::endl;
				}
			}
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("CFastTrie", test);
	}

	CallFunction run(addTest);

}
