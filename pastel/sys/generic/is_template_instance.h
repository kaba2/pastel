// Description: Check whether a type is an instance of a template

#ifndef PASTELSYS_IS_TEMPLATE_INSTANCE_H
#define PASTELSYS_IS_TEMPLATE_INSTANCE_H

#include <type_traits>

namespace Pastel
{

	template <
		typename Type,
		template <typename...> class Template>
	struct IsTemplateInstance
	: std::false_type
	{};

	template <
		template <typename...> class Template,
		typename... ArgumentSet>
	struct IsTemplateInstance<
		Template<ArgumentSet...>, Template>
	: std::true_type
	{};

}

#endif
