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
			using Set = CFastTrie_Map<5, integer>;
			using Key = Set::Key;
			using Value = Set::Value;

			Set a;

			{
				a.insert(1, 11);
				TEST_ENSURE(checkInvariants(a));
				a.insert(9, 19);
				TEST_ENSURE(checkInvariants(a));
				a.insert(5, 15);
				TEST_ENSURE(checkInvariants(a));
				a.insert(4, 14);
				TEST_ENSURE(checkInvariants(a));
				a.insert(6, 16);
				TEST_ENSURE(checkInvariants(a));
				a.insert(3, 13);
				TEST_ENSURE(checkInvariants(a));
				a.insert(7, 17);
				TEST_ENSURE(checkInvariants(a));
				a.insert(8, 18);
				TEST_ENSURE(checkInvariants(a));
				a.insert(2, 12);
				TEST_ENSURE(checkInvariants(a));
				a.insert(0, 10);
				TEST_ENSURE(checkInvariants(a));
				for (auto&& element : a)
				{
					std::cout << element.key().word(0) << " : "
						<< element.chain()->first.word(0) << " : "
						<< element
						<< std::endl;
				}
				a.insert(9, 15);
				TEST_ENSURE(checkInvariants(a));
				a.insert(10, 20);
				TEST_ENSURE(checkInvariants(a));
				a.insert(15, 25);
				TEST_ENSURE(checkInvariants(a));
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
