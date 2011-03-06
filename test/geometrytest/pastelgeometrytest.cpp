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
	log().addLogger(streamLogger(&cout));
	log().addLogger(fileLogger("log.txt"));

	setInvariantFailureAction(
		InvariantFailureAction::Throw);

	geometryTestList().console();

	generateTestReport(geometryTestReport(), log());

	string tmp;
	getline(cin, tmp);

	return 0;
}
