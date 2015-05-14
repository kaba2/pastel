// Description: Logical-not type-function
// Documentation: type_traits.txt

#ifndef PASTELSYS_NOT_H
#define PASTELSYS_NOT_H

#include <type_traits>

namespace Pastel
{

	template <typename Bool>
	struct Not_F
	{
		using type = 
			std::integral_constant<bool, !Bool::value>;
	};

	template <typename Bool>
	using Not = 
		typename Not_F<Bool>::type;

}

#endif
