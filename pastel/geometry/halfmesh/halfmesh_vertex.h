// Description: Half-edge structure vertex

#ifndef PASTELGEOMETRY_HALFMESH_VERTEX_H
#define PASTELGEOMETRY_HALFMESH_VERTEX_H

#include "pastel/geometry/halfmesh/halfmesh_fwd.h"

namespace Pastel
{

	namespace HalfMesh_
	{
	
		template <typename Settings>
		class Vertex
		{
		public:
			using Fwd = HalfMesh_Fwd<Settings>;
			PASTEL_FWD(VertexData);
			PASTEL_FWD(Half_Iterator);
			PASTEL_FWD(Half_ConstIterator);

			template <typename... Type>
			Vertex(Type&&... data)
			: data_(std::forward<Type>(data)...)
			{
			}

			Vertex(const Vertex& that)
			: data_(that.data_)
			{
			}

			Vertex(Vertex&& that)
			: data_(std::move(that.data_))
			{
			}

			VertexData& data()
			{
				return data_;
			}

			const VertexData& data() const
			{
				return data_;
			}

			Half_Iterator half()
			{
				return half_;
			}

			Half_ConstIterator half() const
			{
				return removeConst(*this).half();
			}

			bool free() const
			{
				return isolated() || !findFree().empty();
			}

			Half_ConstIterator findFree() const
			{
				if (isolated())
				{
					return Half_ConstIterator();
				}

				Half_ConstIterator begin = half();
				Half_ConstIterator current = begin;
				do
				{
					if (current->left().empty())
					{
						return current;
					}
					current = current->rotateNext();
				}
				while (current != begin);

				return Half_ConstIterator();
			}

			bool isolated() const
			{
				return half().empty();
			}

		private:
			template <typename, template <typename> class>
			friend class Pastel::HalfMesh;

			BOOST_ATTRIBUTE_NO_UNIQUE_ADDRESS
			VertexData data_;
			Half_Iterator half_;
		};

	}

}

#endif
