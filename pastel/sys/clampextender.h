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

	typedef CountedPtr<ClampExtender> ClampExtenderRef;
	typedef CountedPtr<const ClampExtender> ConstClampExtenderRef;

	//inline ClampExtenderRef clampExtender()
	inline ConstIndexExtenderRef clampExtender()
	{
		return ClampExtenderRef(new ClampExtender);
	}

}

#endif
