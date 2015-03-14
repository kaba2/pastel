// Description: Checks whether a type is a Refines<...> class

#ifndef PASTELSYS_IS_REFINES_CLASS_H
#define PASTELSYS_IS_REFINES_CLASS_H

#include "pastel/sys/concept/refines.h"
#include "pastel/sys/type_traits/is_template_instance.h"

#include <type_traits>

namespace Pastel
{

	//! Returns whether a type is a Refines<...> class.
	template <typename Concept>
	struct IsRefinesClass
		: IsTemplateInstance<Concept, Refines>
	{};

}

#endif
