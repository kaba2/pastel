// Description: Template deduction macros
// Documentation: restrict_deduction.txt

#ifndef PASTELSYS_DEDUCTION_MACROS_H
#define PASTELSYS_DEDUCTION_MACROS_H

#include "pastel/sys/type_traits/identity.h"

#define PASTEL_REMOVE_BRACKETS(x) typename Pastel::Deduction_::RemoveBrackets<void (x)>::Type

namespace Pastel
{

	template <typename Type>
	using NoDeduction = Identity<Type>;

}

#endif
