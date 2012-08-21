// Description: Unit testing for PastelDevice

#ifndef PASTEL_TEST_PASTELDEVICE_H
#define PASTEL_TEST_PASTELDEVICE_H

#include "pastel/sys/log.h"
#include "pastel/sys/testrunner.h"
#include "pastel/sys/testreport.h"
#include "pastel/sys/testsuite.h"
#include "pastel/sys/callfunction.h"

inline Pastel::TestRunner& testRunner()
{
	static Pastel::TestRunner deviceTestRunner("PastelDevice");
	return deviceTestRunner;
}

inline Pastel::TestReport& testReport()
{
	static Pastel::TestReport theTestReport("PastelDevice");
	return theTestReport;
}

#endif
