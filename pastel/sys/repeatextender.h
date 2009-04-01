#ifndef PASTELSYS_REPEATEXTENDER_H
#define PASTELSYS_REPEATEXTENDER_H

#include "pastel/sys/indexextender.h"
#include "pastel/sys/syscommon.h"

namespace Pastel
{

	class RepeatExtender
		: public IndexExtender
	{
	public:
		virtual ~RepeatExtender()
		{
		}

		virtual integer operator()(
			integer index, integer extent) const
		{
			return mod(index, extent);
		}
	};

	typedef CountedPtr<RepeatExtender> RepeatExtenderPtr;
	typedef CountedPtr<const RepeatExtender> ConstRepeatExtenderPtr;

	inline RepeatExtenderPtr repeatExtender()
	{
		return RepeatExtenderPtr(new RepeatExtender);
	}

}

#endif
