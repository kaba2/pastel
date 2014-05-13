// Description: Half-edge structure edge

#ifndef PASTELGEOMETRY_HALFMESH_EDGE_H
#define PASTELGEOMETRY_HALFMESH_EDGE_H

#include "pastel/geometry/halfmesh.h"

namespace Pastel
{
	
	namespace HalfMesh_
	{

		template <typename Settings>
		class Edge
			: boost::less_than_comparable<Edge<Settings>
			, boost::equality_comparable<Edge<Settings>
			> >
			, public HalfMesh_Fwd<Settings>::EdgeData_Class
		{
		public:
			using Fwd = HalfMesh_Fwd<Settings>;
			PASTEL_FWD(EdgeData_Class);
			PASTEL_FWD(Half_Iterator);
			PASTEL_FWD(Half_ConstIterator);

			template <typename, template <typename> class>
			friend class HalfMesh;

			Edge() = default;

			operator EdgeData_Class&()
			{
				return *this;
			}

			operator const EdgeData_Class&() const
			{
				return *this;
			}

			Half_Iterator half()
			{
				return half_;
			}

			Half_ConstIterator half() const
			{
				return half_;
			}

		private:
			Edge(const Edge&) = delete;
			Edge(Edge&&) = delete;
			Edge& operator=(Edge) = delete;

			Half_Iterator half_;
		};

	}

}

#endif
