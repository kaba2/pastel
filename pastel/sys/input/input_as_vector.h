// Description: Input as vector

#ifndef PASTELSYS_INPUT_AS_VECTOR_H
#define PASTELSYS_INPUT_AS_VECTOR_H

#include "pastel/sys/input/input_concept.h"

#include <vector>

namespace Pastel
{

	template <
		typename Input,
		typename Type = Input_Value<Input>>
	std::vector<Type> inputAsVector(Input inputSet)
	{
		std::vector<Type> outputSet;
		outputSet.reserve(inputSet.nHint());
		
		while (!inputSet.empty())
		{
			outputSet.emplace_back(inputSet.get());
			inputSet.pop();
		}

		return outputSet;
	}


}

#endif
