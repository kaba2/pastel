// Description: Find a template base-class

#ifndef PASTELSYS_TEMPLATE_BASE_H
#define PASTELSYS_TEMPLATE_BASE_H

namespace Pastel
{

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
	{
	private:
		template <typename... TypeSet>
		static Template<TypeSet...> test(Template<TypeSet...>&&);
		
		static void test(...);

	public:
		using type =
			decltype(test(std::declval<Type>()));
	};

	template <
		template <typename...> class Template,
		typename Type>
	using TemplateBase =
		typename TemplateBase_F<Template, Type>::type;

}

#endif
