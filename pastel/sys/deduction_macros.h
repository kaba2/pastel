// Description: Template deduction macros
// Documentation: restrict_deduction.txt

#ifndef PASTELSYS_DEDUCTION_MACROS_H
#define PASTELSYS_DEDUCTION_MACROS_H

#define PASTEL_REMOVE_BRACKETS(x) typename Pastel::Deduction_::RemoveBrackets<void (x)>::Type

namespace Pastel
{

	namespace Deduction_
	{

		template <typename Type>
		struct Identity
		{
			using type = Type;
		};

	}

	template <typename Type>
	using NoDeduction = typename Deduction_::Identity<Type>::type;

}

#endif
