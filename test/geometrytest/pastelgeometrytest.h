#ifndef PASTEL_PASTELGEOMETRYTEST_H
#define PASTEL_PASTELGEOMETRYTEST_H

#include "pastel/sys/log.h"
#include "pastel/sys/testrunner.h"
#include "pastel/sys/callfunction.h"

inline Pastel::TestRunner& geometryTestList()
{
	static Pastel::TestRunner geometryTestRunner;
	return geometryTestRunner;
}

#endif
