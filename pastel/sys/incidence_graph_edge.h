#ifndef PASTEL_INCIDENCE_GRAPH_EDGE_H
#define PASTEL_INCIDENCE_GRAPH_EDGE_H

#include "pastel/sys/incidence_graph.h"

namespace Pastel
{

	template <GraphType::Enum Type, typename VertexData, typename EdgeData>
	class Incidence_Graph_Fwd<Type, VertexData, EdgeData>::Edge
		: private PossiblyEmptyMember<EdgeData>
	{
	public:
		friend class Incidence_Graph<Type, VertexData, EdgeData>;

		typedef PossiblyEmptyMember<EdgeData> Base;

		Edge(const Edge& that)
			: from_(that.from_)
			, to_(that.to_)
		{
			if (Base::data())
			{
				new(Base::data()) EdgeData(that.data());
			}
		}

		Edge(Edge&& that)
			: from_(that.from_)
			, to_(that.to_)
		{
			if (Base::data())
			{
				new(Base::data()) EdgeData(std::move(that.data()));
			}
		}

		explicit Edge(EdgeData data)
			: from_(0)
			, to_(0)
		{
			if (Base::data())
			{
				new(Base::data()) EdgeData(std::move(data));
			}
		}

		~Edge()
		{
			if (Base::data())
			{
				Base::data()->~EdgeData();
			}
		}

		Vertex_Iterator from()
		{
			// Note that the to-incidence, which is stored
			// in the to-vertex, stores a reference
			// to the from-vertex of the edge. And vice versa.
			// This is because when examining a vertex, it
			// would not give you any extra-information if
			// the vertex reference were to the vertex itself.
			return to_->vertex();
		}

		Vertex_ConstIterator from() const
		{
			return to_->vertex();
		}

		Vertex_Iterator to()
		{
			return from_->vertex();
		}

		Vertex_ConstIterator to() const
		{
			return from_->vertex();
		}

		const EdgeData& data() const
		{
			PENSURE(Base::data());
			return *Base::data();
		}

		EdgeData& data()
		{
			PENSURE(Base::data());
			return *Base::data();
		}

	private:
		// Deleted
		Edge();
		// Deleted
		Edge& operator=(Edge that);

		Incidence* from_;
		Incidence* to_;
	};

}

#endif
