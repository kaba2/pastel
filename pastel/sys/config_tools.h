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

	void createPropertyList(
		const char** propertyArray,
		integer properties,
		std::vector<std::string>& propertyList);

}

namespace Pastel
{

	void createPropertyList(
		const char** propertyArray,
		integer properties,
		std::vector<std::string>& propertyList)
	{
		std::vector<std::string> result;
		result.reserve(properties);
		
		for (integer i = 0;i < properties;++i)
		{
			result.push_back(
				std::string(propertyArray[i]));
		}
		
		result.swap(propertyList);
	}

}

#endif
