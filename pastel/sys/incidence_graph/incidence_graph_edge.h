#ifndef PASTELSYS_INCIDENCE_GRAPH_EDGE_H
#define PASTELSYS_INCIDENCE_GRAPH_EDGE_H

#include "pastel/sys/incidence_graph.h"
#include "pastel/sys/class.h"

namespace Pastel
{

	namespace IncidenceGraph_
	{

		template <GraphType Type, typename Data>
		class Directed_EdgeData;

		template <typename Data>
		class Directed_EdgeData<GraphType::Directed, Data>
			: public Class<Data>
		{
		public:
			typedef Class<Data> 
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

		private:
			Directed_EdgeData() = delete;
			Directed_EdgeData(Directed_EdgeData&& that) = delete;
			Directed_EdgeData(const Directed_EdgeData& that) = delete;
			Directed_EdgeData& operator=(Directed_EdgeData that) = delete;
		};

		template <typename Data>
		class Directed_EdgeData<GraphType::Undirected, Data>
			: public Class<Data>
		{
		public:
			typedef Class<Data> 
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

		private:
			Directed_EdgeData() = delete;
			Directed_EdgeData(Directed_EdgeData&& that) = delete;
			Directed_EdgeData(const Directed_EdgeData& that) = delete;
			Directed_EdgeData& operator=(Directed_EdgeData that) = delete;
		};

		template <typename Data>
		class Directed_EdgeData<GraphType::Mixed, Data>
			: public Class<Data>
		{
		public:
			typedef Class<Data> 
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
			Directed_EdgeData() = delete;
			Directed_EdgeData(Directed_EdgeData&& that) = delete;
			Directed_EdgeData(const Directed_EdgeData& that) = delete;
			Directed_EdgeData& operator=(Directed_EdgeData that) = delete;
		
		private:
			bool directed_;
		};

	}

	template <typename Settings>
	class IncidenceGraph_Fwd<Settings>::Edge
		: public IncidenceGraph_::Directed_EdgeData<Type, EdgeData>
	{
	public:
		template <typename, template <typename> class>
		friend class Pastel::IncidenceGraph;

		typedef IncidenceGraph_::Directed_EdgeData<Type, EdgeData>
			Base;

		// FIX: Delete after emplace becomes available in Visual Studio.
		Edge(Edge&& that)
			: Base(that.directed(), std::move((EdgeData_Class&&)that))
			, from_(std::move(that.from_))
			, to_(std::move(that.to_))
		{
		}

		//! Assigns to the contained data.
		template <typename Type>
		Edge& operator=(Type&& that)
		{
			((EdgeData_Class&)*this) = std::forward<Type>(that);
			return *this;
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
		Edge() = delete;
		Edge(const Edge& that) = delete;
		Edge& operator=(const Edge& that) = delete;
	
	private:
		explicit Edge(EdgeData_Class data, bool directed)
			: Base(directed, std::move(data))
			, from_(0)
			, to_(0)
		{
		}

		Incidence* from_;
		Incidence* to_;
	};

}

#endif
