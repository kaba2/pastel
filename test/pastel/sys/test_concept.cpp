// Description: Testing for concepts
// DocumentationOf: concept.h

#include "test/test_init.h"

#include <pastel/sys/concept.h>

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

	struct Doo_Concept
	{
		template <typename Type, typename A_Goo>
		auto requires(Type&& t, A_Goo&& a) -> decltype
		(
			conceptCheck(
				Concept::isModelOf<Goo_Concept>(a),
				Concept::hasType<Type&>(t += a)
			)
		);
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

	class Green_Doo
	{
	public:
		Green_Doo& operator+=(const Green_Goo& that)
		{
			return *this;
		}
	};

	class Red_Doo
	{
	public:
		Red_Doo& operator+=(const Green_Goo& that)
		{
			return *this;
		}
	};

	class Blue_Doo
	{
	public:
		Blue_Doo& operator+=(const SuperGreen_FooGoo& that)
		{
			return *this;
		}
	};

	class Something_Else
	{
	public:
		void g();
	};

}

TEST_CASE("Models (concept)")
{
	PASTEL_CONCEPT_CHECK(Green_Goo, Goo_Concept);
	PASTEL_CONCEPT_CHECK(Green_Goo, Empty_Concept);
	PASTEL_CONCEPT_CHECK(Green_Goo, Baseless_Concept);
	PASTEL_CONCEPT_REJECT(Green_Goo, FooGoo_Concept);
	PASTEL_CONCEPT_CHECK(Green_Goo&, Goo_Concept);
	PASTEL_CONCEPT_CHECK(Green_Goo&&, Goo_Concept);
	PASTEL_CONCEPT_CHECK(const Green_Goo, Goo_Concept);
	PASTEL_CONCEPT_CHECK(const Green_Goo&, Goo_Concept);
	PASTEL_CONCEPT_CHECK(const Green_Goo&&, Goo_Concept);

	PASTEL_CONCEPT_CHECK(SuperGreen_FooGoo, Goo_Concept);
	PASTEL_CONCEPT_CHECK(SuperGreen_FooGoo, FooGoo_Concept);
	PASTEL_CONCEPT_CHECK(SuperGreen_FooGoo, Empty_Concept);
	PASTEL_CONCEPT_CHECK(SuperGreen_FooGoo, Baseless_Concept);
	PASTEL_CONCEPT_CHECK(SuperGreen_FooGoo&, Goo_Concept);
	PASTEL_CONCEPT_CHECK(SuperGreen_FooGoo&&, Goo_Concept);
	PASTEL_CONCEPT_CHECK(const SuperGreen_FooGoo, Goo_Concept);
	PASTEL_CONCEPT_CHECK(const SuperGreen_FooGoo&, Goo_Concept);
	PASTEL_CONCEPT_CHECK(const SuperGreen_FooGoo&&, Goo_Concept);

	PASTEL_CONCEPT_CHECK_DIRECT(Something_Else, FooGoo_Concept);
	PASTEL_CONCEPT_REJECT_BASE(Something_Else, FooGoo_Concept);
	PASTEL_CONCEPT_REJECT(Something_Else, FooGoo_Concept);
	PASTEL_CONCEPT_REJECT(Something_Else&, FooGoo_Concept);
	PASTEL_CONCEPT_REJECT(Something_Else&&, FooGoo_Concept);
	PASTEL_CONCEPT_REJECT(const Something_Else, FooGoo_Concept);
}

TEST_CASE("ParametrizedModels (concept)")
{
	PASTEL_CONCEPT_CHECK(Green_Doo, Doo_Concept(Green_Goo));
	PASTEL_CONCEPT_CHECK(Red_Doo, Doo_Concept(Green_Goo));
	PASTEL_CONCEPT_CHECK(Red_Doo, Doo_Concept(SuperGreen_FooGoo));
	PASTEL_CONCEPT_CHECK(Blue_Doo, Doo_Concept(SuperGreen_FooGoo));
	PASTEL_CONCEPT_REJECT(Blue_Doo, Doo_Concept(Green_Goo));
	PASTEL_CONCEPT_REJECT(Something_Else, Doo_Concept(Green_Goo));
}

namespace
{

	template <typename A_Doo>
	void f(const A_Doo& a)
	{
		PASTEL_CONCEPT_CHECK(A_Doo, Doo_Concept(Green_Goo));
	}

	template <
		typename A_Doo,
		Requires<Models<A_Doo, Doo_Concept(Green_Goo)>> = 0>
	std::true_type g(const A_Doo& a)
	{
		return std::true_type();
	}

