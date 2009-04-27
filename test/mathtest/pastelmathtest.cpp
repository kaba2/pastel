#include "pastelmathtest.h"

#include "pastel/sys/mathfunctions.h"

#include "pastel/sys/log.h"
#include "pastel/sys/streamlogobserver.h"
#include "pastel/sys/filelogobserver.h"

#include <iostream>
#include <string>

using namespace Pastel;

int main()
{
	log().addObserver(LogObserverPtr(new StreamLogObserver(&std::cout)));
	log().addObserver(LogObserverPtr(new FileLogObserver("log.txt")));

	mathTestList().console();

	std::string tmp;
	std::getline(std::cin, tmp);

	return 0;
}
