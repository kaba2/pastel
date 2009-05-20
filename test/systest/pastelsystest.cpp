#include "pastelsystest.h"

#include "pastel/sys/log_all.h"

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

	string tmp;
	getline(cin, tmp);

	return 0;
}
