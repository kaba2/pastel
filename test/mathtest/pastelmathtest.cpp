#include "pastelmathtest.h"

#include "pastel/sys/math_functions.h"
#include "pastel/sys/logging.h"
#include "pastel/sys/testreport.h"

#include <iostream>
#include <string>

using namespace Pastel;

int main()
{
	Stream_Logger streamLogger(&std::cout);
	File_Logger fileLogger("log.txt");

	log().addLogger(&streamLogger);
	log().addLogger(&fileLogger);

	setInvariantFailureAction(
		InvariantFailureAction::Throw);

	testRunner().console();

	generateTestReport(testReport(), log());

	std::string tmp;
	std::getline(std::cin, tmp);

	return 0;
}
