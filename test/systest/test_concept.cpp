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

	template <typename Concept, typename Type>
	auto isModelOf(Type&& that)
		-> Models<Type, Concept>
	{
		return Models<Type, Concept>();
	}

	struct Doo_Concept
	{
		template <typename Type, typename A_Goo>
		auto requires(Type&& t, A_Goo&& a) -> decltype(
			conceptCheck(
				isModelOf<Goo_Concept>(a)
			));
	};

	template <typename A_Doo>
	void f()
	{
		PASTEL_STATIC_ASSERT((Models<A_Doo, Doo_Concept(int)>::value));
	}

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
			testModels();
			testRefines();
			testFirstModeledConcept();
			testMostRefinedConcept();
		}

		void testModels()
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
		}

		void testRefines()
		{
			PASTEL_STATIC_ASSERT((!IsRefinesClass<int>::value));
			PASTEL_STATIC_ASSERT((!IsRefinesClass<Goo_Concept>::value));
			PASTEL_STATIC_ASSERT((!IsRefinesClass<FooGoo_Concept>::value));

			PASTEL_STATIC_ASSERT((IsRefinesClass<Refines<>>::value));
			PASTEL_STATIC_ASSERT((IsRefinesClass<Refines<Goo_Concept>>::value));
			PASTEL_STATIC_ASSERT((IsRefinesClass<Refines<Goo_Concept, FooGoo_Concept>>::value));

			PASTEL_STATIC_ASSERT((!IsRefined<Goo_Concept>::value));
			PASTEL_STATIC_ASSERT((!IsRefined<Empty_Concept>::value));

			PASTEL_STATIC_ASSERT((IsRefined<FooGoo_Concept>::value));
			PASTEL_STATIC_ASSERT((!IsRefined<Baseless_Concept>::value));
		}

		void testFirstModeledConcept()
		{
			PASTEL_STATIC_ASSERT((
				std::is_same<
					typename FirstModeledConcept<Green_Goo, Goo_Concept, FooGoo_Concept>::type,
					Goo_Concept
				>::value));

			PASTEL_STATIC_ASSERT((
				std::is_same<
					typename FirstModeledConcept<Green_Goo, FooGoo_Concept, Goo_Concept>::type,
					Goo_Concept
				>::value));

			PASTEL_STATIC_ASSERT((
				std::is_same<
					typename FirstModeledConcept<SuperGreen_FooGoo, Goo_Concept, FooGoo_Concept>::type,
					Goo_Concept
				>::value));

			PASTEL_STATIC_ASSERT((
				std::is_same<
					typename FirstModeledConcept<SuperGreen_FooGoo, FooGoo_Concept, Goo_Concept>::type,
					FooGoo_Concept
				>::value));
		}

		void testMostRefinedConcept()
		{
			PASTEL_STATIC_ASSERT((
				std::is_same<
					typename MostRefinedConcept<Green_Goo, FooGoo_Concept>::type, 
					Goo_Concept
				>::value));

			PASTEL_STATIC_ASSERT((
				std::is_same<
					typename MostRefinedConcept<SuperGreen_FooGoo, FooGoo_Concept>::type, 
					FooGoo_Concept
				>::value));
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
