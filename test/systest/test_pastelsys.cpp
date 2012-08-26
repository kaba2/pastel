#include "test_pastelsys.h"

#include "pastel/sys/logging.h"

#include <iostream>
#include <string>

using namespace Pastel;

int main(integer argc, const char* argv[])
{
	Stream_Logger streamLogger(&std::cout);
	File_Logger fileLogger("log.txt");

	log().addLogger(&streamLogger);
	log().addLogger(&fileLogger);

	setInvariantFailureAction(
		InvariantFailureAction::Throw);

	if (argc > 1 && argv[1] == std::string("-c"))
	{
		testRunner().console();
	}
	else
	{
		testRunner().run();
	}
	
	if (testReport().totalErrors() > 0)
	{
		generateTestReport(testReport(), log());
	}

	return testReport().totalErrors();
}
