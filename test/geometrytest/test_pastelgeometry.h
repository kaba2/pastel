// Description: Testing for PastelGeometry

#ifndef PASTEL_TEST_PASTELGEOMETRY_H
#define PASTEL_TEST_PASTELGEOMETRY_H

#include "pastel/sys/log.h"
#include "pastel/sys/testrunner.h"
#include "pastel/sys/testreport.h"
#include "pastel/sys/testsuite.h"
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
