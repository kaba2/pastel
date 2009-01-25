#ifndef PASTEL_PASTELMATHTEST_H
#define PASTEL_PASTELMATHTEST_H

#include "pastel/sys/testrunner.h"
#include "pastel/sys/callfunction.h"
#include "pastel/sys/log.h"

#include <iostream>
using namespace std;

inline Pastel::TestRunner& mathTestList()
{
	static Pastel::TestRunner mathTestRunner;
	return mathTestRunner;
}

#endif
