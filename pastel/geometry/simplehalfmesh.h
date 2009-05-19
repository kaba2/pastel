#ifndef PASTEL_SIMPLEHALFMESH_H
#define PASTEL_SIMPLEHALFMESH_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/parallelarray.h"
#include "pastel/sys/indexallocator.h"
#include "pastel/sys/tuple.h"

#include "pastel/geometry/geometrylibrary.h"
#include "pastel/geometry/halfmesh.h"

namespace Pastel
{

	template <typename VertexData_, typename HalfData_,
		typename EdgeData_, typename PolygonData_>
	class SimpleHalfMesh_Policy
	{
	public:
		typedef VertexData_ VertexData;
		typedef HalfData_ HalfData;
		typedef EdgeData_ EdgeData;
		typedef PolygonData_ PolygonData;

		// Using default destructor.
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		void swap(SimpleHalfMesh_Policy& that)
		{
		}

		void clear()
		{
		}

	protected:
		void constructVertex(VertexData* data)
		{
		}

		void destructVertex(VertexData* data)
		{
		}

		void constructHalf(HalfData* data)
		{
		}

		void destructHalf(HalfData* data)
		{
		}

		void constructEdge(EdgeData* data)
		{
		}

		void destructEdge(EdgeData* data)
		{
		}

		void constructPolygon(PolygonData* data)
		{
		}

		void destructPolygon(PolygonData* data)
		{
		}
	};

	template <typename VertexData, typename HalfData,
		typename EdgeData, typename PolygonData>
	class SimpleHalfMesh
		: public HalfMesh<SimpleHalfMesh_Policy<
		VertexData, HalfData, EdgeData, PolygonData> >
	{
	};

	typedef HalfMesh<SimpleHalfMesh_Policy<EmptyClass, EmptyClass,
		EmptyClass, EmptyClass> > PureHalfMesh;

}

#include "pastel/geometry/simplehalfmesh.hpp"

#endif
