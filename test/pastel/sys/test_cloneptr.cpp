// Description: Testing for ClonePtr
// DocumentationOf: clone_ptr.h

#include "test/test_init.h"

#include "pastel/sys/pointer/clone_ptr.h"

namespace
{

	class A
	{
	public:
		virtual ~A()
		{
		}

		virtual A* clone() const
		{
			return new A(*this);
		}

		integer data;
	};

	class B
		: public A
	{
	public:
		A* clone() const
		{
			return new B(*this);
		}
	};

	class C 
	{
	public:
		C()
		{
		}

		C(const C& that)
		{
			throw true;
		}
	};

	class D
		: public A
	{
	public:
	};

	class Test
	{
	public:
		virtual void run()
		{
			test();
		}

		void test()
		{
			{
				ClonePtr<A> a;
			}
			{
				ClonePtr<A> a(nullptr);
			}
			{
				ClonePtr<A> a(new A);
				*a;
				a->data = 3;
			}
			{
				ClonePtr<A> b(new B);
				b.reset();
			}
			{
				// This should not compile because C 
				// is incompatible with A.
				//ClonePtr<A> c(new C);
			}

			{
				// This will slice on copy.
				// Unfortunately there is no way
				// to force D to override clone().
				ClonePtr<A> d(new D);
			}
			{
				ClonePtr<C> c(new C);

				bool copyConstructionOccurred = false;
				try 
				{
					ClonePtr<C> c2(c);
				}
				catch(...) 
				{ 
					copyConstructionOccurred = true;					
				}

				REQUIRE(copyConstructionOccurred);
			}
			{
				ClonePtr<A> a;
				REQUIRE(!(a < a));
				REQUIRE(!(a > a));
				REQUIRE(a <= a);
				REQUIRE(a >= a);
				REQUIRE(a == a);
				REQUIRE(!(a != a));

				REQUIRE(!(a < nullptr));
				REQUIRE(!(nullptr < a));
				REQUIRE(!(a > nullptr));
				REQUIRE(!(nullptr > a));
				REQUIRE(a <= nullptr);
				REQUIRE(nullptr <= a);
				REQUIRE(a >= nullptr);
				REQUIRE(nullptr >= a);
				REQUIRE(a == nullptr);
				REQUIRE(nullptr == a);
				REQUIRE(!(a != nullptr));
				REQUIRE(!(nullptr != a));
			}
		}
	};

	TEST_CASE("ClonePtr", "[ClonePtr]")
	{
	}

}
