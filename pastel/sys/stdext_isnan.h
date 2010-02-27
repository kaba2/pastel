// Description: Check if a number is a NaN

#ifndef PASTEL_STDEXT_ISNAN_H
#define PASTEL_STDEXT_ISNAN_H

namespace Pastel
{

	namespace StdExt
	{

		template <typename Type>
		bool isNan(const Type& that)
		{
			return that != that;
		}

	}

}

#endif
