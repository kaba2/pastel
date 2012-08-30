// Description: Check if a number is a NaN
// Documentation: undocumented.txt

#ifndef PASTELSYS_ISNAN_H
#define PASTELSYS_ISNAN_H

namespace Pastel
{

	template <typename Type>
	bool isNan(const Type& that)
	{
		return that != that;
	}

}

#endif
