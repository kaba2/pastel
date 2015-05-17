// Description: Input value-type

#ifndef PASTEL_INPUT_VALUE_H
#define PASTEL_INPUT_VALUE_H

#include "pastel/sys/input/input_concept.h"
#include "pastel/sys/input/input_return.h"
#include "pastel/sys/type_traits/remove_cvref.h"

namespace Pastel
{

	template <typename Input>
	using Input_Value = 
		RemoveCvRef<Input_Return<Input>>;

	template <typename Input>
	using Input_Value_F = 
		Identity_F<Input_Value<Input>>;

}
	
#endif
