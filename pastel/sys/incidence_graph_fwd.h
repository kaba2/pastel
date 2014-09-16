// Description: Types for the incidence graph

#ifndef PASTELSYS_INCIDENCE_GRAPH_FWD_H
#define PASTELSYS_INCIDENCE_GRAPH_FWD_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/class.h"

#include <boost/iterator/iterator_adaptor.hpp>

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
				typename = PASTEL_ENABLE_IF((boost::is_convertible<That, Type>), void)>
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

	template <typename, template <typename> class>
	class IncidenceGraph;

	template <typename Settings_>
	class IncidenceGraph_Fwd
	{
	public:
		using Settings = Settings_;
		using Fwd = Settings;

		static PASTEL_CONSTEXPR GraphType Type = Settings::Type;
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

		static PASTEL_CONSTEXPR int IncidenceTypes = 
			(Type == GraphType::Undirected) ? 
			1 : ((Type == GraphType::Directed) ? 2 : 3);
		
		static PASTEL_CONSTEXPR int Undirected = 
			(Type == GraphType::Directed) ? 
			IncidenceTypes : 0;
		
		static PASTEL_CONSTEXPR int Incoming = 
			(Type == GraphType::Directed) ? 
			0 : ((Type == GraphType::Mixed) ? 1 : IncidenceTypes);
		
		static PASTEL_CONSTEXPR int Outgoing = 
			(Type == GraphType::Directed) ? 
			1 : ((Type == GraphType::Mixed) ? 2 : IncidenceTypes);
	};

}

#endif
