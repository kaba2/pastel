#ifndef PASTEL_PASTELMATHTEST_H
#define PASTEL_PASTELMATHTEST_H

#include "pastel/sys/testrunner.h"
#include "pastel/sys/callfunction.h"
#include "pastel/sys/log.h"
#include "pastel/sys/testsuite.h"

#include <iostream>

inline Pastel::TestRunner& mathTestList()
{
	static Pastel::TestRunner mathTestRunner("PastelMath");
	return mathTestRunner;
}

inline Pastel::TestReport& mathTestReport()
{
	static Pastel::TestReport theMathTestReport("PastelMath");
	return theMathTestReport;
}

#endif
