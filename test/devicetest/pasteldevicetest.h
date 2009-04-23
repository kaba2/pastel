#ifndef PASTEL_PASTELDEVICETEST_H
#define PASTEL_PASTELDEVICETEST_H

#include "pastel/sys/log.h"
#include "pastel/sys/testrunner.h"
#include "pastel/sys/callfunction.h"

inline Pastel::TestRunner& deviceTestList()
{
	static Pastel::TestRunner deviceTestRunner;
	return deviceTestRunner;
}

#endif
