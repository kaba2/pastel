// Description: Input return-type

#ifndef PASTEL_INPUT_RETURN_H
#define PASTEL_INPUT_RETURN_H

#include "pastel/sys/input_concept.h"

#include <type_traits>

namespace Pastel
{

	//! Input return-type
	template <typename Input>
	using Input_Return = decltype(std::declval<Input>().get());

}

#endif
