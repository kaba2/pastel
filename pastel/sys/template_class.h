// Description: Class-wrapper for a class template

#ifndef PASTELSYS_TEMPLATE_OBJECT_H
#define PASTELSYS_TEMPLATE_OBJECT_H

namespace Pastel
{

	//! Class-wrapper for a class template
	/*!
	The parameter of Template_Class captures the 
	name of the class template. The nested alias 
	template captures the template parameters.
	*/
	template <template <typename...> class Template>
	struct Template_Class
	{
		template <typename... ParameterSet>
		using Class = Template<ParameterSet...>;
	};

}

#endif
