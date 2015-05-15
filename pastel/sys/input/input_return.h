// Description: Input return-type

#ifndef PASTEL_INPUT_RETURN_H
#define PASTEL_INPUT_RETURN_H

#include "pastel/sys/input/input_concept.h"
#include "pastel/sys/type_traits/remove_cvref.h"

namespace Pastel
{

	template <typename Input>
	struct Input_Return_F
	: Identity_F<decltype(std::declval<RemoveCvRef<Input>>().get())>
	{};

	template <typename Input>
	using Input_Return = 
		typename Input_Return_F<Input>::type;

}

#endif
