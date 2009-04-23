#include "pastel/sys/log.h"
#include "pastel/sys/streamlogobserver.h"
#include "pastel/sys/filelogobserver.h"

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
	log().addObserver(LogObserverPtr(new StreamLogObserver(&cout)));
	log().addObserver(LogObserverPtr(new FileLogObserver("log.txt")));

	deviceTestList().console();

	string tmp;
	getline(cin, tmp);

	return 0;
}
