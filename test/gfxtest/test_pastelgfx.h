// Description: Testing for PastelGfx

#ifndef PASTEL_TEST_PASTELGFX_H
#define PASTEL_TEST_PASTELGFX_H

#include "pastel/sys/logging/log.h"
#include "pastel/sys/testing/testrunner.h"
#include "pastel/sys/testing/testreport.h"
#include "pastel/sys/testing/testsuite.h"
#include "pastel/sys/callfunction.h"
#include "pastel/sys/anystorage.h"

#include "pastel/gfx/image_file/pcx.h"

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
