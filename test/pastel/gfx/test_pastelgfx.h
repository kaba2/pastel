// Description: Testing for PastelGfx

#ifndef PASTEL_TEST_PASTELGFX_H
#define PASTEL_TEST_PASTELGFX_H

#include "test/catch.hpp"

#include "pastel/sys/logging/log.h"
#include "pastel/sys/callfunction.h"
#include "pastel/sys/anystorage.h"

#include "pastel/gfx/image_file/pcx.h"

inline Pastel::AnyStorage& gfxStorage()
{
	static Pastel::AnyStorage gfxAnyStorage;
	return gfxAnyStorage;
}

using namespace Pastel;

#endif
