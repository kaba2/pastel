// Description: Color type

#ifndef PASTELGFX_COLOR_H
#define PASTELGFX_COLOR_H

#include "pastel/sys/vector.h"
#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	using Color = Vector<real32, 3>;

	using RgbaColor = Vector<real32, 4>;

	using ByteColor = Vector<uint8, 3>;

}

#endif
