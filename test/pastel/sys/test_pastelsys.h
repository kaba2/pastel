// Description: Testing for PastelSys

#ifndef PASTEL_TEST_PASTELSYS_H
#define PASTEL_TEST_PASTELSYS_H

#include "pastel/sys/logging.h"
#include "pastel/sys/testing/testrunner.h"
#include "pastel/sys/testing/testreport.h"
#include "pastel/sys/testing/testsuite.h"
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
