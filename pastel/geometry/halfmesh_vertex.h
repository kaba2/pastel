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

			operator VertexData_Class&()
			{
				return data();
			}

			operator const VertexData_Class&() const
			{
				return data();
			}

			VertexData_Class& data()
			{
				return *this;
			}

			const VertexData_Class& data() const
			{
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
				return findFree().empty();
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

		protected:
			// These functions are protected, rather than
			// private, so that Vertex can be used as a base
			// class. In particular, this is needed for the 
			// Inherited_Class used in List.

			template <typename... Type>
			Vertex(Type&&... data)
			: VertexData_Class(std::forward<Type>(data)...)
			{
			}

			Vertex(const Vertex& that)
			: VertexData_Class(that) 
			{
			}

			Vertex(Vertex&& that)
			: VertexData_Class(std::move(that))
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
