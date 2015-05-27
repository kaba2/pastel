// Description: Types for the incidence graph

#ifndef PASTELSYS_INCIDENCE_GRAPH_FWD_H
#define PASTELSYS_INCIDENCE_GRAPH_FWD_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/generic/class.h"

#include <boost/iterator/iterator_adaptor.hpp>

#include <type_traits>
#include <list>

namespace Pastel
{

	enum class GraphType : integer
	{
		Undirected,
		Directed,
		Mixed,
	};

	namespace IncidenceGraph_
	{

		template <typename Type>
		class Incidence_Iterator
			: public boost::iterator_adaptor<
			Incidence_Iterator<Type>, Type>
		{
		public:
			Incidence_Iterator()
				: Incidence_Iterator::iterator_adaptor_(0) 
			{
			}

			Incidence_Iterator(Type that)
				: Incidence_Iterator::iterator_adaptor_(that) 
			{
			}

			template <
				typename That, 
				Requires<std::is_convertible<That, Type>> = 0>
			Incidence_Iterator(const Incidence_Iterator<That>& that)
				: Incidence_Iterator::iterator_adaptor_(that.base()) 
			{
			}

		private:
			friend class boost::iterator_core_access;

			void increment() 
			{ 
				this->base_reference() = this->base()->next_;
			}
		};

	}

	template <typename Settings_>
	class IncidenceGraph_Fwd
	{
	public:
		using Settings = Settings_;
		using Fwd = Settings;

		static constexpr GraphType Type = Settings::Type;
		PASTEL_FWD(VertexData);
		PASTEL_FWD(EdgeData);

		using VertexData_Class = Class<VertexData>;
		using EdgeData_Class = Class<EdgeData>;

		PASTEL_STATIC_ASSERT(
			Type == GraphType::Undirected ||
			Type == GraphType::Directed ||
			Type == GraphType::Mixed);

		// Vertices

		class Vertex;
		using VertexSet = std::list<Vertex>;
		using Vertex_Iterator =	typename VertexSet::iterator;
		using Vertex_ConstIterator = 
#ifndef __GNUC__
			typename VertexSet::const_iterator;
#else
			Vertex_Iterator;
#endif

		// Edges

		class Edge;
		using EdgeSet = std::list<Edge>;
		using Edge_Iterator = typename EdgeSet::iterator;
		using Edge_ConstIterator = 
#ifndef __GNUC__
			typename EdgeSet::const_iterator;
#else
			Edge_Iterator;
#endif

		// Incidences

		class Incidence_Link;
		class Incidence;
		using Incidence_Iterator = 
			IncidenceGraph_::Incidence_Iterator<Incidence*>;
		using Incidence_ConstIterator = 
			IncidenceGraph_::Incidence_Iterator<const Incidence*>;

		static constexpr int IncidenceTypes = 
			(Type == GraphType::Undirected) ? 
			1 : ((Type == GraphType::Directed) ? 2 : 3);
		
		static constexpr int Undirected = 
			(Type == GraphType::Directed) ? 
			IncidenceTypes : 0;
		
		static constexpr int Incoming = 
			(Type == GraphType::Directed) ? 
			0 : ((Type == GraphType::Mixed) ? 1 : IncidenceTypes);
		
		static constexpr int Outgoing = 
			(Type == GraphType::Directed) ? 
			1 : ((Type == GraphType::Mixed) ? 2 : IncidenceTypes);
	};

}

namespace Pastel
{

	template <typename Settings>
	class No_Incidence_Graph_Customization
	{
	protected:
		using Fwd = IncidenceGraph_Fwd<Settings>;

		PASTEL_FWD(Vertex_Iterator);
		PASTEL_FWD(Vertex_ConstIterator);
		PASTEL_FWD(Edge_Iterator);
		PASTEL_FWD(Edge_ConstIterator);

		No_Incidence_Graph_Customization() {}
		void swap(No_Incidence_Graph_Customization& that) {}

		void onClear() {}
		void onAddVertex(const Vertex_Iterator& vertex) {}
		void onRemoveVertex(const Vertex_Iterator& that) {}
		void onAddEdge(const Edge_Iterator& that) {}
		void onRemoveEdge(const Edge_Iterator& that) {}

	private:
		No_Incidence_Graph_Customization(const No_Incidence_Graph_Customization& that) = delete;
		No_Incidence_Graph_Customization(No_Incidence_Graph_Customization&& that) = delete;
		No_Incidence_Graph_Customization& operator=(No_Incidence_Graph_Customization) = delete;
	};

	template <
		typename Settings, 
		template <typename> class = No_Incidence_Graph_Customization>
	class IncidenceGraph;

}

namespace Pastel
{

	template <
		GraphType Type_, 
		typename VertexData_ = void, 
		typename EdgeData_ = void>
	class IncidenceGraph_Settings
	{
	public:
		static constexpr GraphType Type = Type_;
		using VertexData = VertexData_;
		using EdgeData = EdgeData_;
	};

}

#endif
