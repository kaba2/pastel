// Description: Half-edge structure edge

#ifndef PASTELGEOMETRY_HALFMESH_EDGE_H
#define PASTELGEOMETRY_HALFMESH_EDGE_H

#include "pastel/geometry/halfmesh/halfmesh_fwd.h"

namespace Pastel
{
	
	namespace HalfMesh_
	{

		template <typename Settings>
		class Edge
		{
		public:
			using Fwd = HalfMesh_Fwd<Settings>;
			PASTEL_FWD(EdgeData);
			PASTEL_FWD(Half_Iterator);
			PASTEL_FWD(Half_ConstIterator);

			template <typename... Type>
			Edge(Type&&... data)
			: data_(std::forward<Type>(data)...)
			{
			}

			Edge(const Edge& that)
			: data_(that.data_) 
			{
			}

			Edge(Edge&& that)
			: data_(std::move(that.data_))
			{
			}

			EdgeData& data()
			{
				return data_;
			}

			const EdgeData& data() const
			{
				return data_;
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
			template <typename, template <typename> class>
			friend class Pastel::HalfMesh;

			BOOST_ATTRIBUTE_NO_UNIQUE_ADDRESS
			EdgeData data_;
			Half_Iterator half_;
		};

	}

}

#endif
