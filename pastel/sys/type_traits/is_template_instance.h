// Description: Check whether a type is an instance of a template
// Documentation: type_traits.txt

#ifndef PASTELSYS_IS_TEMPLATE_INSTANCE_H
#define PASTELSYS_IS_TEMPLATE_INSTANCE_H

#include "pastel/sys/type_traits/remove_cvref.h"

namespace Pastel
{

	namespace IsTemplateInstance_
	{

		template <
			typename Type,
			template <typename...> class Template>
		struct IsTemplateInstance__
		: std::false_type
		{};

		template <
			template <typename...> class Template,
			typename... ArgumentSet>
		struct IsTemplateInstance__<
			Template_Class<ArgumentSet...>, Template>
		: std::true_type
		{};

	}

	// Visual Studio 2015 RC has a bug in that
	// RemoveCvRef cannot be used here. Instead,
	// we have to use RemoveCvRef_F.
	template <
		typename Type,
		template <typename...> class Template>
	using IsTemplateInstance =
		IsTemplateInstance_::IsTemplateInstance__<
			typename RemoveCvRef_F<Type>::type, Template>;

}

#endif
