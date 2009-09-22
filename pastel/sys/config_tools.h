// Description: Algorithms for Config class

#ifndef PASTEL_CONFIG_TOOLS_H
#define PASTEL_CONFIG_TOOLS_H

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

	PASTELSYS void createPropertyList(
		const char** propertyArray,
		integer properties,
		std::vector<std::string>& propertyList);

}

#endif
