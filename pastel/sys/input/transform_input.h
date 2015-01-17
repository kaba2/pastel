// Description: Transform input
// Documentation: input.txt

#ifndef PASTELSYS_TRANSFORM_INPUT_H
#define PASTELSYS_TRANSFORM_INPUT_H

#include "pastel/sys/input/input_concept.h"
#include "pastel/sys/function/function_concept.h"

namespace Pastel
{

	//! Transform input
	template <
		typename Input,
		typename Function>
	class Transform_Input
	{
	public:
		Transform_Input()
			: input_()
			, transform_()
		{
		}

		Transform_Input(
			Input input,
			const Function& transform)
			: input_(input)
			, transform_(transform)
		{
		}

		integer nHint() const
		{
			return input_.nHint();
		}

		bool empty() const
		{
			return input_.empty();
		}

		using Return =
			decltype(std::declval<Function>()(std::declval<Input>().get()));

		// FIX: Replace with decltype(auto) after
		// Visual Studio 2013 fixes its bugs.
		Return get() const
		{
			PENSURE(!empty());
			return transform_(input_.get());
		}

		void pop()
		{
			PENSURE(!empty());
			input_.pop();
		}

	private:
		Input input_;
		Function transform_;
	};

	template <
		typename Input,
		typename Function>
	Transform_Input<Input, Function> transformInput(
		Input input,
		const Function& transform)
	{
		return Transform_Input<Input, Function>(input, transform);
	}

}

#endif
