#include "pastelsystest.h"

#include "pastel/sys/configfile.h"

using namespace Pastel;

namespace
{

	void testRead()
	{
		Config config;
		loadConfig("config.txt", config);

		saveConfig(config, "config_output.txt");
	}

	void testBegin()
	{
		testRead();
	}

	void testAdd()
	{
		sysTestList().add("Config", testBegin);
	}

	CallFunction run(testAdd);

}
