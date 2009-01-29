#ifndef PASTELGFX_INDEXWRAPPER_H
#define PASTELGFX_INDEXWRAPPER_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/syscommon.h"
#include "pastel/gfx/gfxlibrary.h"

namespace Pastel
{

	class PASTELGFX MirrorWrapper
	{
	public:
		// Using default constructor.
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		integer operator()(integer index,
			integer width) const
		{
			/*
			const integer periodicIndex = mod(index, 2 * width);
			if (periodicIndex >= width)
			{
				return (2 * width - 1) - periodicIndex;
			}
			*/

			const integer periodicIndex = mod(index, 2 * (width - 1));
			if (periodicIndex >= width)
			{
				return (2 * (width - 1)) - periodicIndex;
			}

			return periodicIndex;
		}
	};

	class PASTELGFX RepeatWrapper
	{
	public:
		// Using default constructor.
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		integer operator()(integer index,
			integer width) const
		{
			return mod(index, width);
		}
	};

	class PASTELGFX ClampWrapper
	{
	public:
		// Using default constructor.
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		integer operator()(integer index,
			integer width) const
		{
			if (index < 0)
			{
				return 0;
			}

			if (index >= width)
			{
				return width - 1;
			}

			return index;
		}
	};

}

#endif
