#include "pastelraytest.h"

#include "pastel/sys/logging.h"

#include <iostream>
#include <string>

using namespace std;
using namespace Pastel;

int main()
{
	Stream_Logger streamLogger(&std::cout);
	File_Logger fileLogger("log.txt");

	log().addLogger(&streamLogger);
	log().addLogger(&fileLogger);

	setInvariantFailureAction(
		InvariantFailureAction::Throw);

	testRunner().console();

	string tmp;
	getline(cin, tmp);

	return 0;
}
