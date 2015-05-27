// Description: Check whether a type derives from a template
// Documentation: type_traits.txt

#ifndef PASTELSYS_IS_TEMPLATE_BASE_OF_H
#define PASTELSYS_IS_TEMPLATE_BASE_OF_H

#include <type_traits>

#include "pastel/sys/type_traits/is_template_instance.h"

namespace Pastel
{

	//! Checks whether a type derives from a template.
	/*!
	This type-trait is named after std::is_base_of.
	*/
	template <
		template <typename...> class Template,
		typename Type>
	struct IsTemplateBaseOf
	{
	private:
		template <typename... TypeSet>
		static std::true_type test(const Template<TypeSet...>&);

		static std::false_type test(...);

	public:
		static constexpr bool value =
			!IsTemplateInstance<Type, Template>::value &&
			decltype(test(std::declval<Type>()))::value;
	};

}

#endif
