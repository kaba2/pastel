#ifndef PASTEL_PASTELSYSTEST_H
#define PASTEL_PASTELSYSTEST_H

#include "pastel/sys/log.h"
#include "pastel/sys/testrunner.h"
#include "pastel/sys/testreport.h"
#include "pastel/sys/testsuite.h"
#include "pastel/sys/callfunction.h"

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

inline Pastel::Log& testLog()
{
	static Pastel::Log theLog;
	return theLog;
}

#endif
