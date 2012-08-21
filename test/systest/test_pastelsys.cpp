#include "test_pastelsys.h"

#include "pastel/sys/logging.h"
#include "pastel/sys/string_algorithms.h"
#include "pastel/sys/constants.h"

#include <iostream>
#include <string>
#include <cstdint>

using namespace std;
using namespace Pastel;

int main()
{
	Stream_Logger streamLogger(&cout);
	File_Logger fileLogger("log.txt");

	log().addLogger(&streamLogger);
	log().addLogger(&fileLogger);

	setInvariantFailureAction(
		InvariantFailureAction::Throw);

	//testRunner().run();
	testRunner().console();

	generateTestReport(testReport(), log());

	string tmp;
	getline(cin, tmp);

	return 0;
}
