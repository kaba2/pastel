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
			, edges_(0)
		{
		}

		Adjacency_Graph(const Adjacency_Graph& that)
			: vertexSet_(that.vertexSet_)
			, edges_(that.edges_)
		{
		}

		Adjacency_Graph(Adjacency_Graph&& that)
			: vertexSet_()
			, edges_(0)
		{
			swap(that);
		}

		Adjacency_Graph& operator=(Adjacency_Graph&& that)
		{
			swap(that);
			return *this;
		}

		Adjacency_Graph& operator=(const Adjacency_Graph& that)
		{
			Adjacency_Graph copy(that);
			swap(copy);
			return *this;
		}

		void swap(Adjacency_Graph& that)
		{
			using std::swap;
			vertexSet_.swap(that.vertexSet_);
			swap(edges_, that.edges_);
		}

		void clear()
		{
			vertexSet_.clear();
			edges_ = 0;
		}

		Vertex_Iterator addVertex(
			const VertexData& vertexData = VertexData())
		{
			vertexSet_.push_back(Vertex(vertexData));

			Vertex_Iterator iter = vertexSet_.end();
			--iter;

			return iter;
		}

		void removeVertex(
			const Vertex_ConstIterator& vertex)
		{
			vertexSet_.erase(vertex);
		}

		Edge_Iterator addEdge(
			const Vertex_ConstIterator& from,
			const Vertex_ConstIterator& to,
			const EdgeData& edgeData = EdgeData())
		{
			Vertex_Iterator mutableFrom =
				vertexSet_.erase(from, from);
			Vertex_Iterator mutableTo =
				vertexSet_.erase(to, to);

			mutableFrom->exidentSet_.push_back(
				Edge(mutableTo, edgeData));

			Edge_Iterator iter = mutableFrom->exidentSet_.end();
			--iter;

			++edges_;

			return iter;
		}

		Vertex_Range vertexRange()
		{
			return range(vertexSet_.begin(), vertexSet_.end());
		}

		Vertex_ConstRange vertexRange() const
		{
			return range(vertexSet_.begin(), vertexSet_.end());
		}

		integer vertices() const
		{
			return vertexSet_.size();
		}

		integer edges() const
		{
			return edges_;
		}

	private:
		VertexSet vertexSet_;
		integer edges_;
	};

}

#endif
