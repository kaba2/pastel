// Description: Half-edge structure vertex

#ifndef PASTELGEOMETRY_HALFMESH_VERTEX_H
#define PASTELGEOMETRY_HALFMESH_VERTEX_H

#include "pastel/geometry/halfmesh.h"

namespace Pastel
{

	namespace HalfMesh_
	{
	
		template <typename Settings>
		class Vertex
			: boost::less_than_comparable<Vertex<Settings>
			, boost::equality_comparable<Vertex<Settings>
			> >
			, public HalfMesh_Fwd<Settings>::VertexData_Class
		{
		public:
			using Fwd = HalfMesh_Fwd<Settings>;
			PASTEL_FWD(VertexData_Class);
			PASTEL_FWD(Half_Iterator);
			PASTEL_FWD(Half_ConstIterator);

			template <typename, template <typename> class>
			friend class HalfMesh;

			Vertex() = default;
			Vertex(const Vertex&) = default;

			operator VertexData_Class&()
			{
				return *this;
			}

			operator const VertexData_Class&() const
			{
				return *this;
			}

			template <typename Type>
			Vertex& operator=(Type&& that)
			{
				((VertexData_Class&)*this) = std::forward<Type>(that);
				return *this;
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
				if (isolated())
				{
					return true;
				}

				Half_ConstIterator begin = half();
				Half_ConstIterator current = begin;
				do
				{
					if (current->left().empty())
					{
						return true;
					}
					current = current->rotateNext();
				}
				while (current != begin);

				return false;
			}

			bool isolated() const
			{
				return half().empty();
			}

		private:
			Vertex(Vertex&&) = delete;
			Vertex& operator=(Vertex) = delete;

			Half_Iterator half_;
		};

	}

}

#endif
