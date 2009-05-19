#ifndef PASTEL_PLYLOADER_HPP
#define PASTEL_PLYLOADER_HPP

#include "pastel/geometry/plyloader.h"

namespace Pastel
{

	inline PlyLoader& plyLoader()
	{
		return PlyLoader::create();
	}

}

#endif
