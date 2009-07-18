#include "pastelsystest.h"

#include "pastel/sys/log_all.h"

#include <iostream>
#include <string>

#include "pastel/sys/subarray.h"

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
