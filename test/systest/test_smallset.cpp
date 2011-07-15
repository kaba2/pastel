// Description: Testing for SmallSet
// DocumentationOf: smallset.h

#include "pastelsystest.h"

#include "pastel/sys/smallset.h"
#include "pastel/sys/log.h"

using namespace Pastel;

namespace
{

	void print(const SmallSet<int>& that)
	{
		log() << "Contents: ";
		for (integer i = 0;i < that.size();++i)
		{
			log() << that[i] << ", ";
		}
		log() << "end." << logNewLine;
		log() << "Size: " << that.size() << logNewLine;
		log() << "Capacity: " << that.capacity() << logNewLine;
	}

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
			testOne();
			testTwoBefore();
			testTwoAfter();
			testTwoSame();
			testThreeMiddle();
			testHundredForward();
			testHundredBackward();
			testOneErase();
			testTwoEraseBefore();
			testTwoEraseAfter();
			testThreeEraseMiddle();
			testEraseNotFound();
			//testSet();
		}

		void testOne()
		{
			SmallSet<int> a;

			{
				a.insert(5);
				print(a);
			}
		}

		void testTwoBefore()
		{
			SmallSet<int> a;

			{
				a.insert(5);
				print(a);
			}
			{
				a.insert(3);
				print(a);
			}
		}

		void testTwoAfter()
		{
			SmallSet<int> a;

			{
				a.insert(5);
				print(a);
			}
			{
				a.insert(7);
				print(a);
			}
		}

		void testTwoSame()
		{
			SmallSet<int> a;

			{
				a.insert(5);
				print(a);
			}
			{
				a.insert(5);
				print(a);
			}
		}

		void testThreeMiddle()
		{
			SmallSet<int> a;

			{
				a.insert(5);
				print(a);
			}
			{
				a.insert(7);
				print(a);
			}
			{
				a.insert(6);
				print(a);
			}
		}

		void testHundredForward()
		{
			SmallSet<int> a;

			for (int i = 0;i < 100;++i)
			{
				a.insert(i);
			}
			print(a);
		}

		void testHundredBackward()
		{
			SmallSet<int> a;

			for (int i = 0;i < 100;++i)
			{
				a.insert(99 - i);
			}
			print(a);
		}

		void testOneErase()
		{
			SmallSet<int> a;

			{
				a.insert(5);
				print(a);
			}

			{
				a.erase(5);
				print(a);
			}
		}

		void testTwoEraseBefore()
		{
			SmallSet<int> a;

			{
				a.insert(5);
				print(a);
			}

			{
				a.insert(7);
				print(a);
			}

			{
				a.erase(5);
				print(a);
			}
		}

		void testTwoEraseAfter()
		{
			SmallSet<int> a;

			{
				a.insert(5);
				print(a);
			}

			{
				a.insert(7);
				print(a);
			}

			{
				a.erase(7);
				print(a);
			}
		}

		void testThreeEraseMiddle()
		{
			SmallSet<int> a;

			{
				a.insert(5);
				print(a);
			}

			{
				a.insert(7);
				print(a);
			}

			{
				a.insert(6);
				print(a);
			}

			{
				a.erase(6);
				print(a);
			}
		}

		void testEraseNotFound()
		{
			SmallSet<int> a;

			{
				a.erase(5);
				print(a);
			}

			{
				a.insert(5);
				print(a);
			}

			{
				a.erase(7);
				print(a);
			}
		}

		/*
		void testSet()
		{
			SmallSet<int> a;

			a.insert(1);
			print(a);
			a.insert(4);
			print(a);
			a.insert(2);
			print(a);
			a.insert(3);
			print(a);

			integer index = 0;

			// 1 2 3 4
			index = a.set(1, 5);
			TEST_ENSURE_OP(index, ==, 3);
			print(a);

			// 1 3 4 5
			index = a.set(0, 2);
			TEST_ENSURE_OP(index, ==, 0);
			print(a);

			// 2 3 4 5
			index = a.set(0, 2);
			TEST_ENSURE_OP(index, ==, 0);
			print(a);

			// 2 3 4 5
			index = a.set(0, 3);
			TEST_ENSURE_OP(index, ==, -1);
			print(a);

			// 3 4 5
			index = a.set(1, 0);
			TEST_ENSURE_OP(index, ==, 0);
			print(a);

			// 0 3 5
			index = a.set(0, 0);
			TEST_ENSURE_OP(index, ==, 0);
			print(a);

			// 0 3 5
			index = a.set(2, 0);
			print(a);

			// 0 3
			a.insert(10);
			print(a);

			// 0 3 10
			a.insert(-10);
			print(a);

			// -10 0 3 10
			index = a.set(a.size() - 1, -20);
			TEST_ENSURE_OP(index, ==, 0);
			print(a);

			// -20 -10 0 3
			index = a.set(0, -40);
			TEST_ENSURE_OP(index, ==, 0);
			print(a);

			// -40 -10 0 3
			index = a.set(0, -30);
			TEST_ENSURE_OP(index, ==, 0);
			print(a);

			// -30 -10 0 3
			index = a.set(0, -30);
			TEST_ENSURE_OP(index, ==, 0);
			print(a);

			// -30 -10 0 3
			index = a.set(0, 20);
			TEST_ENSURE_OP(index, ==, 3);
			print(a);

			// -10 0 3 20
			index = a.set(a.size() - 1, 21);
			TEST_ENSURE_OP(index, ==, a.size() - 1);
			print(a);

			// -10 0 3 21
			index = a.set(a.size() - 1, 20);
			TEST_ENSURE_OP(index, ==, a.size() - 1);
			print(a);

			// -10 0 3 20
			index = a.set(a.size() - 1, 20);
			TEST_ENSURE_OP(index, ==, a.size() - 1);
			print(a);
			// -10 0 3 20
		}
		*/
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("SmallSet", test);
	}

	CallFunction run(addTest);

}
