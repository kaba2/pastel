// Description: Loading and saving of configuration files
// Documentation: configuration.txt

#ifndef PASTELSYS_CONFIGFILE_H
#define PASTELSYS_CONFIGFILE_H

#include "pastel/sys/config.h"

#include <string>

namespace Pastel
{

	enum class LoadConfig_Echo : integer
	{
		None,
		OnlyFinite,
		All,
	};

	bool loadConfig(
		const std::string& fileName,
		Config& configResult,
		LoadConfig_Echo echoPolicy = LoadConfig_Echo::None);

	bool saveConfig(
		const Config& config,
		const std::string& fileName);

}

#endif
