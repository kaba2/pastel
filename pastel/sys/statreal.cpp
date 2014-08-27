#include "pastel/sys/statreal.h"

namespace Pastel
{

	namespace StatReal
	{

		GlobalStats& globalStats()
		{
			static GlobalStats theGlobalStats;
			return theGlobalStats;
		}
	
	}

}


