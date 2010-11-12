#include "pastelsystest.h"

#include "pastel/sys/array.h"
#include "pastel/sys/countingiterator.h"

using namespace Pastel;

namespace
{

	class Array_Test
		: public TestSuite
	{
	public:
		Array_Test()
			: TestSuite(&sysTestReport())
		{
		}

		virtual void run()
		{
			testTrivial();
			testSubArray();
			testIterator();
		}

		void testTrivial()
		{
			Array<int, 2> a(1024, 1024);
			Array<int, 2> b(a);
			a = b;
			b.clear();
			b = a;
			a.clear();
			a.setExtent(53, 45, 15);
		}

		void print(const Array<integer, 2>& that)
		{
			const integer height = that.height();
			const integer width = that.width();
			for (integer y = 0;y < height;++y)
			{
				for (integer x = 0;x < width;++x)
				{
					std::cout << that(x, y) << " ";
				}
				std::cout << std::endl;
			}
		}

		void testIterator()
		{
			Array<integer, 2> a(6, 6);
			for (integer i = 0;i < a.size();++i)
			{
				a(i) = i;
			}
			
			Array<integer, 2> b(6, 6);
			std::copy(
				countingIterator(0), countingIterator(b.size()),
				b.begin());

			TEST_ENSURE(std::equal(a.begin(), a.end(), b.begin()));

			for (integer i = 0;i < a.width();++i)
			{
				a(i, 0) = a(i, 1);
			}

			std::copy(
				a.rowBegin(1), a.rowEnd(1),
				a.rowBegin(0));
			
			TEST_ENSURE(std::equal(a.rowBegin(0), a.rowEnd(0), a.rowBegin(1)));
		}

		void testSubArray()
		{
			Array<integer, 2> a(6, 6);

			a |= 0, 1, 2, 3, 4, 5, 
				6, 7, 8, 9, 10, 11, 
				12, 13, 14, 15, 16, 17, 
				18, 19, 20, 21, 22, 23, 
				24, 25, 26, 27, 28, 29, 
				30, 31, 32, 33, 34, 35;

			std::copy(
				a.begin(), a.end(), 
				std::ostream_iterator<integer>(std::cout, " "));
			std::cout << std::endl;

			TEST_ENSURE(
				std::equal(a.begin(), a.end(),
				countingIterator(0)));

			a(Vector2i(0, 0), Vector2i(3, 3)) = 
				a(Vector2i(3, 3), Vector2i(6, 6));
			
			std::copy(
				a.begin(), a.end(), 
				std::ostream_iterator<integer>(std::cout, " "));
			std::cout << std::endl;

			Array<integer, 2> b(6, 6);
			b |= 21, 22, 23, 3, 4, 5, 
				27, 28, 29, 9, 10, 11, 
				33, 34, 35, 15, 16, 17, 
				18, 19, 20, 21, 22, 23, 
				24, 25, 26, 27, 28, 29, 
				30, 31, 32, 33, 34, 35;
			
			TEST_ENSURE(
				std::equal(a.begin(), a.end(),
				b.begin()));

			a(Vector2i(0, 0), Vector2i(6, 6), Vector2i(2, 2)) = 0;

			b |= 0, 22, 0, 3, 0, 5, 
				27, 28, 29, 9, 10, 11, 
				0, 34, 0, 15, 0, 17, 
				18, 19, 20, 21, 22, 23, 
				0, 25, 0, 27, 0, 29, 
				30, 31, 32, 33, 34, 35;

			TEST_ENSURE(
				std::equal(a.begin(), a.end(),
				b.begin()));

			a(Vector2i(1, 1), Vector2i(6, 6), Vector2i(2, 2)) = 1;

			b |= 0, 22, 0, 3, 0, 5, 
				27, 1, 29, 1, 10, 1, 
				0, 34, 0, 15, 0, 17, 
				18, 1, 20, 1, 22, 1, 
				0, 25, 0, 27, 0, 29, 
				30, 1, 32, 1, 34, 1;

			TEST_ENSURE(
				std::equal(a.begin(), a.end(),
				b.begin()));

			a(Vector2i(2, 2), Vector2i(-1, -1), Vector2i(-1, -1)) = 
				a(Vector2i(3, 0), Vector2i(6, 3));			

			b |= 17, 0, 15, 3, 0, 5, 
				1, 10, 1, 1, 10, 1, 
				5, 0, 3, 15, 0, 17, 
				18, 1, 20, 1, 22, 1, 
				0, 25, 0, 27, 0, 29, 
				30, 1, 32, 1, 34, 1;

			TEST_ENSURE(
				std::equal(a.begin(), a.end(),
				b.begin()));

			SubArray<integer> c(a(Vector2i(0, 0), Vector2i(2, 2)));
			c = 2;

			b |= 2, 2, 15, 3, 0, 5, 
				2, 2, 1, 1, 10, 1, 
				5, 0, 3, 15, 0, 17, 
				18, 1, 20, 1, 22, 1, 
				0, 25, 0, 27, 0, 29, 
				30, 1, 32, 1, 34, 1;

			TEST_ENSURE(
				std::equal(a.begin(), a.end(),
				b.begin()));

			f(c, 3);

			b |= 3, 3, 15, 3, 0, 5, 
				3, 3, 1, 1, 10, 1, 
				5, 0, 3, 15, 0, 17, 
				18, 1, 20, 1, 22, 1, 
				0, 25, 0, 27, 0, 29, 
				30, 1, 32, 1, 34, 1;

			TEST_ENSURE(
				std::equal(a.begin(), a.end(),
				b.begin()));
		}

		void f(const SubArray<integer>& that,
			integer value)
		{
			that = value;
		}
	};

	void testArray()
	{
		Array_Test test;
		test.run();
	}

	void addTest()
	{
		sysTestList().add("Array", testArray);
	}

	CallFunction run(addTest);

}
