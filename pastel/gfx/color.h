// Description: Color type

#ifndef PASTEL_COLOR_H
#define PASTEL_COLOR_H

#include "pastel/sys/vector.h"
#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	typedef Vector<real32, 3> Color;

	typedef Vector<real32, 4> RgbaColor;

	typedef Vector<uint8, 3> ByteColor;

}

#endif
