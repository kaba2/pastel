#ifndef PASTELGEOMETRY_PLYLOADER_HPP
#define PASTELGEOMETRY_PLYLOADER_HPP

#include "pastel/geometry/plyloader.h"

namespace Pastel
{

	inline PlyLoader& plyLoader()
	{
		return PlyLoader::create();
	}

}

#endif
