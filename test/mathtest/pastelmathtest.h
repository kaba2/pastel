// Description: Unit testing for PastelMath

#ifndef PASTEL_PASTELMATHTEST_H
#define PASTEL_PASTELMATHTEST_H

#include "pastel/sys/testrunner.h"
#include "pastel/sys/callfunction.h"
#include "pastel/sys/log.h"
#include "pastel/sys/testsuite.h"

#include <iostream>

inline Pastel::TestRunner& testRunner()
{
	static Pastel::TestRunner theTestRunner("PastelMath");
	return theTestRunner;
}

inline Pastel::TestReport& testReport()
{
	static Pastel::TestReport theTestReport("PastelMath");
	return theTestReport;
}

#endif
