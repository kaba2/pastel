#include "pastel/sys/logging.h"

#include "pastel/geometry/polygon_area.h"

#include "pastel/geometry/triangle.h"
#include "pastel/geometry/simplex.h"

#include <iostream>
using std::cin;

#include <string>
using std::string;
using std::getline;

#include "pastelgeometrytest.h"

using namespace Pastel;
using namespace std;

int main()
{
	Stream_Logger streamLogger(&std::cout);
	File_Logger fileLogger("log.txt");

	log().addLogger(&streamLogger);
	log().addLogger(&fileLogger);

	setInvariantFailureAction(
		InvariantFailureAction::Throw);

	testRunner().console();

	generateTestReport(testReport(), testLog());

	string tmp;
	getline(cin, tmp);

	return 0;
}
