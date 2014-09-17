// Description: Testing for class wrapping
// DocumentationOf: class.h

#include "test_pastelsys.h"

#include <pastel/sys/class.h>

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
			testNonClass();
			testClass();
		}

		class A
		{
		public:
			A()
			: a_(0)
			{
			}

			A(const A& that)
			: a_(that.a_)
			{
			}

			A(A&& that)
			: A()
			{
				swap(that);
			}

			void swap(A& that)
			{
				std::swap(a_, that.a_);
			}

			explicit A(int a)
			: a_(a)
			{
			}

			int a() const
			{
				return a_;
			}

			A& operator=(int a)
			{
				a_ = a;
				return *this;
			}

		private:
			int a_;
		};

		void testClass()
		{
			{
				Class<A> a;
				TEST_ENSURE_OP(a.a(), ==, 0);
			}

			{
				Class<A> a(2);
				TEST_ENSURE_OP(a.a(), ==, 2);
			}

			{
				Class<A> a;
				a = 2;
				TEST_ENSURE_OP(a.a(), ==, 2);
			}

			{
				A b(2);
				
				Class<A> a(std::move(b));
				TEST_ENSURE_OP(a.a(), ==, 2);
				TEST_ENSURE_OP(b.a(), ==, 0);
			}

			{
				A b(2);
				
				Class<A> a(b);
				TEST_ENSURE_OP(a.a(), ==, 2);
				TEST_ENSURE_OP(b.a(), ==, 2);
			}
		}

		void testNonClass()
		{
			{
				Class<int> a(0);
				TEST_ENSURE_OP(a, ==, 0);

				a = 0;
			}

			{
				Class<int> a(5);
				TEST_ENSURE_OP(a, ==, 5);
			}

			{
				Class<int> a(5);
				a = 3;
				TEST_ENSURE_OP(a, ==, 3);
			}

			{
				Class<int> a(5);
				a += 3;
				TEST_ENSURE_OP(a, ==, 8);
			}

			{
				Class<void> c;
				c = nullptr;
				TEST_ENSURE(c == nullptr);
			}

			{
				Class<void> c(nullptr);
				TEST_ENSURE(c == nullptr);
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
		testRunner().add("class", test);
	}

	CallFunction run(addTest);

}
