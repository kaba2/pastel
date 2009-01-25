#include "pastelsystest.h"

#include "pastel/sys/log.h"
#include "pastel/sys/streamlogobserver.h"
#include "pastel/sys/filelogobserver.h"

#include "pastel/sys/vector.h"

#include <iostream>
#include <string>

using namespace std;
using namespace Pastel;

int main()
{
	log().addObserver(LogObserverRef(new StreamLogObserver(&cout)));
	log().addObserver(LogObserverRef(new FileLogObserver("log.txt")));

	//sysTestList().run();
	sysTestList().console();

	string tmp;
	getline(cin, tmp);

	return 0;
}
