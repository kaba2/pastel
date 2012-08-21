// Description: Testing for Config
// DocumentationOf: configfile.h

#include "test_pastelsys.h"

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

	void test()
	{
		testRead();
	}

	void addTest()
	{
		testRunner().add("Config", test);
	}

	CallFunction run(addTest);

}
