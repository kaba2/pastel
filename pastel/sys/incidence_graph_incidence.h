#ifndef PASTELSYS_INCIDENCE_GRAPH_INCIDENCE_H
#define PASTELSYS_INCIDENCE_GRAPH_INCIDENCE_H

#include "pastel/sys/incidence_graph.h"

namespace Pastel
{

	template <typename Settings>
	class IncidenceGraph_Fwd<Settings>::Incidence_Link
	{
	public:
		template <typename>
		friend class IncidenceGraph_::Incidence_Iterator;

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

	template <typename Settings>
	class IncidenceGraph_Fwd<Settings>::Incidence
		: public Incidence_Link
	{
	public:
		template <typename, template <typename> class>
		friend class IncidenceGraph;

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
