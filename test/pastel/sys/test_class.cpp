// Description: Testing for class wrapping
// DocumentationOf: class.h

#include "test/test_init.h"

#include <pastel/sys/generic/class.h>

namespace
{

	class Test
	{
	public:
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
				REQUIRE(a.a() == 0);
			}

			{
				Class<A> a(2);
				REQUIRE(a.a() == 2);
			}

			{
				Class<A> a;
				a = 2;
				REQUIRE(a.a() == 2);
			}

			{
				A b(2);

				Class<A> a(std::move(b));
				REQUIRE(a.a() == 2);
				REQUIRE(b.a() == 0);
			}

			{
				A b(2);

				Class<A> a(b);
				REQUIRE(a.a() == 2);
				REQUIRE(b.a() == 2);
			}
		}

		void testNonClass()
		{
			{
				Class<int> a(0);
				REQUIRE(a == 0);

				a = 0;
			}

			{
				Class<int> a(5);
				REQUIRE(a == 5);
			}

			{
				Class<int> a(5);
				a = 3;
				REQUIRE(a == 3);
			}

			{
				Class<int> a(5);
				a += 3;
				REQUIRE(a == 8);
			}

			{
				Class<void> c;
				c = nullptr;
				REQUIRE(c == nullptr);
			}

			{
				Class<void> c(nullptr);
				REQUIRE(c == nullptr);
			}
		}
	};

	TEST_CASE("class", "[class]")
	{
	}

}
