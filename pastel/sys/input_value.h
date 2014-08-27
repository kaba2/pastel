// Description: Input value-type

#ifndef PASTEL_INPUT_VALUE_H
#define PASTEL_INPUT_VALUE_H

#include "pastel/sys/input_concept.h"
#include "pastel/sys/input_return.h"

#include <type_traits>

namespace Pastel
{

	//! Input value-type
	template <typename Input>
	using Input_Value = std::decay_t<Input_Return<Input>>;

}
	
#endif