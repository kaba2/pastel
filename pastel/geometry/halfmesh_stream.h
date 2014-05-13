// Description: Half-edge structure streaming

#ifndef PASTELGEOMETRY_HALFMESH_STREAM_H
#define PASTELGEOMETRY_HALFMESH_STREAM_H

#include "pastel/geometry/halfmesh.h"

#include <iostream>

namespace Pastel
{

	template <
		typename Settings,
		template <typename> class Customization>
	std::ostream& operator<<(
		std::ostream& stream,
		const HalfMesh<Settings, Customization>& mesh);

}

#include "pastel/geometry/halfmesh_stream.hpp"

#endif
