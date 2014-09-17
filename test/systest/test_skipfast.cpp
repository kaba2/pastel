// Description: Testing for the skip-fast trie
// DocumentationOf: skipfast.h

#include "test_pastelsys.h"

#if 0

#include <pastel/sys/skipfast.h>
#include <boost/range/algorithm/for_each.hpp>

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
			testInsertErase();
			testFind();
			testSet();
			testInsertMore();
		}

		using Map = SkipFast_Map<5, integer>;
		using Set = SkipFast_Set<5>;
		using Key = Map::Key;
		using Value = Map::Value;

		template <typename Range>
		bool keysEqual(
			const Range& a, 
			std::initializer_list<integer> b)
		{
			auto i = std::begin(a);
			auto aEnd = std::end(a);
			auto j = std::begin(b);
			auto bEnd = std::end(b);
			while (i != aEnd && j != bEnd)
			{
				if (i.key() != *j)
				{
					return false;
				}
				++i;
				++j;
			}
			if (i != aEnd || j != bEnd)
			{
				return false;
			}
			//return boost::equal(a.crange().dereferenceToKey(), b);
			return true;
		}

		bool valuesEqual(
			const Map& a, 
			std::initializer_list<integer> b)
		{
				return boost::equal(a, b);
		}

		void testInsertMore()
		{
			SkipFast_Set<6> a;
			TEST_ENSURE(testInvariants(a));

			a.insert(0x36);
			TEST_ENSURE(testInvariants(a));

			a.insert(0x39);
			TEST_ENSURE(testInvariants(a));
		}

		void testInsertErase()
		{
			Map a;

			{
				a.insert(1, 11);
				TEST_ENSURE(testInvariants(a));

				TEST_ENSURE(keysEqual(a, { 1 }));
				TEST_ENSURE(valuesEqual(a, { 11 }));
			}
			{
				TEST_ENSURE(a.clast().key() == 1);
			}
			{
				a.insert(9, 19);
				TEST_ENSURE(testInvariants(a));

				TEST_ENSURE(keysEqual(a, { 1, 9 }));
				TEST_ENSURE(valuesEqual(a, { 11, 19 }));
			}
			{
				a.insert(5, 15);
				TEST_ENSURE(testInvariants(a));

				TEST_ENSURE(keysEqual(a, { 1, 5, 9 }));
				TEST_ENSURE(valuesEqual(a, { 11, 15, 19 }));
			}
			{
				a.insert(4, 14);
				TEST_ENSURE(testInvariants(a));

				TEST_ENSURE(keysEqual(a, { 1, 4, 5, 9 }));
				TEST_ENSURE(valuesEqual(a, { 11, 14, 15, 19 }));
			}
			{
				a.insert(6, 16);
				TEST_ENSURE(testInvariants(a));

				TEST_ENSURE(keysEqual(a, { 1, 4, 5, 6, 9 }));
				TEST_ENSURE(valuesEqual(a, { 11, 14, 15, 16, 19 }));
			}
			{
				a.insert(3, 13);
				TEST_ENSURE(testInvariants(a));

				TEST_ENSURE(keysEqual(a, { 1, 3, 4, 5, 6, 9 }));
				TEST_ENSURE(valuesEqual(a, { 11, 13, 14, 15, 16, 19 }));
			}
			{
				a.insert(7, 17);
				TEST_ENSURE(testInvariants(a));

				TEST_ENSURE(keysEqual(a, { 1, 3, 4, 5, 6, 7, 9 }));
				TEST_ENSURE(valuesEqual(a, { 11, 13, 14, 15, 16, 17, 19 }));
			}
			{
				a.insert(8, 18);
				TEST_ENSURE(testInvariants(a));

				TEST_ENSURE(keysEqual(a, { 1, 3, 4, 5, 6, 7, 8, 9 }));
				TEST_ENSURE(valuesEqual(a, { 11, 13, 14, 15, 16, 17, 18, 19 }));
			}
			{
				a.insert(2, 12);
				TEST_ENSURE(testInvariants(a));

				TEST_ENSURE(keysEqual(a, { 1, 2, 3, 4, 5, 6, 7, 8, 9 }));
				TEST_ENSURE(valuesEqual(a, { 11, 12, 13, 14, 15, 16, 17, 18, 19 }));
			}
			{
				a.insert(0, 10);
				TEST_ENSURE(testInvariants(a));

				TEST_ENSURE(keysEqual(a, { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }));
				TEST_ENSURE(valuesEqual(a, { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 }));
			}
			{
				a.insert(9, 19);
				TEST_ENSURE(testInvariants(a));

				TEST_ENSURE(keysEqual(a, { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
				TEST_ENSURE(valuesEqual(a, { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 }));
			}
			{
				a.insert(10, 20);
				TEST_ENSURE(testInvariants(a));

				TEST_ENSURE(keysEqual(a, { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }));
				TEST_ENSURE(valuesEqual(a, { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 }));
			}
			{
				a.insert(15, 25);
				TEST_ENSURE(testInvariants(a));

				TEST_ENSURE(keysEqual(a, { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15 }));
				TEST_ENSURE(valuesEqual(a, { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 25 }));
			}
			{
				a.erase(9);
				TEST_ENSURE(testInvariants(a));

				TEST_ENSURE(keysEqual(a, { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 15 }));
				TEST_ENSURE(valuesEqual(a, { 10, 11, 12, 13, 14, 15, 16, 17, 18, 20, 25 }));
			}
			{
				a.erase(0);
				TEST_ENSURE(testInvariants(a));

				TEST_ENSURE(keysEqual(a, { 1, 2, 3, 4, 5, 6, 7, 8, 10, 15 }));
				TEST_ENSURE(valuesEqual(a, { 11, 12, 13, 14, 15, 16, 17, 18, 20, 25 }));
			}
			{
				a.erase(6);
				TEST_ENSURE(testInvariants(a));

				TEST_ENSURE(keysEqual(a, { 1, 2, 3, 4, 5, 7, 8, 10, 15 }));
				TEST_ENSURE(valuesEqual(a, { 11, 12, 13, 14, 15, 17, 18, 20, 25 }));
			}
			/*
			{
				a.erase(7);
				TEST_ENSURE(testInvariants(a));

				TEST_ENSURE(keysEqual(a, { 1, 2, 3, 4, 5, 8, 10, 15 }));
				TEST_ENSURE(valuesEqual(a, { 11, 12, 13, 14, 15, 18, 20, 25 }));
			}
			{
				a.erase(8);
				TEST_ENSURE(testInvariants(a));

				TEST_ENSURE(keysEqual(a, { 1, 2, 3, 4, 5, 10, 15 }));
				TEST_ENSURE(valuesEqual(a, { 11, 12, 13, 14, 15, 20, 25 }));
			}
			{
				a.erase(2);
				TEST_ENSURE(testInvariants(a));

				TEST_ENSURE(keysEqual(a, { 1, 3, 4, 5, 10, 15 }));
				TEST_ENSURE(valuesEqual(a, { 11, 13, 14, 15, 20, 25 }));
			}
			{
				a.erase(3);
				TEST_ENSURE(testInvariants(a));

				TEST_ENSURE(keysEqual(a, { 1, 4, 5, 10, 15 }));
				TEST_ENSURE(valuesEqual(a, { 11, 14, 15, 20, 25 }));
			}
			{
				a.erase(4);
				TEST_ENSURE(testInvariants(a));

				TEST_ENSURE(keysEqual(a, { 1, 5, 10, 15 }));
				TEST_ENSURE(valuesEqual(a, { 11, 15, 20, 25 }));
			}
			{
				a.erase(5);
				TEST_ENSURE(testInvariants(a));

				TEST_ENSURE(keysEqual(a, { 1, 10, 15 }));
				TEST_ENSURE(valuesEqual(a, { 11, 20, 25 }));
			}
			{
				a.erase(1);
				TEST_ENSURE(testInvariants(a));

				TEST_ENSURE(keysEqual(a, { 10, 15 }));
				TEST_ENSURE(valuesEqual(a, { 20, 25 }));
			}
			{
				a.erase(15);
				TEST_ENSURE(testInvariants(a));

				TEST_ENSURE(keysEqual(a, { 10}));
				TEST_ENSURE(valuesEqual(a, { 20 }));
			}
			{
				a.erase(10);
				TEST_ENSURE(testInvariants(a));

				TEST_ENSURE(keysEqual(a, {}));
				TEST_ENSURE(valuesEqual(a, {}));
			}
			*/
		}

		template <int N>
		void print(const SkipFast_Set<N>& a)
		{
			auto element = a.cbegin();
			while (element != a.cend())
			{
				std::cout << element.key().word(0) << " ";
				++element;
			}
			std::cout << std::endl;
		}

		template <int N, typename Value>
		void print(const SkipFast_Map<N, Value>& a)
		{
			auto group = a.cgroupBegin();
			while (group != a.cgroupEnd())
			{
				std::cout << "{";
				auto chain = group->begin();
				while (chain != group->end())
				{
					std::cout << "[";
					auto element = chain->elementSet_.cbegin();
					while (element != chain->elementSet_.cend())
					{
						std::cout << element.key().word(0);
						++element;
						if (element != chain->elementSet_.cend())
						{
							std::cout << ", ";
						}
					}
					std::cout << "] ";

					++chain;
				}
				std::cout << "}";
				++group;
			}
			std::cout << std::endl;
		}

		void testFind()
		{
			Map a({1, 5, 4, 7, 8, 3, 8, 9});
			
			TEST_ENSURE(a.exists(1));
			TEST_ENSURE(a.exists(3));
			TEST_ENSURE(a.exists(4));
			TEST_ENSURE(a.exists(5));
			TEST_ENSURE(a.exists(7));
			TEST_ENSURE(a.exists(8));
			TEST_ENSURE(a.exists(9));

			TEST_ENSURE(!a.exists(0));
			TEST_ENSURE(!a.exists(2));
			TEST_ENSURE(!a.exists(6));
			TEST_ENSURE(!a.exists(10));
			TEST_ENSURE(!a.exists(11));
			TEST_ENSURE(!a.exists(12));
			TEST_ENSURE(!a.exists(13));
			TEST_ENSURE(!a.exists(14));
			TEST_ENSURE(!a.exists(15));

			TEST_ENSURE(a.find(1) == a.lowerBound(1));
			TEST_ENSURE(a.find(3) == a.lowerBound(3));
			TEST_ENSURE(a.find(4) == a.lowerBound(4));
			TEST_ENSURE(a.find(5) == a.lowerBound(5));
			TEST_ENSURE(a.find(7) == a.lowerBound(7));
			TEST_ENSURE(a.find(8) == a.lowerBound(8));
			TEST_ENSURE(a.find(9) == a.lowerBound(9));

			TEST_ENSURE(a.lowerBound(0).key() == 1);
			TEST_ENSURE(a.lowerBound(1).key() == 1);
			TEST_ENSURE(a.lowerBound(2).key() == 3);
			TEST_ENSURE(a.lowerBound(3).key() == 3);
			TEST_ENSURE(a.lowerBound(4).key() == 4);
			TEST_ENSURE(a.lowerBound(5).key() == 5);
			TEST_ENSURE(a.lowerBound(6).key() == 7);
			TEST_ENSURE(a.lowerBound(7).key() == 7);
			TEST_ENSURE(a.lowerBound(8).key() == 8);
			TEST_ENSURE(a.lowerBound(9).key() == 9);
			TEST_ENSURE(a.lowerBound(10) == a.end());
			TEST_ENSURE(a.lowerBound(11) == a.end());
			TEST_ENSURE(a.lowerBound(12) == a.end());
			TEST_ENSURE(a.lowerBound(13) == a.end());
			TEST_ENSURE(a.lowerBound(14) == a.end());
			TEST_ENSURE(a.lowerBound(15) == a.end());

			TEST_ENSURE(a.upperBound(0).key() == 1);
			TEST_ENSURE(a.upperBound(1).key() == 3);
			TEST_ENSURE(a.upperBound(2).key() == 3);
			TEST_ENSURE(a.upperBound(3).key() == 4);
			TEST_ENSURE(a.upperBound(4).key() == 5);
			TEST_ENSURE(a.upperBound(5).key() == 7);
			TEST_ENSURE(a.upperBound(6).key() == 7);
			TEST_ENSURE(a.upperBound(7).key() == 8);
			TEST_ENSURE(a.upperBound(8).key() == 9);
			TEST_ENSURE(a.upperBound(9) == a.end());
			TEST_ENSURE(a.upperBound(10) == a.end());
			TEST_ENSURE(a.upperBound(11) == a.end());
			TEST_ENSURE(a.upperBound(12) == a.end());
			TEST_ENSURE(a.upperBound(13) == a.end());
			TEST_ENSURE(a.upperBound(14) == a.end());
			TEST_ENSURE(a.upperBound(15) == a.end());

			// Visual Studio 2013, even with the
			// November 2013 CTP compiler, has a bug
			// with using an initializer-list of pairs
			// in the constructor, when using the 
			// () form. This is fixed by removing ().

			//Map b({ { 0, 10 }, { 1, 11 }, { 2, 12 } });
			Map b { { 0, 10 }, { 1, 11 }, { 2, 12 } };

			TEST_ENSURE(keysEqual(b, {0, 1, 2}));
			TEST_ENSURE(valuesEqual(b, { 10, 11, 12 }));
		}

		void testSet()
		{
			{
				Set a({ 3 });
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(keysEqual(a, { 3 }));
			}

			{
				Set a({ 3, 2 });
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(keysEqual(a, { 2, 3 }));
			}
			{
				Set a({ 3, 2, 0, 1 });
				TEST_ENSURE(keysEqual(a, { 0, 1, 2, 3 }));
				*(a.begin());
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
		testRunner().add("SkipFast", test);
	}

	CallFunction run(addTest);

}

#endif