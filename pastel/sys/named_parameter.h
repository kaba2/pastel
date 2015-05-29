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

	namespace Argument_
	{

		struct None {};

		constexpr None none()
		{
			return None();
		}

		template <
			tag_integer KeyHash,
			typename Default,
			typename Condition
		>
		constexpr decltype(auto) argument_(
			Default&& defaultValue,
			Condition&&)
		{
			// No optional parameters were given;
			// return the default value.
			return defaultValue();
		}

		template <
			int KeyHash,
			typename Condition,
			typename Value,
			typename... ArgumentSet,
			Requires<
				Not<IsTag<Value>>
			> ConceptCheck = 0
		>
		decltype(auto) argument_found(
			Condition&& condition, 
			Value&& value, 
			ArgumentSet&&... argumentSet)
		{
			// FIX: Turn into constexpr once Visual Studio 
			// supports the C++14 constexpr.

			// Check that there are no other matches
			using Match = RemoveCvRef<decltype(
				argument_<KeyHash>(
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
			tag_integer KeyHash,
			typename Default,
			typename Condition,
			typename Value,
			typename... ArgumentSet,
			Requires<
				Not<IsTag<Value>>
			> = 0
		>
		constexpr decltype(auto) argument_branch(
			Bool<true>,
			Default&&,
			Condition&& condition, 
			Value&& value, 
			ArgumentSet&&... argumentSet)
		{
			// The list begins with a value, and the value
			// satisfies the condition.

			// Return the value.
			return argument_found<KeyHash>(
				std::forward<Condition>(condition),
				std::forward<Value>(value),
				std::forward<ArgumentSet>(argumentSet)...);
		}

		template <
			tag_integer KeyHash,
			typename Default,
			typename Condition,
			typename Value,
			typename... ArgumentSet,
			Requires<
				Not<IsTag<Value>>
			> = 0
		>
		constexpr decltype(auto) argument_branch(
			Bool<false>,
			Default&& defaultValue,
			Condition&& condition, 
			Value&&, 
			ArgumentSet&&... argumentSet)
		{
			// The list begins with a value, but the value
			// does not satisfy the condition.

			// Move on.
			return argument_<KeyHash>(
				std::forward<Default>(defaultValue),
				std::forward<Condition>(condition),
				std::forward<ArgumentSet>(argumentSet)...);
		}

		template <
			tag_integer KeyHash,
			typename Default,
			typename Condition,
			typename Value,
			typename... ArgumentSet,
			Requires<
				Not<IsTag<Value>>
			> = 0
		>
		constexpr decltype(auto) argument_(
			Default&& defaultValue,
			Condition&& condition, 
			Value&& value, 
			ArgumentSet&&... argumentSet)
		{
			// The list begins with a value, so it is
			// missing a key-tag.

			// Branch based on whether the condition is fulfilled or not.
			return argument_branch<KeyHash>(
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
			tag_integer KeyHash,
			typename Default,
			typename Condition,
			typename Key,
			typename... ArgumentSet,
			Requires<
				IsTag<Key>,
				Bool<Tag_Hash<Key>::value == KeyHash>
			> ConceptCheck = 0
		>
		bool argument_found_flag(
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
			return argument_found<KeyHash>(
				std::forward<Condition>(condition),
				true,
				std::forward<ArgumentSet>(argumentSet)...);
		}

		template <
			tag_integer KeyHash,
			typename Default,
			typename Condition,
			typename Key,
			Requires<
				IsTag<Key>,
				Bool<Tag_Hash<Key>::value == KeyHash>
			> ConceptCheck = 0
		>
		constexpr decltype(auto) argument_(
			Default&& defaultValue,
			Condition&& condition, 
			Key&& key)
		{
			// The list consists of a single key-tag,
			// which is the searched key.

			// Reduce to the flag case.
			return argument_found_flag<KeyHash>(
				std::forward<Default>(defaultValue),
				std::forward<Condition>(condition),
				std::forward<Key>(key));
		}

		template <
			tag_integer KeyHash,
			typename Default,
			typename Condition,
			typename Key,
			Requires<
				IsTag<Key>,
				Bool<Tag_Hash<Key>::value != KeyHash>
			> ConceptCheck = 0
		>
		constexpr decltype(auto) argument_(
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
			tag_integer KeyHash,
			typename Default,
			typename Condition,
			typename A_Key,
			typename B_Key,
			typename... ArgumentSet,
			Requires<
				IsTag<A_Key>,
				IsTag<B_Key>
			> ConceptCheck = 0
		>
		constexpr decltype(auto) argument_(
			Default&& defaultValue,
			Condition&& condition, 
			A_Key&& aKey, 
			B_Key&& bKey, 
			ArgumentSet&&... argumentSet)
		{
			// The list begins with two subsequent key-tags.

			// Reduce to the key-value case.
			return argument_<KeyHash>(
				std::forward<Default>(defaultValue),
				std::forward<Condition>(condition),
				std::forward<A_Key>(aKey),
				true,
				std::forward<B_Key>(bKey),
				std::forward<ArgumentSet>(argumentSet)...);
		}

		template <
			tag_integer KeyHash,
			typename Default,
			typename Condition,
			typename Key,
			typename Value,
			typename... ArgumentSet,
			Requires<
				IsTag<Key>,
				Not<IsTag<Value>>,
				Bool<Tag_Hash<Key>::value == KeyHash>
			> ConceptCheck = 0
		>
		decltype(auto) argument_(
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
			return argument_found<KeyHash>(
				std::forward<Condition>(condition),
				std::forward<Value>(value),
				std::forward<ArgumentSet>(argumentSet)...);
		}

		template <
			tag_integer KeyHash,
			typename Default,
			typename Condition,
			typename Key,
			typename Value,
			typename... ArgumentSet,
			Requires<
				IsTag<Key>,
				Bool<Tag_Hash<Key>::value != KeyHash>,
				Not<IsTag<Value>>
			> ConceptCheck = 0
		>
		constexpr decltype(auto) argument_(
			Default&& defaultValue,
			Condition&& condition, 
			Key&&, 
			Value&&, 
			ArgumentSet&&... rest)
		{
			// The list begins with a key-value pair,
			// where the key is _not_ the searched tag.

			// Continue searching from the next key-value pair.
			return argument_<KeyHash>(
				std::forward<Default>(defaultValue), 
				std::forward<Condition>(condition), 
				std::forward<ArgumentSet>(rest)...);
		}

	}

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

		return Argument_::argument_<KeyHash>(
			std::forward<Default>(defaultValue),
			std::forward<Condition>(condition),
			std::forward<ArgumentSet>(argumentSet)...);
	}

}

#endif
