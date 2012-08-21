// Description: Algorithms for Config class

#ifndef PASTEL_CONFIG_TOOLS_H
#define PASTEL_CONFIG_TOOLS_H

#include "pastel/sys/syslibrary.h"
#include "pastel/sys/config.h"
#include "pastel/sys/configfile.h"

namespace Pastel
{

	template <typename String_ConstRange>
	bool checkCreated(
		const Config& config,
		const String_ConstRange& propertySet,
		bool reportMissing = true);

}

#include "pastel/sys/config_tools.hpp"

#endif
