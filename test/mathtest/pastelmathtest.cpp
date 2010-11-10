#include "pastelmathtest.h"

#include "pastel/sys/math_functions.h"
#include "pastel/sys/log_all.h"
#include "pastel/sys/testreport.h"

#include <iostream>
#include <string>

using namespace Pastel;

int main()
{
	log().addLogger(streamLogger(&std::cout));
	log().addLogger(fileLogger("log.txt"));

	mathTestList().console();

	generateTestReport(mathTestReport(), log());

	std::string tmp;
	std::getline(std::cin, tmp);

	return 0;
}
