// Documentation: indexextender.txt

#ifndef PASTEL_MIRROREXTENDER_H
#define PASTEL_MIRROREXTENDER_H

#include "pastel/sys/indexextender.h"

namespace Pastel
{

	class MirrorExtender
		: public IndexExtender
	{
	public:
		virtual ~MirrorExtender()
		{
		}

		virtual integer operator()(
			integer index, integer extent) const
		{
			// -1 => 1
			// extent => extent - 2

			const integer doubleExtent = 2 * extent;

			integer newIndex =
				mod(index, doubleExtent - 1);

			if (newIndex >= extent)
			{
				newIndex = (doubleExtent - 1) - newIndex;
			}

			return newIndex;
		}
	};

	typedef CountedPtr<MirrorExtender> MirrorExtenderPtr;
	typedef CountedPtr<const MirrorExtender> ConstMirrorExtenderPtr;

	inline MirrorExtenderPtr mirrorExtender()
	{
		return MirrorExtenderPtr(new MirrorExtender);
	}

}

#endif
