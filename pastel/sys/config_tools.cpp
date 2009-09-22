#include "pastel/sys/config_tools.h"
#include "pastel/sys/log.h"

namespace Pastel
{

	PASTELSYS bool checkCreated(
		const Config& config,
		const std::vector<std::string>& propertyList,
		bool reportMissing)
	{
		bool allFound = true;

		const integer properties = propertyList.size();
		for (integer i = 0;i < properties;++i)
		{
			if (!config.created(propertyList[i]))
			{
				log() << "Property '" << propertyList[i] <<
					"' not found." << logNewLine;

				allFound = false;
			}
		}

		return allFound;
	}

	PASTELSYS void createPropertyList(
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
