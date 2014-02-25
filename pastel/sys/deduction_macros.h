// Description: Template deduction macros
// Documentation: techniques_deduction.txt

#ifndef PASTELSYS_DEDUCTION_MACROS_H
#define PASTELSYS_DEDUCTION_MACROS_H

#define PASTEL_REMOVE_BRACKETS(x) typename Pastel::Deduction_::RemoveBrackets<void (x)>::Type

#define PASTEL_NO_DEDUCTION(x) PASTEL_REMOVE_BRACKETS(x)

namespace Pastel
{

	namespace Deduction_
	{

		//! Removes brackets around a type.
		/*!
		To transmit a type containing commas through a macro,
		it needs to be enclosed in brackets. However, then it
		is possible that the type does not make sense anymore.
		Therefore, PASTEL_REMOVE_BRACKETS macro turns this
		bracketed type into a function type 'void (T)'. From
		this form we can again regain the type inside the
		brackets using RemoveBrackets.
		*/
		template <typename T>
		class RemoveBrackets {};

		template <typename T>
		class RemoveBrackets<void (T)>
		{
		public:
			using Type = T;
		};

		template <>
		class RemoveBrackets<void ()>
		{
		public:
			using Type = void;
		};

	}

}

#endif
