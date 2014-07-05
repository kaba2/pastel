// Description: Repeating index extender
// Documentation: indexextenders.txt

#ifndef PASTELSYS_REPEATEXTENDER_H
#define PASTELSYS_REPEATEXTENDER_H

#include "pastel/sys/indexextender.h"

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

	using RepeatExtenderPtr = CountedPtr<RepeatExtender>;
	using ConstRepeatExtenderPtr = CountedPtr<const RepeatExtender>;

	inline RepeatExtenderPtr repeatExtender()
	{
		return RepeatExtenderPtr(new RepeatExtender);
	}

}

#endif
