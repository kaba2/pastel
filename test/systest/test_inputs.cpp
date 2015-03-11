// Description: Testing for inputs
// Documentation: input.txt

#include "test_pastelsys.h"

#include "pastel/sys/input.h"

#include <vector>

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
			testSequential();
			testIndexed();
			testScalar();
			test();
		}

		void testSequential()
		{
			std::list<integer> aSet = { 0, 1, 2, 3 };
			auto input = rangeInput(aSet.begin(), aSet.end());
			TEST_ENSURE(!input.empty());

			// The length of the sequence is not known,
			// since the input range was constructed from two
			// iterators.
			TEST_ENSURE_OP(input.nHint(), == , 0);

			// Since the range is no random-access, this 
			// should not compile.
			//TEST_ENSURE_OP(input[0], == , 0);
		}

		void testIndexed()
		{
			std::vector<integer> aSet = { 0, 1, 2, 3 };
			auto input = rangeInput(aSet.begin(), aSet.end());
			TEST_ENSURE(!input.empty());

			// Since the range is random-access, so is the input.
			TEST_ENSURE_OP(input.n(), == , 4);
			TEST_ENSURE_OP(input[0], == , 0);
			TEST_ENSURE_OP(input[1], == , 1);
			TEST_ENSURE_OP(input[2], == , 2);
			TEST_ENSURE_OP(input[3], == , 3);
		}

		void testScalar()
		{
			{
				auto input = Scalar_Input<integer>();
				TEST_ENSURE_OP(input.n(), == , 1);
				TEST_ENSURE(!input.empty());

				TEST_ENSURE_OP(input.get(), == , 0);
				TEST_ENSURE_OP(input[0], == , 0);
				
				input.pop();
				TEST_ENSURE(input.empty());
			}

			integer n = 10;
			{
				auto input = scalarInput(5, n);
				for (integer i = 0; i < n; ++i)
				{
					TEST_ENSURE(!input.empty());
					TEST_ENSURE_OP(input.n(), == , n);

					integer j = input[i];
					TEST_ENSURE_OP(j, == , 5);
				}
			}
			
			{
				auto input = scalarInput(5, n);
				for (integer i = 0; i < n; ++i)
				{
					TEST_ENSURE(!input.empty());
					TEST_ENSURE_OP(input.n(), == , n - i);

					integer j = input.get();
					TEST_ENSURE_OP(j, == , 5);

					input.pop();
				}
				TEST_ENSURE(input.empty());
			}

			{
				auto input = scalarInput(5);
				TEST_ENSURE(!input.empty());
				TEST_ENSURE_OP(input.n(), == , 1);

				integer j = input.get();
				TEST_ENSURE_OP(j, == , 5);

				input.pop();
				TEST_ENSURE(input.empty());
			}
		}

		void test()
		{
			{
				auto input = countingInput();
				for (integer i = 0; i < 50;++i)
				{
					TEST_ENSURE(!input.empty());
					
					integer j = input.get();
					input.pop();
					TEST_ENSURE_OP(j, ==, i);
				}
			}

			{
				auto input = countingInput(-50);
				for (integer i = -50; i < 50;++i)
				{
					TEST_ENSURE(!input.empty());

					integer j = input.get();
					input.pop();
					TEST_ENSURE_OP(j, ==, i);
				}
			}

			{
				auto input = transformInput(
					countingInput(-50),
					[&](integer i)
				{
					return i * i;
				});

				for (integer i = -50; i < 50; ++i)
				{
					TEST_ENSURE(!input.empty());

					integer j = input.get();
					input.pop();
					TEST_ENSURE_OP(j, ==, i * i);
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
		testRunner().add("Inputs", test);
	}

	CallFunction run(addTest);

}
