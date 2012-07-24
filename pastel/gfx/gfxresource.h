#ifndef PASTEL_GFXRESOURCE_H
#define PASTEL_GFXRESOURCE_H

#include "pastel/gfx/color.h"

#include <unordered_map>

namespace Pastel
{

	typedef std::unordered_map<std::string, Color> ColorResource;

}

#endif
