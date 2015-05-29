// Description: Named parameters

#ifndef PASTELSYS_NAMED_PARAMETER_H
#define PASTELSYS_NAMED_PARAMETER_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/generic/tag.h"
#include "pastel/sys/type_traits/is_template_instance.h"

#define PASTEL_PARAMETER(Type, name) \
	Self& name(Type name##__) \
	{ \
		name##_ = std::move(name##__); \
		return *this; \
	} \
	Type name##_

#define PASTEL_PARAMETER_DEFAULT(Type, name, defaultValue) \
	Self& name(Type name##__) \
	{ \
		name##_ = std::move(name##__); \
		return *this; \
	} \
	\
	Type name##_ = defaultValue

#define PASTEL_ARG(name, ...) argument<#name##_tag>(__VA_ARGS__, std::forward<ArgumentSet>(argumentSet)...);
#define PASTEL_ARG_S(name, def) PASTEL_ARG(name, [&](){return def;}, [](auto) {return explicitArgument();})

namespace Pastel
{

	template <typename Condition>
	struct ExplicitArgument
	{
		static constexpr bool value = Condition::value;
	};

	template <typename Type>
	using IsExplicitArgument =
		IsTemplateInstance<Type, ExplicitArgument>;

	template <typename Condition = std::true_type>
	constexpr ExplicitArgument<Condition> explicitArgument(
		Condition&& condition = Condition())
	{
		return {};
	}

	//! Optional argument parsing
	/*!
	The functions have been collected into this
	class because of name lookup. Since the 
	overloads of the argument() refer to each
	other in both ways, we would need to use
	forward-declarations. Using a class avoids
	this.
	*/
	template <tag_integer KeyHash>
	struct Argument
	{
		struct None {};

		static constexpr None none()
		{
			return None();
		}

		template <
			typename Default,
			typename Condition
		>
		static constexpr decltype(auto) argument(
			Default&& defaultValue,
			Condition&&)
		{
			// No optional parameters were given;
			// return the default value.
			return defaultValue();
		}

		template <
			typename Condition,
			typename Value,
			typename... ArgumentSet,
			Requires<
				Not<IsTag<Value>>
			> ConceptCheck = 0
		>
		static decltype(auto) argumentFound(
			Condition&& condition, 
			Value&& value, 
			ArgumentSet&&... argumentSet)
		{
			// FIX: Turn into constexpr once Visual Studio 
			// supports the C++14 constexpr.

			// Check that there are no other matches
			using Match = RemoveCvRef<decltype(
				argument(
					none,
					std::forward<Condition>(condition),
					std::forward<ArgumentSet>(argumentSet)...
				))>;

			static constexpr bool UniqueMatch =
				std::is_same<Match, None>::value;

			static_assert(UniqueMatch,
				"Multiple optional arguments match the parameter.");

			return std::forward<Value>(value);
		}

		template <
			typename Default,
			typename Condition,
			typename Key,
			typename... ArgumentSet,
			Requires<
				IsTag<Key>,
				Bool<(Tag_Hash<Key>::value == KeyHash)>
			> ConceptCheck = 0
		>
		static decltype(auto) argumentFoundFlag(
			Default&& defaultValue,
			Condition&& condition, 
			Key&& key,
			ArgumentSet&&... argumentSet)
		{
			// FIX: Turn into constexpr once Visual Studio 
			// supports the C++14 constexpr.

			using ConditionType =
				decltype(condition(true));

			// Note that we also accept the value when
			// the argument is not explicit; the explicitness
			// requires that the key be specified, 
			// not the value.
			static constexpr bool ValueSatisfiesCondition =
				ConditionType::value;

			// Check the argument satisfies the condition.
			static_assert(ValueSatisfiesCondition,
				"Optional argument (implicit true) is not valid for the given parameter.");

			// Interpret the value as boolean true.
			return argumentFound(
				std::forward<Condition>(condition),
				true,
				std::forward<ArgumentSet>(argumentSet)...);
		}

		template <
			typename Default,
			typename Condition,
			typename Value,
			typename... ArgumentSet,
			Requires<
				Not<IsTag<Value>>
			> = 0
		>
		static constexpr decltype(auto) argumentbranch(
			Bool<true>,
			Default&&,
			Condition&& condition, 
			Value&& value, 
			ArgumentSet&&... argumentSet)
		{
			// The list begins with a value, and the value
			// satisfies the condition.

			// Return the value.
			return argumentFound(
				std::forward<Condition>(condition),
				std::forward<Value>(value),
				std::forward<ArgumentSet>(argumentSet)...);
		}

		template <
			typename Default,
			typename Condition,
			typename Value,
			typename... ArgumentSet,
			Requires<
				Not<IsTag<Value>>
			> = 0
		>
		static constexpr decltype(auto) argumentbranch(
			Bool<false>,
			Default&& defaultValue,
			Condition&& condition, 
			Value&&, 
			ArgumentSet&&... argumentSet)
		{
			// The list begins with a value, but the value
			// does not satisfy the condition.

			// Move on.
			return argument(
				std::forward<Default>(defaultValue),
				std::forward<Condition>(condition),
				std::forward<ArgumentSet>(argumentSet)...);
		}

		template <
			typename Default,
			typename Condition,
			typename Value,
			typename... ArgumentSet,
			Requires<
				Not<IsTag<Value>>
			> = 0
		>
		static constexpr decltype(auto) argument(
			Default&& defaultValue,
			Condition&& condition, 
			Value&& value, 
			ArgumentSet&&... argumentSet)
		{
			// The list begins with a value, so it is
			// missing a key-tag.

			// Branch based on whether the condition is fulfilled or not.
			return argumentbranch(
				Bool<
					decltype(condition(value))::value &&
					!IsExplicitArgument<decltype(condition(value))>::value
				>(),
				std::forward<Default>(defaultValue),
				std::forward<Condition>(condition),
				std::forward<Value>(value),
				std::forward<ArgumentSet>(argumentSet)...);
		}

		template <
			typename Default,
			typename Condition,
			typename Key,
			Requires<
				IsTag<Key>,
				Bool<(Tag_Hash<Key>::value == KeyHash)>
			> ConceptCheck = 0
		>
		static constexpr decltype(auto) argument(
			Default&& defaultValue,
			Condition&& condition, 
			Key&& key)
		{
			// The list consists of a single key-tag,
			// which is the searched key.

			// Reduce to the flag case.
			return argumentFoundFlag(
				std::forward<Default>(defaultValue),
				std::forward<Condition>(condition),
				std::forward<Key>(key));
		}

		template <
			typename Default,
			typename Condition,
			typename Key,
			Requires<
				IsTag<Key>,
				Bool<(Tag_Hash<Key>::value != KeyHash)>
			> ConceptCheck = 0
		>
		static constexpr decltype(auto) argument(
			Default&& defaultValue,
			Condition&&,
			Key&&)
		{
			// The list consists of a single key-tag,
			// which is _not_ the searched key.

			// Return the default-value.
			return defaultValue();
		}

		template <
			typename Default,
			typename Condition,
			typename Key,
			typename Value,
			typename... ArgumentSet,
			Requires<
				IsTag<Key>,
				Not<IsTag<Value>>,
				Bool<(Tag_Hash<Key>::value == KeyHash)>
			> ConceptCheck = 0
		>
		static decltype(auto) argument(
			Default&&,
			Condition&& condition, 
			Key&& key, 
			Value&& value, 
			ArgumentSet&&... argumentSet)
		{
			// FIX: Turn into constexpr once Visual Studio 
			// supports the C++14 constexpr.

			// The list begins with a key-value pair,
			// where the key is the searched tag.

			static constexpr bool ValueSatisfiesCondition =
				decltype(condition(value))::value;

			// Check the argument satisfies the condition.
			static_assert(ValueSatisfiesCondition,
				"Optional argument is not valid for the given parameter.");

			// We have found the searched argument; return it.
			return argumentFound(
				std::forward<Condition>(condition),
				std::forward<Value>(value),
				std::forward<ArgumentSet>(argumentSet)...);
		}

		template <
			typename Default,
			typename Condition,
			typename Key,
			typename Value,
			typename... ArgumentSet,
			Requires<
				IsTag<Key>,
				Bool<(Tag_Hash<Key>::value != KeyHash)>,
				Not<IsTag<Value>>
			> ConceptCheck = 0
		>
		static constexpr decltype(auto) argument(
			Default&& defaultValue,
			Condition&& condition, 
			Key&&, 
			Value&&, 
			ArgumentSet&&... rest)
		{
			// The list begins with a key-value pair,
			// where the key is _not_ the searched tag.

			// Continue searching from the next key-value pair.
			return argument(
				std::forward<Default>(defaultValue), 
				std::forward<Condition>(condition), 
				std::forward<ArgumentSet>(rest)...);
		}

		template <
			typename Default,
			typename Condition,
			typename A_Key,
			typename B_Key,
			typename... ArgumentSet,
			Requires<
				IsTag<A_Key>,
				IsTag<B_Key>,
				Bool<(Tag_Hash<A_Key>::value == KeyHash)>
			> ConceptCheck = 0
		>
		static constexpr decltype(auto) argument(
			Default&& defaultValue,
			Condition&& condition, 
			A_Key&& aKey, 
			B_Key&& bKey, 
			ArgumentSet&&... argumentSet)
		{
			// The list begins with two subsequent key-tags,
			// the first of which is the searched key.

			// We found a match from a flag.
			return argumentFoundFlag(
				std::forward<Default>(defaultValue),
				std::forward<Condition>(condition),
				std::forward<A_Key>(aKey),
				std::forward<B_Key>(bKey),
				std::forward<ArgumentSet>(argumentSet)...);
		}

		template <
			typename Default,
			typename Condition,
			typename A_Key,
			typename B_Key,
			typename... ArgumentSet,
			Requires<
				IsTag<A_Key>,
				IsTag<B_Key>,
				Bool<(Tag_Hash<A_Key>::value != KeyHash)>
			> ConceptCheck = 0
		>
		static constexpr decltype(auto) argument(
			Default&& defaultValue,
			Condition&& condition, 
			A_Key&&, 
			B_Key&& bKey, 
			ArgumentSet&&... argumentSet)
		{
			// The list begins with two subsequent key-tags,
			// the first of which is _not_ the searched key.

			// Move on.
			return argument(
				std::forward<Default>(defaultValue),
				std::forward<Condition>(condition),
				std::forward<B_Key>(bKey),
				std::forward<ArgumentSet>(argumentSet)...);
		}

	};

	template <
		tag_integer KeyHash,
		typename Default,
		typename Condition,
		typename... ArgumentSet
	>
	decltype(auto) argument(
		Default&& defaultValue,
		Condition&& condition,
		ArgumentSet&&... argumentSet)
	{
		// FIX: Turn into constexpr once Visual Studio 
		// supports the C++14 constexpr.

		static constexpr bool DefaultValueSatisfiesCondition =
			decltype(condition(defaultValue()))::value;

		// Check the argument satisfies the condition.
		static_assert(DefaultValueSatisfiesCondition,
			"Default optional argument is not valid for the given parameter.");

		return Argument<KeyHash>::argument(
			std::forward<Default>(defaultValue),
			std::forward<Condition>(condition),
			std::forward<ArgumentSet>(argumentSet)...);
	}

}

#endif
