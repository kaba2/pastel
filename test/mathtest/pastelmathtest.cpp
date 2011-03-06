#include "pastelmathtest.h"

#include "pastel/sys/math_functions.h"
#include "pastel/sys/logging.h"
#include "pastel/sys/testreport.h"

#include <iostream>
#include <string>

using namespace Pastel;

int main()
{
	log().addLogger(streamLogger(&std::cout));
	log().addLogger(fileLogger("log.txt"));

	setInvariantFailureAction(
		InvariantFailureAction::Throw);

	mathTestList().console();

	generateTestReport(mathTestReport(), log());

	std::string tmp;
	std::getline(std::cin, tmp);

	return 0;
}
