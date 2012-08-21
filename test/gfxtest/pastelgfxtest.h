// Description: Unit testing for PastelGfx

#ifndef PASTEL_PASTELGFXTEST_H
#define PASTEL_PASTELGFXTEST_H

#include "pastel/sys/log.h"
#include "pastel/sys/testrunner.h"
#include "pastel/sys/testreport.h"
#include "pastel/sys/testsuite.h"
#include "pastel/sys/callfunction.h"
#include "pastel/sys/anystorage.h"

#include "pastel/gfx/pcx.h"

inline Pastel::TestRunner& testRunner()
{
	static Pastel::TestRunner theTestRunner("PastelGfx");
	return theTestRunner;
}

inline Pastel::TestReport& testReport()
{
	static Pastel::TestReport theTestReport("PastelGfx");
	return theTestReport;
}

inline Pastel::AnyStorage& gfxStorage()
{
	static Pastel::AnyStorage gfxAnyStorage;
	return gfxAnyStorage;
}

#endif
