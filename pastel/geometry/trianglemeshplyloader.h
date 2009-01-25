#ifndef PASTELGEOMETRY_REALMESHPLYLOADER_H
#define PASTELGEOMETRY_REALMESHPLYLOADER_H

#include "pastel/geometry/geometrylibrary.h"
#include "pastel/geometry/trianglemesh.h"

#include <string>

namespace Pastel
{

	PASTELGEOMETRY void loadPlyMesh(const std::string& fileName,
		TriangleMesh& mesh);

}

#endif
