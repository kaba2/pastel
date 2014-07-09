// Description: Transform input
// Documentation: inputs.txt

#ifndef PASTELSYS_TRANSFORM_INPUT_H
#define PASTELSYS_TRANSFORM_INPUT_H

#include "pastel/sys/input_concept.h"
#include "pastel/sys/function_concept.h"

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
			return input_.hint();
		}

		bool empty() const
		{
			return input_.empty();
		}

		// FIX: Replace with decltype(auto) after
		// Visual Studio 2013 fixes its bugs.
		decltype(std::declval<Function>()(std::declval<Input>()())) operator()()
		{
			return transform_(input_());
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
