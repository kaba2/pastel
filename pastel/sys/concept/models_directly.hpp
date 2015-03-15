#ifndef PASTELSYS_MODELS_DIRECTLY_HPP
#define PASTELSYS_MODELS_DIRECTLY_HPP

#include "pastel/sys/concept/models_directly.h"
#include "pastel/sys/type_traits/compiles.h"

#include <type_traits>

namespace Pastel
{

	template <
		typename Type, 
		typename Concept>
	struct Models_Directly
		: Models_Directly<Type, Concept()>
	{
		// The concept is not in functional form.
		// Convert it to such, assuming no parameters.
	};

	template <
		typename Type,
		typename Concept,
		typename... ParameterSet>
	struct Models_Directly<Type, Concept(ParameterSet...)>
	{
	private:
		template <typename T>
		struct Test
		{
			// Type models Concept if it is possible to call
			// the requires() member function of Concept.
			// The concept is always given non-const, non-volatile,
			// non-reference arguments to work with, no matter what 
			// the passed-in types are.
			
			using type = 
				decltype(
					std::declval<Concept>().requires(
						std::declval<
							typename std::remove_cv<
								typename std::remove_reference<T>::type
							>::type
						>(), 
						std::declval<
							typename std::remove_cv<
								typename std::remove_reference<ParameterSet>::type
							>::type
						>()...
					)
				);
			
			// For some reason using std::remove_cv_t here does not 
			// work in Visual Studio 2015 CTP6. Perhaps a bug?
		};

	public:
		static PASTEL_CONSTEXPR bool value =
			Compiles<Test, Type>::value;
	};

}

#endif
