// Description: Unit testing for PastelRay

#ifndef PASTEL_PASTELRAYTEST_H
#define PASTEL_PASTELRAYTEST_H

#include "pastel/sys/testrunner.h"
#include "pastel/sys/testreport.h"
#include "pastel/sys/testsuite.h"
#include "pastel/sys/callfunction.h"
#include "pastel/sys/log.h"

inline Pastel::TestRunner& testRunner()
{
	static Pastel::TestRunner theTestRunner("PastelRay");
	return theTestRunner;
}

inline Pastel::TestReport& testReport()
{
	static Pastel::TestReport theTestReport("PastelRay");
	return theTestReport;
}

#endif
