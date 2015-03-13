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

	//! Checks whether a concept is refined.
	template <typename Concept>
	struct IsRefined
	{
	private:
		template <typename... ConceptSet>
		static std::true_type test(Refines<ConceptSet...>&&);
		
		static std::false_type test(...);

	public:
		static constexpr bool value =
			decltype(test(std::declval<Concept>()))::value;
	};

}

#endif
