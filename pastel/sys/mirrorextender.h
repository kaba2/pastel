#ifndef PASTELSYS_MIRROREXTENDER_H
#define PASTELSYS_MIRROREXTENDER_H

#include "pastel/sys/indexextender.h"
#include "pastel/sys/syscommon.h"

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
			const integer doubleExtent = 2 * extent;

			integer newIndex =
				mod(index, doubleExtent - 1);

			if (newIndex >= extent)
			{
				newIndex = doubleExtent - newIndex;
			}

			return newIndex;
		}
	};

	typedef CountedPtr<MirrorExtender> MirrorExtenderRef;
	typedef CountedPtr<const MirrorExtender> ConstMirrorExtenderRef;

	inline MirrorExtenderRef mirrorExtender()
	{
		return MirrorExtenderRef(new MirrorExtender);
	}

}

#endif
