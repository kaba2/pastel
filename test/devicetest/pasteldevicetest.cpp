#include "pastel/sys/log_all.h"

#include <iostream>
using std::cin;

#include <string>
using std::string;
using std::getline;

#include "pasteldevicetest.h"

using namespace Pastel;
using namespace std;

int main()
{
	log().addLogger(streamLogger(&std::cout));
	log().addLogger(fileLogger("log.txt"));

	deviceTestList().console();

	string tmp;
	getline(cin, tmp);

	return 0;
}
