#ifndef PASTEL_PASTELDSPTEST_H
#define PASTEL_PASTELDSPTEST_H

#include "pastel/sys/log.h"
#include "pastel/sys/testrunner.h"
#include "pastel/sys/testreport.h"
#include "pastel/sys/testsuite.h"
#include "pastel/sys/callfunction.h"
#include "pastel/sys/anystorage.h"

inline Pastel::TestRunner& dspTestList()
{
	static Pastel::TestRunner dspTestRunner("PastelDsp");
	return dspTestRunner;
}

inline Pastel::TestReport& dspTestReport()
{
	static Pastel::TestReport theDspTestReport("PastelDsp");
	return theDspTestReport;
}

inline Pastel::AnyStorage& dspStorage()
{
	static Pastel::AnyStorage dspAnyStorage;
	return dspAnyStorage;
}

#endif
