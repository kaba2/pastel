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
			Template<ArgumentSet...>, Template>
		: std::true_type
		{};

	}

	template <
		typename Type,
		template <typename...> class Template>
	using IsTemplateInstance =
		IsTemplateInstance_::IsTemplateInstance__<
			RemoveCvRef<Type>, Template>;

}

#endif
