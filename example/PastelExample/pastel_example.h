// Description: Examples for Pastel

#ifndef PASTEL_PASTEL_EXAMPLE_H
#define PASTEL_PASTEL_EXAMPLE_H

#include "pastel/sys/logging.h"
#include "pastel/sys/testing/testrunner.h"
#include "pastel/sys/testing/testreport.h"
#include "pastel/sys/testing/testsuite.h"
#include "pastel/sys/callfunction.h"

#include <iostream>

inline Pastel::TestRunner& testRunner()
{
	static Pastel::TestRunner theTestRunner("PastelExample");
	return theTestRunner;
}

inline Pastel::TestReport& testReport()
{
	static Pastel::TestReport theTestReport("PastelExample");
	return theTestReport;
}

#endif
