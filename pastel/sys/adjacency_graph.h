#ifndef PASTEL_ADJACENCY_GRAPH_H
#define PASTEL_ADJACENCY_GRAPH_H

#include "pastel/sys/adjacency_graph_fwd.h"

namespace Pastel
{

	template <typename VertexData = EmptyClass, typename EdgeData = EmptyClass>
	class Adjacency_Graph
		: public Adjacency_Graph_Fwd<VertexData, EdgeData>
	{
	public:
		Adjacency_Graph()
			: vertexSet_()
			, edgeSet_()
		{
		}

		Adjacency_Graph(Adjacency_Graph&& that)
			: vertexSet_()
			, edgeSet_()
		{
			swap(that);
		}

		Adjacency_Graph& operator=(Adjacency_Graph that)
		{
			swap(that);
			return *this;
		}

		void swap(Adjacency_Graph& that)
		{
			vertexSet_.swap(that.vertexSet_);
			edgeSet_.swap(that.edgeSet_);
		}

		void clear()
		{
			vertexSet_.clear();
			edgeSet_.clear();
		}

		// Vertices

		Vertex_Iterator addVertex(
			VertexData vertexData = VertexData())
		{
			vertexSet_.emplace_back(
				Vertex(std::move(vertexData), edgeSet_.end()));

			Vertex_Iterator vertex = vertexSet_.end();
			--vertex;

			return vertex;
		}

		void removeVertex(
			const Vertex_ConstIterator& vertex)
		{
			// Remove all the edges that leave
			// from this vertex.
			edgeSet_.erase(
				vertex->cbegin(),
				vertex->cend());
			cast(vertex)->erase(edgeSet_.end());

			// Remove the vertex.
			vertexSet_.erase(vertex);
		}

		Vertex_Iterator cast(
			const Vertex_ConstIterator& vertex)
		{
			return vertexSet_.erase(vertex, vertex);
		}

		Vertex_Iterator vertexBegin()
		{
			return vertexSet_.begin();
		}

		Vertex_ConstIterator cVertexBegin() const
		{
			return vertexSet_.cbegin();
		}

		Vertex_Iterator vertexEnd()
		{
			return vertexSet_.end();
		}

		Vertex_ConstIterator cVertexEnd() const
		{
			return vertexSet_.cend();
		}

		integer vertices() const
		{
			return vertexSet_.size();
		}

		// Edges

		Edge_Iterator addEdge(
			const Vertex_ConstIterator& from,
			const Vertex_ConstIterator& to,
			EdgeData edgeData = EdgeData())
		{
			Vertex_Iterator mutableFrom = cast(from);
			Vertex_Iterator mutableTo = cast(to);

			edgeSet_.emplace_back(
				Edge(mutableFrom, mutableTo, 
				std::move(edgeData)));

			Edge_Iterator edge = edgeSet_.end();
			--edge;

			edgeSet_.splice(
				mutableFrom->end(),
				edgeSet_,
				edge);

			mutableFrom->insert(edge);

			return edge;
		}

		void removeEdge(
			const Edge_ConstIterator& edge)
		{
			Vertex_Iterator from =
				cast(edge->from());
			
			from->erase(cast(edge), edgeSet_.end());

			edgeSet_.erase(edge);
		}

		Edge_Iterator cast(
			const Edge_ConstIterator& edge)
		{
			return edgeSet_.erase(edge, edge);
		}

		Edge_Iterator edgeBegin()
		{
			return edgeSet_.begin();
		}

		Edge_ConstIterator cEdgeBegin() const
		{
			return edgeSet_.cbegin();
		}

		Edge_Iterator edgeEnd()
		{
			return edgeSet_.end();
		}

		Edge_ConstIterator cEdgeEnd() const
		{
			return edgeSet_.cend();
		}

		integer edges() const
		{
			return edgeSet_.size();
		}

	private:
		// TODO: Implement
		Adjacency_Graph(const Adjacency_Graph& that);

		VertexSet vertexSet_;
		EdgeSet edgeSet_;
	};

}

#endif
