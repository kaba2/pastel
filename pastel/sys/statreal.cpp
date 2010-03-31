#include "pastel/sys/statreal.h"

namespace Pastel
{

	namespace StatReal
	{

		PASTELSYS GlobalStats& globalStats()
		{
			static GlobalStats theGlobalStats;
			return theGlobalStats;
		}
	
	}

}


