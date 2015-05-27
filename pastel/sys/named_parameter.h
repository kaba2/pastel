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
		typename Key,
		Requires<
			IsTag<Key>
		> = 0
	>
	decltype(auto) argument(
		Default&& defaultValue, 
		Key&& key)
	{
		// The parameter is missing its value; raise an error.
		static_assert(
			!std::is_same<Default, Default>::value, 
			"Named parameter does not have a value; end of argument-list.");
		return 0;
	}

	template <
		tag_integer KeyHash,
		typename Default,
		typename Key,
		typename OtherKey,
		typename... ArgumentSet,
		Requires<
			IsTag<Key>,
			IsTag<OtherKey>
		> = 0
	>
	decltype(auto) argument(Default&& defaultValue, Key&& key, OtherKey&& otherKey, ArgumentSet&&...)
	{
		// The parameter is missing its value; raise an error.
		static_assert(
			!std::is_same<Default, Default>::value, 
			"Named parameter does not have a value; two sub-sequent tags.");
		return 0;
	}

	template <
		tag_integer KeyHash,
		typename Default,
		typename Key,
		typename Value,
		typename... ArgumentSet,
		Requires<
			IsTag<Key>,
			BoolConstant<Tag_Hash<Key>::value == KeyHash>,
			Not<IsTag<Value>>
		> ConceptCheck = 0
	>
	decltype(auto) argument(
		Default&& defaultValue, 
		Key&& key, 
		Value&& value, 
		ArgumentSet&&...)
	{
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
	decltype(auto) argument(Default&& defaultValue, Key&& key, Value&& value, ArgumentSet&&... rest)
	{
		// This is not the parameter we are searching for.
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
