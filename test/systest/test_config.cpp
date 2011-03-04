// Description: Testing for Config
// DocumentationOf: configfile.h

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

	void test()
	{
		testRead();
	}

	void addTest()
	{
		sysTestList().add("Config", test);
	}

	CallFunction run(addTest);

}
