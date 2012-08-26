// Description: Testing for insertion sort
// DocumentationOf: insertion_sort.h

#include "test_pastelsys.h"

#include "pastel/sys/insertion_sort.h"

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
			testInsertionSort();
		}

		class A
		{
		public:
			A()
				: data_(0)
			{
			}

			explicit A(integer data)
				: data_(data)
			{
			}

			void swap(A& that)
			{
				std::swap(data_, that.data_);
			}

		private:
			integer data_;
		};

		void swap(A& left, A& right)
		{
			left.swap(right);
		}

		void testInsertionSort()
		{
			const integer n = 100;

			std::vector<integer> v;
			v.reserve(n);
			for (integer i = 0;i < n;++i)
			{
				v.push_back(i);
			}

			std::random_shuffle(v.begin(), v.end());

			insertionSort(v.begin(), v.end());

			for (integer i = 0;i < n;++i)
			{
				TEST_ENSURE_OP(v[i], ==, i);
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
		testRunner().add("insertionSort ", test);
	}

	CallFunction run(addTest);

}
