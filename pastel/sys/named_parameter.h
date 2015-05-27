// Description: Named parameters

#ifndef PASTELSYS_NAMED_PARAMETER_H
#define PASTELSYS_NAMED_PARAMETER_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/generic/tag.h"

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

#define PASTEL_ARG(name, def) argument<#name##_tag>(def, std::forward<ArgumentSet>(argumentSet)...);
#define PASTEL_ARG_S(name, def) PASTEL_ARG(name, [&](){return def;})
#define PASTEL_ARG_T(name, def) Argument<#name##_tag, def (*)(), ArgumentSet...>

namespace Pastel
{

	template <
		tag_integer KeyHash,
		typename Default>
	decltype(auto) argument(Default&& defaultValue)
	{
		// No optional parameters were given;
		// return the default value.
		return defaultValue();
	}

	template <
		tag_integer KeyHash,
		typename Default,
		typename Value,
		typename... ArgumentSet,
		Requires<
			Not<IsTag<Value>>
		> = 0
	>
	decltype(auto) argument(
		Default&& defaultValue, 
		Value&& value, 
		ArgumentSet&&...)
	{
		// The list begins with a value.
		unused(defaultValue);
		unused(value);

		static constexpr bool precededByKeyTag =
			!std::is_same<Default, Default>::value;

		// Report an error of a missing key-tag.
		static_assert(precededByKeyTag,
			"An optional value is not preceded by a key-tag.");

		// Suppress possible compiler errors
		// by returning a meaningful object.
		return defaultValue();
	}

	template <
		tag_integer KeyHash,
		typename Default,
		typename Key,
		Requires<
			IsTag<Key>,
			BoolConstant<Tag_Hash<Key>::value == KeyHash>
		> ConceptCheck = 0
	>
	bool argument(
		Default&& defaultValue, 
		Key&& key)
	{
		// The list consists of a single key-tag,
		// which is the searched tag.
		unused(defaultValue);
		unused(key);

		// Interpret the value as boolean true.
		return true;
	}

	template <
		tag_integer KeyHash,
		typename Default,
		typename Key,
		Requires<
			IsTag<Key>,
			BoolConstant<Tag_Hash<Key>::value != KeyHash>
		> ConceptCheck = 0
	>
	decltype(auto) argument(
		Default&& defaultValue, 
		Key&& key)
	{
		// The list consists of a single key-tag,
		// which is _not_ the searched tag.
		unused(key);

		// Return the default-value.
		return defaultValue();
	}

	template <
		tag_integer KeyHash,
		typename Default,
		typename A_Key,
		typename B_Key,
		typename... ArgumentSet,
		Requires<
			IsTag<A_Key>,
			IsTag<B_Key>,
			BoolConstant<Tag_Hash<A_Key>::value == KeyHash>
		> ConceptCheck = 0
	>
	bool argument(
		Default&& defaultValue, 
		A_Key&& aKey, 
		B_Key&& bKey, 
		ArgumentSet&&...)
	{
		// The list begins with two subsequent key-tags,
		// the first of which is the searched tag.
		unused(defaultValue);
		unused(aKey);
		unused(bKey);

		// Interpret the value as boolean true.
		return true;
	}

	template <
		tag_integer KeyHash,
		typename Default,
		typename A_Key,
		typename B_Key,
		typename... ArgumentSet,
		Requires<
			IsTag<A_Key>,
			IsTag<B_Key>,
			BoolConstant<Tag_Hash<A_Key>::value != KeyHash>
		> ConceptCheck = 0
	>
	decltype(auto) argument(
		Default&& defaultValue, 
		A_Key&& aKey, 
		B_Key&& bKey, 
		ArgumentSet&&... rest)
	{
		// The list begins with two subsequent key-tags,
		// the first of which is _not_ the searched tag.
		unused(aKey);

		// Continue searching from the next key-value pair.
		return argument<KeyHash>(
			std::forward<Default>(defaultValue), 
			std::forward<B_Key>(bKey),
			std::forward<ArgumentSet>(rest)...);
	}

	template <
		tag_integer KeyHash,
		typename Default,
		typename Key,
		typename Value,
		typename... ArgumentSet,
		Requires<
			IsTag<Key>,
			Not<IsTag<Value>>,
			BoolConstant<Tag_Hash<Key>::value == KeyHash>
		> ConceptCheck = 0
	>
	decltype(auto) argument(
		Default&& defaultValue, 
		Key&& key, 
		Value&& value, 
		ArgumentSet&&...)
	{
		// The list begins with a key-value pair,
		// where the key is the searched tag.
		unused(defaultValue);
		unused(key);

		// We have found the searched argument; return it.
		return std::forward<Value>(value);
	}

	template <
		tag_integer KeyHash,
		typename Default,
		typename Key,
		typename Value,
		typename... ArgumentSet,
		Requires<
			IsTag<Key>,
			BoolConstant<Tag_Hash<Key>::value != KeyHash>,
			Not<IsTag<Value>>
		> ConceptCheck = 0
	>
	decltype(auto) argument(
		Default&& defaultValue, 
		Key&& key, 
		Value&& value, 
		ArgumentSet&&... rest)
	{
		// The list begins with a key-value pair,
		// where the key is _not_ the searched tag.
		unused(key);
		unused(value);

		// Continue searching from the next key-value pair.
		return argument<KeyHash>(
			std::forward<Default>(defaultValue), 
			std::forward<ArgumentSet>(rest)...);
	}

	template <
		tag_integer KeyHash,
		typename Default,
		typename... ArgumentSet>
	struct Argument_F
	{
		using type = decltype(argument<KeyHash>(std::declval<Default>(), std::declval<ArgumentSet>()...));
	};

	template <
		tag_integer KeyHash,
		typename Default,
		typename... ArgumentSet>
	using Argument = typename Argument_F<KeyHash, Default, ArgumentSet...>::type;

}

#endif
