#ifndef PASTEL_PASTELSYSTEST_H
#define PASTEL_PASTELSYSTEST_H

#include "pastel/sys/log.h"
#include "pastel/sys/testrunner.h"
#include "pastel/sys/callfunction.h"

inline Pastel::TestRunner& sysTestList()
{
	static Pastel::TestRunner sysTestRunner("PastelSys");
	return sysTestRunner;
}

#endif
