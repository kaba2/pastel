// Description: Half-edge structure half-edge

#ifndef PASTELGEOMETRY_HALFMESH_HALF_H
#define PASTELGEOMETRY_HALFMESH_HALF_H

#include "pastel/geometry/halfmesh_fwd.h"

namespace Pastel
{
	
	namespace HalfMesh_
	{

		template <typename Settings>
		class Half
			: boost::less_than_comparable<Half<Settings>
			, boost::equality_comparable<Half<Settings>
			> >
			, public HalfMesh_Fwd<Settings>::HalfData_Class
		{
		public:
			using Fwd = HalfMesh_Fwd<Settings>;
			PASTEL_FWD(Vertex_Iterator);
			PASTEL_FWD(Vertex_ConstIterator);
			PASTEL_FWD(Half_Iterator);
			PASTEL_FWD(Half_ConstIterator);
			PASTEL_FWD(Edge_Iterator);
			PASTEL_FWD(Edge_ConstIterator);
			PASTEL_FWD(Polygon_Iterator);
			PASTEL_FWD(Polygon_ConstIterator);
			PASTEL_FWD(HalfData_Class);

			operator HalfData_Class&()
			{
				return data();
			}

			operator const HalfData_Class&() const
			{
				return data();
			}

			HalfData_Class& data()
			{
				return *this;
			}

			const HalfData_Class& data() const
			{
				return *this;
			}

			Half_Iterator rotateNext()
			{
				ASSERT(pair().isNormal());
				return pair()->next();
			}

			Half_ConstIterator rotateNext() const
			{
				return removeConst(*this).rotateNext();
			}

			Half_Iterator rotatePrevious()
			{
				ASSERT(previous().isNormal());
				return previous()->pair();
			}

			Half_ConstIterator rotatePrevious() const
			{
				return removeConst(*this).rotatePrevious();
			}

			Half_Iterator next()
			{
				return next_;
			}

			Half_ConstIterator next() const
			{
				return removeConst(*this).next();
			}

			Half_Iterator previous()
			{
				return previous_;
			}

			Half_ConstIterator previous() const
			{
				return removeConst(*this).previous();
			}

			Half_Iterator pair()
			{
				return pair_;
			}

			Half_ConstIterator pair() const
			{
				return removeConst(*this).pair();
			}

			Edge_Iterator edge()
			{
				return edge_;
			}

			Edge_ConstIterator edge() const
			{
				return removeConst(*this).edge();
			}

			Polygon_Iterator left()
			{
				return left_;
			}

			Polygon_ConstIterator left() const
			{
				return removeConst(*this).left();
			}

			Polygon_Iterator right()
			{
				ASSERT(pair().isNormal());
				return pair()->left();
			}

			Polygon_ConstIterator right() const
			{
				return removeConst(*this).right();
			}

			Vertex_Iterator origin()
			{
				return origin_;
			}

			Vertex_ConstIterator origin() const
			{
				return removeConst(*this).origin();
			}

			Vertex_Iterator destination()
			{
				ASSERT(pair().isNormal());
				return pair()->origin();
			}

			Vertex_ConstIterator destination() const
			{
				return removeConst(*this).destination();
			}

			bool free() const
			{
				return left().empty();
			}

		protected:
			// Why protected rather than private?
			// See corresponding region for Vertex.

			template <typename... Type>
			Half(Type&&... data)
			: HalfData_Class(std::forward<Type>(data)...)
			{
			}

			Half(const Half& that)
			: HalfData_Class(that) 
			{
			}

			Half(Half&& that)
			: HalfData_Class(std::move(that))
			{
			}

		private:
			template <typename, template <typename> class>
			friend class Pastel::HalfMesh;

			Half_Iterator next_;
			Half_Iterator previous_;
			Half_Iterator pair_;
			Vertex_Iterator origin_;
			Edge_Iterator edge_;
			Polygon_Iterator left_;
		};

	}

}

#endif
