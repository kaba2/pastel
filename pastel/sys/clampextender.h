#ifndef PASTELSYS_CLAMPEXTENDER_H
#define PASTELSYS_CLAMPEXTENDER_H

#include "pastel/sys/indexextender.h"

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

	typedef CountedPtr<ClampExtender> ClampExtenderPtr;
	typedef CountedPtr<const ClampExtender> ConstClampExtenderPtr;

	//inline ClampExtenderPtr clampExtender()
	inline ConstIndexExtenderPtr clampExtender()
	{
		return ClampExtenderPtr(new ClampExtender);
	}

}

#endif
