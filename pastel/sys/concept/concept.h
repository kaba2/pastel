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

	template <
		typename A_Refines,
		typename B_Refines>
	struct JoinRefines;

	template <
		typename... A_ConceptSet,
		typename... B_ConceptSet>
	struct JoinRefines<Refines<A_ConceptSet...>, Refines<B_ConceptSet...>>
	{
		using type = Refines<A_ConceptSet..., B_ConceptSet...>;
	};

}

namespace Pastel
{

	namespace Concept_
	{

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

	//! Retrieves the base-concepts.
	/*!
	returns:
	The base-concepts of each concept combined 
	into a Refine<...> class.
	*/
	template <typename... ConceptSet>
	struct BaseConcepts;

	template <
		typename Concept,
		typename... ConceptSet>
	struct BaseConcepts<Concept, ConceptSet...>
	{
		using type = 
			typename JoinRefines<
				typename Concept_::BaseConcepts<Concept>::type,
				typename BaseConcepts<ConceptSet...>::type
			>::type;
	};

	template <>
	struct BaseConcepts<>
	{
		using type = Refines<>;
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

	//! Returns the first modeled concept.
	/*!
	returns:
	The first concept in ConceptSet whose model Type is.
	If there is no such concept, then void.
	*/
	template <typename Type, typename... ConceptSet>
	struct FirstModeledConcept;

	template <
		typename Type,
		typename Concept,
		typename... ConceptSet>
	struct FirstModeledConcept<Type, Concept, ConceptSet...>
	{
		using type = 
			typename std::conditional<
				Models<Type, Concept>::value,
				Concept,
				typename FirstModeledConcept<Type, ConceptSet...>::type
			>::type;
	};

	template <typename Type>
	struct FirstModeledConcept<Type>
	{
		using type = void;
	};

}

namespace Pastel
{

	namespace Concept_
	{

		template <
			typename Type,
			typename... ConceptSet>
		struct MostRefinedConcept
		{
		private:
			using ModeledConcept =
				typename FirstModeledConcept<
					Type, ConceptSet...
				>::type;

		public:
			template <typename>
			struct BreadthFirst;

			template <typename... BaseSet>
			struct BreadthFirst<Refines<BaseSet...>>
			{
				using type =
					typename MostRefinedConcept<
						Type, BaseSet...
					>::type;
			};

			using BaseSet =
				typename BaseConcepts<ConceptSet...>::type;

			using type =
				typename std::conditional<
					std::is_same<ModeledConcept, void>::value,
					typename BreadthFirst<BaseSet>::type,
					ModeledConcept
				>::type;
		};

		template <typename Type>
		struct MostRefinedConcept<Type>
		{
			using type = void;
		};

	}

	//! Returns the most refined concept modeled by Type.
	/*!
	This is a breadth-first search over the Refines<...>
	inheritance hierarchy. Sequences (i.e. arguments of 
	Refines<...>) are searched from first to last.

	returns:
	The first matching concept. If there is none,
	then void.
	*/
	template <
		typename Type,
		typename Concept>
	struct MostRefinedConcept
	{
		using type = 
			typename Concept_::MostRefinedConcept<Type, Concept>::type;
	};

}

#endif
