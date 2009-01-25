#include "pastelsystest.h"

#include "pastel/sys/configfile.h"

using namespace Pastel;

namespace
{

	void testRead()
	{
		Config config;
		loadConfig("test_config.txt", config);

		saveConfig(config, "test_config_output.txt");
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
