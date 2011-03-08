// Description: SimpleHalfMesh_Policy class
// Detail: Runs no additional code run besides construction and destruction.
// Documentation: halfmesh.txt

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
			if (data)
			{
				new(data) VertexData;
			}
		}

		void destructVertex(VertexData* data)
		{
			destruct(data);
		}

		void constructHalf(HalfData* data)
		{
			if (data)
			{
				new(data) HalfData;
			}
		}

		void destructHalf(HalfData* data)
		{
			destruct(data);
		}

		void constructEdge(EdgeData* data)
		{
			if (data)
			{
				new(data) EdgeData;
			}
		}

		void destructEdge(EdgeData* data)
		{
			destruct(data);
		}

		void constructPolygon(PolygonData* data)
		{
			if (data)
			{
				new(data) PolygonData;
			}
		}

		void destructPolygon(PolygonData* data)
		{
			destruct(data);
		}
	};

	template <typename VertexData, typename HalfData,
		typename EdgeData, typename PolygonData>
	class SimpleHalfMesh
		: public HalfMesh<SimpleHalfMesh_Policy<
		VertexData, HalfData, EdgeData, PolygonData> >
	{
	};

	typedef SimpleHalfMesh_Policy<EmptyClass, EmptyClass,
		EmptyClass, EmptyClass> PureHalfMesh_Policy;

	typedef HalfMesh<PureHalfMesh_Policy> PureHalfMesh;

}

#include "pastel/geometry/simplehalfmesh.hpp"

#endif
