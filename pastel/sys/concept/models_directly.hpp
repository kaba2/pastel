#ifndef PASTELSYS_MODELS_DIRECTLY_HPP
#define PASTELSYS_MODELS_DIRECTLY_HPP

#include "pastel/sys/concept/models_directly.h"
#include "pastel/sys/type_traits/compiles.h"
#include "pastel/sys/type_traits/remove_cvref.h"

#include <type_traits>

namespace Pastel
{

	template <
		typename Type, 
		typename Concept>
	struct Models_Directly
	{
		// The concept is not in functional form.
		// Convert it to such, assuming no parameters.
		static constexpr bool value = 
			Models_Directly<Type, Concept()>::value;
	};

	template <
		typename Type,
		typename Concept,
		typename... ParameterSet>
	struct Models_Directly<Type, Concept(ParameterSet...)>
	{
	private:
		// Type models Concept if it is possible to call
		// the requires() member function of Concept.
		// The concept is always given non-const, non-volatile,
		// non-reference arguments to work with, no matter what 
		// the passed-in types are.
		template <
			typename T,
			typename =
				decltype(
					std::declval<Concept>().requires(
						std::declval<
							RemoveCvRef<T>
						>(), 
						std::declval<
							ParameterSet
						>()...
					)
				)
		>
		struct Test
		{
			// The class needs to be defined here ({};); otherwise 
			// it triggers a bug in Visual Studio 2015 RC.
		};

	public:
		static constexpr bool value =
			Compiles<Test, Type>::value;
	};

}

#endif
