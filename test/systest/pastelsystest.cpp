#include "pastelsystest.h"

#include "pastel/sys/log_all.h"
#include "pastel/sys/string_tools.h"
#include "pastel/sys/constants.h"

#include <iostream>
#include <string>

using namespace std;
using namespace Pastel;

int main()
{


	log().addObserver(streamLogObserver(&cout));
	log().addObserver(fileLogObserver("log.txt"));

	//sysTestList().run();
	sysTestList().console();

	generateTestReport(sysTestReport(), log());

	string tmp;
	getline(cin, tmp);

	return 0;
}
