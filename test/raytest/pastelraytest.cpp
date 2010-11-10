#include "pastelraytest.h"

#include "pastel/sys/log_all.h"

#include <iostream>
#include <string>

using namespace std;
using namespace Pastel;

int main()
{
	log().addLogger(streamLogger(&cout));
	log().addLogger(fileLogger("log.txt"));

	rayTestList().console();

	string tmp;
	getline(cin, tmp);

	return 0;
}
