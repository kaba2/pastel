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
			, fromIncidence_(that.fromIncidence_)
			, toIncidence_(that.toIncidence_)
		{
			if (Base::data())
			{
				new(Base::data()) EdgeData(that.data());
			}
		}

		Edge(Edge&& that)
			: from_(that.from_)
			, to_(that.to_)
			, fromIncidence_(that.fromIncidence_)
			, toIncidence_(that.toIncidence_)
		{
			if (Base::data())
			{
				new(Base::data()) EdgeData(std::move(that.data()));
			}
		}

		explicit Edge(
			Vertex_Iterator from,
			Vertex_Iterator to,
			EdgeData data)
			: from_(from)
			, to_(to)
			, fromIncidence_(0)
			, toIncidence_(0)
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
			return from_;
		}

		Vertex_ConstIterator from() const
		{
			return from_;
		}

		Vertex_Iterator to()
		{
			return to_;
		}

		Vertex_ConstIterator to() const
		{
			return to_;
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

		Vertex_Iterator from_;
		Vertex_Iterator to_;
		Incidence* fromIncidence_;
		Incidence* toIncidence_;
	};

}

#endif
