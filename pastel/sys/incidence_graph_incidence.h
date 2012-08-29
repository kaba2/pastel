#ifndef PASTELSYS_INCIDENCE_GRAPH_INCIDENCE_H
#define PASTELSYS_INCIDENCE_GRAPH_INCIDENCE_H

#include "pastel/sys/incidence_graph.h"

namespace Pastel
{

	template <GraphType::Enum Type, typename VertexData, typename EdgeData>
	class Incidence_Graph_Fwd<Type, VertexData, EdgeData>::Incidence_Link
	{
	public:
		template <typename Type>
		friend class Incidence_Graph_::Incidence_Iterator;

		friend class Vertex;

		Incidence_Link()
			: next_(0)
			, prev_(0)
		{
			next_ = (Incidence*)this;
			prev_ = (Incidence*)this;
		}

	protected:
		Incidence* next_;
		Incidence* prev_;
	};

	template <GraphType::Enum Type, typename VertexData, typename EdgeData>
	class Incidence_Graph_Fwd<Type, VertexData, EdgeData>::Incidence
		: public Incidence_Link
	{
	public:
		friend class Incidence_Graph<Type, VertexData, EdgeData>;

		Incidence()
			: vertex_()
			, edge_()
		{
		}

		explicit Incidence(Vertex_Iterator vertex)
			: vertex_(vertex)
			, edge_()
		{
		}

		Vertex_Iterator vertex()
		{
			return vertex_;
		}

		Vertex_ConstIterator vertex() const
		{
			return vertex_;
		}

		Edge_Iterator edge()
		{
			return edge_;
		}

		Edge_ConstIterator edge() const
		{
			return edge_;
		}

	private:
		Vertex_Iterator vertex_;
		Edge_Iterator edge_;
	};

}

#endif
