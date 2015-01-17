// Description: Clamping index extended
// Documentation: indexextenders.txt

#ifndef PASTELSYS_CLAMPEXTENDER_H
#define PASTELSYS_CLAMPEXTENDER_H

#include "pastel/sys/extender/indexextender.h"

#include <cmath>

namespace Pastel
{

	class ClampExtender
		: public IndexExtender
	{
	public:
		virtual ~ClampExtender()
		{
		}

		virtual integer operator()(
			integer index, integer extent) const
		{
			if (index < 0)
			{
				return 0;
			}

			if (index >= extent)
			{
				return extent - 1;
			}

			return index;
		}
	};

	using ClampExtenderPtr = CountedPtr<ClampExtender>;
	using ConstClampExtenderPtr = CountedPtr<const ClampExtender>;

	//inline ClampExtenderPtr clampExtender()
	inline ConstIndexExtenderPtr clampExtender()
	{
		return ClampExtenderPtr(new ClampExtender);
	}

}

#endif
