#ifndef PASTEL_PASTELGFXTEST_H
#define PASTEL_PASTELGFXTEST_H

#include "pastel/sys/log.h"
#include "pastel/sys/testrunner.h"
#include "pastel/sys/testreport.h"
#include "pastel/sys/testsuite.h"
#include "pastel/sys/callfunction.h"
#include "pastel/sys/anystorage.h"

#include "pastel/gfx/pcx.h"

inline Pastel::TestRunner& gfxTestList()
{
	static Pastel::TestRunner gfxTestRunner("PastelGfx");
	return gfxTestRunner;
}

inline Pastel::TestReport& gfxTestReport()
{
	static Pastel::TestReport theGfxTestReport("PastelGfx");
	return theGfxTestReport;
}

inline Pastel::AnyStorage& gfxStorage()
{
	static Pastel::AnyStorage gfxAnyStorage;
	return gfxAnyStorage;
}

#endif
