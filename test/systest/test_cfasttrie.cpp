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
			using Set = CFastTrie_Map<4, integer>;
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
				a.insert(9, 15);
				TEST_ENSURE(checkInvariants(a));
				a.insert(10, 20);
				TEST_ENSURE(checkInvariants(a));
				a.insert(15, 25);
				TEST_ENSURE(checkInvariants(a));
			}
			{
				a.erase(9);
				TEST_ENSURE(checkInvariants(a));
				a.erase(0);
				TEST_ENSURE(checkInvariants(a));
				a.erase(6);
				TEST_ENSURE(checkInvariants(a));
				a.erase(7);
				TEST_ENSURE(checkInvariants(a));
				a.erase(8);
				TEST_ENSURE(checkInvariants(a));
				a.erase(2);
				TEST_ENSURE(checkInvariants(a));
				a.erase(3);
				TEST_ENSURE(checkInvariants(a));
				a.erase(4);
				TEST_ENSURE(checkInvariants(a));
				a.erase(5);
				TEST_ENSURE(checkInvariants(a));
				a.erase(1);
				TEST_ENSURE(checkInvariants(a));
				a.erase(15);
				TEST_ENSURE(checkInvariants(a));
				a.erase(10);
				TEST_ENSURE(checkInvariants(a));
				for (auto&& element : a)
				{
					std::cout << element.key().word(0) << " : "
						<< element.chain()->first.word(0) << " : "
						<< element
						<< std::endl;
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
