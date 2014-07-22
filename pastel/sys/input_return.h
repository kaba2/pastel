// Description: Input return-type

#ifndef PASTEL_INPUT_RETURN_H
#define PASTEL_INPUT_RETURN_H

#include "pastel/sys/input_concept.h"

#include <type_traits>

namespace Pastel
{

	namespace Input_
	{

		template <typename Input>
		class Input_Return
		{
		public:
			using type = decltype(std::declval<Input>.get());
		};

	}

	//! Input return-type
	template <typename Input>
	using Input_Return = typename Input_::Input_Return<Input>::type;

}

#endif
