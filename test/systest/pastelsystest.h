#ifndef PASTEL_PASTELSYSTEST_H
#define PASTEL_PASTELSYSTEST_H

#include "pastel/sys/log.h"
#include "pastel/sys/testrunner.h"
#include "pastel/sys/testreport.h"
#include "pastel/sys/testsuite.h"
#include "pastel/sys/callfunction.h"

#include <iostream>

inline Pastel::TestRunner& testRunner()
{
	static Pastel::TestRunner theTestRunner("PastelSys");
	return theTestRunner;
}

inline Pastel::TestReport& testReport()
{
	static Pastel::TestReport theTestReport("PastelSys");
	return theTestReport;
}

#endif
