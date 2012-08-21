// Description: Unit testing for PastelDsp

#ifndef PASTEL_PASTELDSPTEST_H
#define PASTEL_PASTELDSPTEST_H

#include "pastel/sys/log.h"
#include "pastel/sys/testrunner.h"
#include "pastel/sys/testreport.h"
#include "pastel/sys/testsuite.h"
#include "pastel/sys/callfunction.h"
#include "pastel/sys/anystorage.h"

inline Pastel::TestRunner& testRunner()
{
	static Pastel::TestRunner theTestRunner("PastelDsp");
	return theTestRunner;
}

inline Pastel::TestReport& testReport()
{
	static Pastel::TestReport theTestReport("PastelDsp");
	return theTestReport;
}

inline Pastel::AnyStorage& dspStorage()
{
	static Pastel::AnyStorage dspAnyStorage;
	return dspAnyStorage;
}

#endif
