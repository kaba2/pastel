// Description: Concept-checking

#ifndef PASTELSYS_CONCEPT_H
#define PASTELSYS_CONCEPT_H

#include <type_traits>

namespace Pastel
{

	//! List expressions in a concept-check.
	/*!
	This function exists to avoid comma-separated
	expressions from being interpreted as being
	chained by the comma-operator.
	*/
	template <typename... TypeSet>
	void conceptCheck(TypeSet&&... that);

	//! Checks whether a type models the given concepts.
	template <typename Type, typename... ConceptSet>
	struct Models;

}

namespace Pastel
{

	//! Refined concept
	/*!
	The Refines-class has two roles.
	First, it is a concept, which requires
	all the requirements in the given set of
	concepts. Second, it is used to denote 
	concept-refinement when used as a base-class.
	*/
	template <typename... ConceptSet>
	struct Refines 
	{	
		// Refines is also a concept;
		// it requires all the requirements
		// of its concepts. This way a
		// refined concept does not need to
		// implement requires(), if it does
		// not add any new requirements.

		template <typename Type>
		auto requires(Type&& t) -> decltype(
			conceptCheck(
				Models<Type, ConceptSet...>::value
			));
	};

}

namespace Pastel
{

	//! Checks direct requirements of a concept.
	/*!
	The requirements of base-concepts will not 
	be checked.
	*/
	template <
		typename Type, 
		typename Concept>
	struct Models_Directly
	{
	private:
		template <
			typename T,
			typename = decltype(std::declval<Concept>().requires(std::declval<T>()))>
		static std::true_type test();
		
		template <typename T>
		static std::false_type test(...);

	public:
		static constexpr bool value =
			decltype(test<Type>())::value;
	};

	//! Checks whether a type models base-concepts.
	/*!
	The direct requirements of the concept will 
	not be checked.
	*/
	template <
		typename Type, 
		typename Concept>
	struct Models_Base
	{
	private:
		template <typename... ConceptSet>
		static auto test(Refines<ConceptSet...>&&)
		-> std::integral_constant<bool, Models<Type, ConceptSet...>::value>;
		
		// The concept does not refine other concepts.
		static std::true_type test(...);

	public:
		static constexpr bool value =
			decltype(test(std::declval<Concept>()))::value;
	};

	//! Checks whether a type models a concept.
	template <
		typename Type, 
		typename Concept, 
		typename... ConceptSet>
	struct Models<Type, Concept, ConceptSet...>
	{
		static constexpr bool value =
			Models_Directly<Type, Concept>::value &&
			Models_Base<Type, Concept>::value &&
			Models<Type, ConceptSet...>::value;
	};

	template <typename Type>
	struct Models<Type>
		: std::true_type
	{};

}

namespace Pastel
{

	//! Returns whether a type is a Refines<...> class.
	template <typename Concept>
	struct IsRefinesClass
		: std::false_type
	{};

	template <typename... ConceptSet>
	struct IsRefinesClass<Refines<ConceptSet...>>
		: std::true_type
	{};

}

namespace Pastel
{

	//! Retrieves the base-concepts.
	/*!
	returns:
	The base-concepts in a Refine<...> class.
	*/
	template <typename Concept>
	struct BaseConcepts
	{
	private:
		template <typename... ConceptSet>
		static Refines<ConceptSet...> test(Refines<ConceptSet...>&&);
		
		static Refines<> test(...);

	public:
		using type = 
			typename std::conditional<
				IsRefinesClass<Concept>::value,
				void,
				decltype(test(std::declval<Concept>()))
				>::type;
	};

}

namespace Pastel
{

	//! Checks whether a concept is refined.
	template <typename Concept>
	struct IsRefined
		: std::integral_constant<bool, !std::is_same<
		typename BaseConcepts<Concept>::type, Refines<>>::value>
	{};

}

namespace Pastel
{

	template <typename Type, typename Concept>
	struct MostRefined
	{
	};

}

#endif
