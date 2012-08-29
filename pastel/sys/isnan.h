// Description: Check if a number is a NaN
// Documentation: algorithms.txt

#ifndef PASTEL_ISNAN_H
#define PASTEL_ISNAN_H

namespace Pastel
{

	template <typename Type>
	bool isNan(const Type& that)
	{
		return that != that;
	}

}

#endif
