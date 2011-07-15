#ifndef PASTEL_PASTELDEVICETEST_H
#define PASTEL_PASTELDEVICETEST_H

#include "pastel/sys/log.h"
#include "pastel/sys/testrunner.h"
#include "pastel/sys/callfunction.h"

inline Pastel::TestRunner& testRunner()
{
	static Pastel::TestRunner deviceTestRunner("PastelDevice");
	return deviceTestRunner;
}

#endif