	template <
		typename A_Doo,
		DisableIf<Models<A_Doo, Doo_Concept(Green_Goo)>> = 0>
	std::false_type g(const A_Doo& a)
	{
		return std::false_type();
	}
}

TEST_CASE("FunctionModels (concept)")
{
	f(Green_Doo());
	f(Red_Doo());

	// This should trigger a nice error.
	//f(Blue_Doo());

	PASTEL_STATIC_ASSERT(decltype(g(Green_Doo()))::value);
	PASTEL_STATIC_ASSERT(decltype(g(Red_Doo()))::value);

	PASTEL_STATIC_ASSERT(!decltype(g(Green_Goo()))::value);
	PASTEL_STATIC_ASSERT(!decltype(g(Something_Else()))::value);
}

namespace
{

	template <typename A_Doo>
	class TestClass
	{
		PASTEL_CONCEPT_CHECK(A_Doo, Doo_Concept(Green_Goo));
	};

}

TEST_CASE("ClassModels (concept)")
{
	TestClass<Green_Doo> green;
	TestClass<Red_Doo> red;

	// This should trigger a nice error.
	//TestClass<Blue_Doo> blue;

	unused(red, green);
}

TEST_CASE("Refines (concept)")
{
	PASTEL_STATIC_ASSERT((!IsRefined<Goo_Concept>::value));
	PASTEL_STATIC_ASSERT((!IsRefined<Empty_Concept>::value));

	PASTEL_STATIC_ASSERT((IsRefined<FooGoo_Concept>::value));
	PASTEL_STATIC_ASSERT((!IsRefined<Baseless_Concept>::value));
}

TEST_CASE("BaseConcepts (concept)")
{
	PASTEL_STATIC_ASSERT((
		std::is_same<
			BaseConcepts<Goo_Concept>,
			Refines<>
		>::value));

	PASTEL_STATIC_ASSERT((
		std::is_same<
			BaseConcepts<FooGoo_Concept>,
			Refines<Goo_Concept>
		>::value));
}

TEST_CASE("FirstModeledConcept (concept)")
{
	PASTEL_STATIC_ASSERT((
		std::is_same<
			FirstModeledConcept<Green_Goo, Goo_Concept, FooGoo_Concept>,
			Goo_Concept
		>::value));

	PASTEL_STATIC_ASSERT((
		std::is_same<
			FirstModeledConcept<Green_Goo, FooGoo_Concept, Goo_Concept>,
			Goo_Concept
		>::value));

	PASTEL_STATIC_ASSERT((
		std::is_same<
			FirstModeledConcept<SuperGreen_FooGoo, Goo_Concept, FooGoo_Concept>,
			Goo_Concept
		>::value));

	PASTEL_STATIC_ASSERT((
		std::is_same<
			FirstModeledConcept<SuperGreen_FooGoo, FooGoo_Concept, Goo_Concept>,
			FooGoo_Concept
		>::value));

	PASTEL_STATIC_ASSERT((
		std::is_same<
			FirstModeledConcept<Green_Goo>,
			void
		>::value));

	PASTEL_STATIC_ASSERT((
		std::is_same<
			FirstModeledConcept<Something_Else, Goo_Concept>,
			void
		>::value));
}

TEST_CASE("MostRefinedConcept (concept)")
{
	PASTEL_STATIC_ASSERT((
		std::is_same<
			MostRefinedConcept<Green_Goo, FooGoo_Concept>, 
			Goo_Concept
		>::value));

	PASTEL_STATIC_ASSERT((
		std::is_same<
			MostRefinedConcept<SuperGreen_FooGoo, FooGoo_Concept>, 
			FooGoo_Concept
		>::value));

	PASTEL_STATIC_ASSERT((
		std::is_same<
			MostRefinedConcept<Something_Else, FooGoo_Concept>, 
			void
		>::value));
}

TEST_CASE("CoarsestFailedConcept (concept)")
{
	PASTEL_STATIC_ASSERT((
		std::is_same<
			CoarsestFailedConcept<Green_Goo, FooGoo_Concept>, 
			Refines<FooGoo_Concept>
		>::value));

	PASTEL_STATIC_ASSERT((
		std::is_same<
			CoarsestFailedConcept<SuperGreen_FooGoo, FooGoo_Concept>, 
			Refines<>
		>::value));

	PASTEL_STATIC_ASSERT((
		std::is_same<
			CoarsestFailedConcept<Something_Else, FooGoo_Concept>, 
			Refines<Goo_Concept>
		>::value));
}

