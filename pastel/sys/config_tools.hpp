#ifndef PASTEL_CONFIG_TOOLS_HPP
#define PASTEL_CONFIG_TOOLS_HPP

#include "pastel/sys/config_tools.h"

namespace Pastel
{

	template <typename String_ConstIterator>
	bool checkCreated(
			const Config& config,
			const ForwardRange<String_ConstIterator>& propertySet,
			bool reportMissing)
	{
		bool allFound = true;

		String_ConstIterator iter = propertySet.begin();
		const String_ConstIterator iterEnd = propertySet.end();

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
