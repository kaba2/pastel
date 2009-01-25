#ifndef PASTEL_PASTELRAYTEST_H
#define PASTEL_PASTELRAYTEST_H

#include "pastel/sys/testrunner.h"
#include "pastel/sys/callfunction.h"
#include "pastel/sys/log.h"

inline Pastel::TestRunner& rayTestList()
{
	static Pastel::TestRunner rayTestRunner;
	return rayTestRunner;
}

#endif
