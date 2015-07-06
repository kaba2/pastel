// Description: Testing for PastelGeometry

#ifndef PASTEL_TEST_PASTELGEOMETRY_H
#define PASTEL_TEST_PASTELGEOMETRY_H

#include "pastel/sys/logging/log.h"
#include "pastel/sys/testing/testrunner.h"
#include "pastel/sys/testing/testreport.h"
#include "pastel/sys/testing/testsuite.h"
#include "pastel/sys/callfunction.h"

inline Pastel::TestRunner& testRunner()
{
	static Pastel::TestRunner geometryTestRunner("PastelGeometry");
	return geometryTestRunner;
}

inline Pastel::TestReport& testReport()
{
	static Pastel::TestReport theGeometryTestReport("PastelGeometry");
	return theGeometryTestReport;
}

#endif
