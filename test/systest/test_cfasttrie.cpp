// Description: Testing for the c-fast trie
// DocumentationOf: cfasttrie.h

#include "test_pastelsys.h"

#include <pastel/sys/cfasttrie.h>
#include <boost/range/algorithm/for_each.hpp>

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

			auto keysEqual = [&](std::initializer_list<integer> b)
			{
				return boost::equal(a.ckeyRange(), b);
			};

			auto valuesEqual = [&](std::initializer_list<integer> b)
			{
				return boost::equal(a, b);
			};

			{
				a.insert(1, 11);
				TEST_ENSURE(checkInvariants(a));

				TEST_ENSURE(keysEqual({ 1 }));
				TEST_ENSURE(valuesEqual({ 11 }));
			}
			{
				a.insert(9, 19);
				TEST_ENSURE(checkInvariants(a));

				TEST_ENSURE(keysEqual({ 1, 9 }));
				TEST_ENSURE(valuesEqual({ 11, 19 }));
			}
			{
				a.insert(5, 15);
				TEST_ENSURE(checkInvariants(a));

				TEST_ENSURE(keysEqual({ 1, 5, 9 }));
				TEST_ENSURE(valuesEqual({ 11, 15, 19 }));
			}
			{
				a.insert(4, 14);
				TEST_ENSURE(checkInvariants(a));

				TEST_ENSURE(keysEqual({ 1, 4, 5, 9 }));
				TEST_ENSURE(valuesEqual({ 11, 14, 15, 19 }));
			}
			{
				a.insert(6, 16);
				TEST_ENSURE(checkInvariants(a));
				
				TEST_ENSURE(keysEqual({ 1, 4, 5, 6, 9 }));
				TEST_ENSURE(valuesEqual({ 11, 14, 15, 16, 19 }));
			}
			{
				a.insert(3, 13);
				TEST_ENSURE(checkInvariants(a));

				TEST_ENSURE(keysEqual({ 1, 3, 4, 5, 6, 9 }));
				TEST_ENSURE(valuesEqual({ 11, 13, 14, 15, 16, 19 }));
			}
			{
				a.insert(7, 17);
				TEST_ENSURE(checkInvariants(a));

				TEST_ENSURE(keysEqual({ 1, 3, 4, 5, 6, 7, 9 }));
				TEST_ENSURE(valuesEqual({ 11, 13, 14, 15, 16, 17, 19 }));
			}
			{
				a.insert(8, 18);
				TEST_ENSURE(checkInvariants(a));

				TEST_ENSURE(keysEqual({ 1, 3, 4, 5, 6, 7, 8, 9 }));
				TEST_ENSURE(valuesEqual({ 11, 13, 14, 15, 16, 17, 18, 19 }));
			}
			{
				a.insert(2, 12);
				TEST_ENSURE(checkInvariants(a));

				TEST_ENSURE(keysEqual({ 1, 2, 3, 4, 5, 6, 7, 8, 9 }));
				TEST_ENSURE(valuesEqual({ 11, 12, 13, 14, 15, 16, 17, 18, 19 }));
			}
			{
				a.insert(0, 10);
				TEST_ENSURE(checkInvariants(a));

				TEST_ENSURE(keysEqual({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }));
				TEST_ENSURE(valuesEqual({ 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 }));
			}
			{
				a.insert(9, 15);
				TEST_ENSURE(checkInvariants(a));

				TEST_ENSURE(keysEqual({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
				TEST_ENSURE(valuesEqual({ 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 }));
			}
			{
				a.insert(10, 20);
				TEST_ENSURE(checkInvariants(a));

				TEST_ENSURE(keysEqual({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }));
				TEST_ENSURE(valuesEqual({ 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 }));
			}
			{
				a.insert(15, 25);
				TEST_ENSURE(checkInvariants(a));

				TEST_ENSURE(keysEqual({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15 }));
				TEST_ENSURE(valuesEqual({ 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 25 }));
			}
			{
				a.erase(9);
				TEST_ENSURE(checkInvariants(a));

				TEST_ENSURE(keysEqual({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 15 }));
				TEST_ENSURE(valuesEqual({ 10, 11, 12, 13, 14, 15, 16, 17, 18, 20, 25 }));
			}
			{
				a.erase(0);
				TEST_ENSURE(checkInvariants(a));

				TEST_ENSURE(keysEqual({ 1, 2, 3, 4, 5, 6, 7, 8, 10, 15 }));
				TEST_ENSURE(valuesEqual({ 11, 12, 13, 14, 15, 16, 17, 18, 20, 25 }));
			}
			{
				a.erase(6);
				TEST_ENSURE(checkInvariants(a));

				TEST_ENSURE(keysEqual({ 1, 2, 3, 4, 5, 7, 8, 10, 15 }));
				TEST_ENSURE(valuesEqual({ 11, 12, 13, 14, 15, 17, 18, 20, 25 }));
			}
			{
				a.erase(7);
				TEST_ENSURE(checkInvariants(a));

				TEST_ENSURE(keysEqual({ 1, 2, 3, 4, 5, 8, 10, 15 }));
				TEST_ENSURE(valuesEqual({ 11, 12, 13, 14, 15, 18, 20, 25 }));
			}
			{
				a.erase(8);
				TEST_ENSURE(checkInvariants(a));

				TEST_ENSURE(keysEqual({ 1, 2, 3, 4, 5, 10, 15 }));
				TEST_ENSURE(valuesEqual({ 11, 12, 13, 14, 15, 20, 25 }));
			}
			{
				a.erase(2);
				TEST_ENSURE(checkInvariants(a));

				TEST_ENSURE(keysEqual({ 1, 3, 4, 5, 10, 15 }));
				TEST_ENSURE(valuesEqual({ 11, 13, 14, 15, 20, 25 }));
			}
			{
				a.erase(3);
				TEST_ENSURE(checkInvariants(a));

				TEST_ENSURE(keysEqual({ 1, 4, 5, 10, 15 }));
				TEST_ENSURE(valuesEqual({ 11, 14, 15, 20, 25 }));
			}
			{
				a.erase(4);
				TEST_ENSURE(checkInvariants(a));

				TEST_ENSURE(keysEqual({ 1, 5, 10, 15 }));
				TEST_ENSURE(valuesEqual({ 11, 15, 20, 25 }));
			}
			{
				a.erase(5);
				TEST_ENSURE(checkInvariants(a));

				TEST_ENSURE(keysEqual({ 1, 10, 15 }));
				TEST_ENSURE(valuesEqual({ 11, 20, 25 }));
			}
			{
				a.erase(1);
				TEST_ENSURE(checkInvariants(a));

				TEST_ENSURE(keysEqual({ 10, 15 }));
				TEST_ENSURE(valuesEqual({ 20, 25 }));
			}
			{
				a.erase(15);
				TEST_ENSURE(checkInvariants(a));

				TEST_ENSURE(keysEqual({ 10}));
				TEST_ENSURE(valuesEqual({ 20 }));
			}
			{
				a.erase(10);
				TEST_ENSURE(checkInvariants(a));

				TEST_ENSURE(keysEqual({}));
				TEST_ENSURE(valuesEqual({}));
			}
			{
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
