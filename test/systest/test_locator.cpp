// Description: Testing for locators
// DocumentationOf: locator.h

#include "test_pastelsys.h"

#include <pastel/sys/locator.h>

namespace
{

	using namespace Pastel;

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
			testScalar();
			testArray();
			testTransform();
			testTypes();
		}

		void test()
		{
			enum : integer
			{
				D = 3
			};
			
			using Locator = Pointer_Locator<real, D>;
			PASTEL_CONCEPT_CHECK(Locator, Locator_Concept);

			Locator locator(D);
			real dataSet[] = {0, 1, 2, 3, 4, 5, 6};
			integer n = sizeof(dataSet) / sizeof(real);
			
			for (integer i = 0;i < n - D;++i)
			{
				for (integer j = 0;j < D;++j)
				{
					TEST_ENSURE(locator(dataSet + i, j) == i + j);
				}
			}
		}

		void testScalar()
		{
			using DataSet = std::vector<real>;
			using Data_ConstIterator = 
				DataSet::const_iterator;

			DataSet dataSet = {0, 7, 3, 5, 11, 13};

			auto locator = scalarLocator<real, 1>();

			for (integer i = 0;i < dataSet.size();++i)
			{
				TEST_ENSURE_OP(locator(dataSet[i], 0), ==, dataSet[i]);
			}
		}

		void testTransform()
		{
			using DataSet = std::vector<real>;
			using Data_ConstIterator = 
				DataSet::const_iterator;

			DataSet dataSet = {0, 1, 2, 3, 4, 5};

			auto indirectLocator = 
				transformLocator<Data_ConstIterator>(
					scalarLocator<real>(1),
					[](const Data_ConstIterator& iPoint)
					{
						return *iPoint;
					}
				);

			for (integer i = 0;i < dataSet.size();++i)
			{
				TEST_ENSURE_OP(indirectLocator(dataSet.begin() + i, 0), ==, i);
			}
		}

		void testArray()
		{
			{
				using Locator = Array_Locator<real, 1>;
				PASTEL_CONCEPT_CHECK(Locator, Locator_Concept);
			}
			{
				using Point = std::array<real, 2>;
				using Locator = Array_Locator<real, 2>;
				PASTEL_CONCEPT_CHECK(Locator, Locator_Concept);

				PASTEL_STATIC_ASSERT(
					(std::is_same<Locator_Real<Locator>, real>::value));

				PASTEL_STATIC_ASSERT(
					(std::is_same<Locator_Point<Locator>, Point>::value));
				
				PASTEL_STATIC_ASSERT(
					Locator_N<Locator>::value == 2);
			}
		}

		void testTypes()
		{
			PASTEL_STATIC_ASSERT(
				(std::is_same<Locator_Real<Pointer_Locator<real>>, real>::value));

			PASTEL_STATIC_ASSERT(
				(std::is_same<Locator_Real<Pointer_Locator<real>, Pointer_Locator<integer>>, real>::value));

			PASTEL_STATIC_ASSERT(
				(std::is_same<Locator_Real<Pointer_Locator<integer>, Pointer_Locator<real>>, real>::value));

			PASTEL_STATIC_ASSERT(
				(std::is_same<Locator_Point<Pointer_Locator<integer>>, const integer*>::value));

			TEST_ENSURE_OP((Locator_N<Pointer_Locator<real, 0>>::value), ==, 0);
			TEST_ENSURE_OP((Locator_N<Pointer_Locator<real, 1>>::value), ==, 1);
			TEST_ENSURE_OP((Locator_N<Pointer_Locator<real, 2>>::value), ==, 2);
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("Locator", test);
	}

	CallFunction run(addTest);

}
