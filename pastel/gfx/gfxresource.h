// Description: Gfx resource storage
// Documentation: undocumented.txt

#ifndef PASTELGFX_GFXRESOURCE_H
#define PASTELGFX_GFXRESOURCE_H

#include "pastel/gfx/color/color.h"

#include <unordered_map>

namespace Pastel
{

	using ColorResource = std::unordered_map<std::string, Color>;

}

#endif
