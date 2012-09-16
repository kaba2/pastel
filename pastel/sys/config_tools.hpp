#ifndef PASTELSYS_CONFIG_TOOLS_HPP
#define PASTELSYS_CONFIG_TOOLS_HPP

#include "pastel/sys/config_tools.h"
#include "pastel/sys/log.h"

#include <string>

namespace Pastel
{

	template <typename String_ConstRange>
	bool checkCreated(
			const Config& config,
			const String_ConstRange& propertySet,
			bool reportMissing)
	{
		bool allFound = true;

		auto iter = propertySet.begin();
		auto iterEnd = propertySet.end();

		while(iter != iterEnd)
		{
			const std::string& name = *iter;
			if (!config.created(name))
			{
				log() << "Property '" << name <<
					"' not found." << logNewLine;

				allFound = false;
			}

			++iter;
		}

		return allFound;
	}

}

#endif
