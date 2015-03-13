// Description: Testing for concepts
// DocumentationOf: concept.h

#include "test_pastelsys.h"

#include <pastel/sys/concept.h>

using namespace Pastel;

namespace
{

	struct Goo_Concept
	{
		template <typename Type>
		auto requires(Type&& t) -> decltype(
			conceptCheck(
				++t,
				t * t,
				t.f()
			));
	};

	struct FooGoo_Concept
		: Refines<Goo_Concept>
	{
		template <typename Type>
		auto requires(Type&& t) -> decltype(
			conceptCheck(
				// The return-type of g() is void; 
				// the comma can be used to assign
				// an argument.
				(t.g(), 0)
			));
	};

	struct Empty_Concept
	{
		template <typename Type>
		void requires(Type&& t);
	};

	struct Baseless_Concept
		: Refines<>
	{};

	class Green_Goo
	{
	public:
		Green_Goo& operator++();
		Green_Goo& operator*(const Green_Goo& that);
		integer f();
	};

	class SuperGreen_FooGoo
		: public Green_Goo
	{
	public:
		void g();
	};

	class Something_Else
	{
	public:
		void g();
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
			PASTEL_STATIC_ASSERT((Models<Green_Goo, Goo_Concept>::value));
			PASTEL_STATIC_ASSERT((Models<Green_Goo, Goo_Concept>::value));
			PASTEL_STATIC_ASSERT((Models<Green_Goo, Empty_Concept>::value));
			PASTEL_STATIC_ASSERT((Models<Green_Goo, Baseless_Concept>::value));
			PASTEL_STATIC_ASSERT((!Models<Green_Goo, FooGoo_Concept>::value));

			PASTEL_STATIC_ASSERT((Models<SuperGreen_FooGoo, Goo_Concept>::value));
			PASTEL_STATIC_ASSERT((Models<SuperGreen_FooGoo, FooGoo_Concept>::value));
			PASTEL_STATIC_ASSERT((Models<SuperGreen_FooGoo, Empty_Concept>::value));
			PASTEL_STATIC_ASSERT((Models<SuperGreen_FooGoo, Baseless_Concept>::value));

			PASTEL_STATIC_ASSERT((Models_Directly<Something_Else, FooGoo_Concept>::value));
			PASTEL_STATIC_ASSERT((!Models_Base<Something_Else, FooGoo_Concept>::value));
			PASTEL_STATIC_ASSERT((!Models<Something_Else, FooGoo_Concept>::value));

			PASTEL_STATIC_ASSERT((!IsRefined<Goo_Concept>::value));
			PASTEL_STATIC_ASSERT((!IsRefined<Empty_Concept>::value));
			PASTEL_STATIC_ASSERT((IsRefined<FooGoo_Concept>::value));
			PASTEL_STATIC_ASSERT((IsRefined<Baseless_Concept>::value));
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("concept", test);
	}

	CallFunction run(addTest);

}
