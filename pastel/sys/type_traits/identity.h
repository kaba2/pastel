// Description: Identity type-function
// Documentation: type_traits.txt

#ifndef PASTELSYS_IDENTITY_H
#define PASTELSYS_IDENTITY_H

namespace Pastel
{

	template <typename Type>
	struct Identity_F
	{
		using type = Type;
	};

	template <typename Type>
	using Identity = typename Identity_F<Type>::type;

}

#endif
