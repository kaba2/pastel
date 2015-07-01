// Description: Testing for multi-sets
// Documentation: unit_testing.txt

#include "test_pastelsys.h"

#include "pastel/sys/set.h"
#include "pastel/sys/function.h"

#include <unordered_set>

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
			testInterval();
			testConstant();
			testUnion();
			testTransformed();
		}

		template <typename Set, typename Element>
		using CorrectElement = 
			std::is_same<
				Set_Element<Set>,
				Element
			>;

		void testInterval()
		{
			PASTEL_CONCEPT_CHECK(
				Interval_Set<integer>, Set_Concept);
			PASTEL_STATIC_ASSERT(
				CorrectElement<Interval_Set<integer>, integer>::value);
			{
				integer n = 8;
				auto set = intervalSet((integer)3, (integer)3 + n);
				auto index = set.index();
				set.next(index);
				TEST_ENSURE_OP(set.n(), ==, n);

				PASTEL_STATIC_ASSERT(
					CorrectElement<decltype(set), integer>::value);

				std::unordered_set<integer> actualSet;

				forEach(set, [&](integer a)
				{
					actualSet.insert(a);
					return true;
				});
				TEST_ENSURE_OP(actualSet.size(), ==, n);

				for (integer i = 0;i < n;++i)
				{
					TEST_ENSURE(actualSet.count(3 + i) == 1);
				}
			}
		}

		void testConstant()
		{
			PASTEL_CONCEPT_CHECK(
				Constant_Set<integer>, Set_Concept);

			{
				integer n = 4;
				integer element = 7;
				Constant_Set<integer> set(n, element);

				PASTEL_STATIC_ASSERT(
					CorrectElement<decltype(set), integer>::value);

				integer m = 0;
				bool correct = forEach(set, [&](integer a)
				{
					if (a != element)
					{
						return false;
					}
					++m;

					return true;
				});

				TEST_ENSURE(correct);
				TEST_ENSURE_OP(m, ==, n);
			}
			{
				integer n = 0;
				integer element = 7;
				Constant_Set<integer> set(n, element);

				bool correct = forEach(set, [&](integer a)
				{
					return false;
				});

				TEST_ENSURE(correct);
			}
		}

		void testUnion()
		{
			using Union = Union_Set<
					Constant_Set<integer>,
					Constant_Set<integer>>;
			PASTEL_CONCEPT_CHECK(Union, Set_Concept);

			{
				integer nA = 5;
				integer aElement = 7;
				Constant_Set<integer> aSet(nA, aElement);

				integer nB = 3;
				integer bElement = 3;
				Constant_Set<integer> bSet(nB, bElement);

				auto abSet = unionSet(aSet, bSet);
				
				TEST_ENSURE_OP(abSet.n(), ==, 5 + 3);
				PASTEL_STATIC_ASSERT(
					std::is_same<
						Set_Element<decltype(abSet)>,
						integer
					>::value);

				std::unordered_multiset<integer> actualSet;
				forEach(abSet, [&](integer a)
				{
					actualSet.insert(a);
					return true;
				});

				TEST_ENSURE_OP(actualSet.count(aElement), ==, 5);
				TEST_ENSURE_OP(actualSet.count(bElement), ==, 3);
			}
		}

		void testTransformed()
		{
			std::vector<integer> dataSet = {0, 1, 2, 3, 4};
			integer n = dataSet.size();

			auto inputSet = intervalSet(
				dataSet.begin(), dataSet.end());
			
			auto dereferencedSet = transformedSet(
				inputSet, Dereference_Function());

			std::unordered_set<integer> actualSet;
			forEach(dereferencedSet, [&](integer x)
			{
				actualSet.insert(x);
				return true;
			});

			TEST_ENSURE_OP(actualSet.size(), ==, n);
			for (integer i = 0;i < n;++i)
			{
				TEST_ENSURE_OP(actualSet.count(i), ==, 1);
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
		testRunner().add("Set", test);
	}

	CallFunction run(addTest);

}
