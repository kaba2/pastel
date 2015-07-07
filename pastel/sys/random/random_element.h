// Description: Random element in a range

#ifndef PASTELSYS_RANDOM_ELEMENT_H
#define PASTELSYS_RANDOM_ELEMENT_H

#include "pastel/sys/input/indexed_input_concept.h"
#include "pastel/sys/input/range_input.h"
#include "pastel/sys/random/random_integer.h"

#include <initializer_list>

namespace Pastel
{

	template <
		typename Indexed_Input,
		Requires<
			Models<Indexed_Input, Indexed_Input_Concept>
		> = 0>
	decltype(auto) randomElement(
		Indexed_Input input)
	{
		PENSURE_OP(input.n(), >, 0);
		return input[randomInteger(input.n())];
	}

	template <typename Type>
	decltype(auto) randomElement(
		const std::initializer_list<Type>& elementSet)
	{
		using Pastel::randomElement;
		return randomElement(rangeInput(elementSet));
	}

}

#endif
