#ifndef PASTEL_PASTELGFXTEST_H
#define PASTEL_PASTELGFXTEST_H

#include "pastel/sys/log.h"
#include "pastel/sys/testrunner.h"
#include "pastel/sys/callfunction.h"

inline Pastel::TestRunner& gfxTestList()
{
	static Pastel::TestRunner gfxTestRunner("PastelGfx");
	return gfxTestRunner;
}

#endif
