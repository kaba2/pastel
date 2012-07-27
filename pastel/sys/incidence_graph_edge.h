#ifndef PASTEL_INCIDENCE_GRAPH_EDGE_H
#define PASTEL_INCIDENCE_GRAPH_EDGE_H

#include "pastel/sys/incidence_graph.h"
#include "pastel/sys/object_forwarding.h"

// Visual Studio generates "multiple assignment operators" warning,
// because it does not implement the deletion of functions 
// (which we simulate below).
#if (defined _WIN32 || defined _WIN64)
#	pragma warning(push)
#	pragma warning(disable: 4522)
#endif

namespace Pastel
{

	namespace Incidence_Graph_
	{

		template <GraphType::Enum Type, typename Data>
		class Directed_EdgeData;

		template <typename Data>
		class Directed_EdgeData<GraphType::Directed, Data>
			: public MakeClass<Data>::type
		{
		public:
			typedef typename MakeClass<Data>::type 
				Base;

			template <typename Data_>
			Directed_EdgeData(
				bool directed,
				Data_&& data)
				: Base(std::forward<Data_>(data))
			{
			}

			bool directed() const
			{
				return true;
			}

		protected:
			void setDirected(bool directed)
			{
			}
		};

		template <typename Data>
		class Directed_EdgeData<GraphType::Undirected, Data>
			: public MakeClass<Data>::type
		{
		public:
			typedef typename MakeClass<Data>::type 
				Base;

			template <typename Data_>
			Directed_EdgeData(
				bool directed,
				Data_&& data)
				: Base(std::forward<Data_>(data))
			{
			}

			bool directed() const
			{
				return false;
			}

		protected:
			void setDirected(bool directed)
			{
			}
		};

		template <typename Data>
		class Directed_EdgeData<GraphType::Mixed, Data>
			: public MakeClass<Data>::type
		{
		public:
			typedef typename MakeClass<Data>::type 
				Base;

			template <typename Data_>
			Directed_EdgeData(
				bool directed,
				Data_&& data)
				: Base(std::forward<Data_>(data))
				, directed_(directed)
			{
			}

			bool directed() const
			{
				return directed_;
			}

		protected:
			void setDirected(bool directed)
			{
				directed_ = directed;
			}

		private:
			bool directed_;
		};

	}

	template <GraphType::Enum Type, typename VertexData, typename EdgeData>
	class Incidence_Graph_Fwd<Type, VertexData, EdgeData>::Edge
		: public Incidence_Graph_::Directed_EdgeData<Type, EdgeData>
	{
	public:
		friend class Incidence_Graph<Type, VertexData, EdgeData>;
		typedef Incidence_Graph_::Directed_EdgeData<Type, EdgeData>
			Base;

		using Base::operator=;

		Edge(Edge&& that)
			: Base(that.directed(), std::move((EdgeData&&)that))
			, from_(std::move(that.from_))
			, to_(std::move(that.to_))
		{
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

		bool directed() const
		{
			return Base::directed();
		}

	private:
		Edge() PASTEL_DELETE;
		Edge(const Edge& that) PASTEL_DELETE;
		Edge& operator=(Edge that) PASTEL_DELETE;

		explicit Edge(EdgeData data, bool directed)
			: Base(directed, std::move(data))
			, from_(0)
			, to_(0)
		{
		}

		Incidence* from_;
		Incidence* to_;
	};

}

#if (defined _WIN32 || defined _WIN64)
#	pragma warning(pop)
#endif

#endif
