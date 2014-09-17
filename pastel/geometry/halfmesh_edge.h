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

			operator EdgeData_Class&()
			{
				return data();
			}

			operator const EdgeData_Class&() const
			{
				return data();
			}

			EdgeData_Class& data()
			{
				return *this;
			}

			const EdgeData_Class& data() const
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

		protected:
			// Why protected rather than private?
			// See corresponding region for Vertex.

			template <typename... Type>
			Edge(Type&&... data)
			: EdgeData_Class(std::forward<Type>(data)...)
			{
			}

			Edge(const Edge& that)
			: EdgeData_Class(that) 
			{
			}

			Edge(Edge&& that)
			: EdgeData_Class(std::move(that))
			{
			}

		private:
			template <typename, template <typename> class>
			friend class Pastel::HalfMesh;

			Half_Iterator half_;
		};

	}

}

#endif
