#include "pastel/sys/log.h"

namespace Pastel
{

	Log& log()
	{
		static Log theLog;
		return theLog;
	}

}

