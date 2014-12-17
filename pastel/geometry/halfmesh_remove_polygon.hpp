// Description: Removes a polygon.

#ifndef PASTELGEOMETRY_HALFMESH_REMOVE_POLYGON_HPP
#define PASTELGEOMETRY_HALFMESH_REMOVE_POLYGON_HPP

#include "pastel/geometry/halfmesh.h"

namespace Pastel
{

	template <
		typename Settings, 
		template <typename> class Customization>
	auto HalfMesh<Settings, Customization>::removePolygon(
		const Polygon_ConstIterator& polygon)
	-> Polygon_Iterator
	{
		if (polygon.isEnd() || polygon.empty())
		{
			return cast(polygon);
		}

		this->onRemovePolygon(cast(polygon));

		Half_Iterator begin = cast(polygon->half());
		ASSERT(!begin.empty());

		Half_Iterator current = begin;
		do
		{
			current->left_ = Polygon_Iterator();
			current = current->next();
		}
		while (current != begin);

		return polygonSet_.erase(polygon);
	}

}

#endif
