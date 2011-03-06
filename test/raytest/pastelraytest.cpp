#include "pastelraytest.h"

#include "pastel/sys/logging.h"

#include <iostream>
#include <string>

using namespace std;
using namespace Pastel;

int main()
{
	log().addLogger(streamLogger(&cout));
	log().addLogger(fileLogger("log.txt"));

	setInvariantFailureAction(
		InvariantFailureAction::Throw);

	rayTestList().console();

	string tmp;
	getline(cin, tmp);

	return 0;
}
