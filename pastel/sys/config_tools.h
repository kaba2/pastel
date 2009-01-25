#ifndef PASTELSYS_CONFIG_TOOLS_H
#define PASTELSYS_CONFIG_TOOLS_H

#include "pastel/sys/syslibrary.h"
#include "pastel/sys/config.h"

#include <string>
#include <vector>

namespace Pastel
{

	PASTELSYS bool checkCreated(
		const Config& config,
		const std::vector<std::string>& propertyList,
		bool reportMissing = true);

}

#endif
