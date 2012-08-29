#ifndef PASTELSYS_STATREAL_TOOLS_H
#define PASTELSYS_STATREAL_TOOLS_H

#include "pastel/sys/statreal.h"

namespace Pastel
{

	PASTELSYS std::ostream& operator<<(
		std::ostream& stream,
		const StatReal::GlobalStats& stats);

}

#endif
