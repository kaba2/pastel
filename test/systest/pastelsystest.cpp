#include "pastelsystest.h"

#include "pastel/sys/logging.h"
#include "pastel/sys/string_algorithms.h"
#include "pastel/sys/constants.h"

#include <iostream>
#include <string>

using namespace std;
using namespace Pastel;

int main()
{
	log().addLogger(streamLogger(&cout));
	log().addLogger(fileLogger("log.txt"));

	setInvariantFailureAction(
		InvariantFailureAction::Throw);

	//sysTestList().run();
	sysTestList().console();

	generateTestReport(sysTestReport(), log());

	string tmp;
	getline(cin, tmp);

	return 0;
}
