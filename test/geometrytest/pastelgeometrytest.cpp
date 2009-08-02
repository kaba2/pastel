#include "pastel/sys/log_all.h"

#include "pastel/geometry/polygonarea.h"

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
	log().addObserver(streamLogObserver(&cout));
	log().addObserver(fileLogObserver("log.txt"));

	geometryTestList().console();

	generateTestReport(geometryTestReport(), log());

	string tmp;
	getline(cin, tmp);

	return 0;
}
