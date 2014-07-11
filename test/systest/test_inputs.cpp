// Description: Testing for inputs
// Documentation: inputs.txt

#include "test_pastelsys.h"

#include "pastel/sys/inputs.h"

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
