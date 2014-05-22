// Description: Testing for the skip-fast trie
// DocumentationOf: skipfast.h

#include "test_pastelsys.h"

#include <pastel/sys/skipfast.h>
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
			testInsertErase();
			/*
			testFind();
			testSet();
			testInsertMore();
			*/
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
			/*
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
			*/
			/*
			{
				a.erase(9);
				TEST_ENSURE(testInvariants(a));

				TEST_ENSURE(keysEqual(a, { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 15 }));
				TEST_ENSURE(valuesEqual(a, { 10, 11, 12, 13, 14, 15, 16, 17, 18, 20, 25 }));
			}
			{
				a.erase(Map::Key(0));
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
			auto chain = a.cbegin();
			while (chain != a.cend())
			{
				std::cout << chain.key().word(0) << " ";
				++chain;
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

		template <int N>
		bool keyCreates(SkipFast_Set<N>& a, integer key, integer chain)
		{
			bool chainExistsBefore =
				a.chainExists(chain);

			a.insert(key);

			bool chainExistsAfter =
				a.chainExists(chain);
			bool chainCreated =
				!chainExistsBefore && chainExistsAfter;

			return testInvariants(a) && chainCreated;
		};

		void testInsertMore()
		{
			{
				SkipFast_Set<6> a{ 31, 61, 45, 2, 33, 26, 63, 50, 8, 17, 11, 28, 54, 62, 52, 
					9, 25, 7, 20, 30, 40, 1, 36, 18, 15, 51, 37, 57, 49, 13, 14, 29, 4, 44, 
					0, 32, 60, 41, 19};
				TEST_ENSURE(testInvariants(a));

				a.insert(47);
				TEST_ENSURE(testInvariants(a));
				print(a);
			}
			/*
			{
				SkipFast_Set<1> a;
				TEST_ENSURE(testInvariants(a));

				auto test = [&](integer key, integer chain)
				{
					return keyCreates(a, key, chain);
				};

				TEST_ENSURE(test(0, 0));
				TEST_ENSURE(test(1, 1));
			}
			{
				SkipFast_Set<2> a;
				TEST_ENSURE(testInvariants(a));

				auto test = [&](integer key, integer chain)
				{
					return keyCreates(a, key, chain);
				};

				TEST_ENSURE(test(0, 0));
				TEST_ENSURE(test(1, 1));
				TEST_ENSURE(test(2, 3));
				TEST_ENSURE(test(3, 2));
			}
			{
				SkipFast_Set<3> a;
				TEST_ENSURE(testInvariants(a));

				auto test = [&](integer key, integer chain)
				{
					return keyCreates(a, key, chain);
				};

				TEST_ENSURE(test(0, 0));
				TEST_ENSURE(test(1, 1));
				TEST_ENSURE(test(2, 3));
				TEST_ENSURE(test(3, 2));
				TEST_ENSURE(test(4, 7));
				TEST_ENSURE(test(5, 4));
				TEST_ENSURE(test(6, 6));
				TEST_ENSURE(test(7, 5));
			}
			{
				SkipFast_Set<4> a;
				TEST_ENSURE(testInvariants(a));

				auto test = [&](integer key, integer chain)
				{
					return keyCreates(a, key, chain);
				};

				TEST_ENSURE(test(0, 0));
				TEST_ENSURE(test(1, 1));
				TEST_ENSURE(test(2, 3));
				TEST_ENSURE(test(3, 2));
				TEST_ENSURE(test(4, 7));
				TEST_ENSURE(test(5, 4));
				TEST_ENSURE(test(6, 6));
				TEST_ENSURE(test(7, 5));
				TEST_ENSURE(test(8, 15));
				TEST_ENSURE(test(9, 8));
				TEST_ENSURE(test(10, 11));
				TEST_ENSURE(test(11, 10));
				TEST_ENSURE(test(12, 12));
				TEST_ENSURE(test(13, 13));
				TEST_ENSURE(test(14, 14));
				TEST_ENSURE(test(15, 9));
			}
			*/

			{
				SkipFast_Set<5> a{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
				TEST_ENSURE(testInvariants(a));
			}
			/*
			{
				SkipFast_Set<5> a{ 5, 9, 23, 25, 26, 31};
				a.insert(19);
				TEST_ENSURE(testInvariants(a));
			}
			{
				SkipFast_Set<5> a{ 1, 4, 12, 18, 24 };
				a.insert(13);
				TEST_ENSURE(testInvariants(a));
			}
			*/
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
