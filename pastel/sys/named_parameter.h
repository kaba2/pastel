// Description: Named parameters
// Documentation: named_parameter.txt

#ifndef PASTELSYS_NAMED_PARAMETER_H
#define PASTELSYS_NAMED_PARAMETER_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/type_traits/is_template_instance.h"

#define PASTEL_ARG(name, ...) Pastel::argument<Pastel::tagHash(#name)>(__VA_ARGS__, std::forward<ArgumentSet>(argumentSet)...)
#define PASTEL_ARG_S(name, def) PASTEL_ARG(name, [&](){return def;}, Pastel::Argument_::returnTrue)
#define PASTEL_ARG_C(name, def, Concept) PASTEL_ARG(name, [&](){return def;}, [](auto input){return std::bool_constant<Concept<decltype(input)>>();})
#define PASTEL_ARG_C1(name, def, Concept, A) PASTEL_ARG(name, [&](){return def;}, [](auto input){return std::bool_constant<Concept<decltype(input), A>>();})
#define PASTEL_ARG_ENUM(name, def) PASTEL_ARG(name, [&](){return def;}, [](auto input){return std::is_same<RemoveCvRef<decltype(input)>, RemoveCvRef<decltype(def)>>();})

#define PASTEL_ARG_T(name, ...) decltype(PASTEL_ARG(name, __VA_ARGS__))
#define PASTEL_ARG_S_T(name, def) decltype(PASTEL_ARG_S(name, def))
#define PASTEL_ARG_ENUM_T(name, def) decltype(PASTEL_ARG_ENUM(name, def))

#define PASTEL_ARG_MATCHES(name, ...) Pastel::Argument<Pastel::tagHash(#name##)>::matches(__VA_ARGS__, std::forward<ArgumentSet>(argumentSet)...).value
#define PASTEL_ARG_S_MATCHES(name) PASTEL_ARG_MATCHES(name, Pastel::Argument_::returnTrue)

namespace Pastel
{

	namespace Argument_
	{

		constexpr decltype(auto) returnTrue(...)
		{
			return std::true_type();
		}

		template <typename Type>
		struct False : std::false_type {};
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

		//! Returns whether the condition matches some non-default argument.
		template <
			typename Condition,
			typename... ArgumentSet
		>
		static constexpr decltype(auto) matches(
			Condition&& condition, 
			ArgumentSet&&... argumentSet)
		{
			return
				Not<std::is_same<
					RemoveCvRef<decltype(
						argument(
							none,
							std::forward<Condition>(condition),
							std::forward<ArgumentSet>(argumentSet)...
						)
					)>, 
					None
				>>();
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
			typename Default,
			typename Condition,
			typename Key,
			typename... ArgumentSet
		>
		requires 
			IsTag<Key>::value && 
			(Tag_Hash<Key>::value == KeyHash)
		static constexpr bool argumentFoundFlag(
			Default&& defaultValue,
			Condition&& condition, 
			Key&& key,
			ArgumentSet&&... argumentSet)
		{
			// Note that we also accept the value when
			// the argument is not explicit; the explicitness
			// requires that the key be specified, 
			// not the value.

			// Check that the argument satisfies the condition.
			static_assert(
				decltype(condition(true))::value,
				"Optional argument (implicit true) is not valid for the given parameter.");

			// Check that the match is unique.
			static_assert(
				!decltype(
					matches(
						std::forward<Condition>(condition),
						std::forward<ArgumentSet>(argumentSet)...
					)
				)::value,
				"Named parameter defined multiple times.");

			// Interpret the value as boolean true.
			return true;
		}

		template <
			typename Default,
			typename Condition,
			typename Value,
			typename... ArgumentSet
		>
		requires (!IsTag<Value>::value)
		static constexpr decltype(auto) argument(
			Default&& defaultValue,
			Condition&& condition, 
			Value&& value, 
			ArgumentSet&&... argumentSet)
		{
			// The list begins with a value, so it is
			// missing a key-tag.
			static_assert(Argument_::False<Value>::value, "Optional arguments must be given in tag-value pairs.");
			return defaultValue;
		}

		template <
			typename Default,
			typename Condition,
			typename Key
		>
		requires
			IsTag<Key>::value &&
			(Tag_Hash<Key>::value == KeyHash)
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
			typename Key
		>
		requires
			IsTag<Key>::value &&
			(Tag_Hash<Key>::value != KeyHash)
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
			typename... ArgumentSet
		>
		requires
			IsTag<Key>::value &&
			(!IsTag<Value>::value) &&
			(Tag_Hash<Key>::value == KeyHash)
		static constexpr decltype(auto) argument(
			Default&&,
			Condition&& condition, 
			Key&& key, 
			Value&& value, 
			ArgumentSet&&... argumentSet)
		{
			// The list begins with a key-value pair,
			// where the key is the searched tag.

			// Check the argument satisfies the condition.
			static_assert(
				decltype(condition(value))::value,
				"Optional argument is not valid for the given parameter.");

			// Check that the match is unique.
			static_assert(
				!decltype(
					matches(
						std::forward<Condition>(condition),
						std::forward<ArgumentSet>(argumentSet)...
					)
				)::value,
				"Named parameter defined multiple times.");

			// We have found the searched argument; return it.
			return std::forward<Value>(value);
		}

		template <
			typename Default,
			typename Condition,
			typename Key,
			typename Value,
			typename... ArgumentSet
		>
		requires
			IsTag<Key>::value &&
			(Tag_Hash<Key>::value != KeyHash) &&
			(!IsTag<Value>::value)
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
			typename... ArgumentSet
		>
		requires 
			IsTag<A_Key>::value &&
			IsTag<B_Key>::value &&
			(Tag_Hash<A_Key>::value == KeyHash)
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
			typename... ArgumentSet
		>
		requires
			IsTag<A_Key>::value &&
			IsTag<B_Key>::value &&
			(Tag_Hash<A_Key>::value != KeyHash)
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
	constexpr decltype(auto) argument(
		Default&& defaultValue,
		Condition&& condition,
		ArgumentSet&&... argumentSet)
	{
		// Check that the argument satisfies the condition.
		static_assert(decltype(condition(defaultValue()))::value,
			"Default optional argument is not valid for the given parameter.");

		return Argument<KeyHash>::argument(
			std::forward<Default>(defaultValue),
			std::forward<Condition>(condition),
			std::forward<ArgumentSet>(argumentSet)...);
	}

	template <
		tag_integer KeyHash,
		typename Condition,
		typename... ArgumentSet
	>
	constexpr decltype(auto) argumentMatches(
		Condition&& condition,
		ArgumentSet&&... argumentSet)
	{
		return Argument<KeyHash>::matches(
				std::forward<Condition>(condition), 
				std::forward<ArgumentSet>(argumentSet)...);
	}

}

#endif
