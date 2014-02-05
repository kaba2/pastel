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
				a.insert(9);
				a.insert(5);
				a.insert(4);
				a.insert(6);
				a.insert(3);
				a.insert(7);
				a.insert(8);
				a.insert(2);
				a.insert(0);

				integer correctSet[] = 
				{
					0, 1, 2, 3, 4, 5, 6, 7, 8, 9
				};

				/*
				TEST_ENSURE(boost::equal(a, correctSet, 
					[&](const Data& left, integer right)
					{
						return left.key() == right;
					}));
				*/
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
		//testRunner().add("CFastTrie", test);
	}

	CallFunction run(addTest);

}
