#include "pastel_example.h"

#include "pastel/sys/logging.h"

#include <iostream>
#include <string>
#include <cstdint>

using namespace std;
using namespace Pastel;

int main()
{
	//testRunner().run();
	testRunner().console();

	generateTestReport(testReport(), log());

	string tmp;
	getline(cin, tmp);

	return 0;
}
