// Description: Loading and saving of configuration files
// Documentation: configuration.txt

#ifndef PASTELSYS_CONFIGFILE_H
#define PASTELSYS_CONFIGFILE_H

#include "pastel/sys/config.h"
#include "pastel/sys/syslibrary.h"

#include <string>

namespace Pastel
{

	class LoadConfig_Echo
	{
	public:
		enum Enum
		{
			None,
			OnlyFinite,
			All,
		};
	};

	PASTELSYS bool loadConfig(
		const std::string& fileName,
		Config& configResult,
		LoadConfig_Echo::Enum echoPolicy = LoadConfig_Echo::None);

	PASTELSYS bool saveConfig(
		const Config& config,
		const std::string& fileName);

}

#endif
