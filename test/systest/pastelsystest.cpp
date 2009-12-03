#include "pastelsystest.h"

#include "pastel/sys/log_all.h"
#include "pastel/sys/pastelomp.h"
#include "pastel/sys/eps.h"

#include <boost/integer_fwd.hpp>
#include <boost/config/compiler/visualc.hpp>

#include <iostream>
#include <string>

#include "pastel/sys/subarray.h"

using namespace std;
using namespace Pastel;

int main()
{
	log().addObserver(streamLogObserver(&cout));
	log().addObserver(fileLogObserver("log.txt"));

	log() << (nextGreater((real64)1) > 1) << logNewLine;
	log() << (nextSmaller((real64)1) < 1) << logNewLine;

	log() << (nextGreater((real32)1) > 1) << logNewLine;
	log() << (nextSmaller((real32)1) < 1) << logNewLine;

	log() << nextGreater((real64)1) << ", " << nextGreater((real64)2) << ", " << nextGreater((real64)4) 
		<< ", " << nextGreater((real64)10000) << logNewLine;

	log() << nextGreater((real32)1) << ", " << nextGreater((real32)2) << ", " << nextGreater((real32)4) 
		<< ", " << nextGreater((real32)10000) << logNewLine;

	//sysTestList().run();
	sysTestList().console();

	generateTestReport(sysTestReport(), log());

	string tmp;
	getline(cin, tmp);

	return 0;
}
