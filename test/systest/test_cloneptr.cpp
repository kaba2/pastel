// Description: Testing for ClonePtr
// DocumentationOf: clone_ptr.h

#include "pastelsystest.h"

#include "pastel/sys/clone_ptr.h"

using namespace Pastel;
using namespace std;

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
	};

	class B
		: public A
	{
	public:
		A* clone() const override
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
				ClonePtr<A> a;
			}
			{
				ClonePtr<A> a(nullptr);
			}
			{
				ClonePtr<A> a(new A);
			}
			{
				ClonePtr<A> b(new B);
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

				TEST_ENSURE(copyConstructionOccurred);
			}
			{
				ClonePtr<A> a;
				TEST_ENSURE(!(a < a));
				TEST_ENSURE(!(a > a));
				TEST_ENSURE(a <= a);
				TEST_ENSURE(a >= a);
				TEST_ENSURE(a == a);
				TEST_ENSURE(!(a != a));
				
				TEST_ENSURE(!(a < nullptr));
				TEST_ENSURE(!(nullptr < a));
				TEST_ENSURE(!(a > nullptr));
				TEST_ENSURE(!(nullptr > a));
				TEST_ENSURE(a <= nullptr);
				TEST_ENSURE(nullptr <= a);
				TEST_ENSURE(a >= nullptr);
				TEST_ENSURE(nullptr >= a);
				TEST_ENSURE(a == nullptr);
				TEST_ENSURE(nullptr == a);
				TEST_ENSURE(!(a != nullptr));
				TEST_ENSURE(!(nullptr != a));
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
		testRunner().add("ClonePtr", test);
	}

	CallFunction run(addTest);

}
