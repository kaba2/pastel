#ifndef PASTELSYS_INCIDENCE_GRAPH_EDGE_H
#define PASTELSYS_INCIDENCE_GRAPH_EDGE_H

#include "pastel/sys/incidence_graph.h"

namespace Pastel
{

	template <typename Settings>
	class IncidenceGraph_Fwd<Settings>::Edge
	{
	public:
		template <typename, template <typename> class>
		friend class Pastel::IncidenceGraph;

		// FIX: Delete after emplace becomes available in Visual Studio.
		Edge(Edge&& that)
			: data_(std::move(that.data_))
			, directed_(that.directed_)
			, from_(std::move(that.from_))
			, to_(std::move(that.to_))
		{
		}

		//! Assigns to the contained data.
		template <typename Type>
		Edge& operator=(Type&& that)
		{
			data() = std::forward<Type>(that);
			return *this;
		}

		EdgeData& data() {
			return data_;
		}

		const EdgeData& data() const {
			return data_;
		}

		Vertex_Iterator from()
		{
			// Note that the to-incidence, which is stored
			// in the to-vertex, stores a reference
			// to the from-vertex of the edge. And vice versa.
			// This is because when examining a vertex, it
			// would not give you any extra-information if
			// the vertex reference of its incidence were 
			// to the vertex itself.
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

		void setDirected(bool directed) {
			if constexpr (Type == GraphType::Mixed) {
				directed_ = directed;
			}
		}

		bool directed() const
		{
			if constexpr (Type == GraphType::Directed) {
				return true;
			} else if constexpr (Type == GraphType::Undirected) {
				return false;
			} else {
				return directed_;
			}
		}

	private:
		Edge() = delete;
		Edge(const Edge& that) = delete;
		Edge& operator=(const Edge& that) = delete;
	
	private:

		class DirectedEmpty {
		public:
			DirectedEmpty(bool directed) {};
		};

		using Directed = std::conditional_t<
			Type == GraphType::Mixed,
			bool,
			DirectedEmpty
		>;

		explicit Edge(EdgeData data, bool directed)
			: data_(std::move(data))
			, directed_(directed)
			, from_(0)
			, to_(0)
		{
		}

		BOOST_ATTRIBUTE_NO_UNIQUE_ADDRESS
		EdgeData data_;
		BOOST_ATTRIBUTE_NO_UNIQUE_ADDRESS
		Directed directed_;
		Incidence* from_;
		Incidence* to_;
	};

}

#endif
