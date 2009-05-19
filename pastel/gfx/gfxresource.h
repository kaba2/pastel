#ifndef PASTEL_GFXRESOURCE_H
#define PASTEL_GFXRESOURCE_H

#include "pastel/sys/unorderedmap.h"

#include "pastel/gfx/color.h"

namespace Pastel
{

	typedef UnorderedMap<std::string, Color> ColorResource;

}

#endif
