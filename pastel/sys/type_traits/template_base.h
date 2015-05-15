// Description: Find a template base-class
// Documentation: type_traits.txt

#ifndef PASTELSYS_TEMPLATE_BASE_H
#define PASTELSYS_TEMPLATE_BASE_H

namespace Pastel
{

	namespace Template_Base_
	{

		template <template <typename...> class Template, typename... TypeSet>
		Template<TypeSet...> test(Template<TypeSet...>&&);
		
		template <template <typename...> class Template>
		inline void test(...);

	}

	//! Returns a template base-class.
	/*!
	returns:
	A base-class, or the type itself, which is an instance 
	of the given class-template.
	*/
	template <
		template <typename...> class Template,
		typename Type>
	struct TemplateBase_F
	: Identity_F<decltype(Template_Base_::test<Template>(std::declval<Type>()))>
	{};

	template <
		template <typename...> class Template,
		typename Type>
	using TemplateBase =
		typename TemplateBase_F<Template, Type>::type;

}

#endif
