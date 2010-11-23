#include "pasteldsptest.h"

#include "pastel/sys/log_all.h"

using namespace Pastel;

#include <string>
#include <iostream>

using namespace std;

int main()
{
	log().addLogger(streamLogger(&std::cout));
	log().addLogger(fileLogger("log.txt"));

	dspTestList().console();

	generateTestReport(dspTestReport(), log());

	std::string tmp;
	std::getline(std::cin, tmp);

	return 0;
}
