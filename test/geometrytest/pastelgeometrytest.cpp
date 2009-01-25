#include "pastel/sys/log.h"
#include "pastel/sys/streamlogobserver.h"
#include "pastel/sys/filelogobserver.h"

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
	Simplex2 simplex;
	Triangle2 triangle(simplex);
	Triangle2 triangle2 = simplex;
	simplex = triangle;
	triangle = simplex;

	log().addObserver(LogObserverRef(new StreamLogObserver(&cout)));
	log().addObserver(LogObserverRef(new FileLogObserver("log.txt")));

	geometryTestList().console();

	string tmp;
	getline(cin, tmp);

	return 0;
}
