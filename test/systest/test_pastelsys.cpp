#include "test_pastelsys.h"

#include "pastel/sys/logging.h"

#include <iostream>

using namespace Pastel;

int main(int argc, const char* argv[])
{
	Stream_Logger streamLogger(&std::cout);
	File_Logger fileLogger("log.txt");

	log().addLogger(&streamLogger);
	log().addLogger(&fileLogger);

	if (argc > 1 && argv[1] == std::string("-r"))
	{
		setInvariantFailureAction(
			InvariantFailureAction::Throw);
		testRunner().run();
	}
	else
	{
		testRunner().console();
	}
	
	if (testReport().totalErrors() > 0)
	{
		generateTestReport(testReport(), log());
	}

	return testReport().totalErrors();
}
