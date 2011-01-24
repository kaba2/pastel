// Description: Algorithms for Config class

#ifndef PASTEL_CONFIG_TOOLS_H
#define PASTEL_CONFIG_TOOLS_H

#include "pastel/sys/syslibrary.h"
#include "pastel/sys/config.h"
#include "pastel/sys/configfile.h"
#include "pastel/sys/forwardrange.h"

#include <string>
#include <vector>

namespace Pastel
{

	template <typename String_ConstIterator>
	bool checkCreated(
		const Config& config,
		const ForwardRange<String_ConstIterator>& propertySet,
		bool reportMissing = true);

}

#include "pastel/sys/config_tools.hpp"

#endif
