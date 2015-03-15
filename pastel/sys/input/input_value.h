// Description: Input value-type

#ifndef PASTEL_INPUT_VALUE_H
#define PASTEL_INPUT_VALUE_H

#include "pastel/sys/input/input_concept.h"
#include "pastel/sys/input/input_return.h"
#include "pastel/sys/type_traits/remove_cvref.h"

namespace Pastel
{

	//! Input value-type
	template <typename Input>
	struct Input_Value_F
	{
		using type = 
			RemoveCvRef<Input_Return<Input>>;
	};

	template <typename Input>
	using Input_Value = 
		typename Input_Value_F<Input>::type;

}
	
#endif
