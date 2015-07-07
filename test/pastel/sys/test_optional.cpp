// Description: Testing for optional objects
// DocumentationOf: optional.h

#include "test/test_init.h"

#include "pastel/sys/optional/optional.h"

namespace
{

	class Test
	{
	public:
		virtual void run()
		{
			test();
		}

		class B
		{
		public:
			B()
				: b(0)
			{
			}

			explicit B(int b_)
				: b(b_)
			{
			}

			int b;
		};

		void f(integer x)
		{
		}

		void g(Optional<integer> x)
		{
		}

		void test()
		{
			Optional<integer> a;
			REQUIRE(a.empty());
			// The zero is treated as a non-empty
			// optional: therefore it should not
			// match.
			REQUIRE(a != 0);
			REQUIRE(a != 0.0);

			a = 2;
			REQUIRE(!a.empty());
			REQUIRE(a == 2);
			REQUIRE(a == 2.0);

			a.clear();
			REQUIRE(a.empty());

			Optional<B> b;
			REQUIRE(b.empty());
			REQUIRE(b.b == 0);

			b.b = 3;
			REQUIRE(b.empty());
			REQUIRE(b.b == 3);

			b = B(2);
			REQUIRE(!b.empty());
			REQUIRE(b.b == 2);

			// Optional<Type> is implicitly convertible 
			// to type.
			f(a);

			// Type is implicitly convertible to Optional<Type>.
			g(3);
		}
	};

	TEST_CASE("Optional", "[Optional]")
	{
	}

}
