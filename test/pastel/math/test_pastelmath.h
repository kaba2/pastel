// Description: Testing for PastelMath

#ifndef PASTEL_TEST_PASTELMATH_H
#define PASTEL_TEST_PASTELMATH_H

#include "pastel/sys/testing/testrunner.h"
#include "pastel/sys/callfunction.h"
#include "pastel/sys/logging/log.h"
#include "pastel/sys/testing/testsuite.h"

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
