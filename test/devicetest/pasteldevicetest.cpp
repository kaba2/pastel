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
	log().addObserver(streamLogObserver(&std::cout));
	log().addObserver(fileLogObserver("log.txt"));

	deviceTestList().console();

	string tmp;
	getline(cin, tmp);

	return 0;
}
