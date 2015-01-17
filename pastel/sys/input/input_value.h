// Description: Input value-type

#ifndef PASTEL_INPUT_VALUE_H
#define PASTEL_INPUT_VALUE_H

#include "pastel/sys/input/input_concept.h"
#include "pastel/sys/input/input_return.h"

#include <type_traits>

namespace Pastel
{

	namespace Input_Value_
	{

		template <typename Input>
		struct Compute
		{
			using type = typename std::decay<Input_Return<Input>>::type;
		};

	}

	//! Input value-type
	template <typename Input>
	using Input_Value = typename Input_Value_::Compute<Input>::type;

}
	
#endif
