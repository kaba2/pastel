#ifndef PASTELGEOMETRY_SIMPLEXMESH_DATAPOLICIES_H
#define PASTELGEOMETRY_SIMPLEXMESH_DATAPOLICIES_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	template <typename VertexData_, typename SimplexData_>
	class SimplexMesh_DefaultDataPolicy
	{
	public:
		typedef VertexData_ VertexData;
		typedef SimplexData_ SimplexData;

		void swap(SimplexMesh_DefaultDataPolicy& that)
		{
		}

		void clear()
		{
		}

		void constructVertex(VertexData* vertex)
		{
		}

		void constructSimplex(SimplexData* simplex)
		{
		}

		void destructVertex(VertexData* vertex)
		{
		}

		void destructSimplex(SimplexData* simplex)
		{
		}
	};

	typedef SimplexMesh_DefaultDataPolicy<EmptyClass, EmptyClass>
		SimplexMesh_EmptyDataPolicy;

}

#endif
