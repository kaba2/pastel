#ifndef PASTEL_INCIDENCE_GRAPH_FWD_H
#define PASTEL_INCIDENCE_GRAPH_FWD_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/object_forwarding.h"

#include <boost/iterator/iterator_adaptor.hpp>

#include <list>

namespace Pastel
{

	class GraphType
	{
	public:
		enum Enum
		{
			Undirected,
			Directed,
			Mixed,
		};
	};

	namespace Incidence_Graph_
	{

		template <typename Type>
		class Incidence_Iterator
			: public boost::iterator_adaptor<
			Incidence_Iterator<Type>, Type>
		{
		private:
			struct enabler {};

		public:
			Incidence_Iterator()
				: Incidence_Iterator::iterator_adaptor_(0) 
			{
			}

			Incidence_Iterator(Type that)
				: Incidence_Iterator::iterator_adaptor_(that) 
			{
			}

			template <typename That>
			Incidence_Iterator(
				const Incidence_Iterator<That>& that,
				typename boost::enable_if<
				boost::is_convertible<That, Type>, 
				enabler>::type = enabler())
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

	template <GraphType::Enum Type, typename VertexData, typename EdgeData>
	class Incidence_Graph;

	template <GraphType::Enum Type, typename VertexData, typename EdgeData>
	class Incidence_Graph_Fwd
	{
	public:
		PASTEL_STATIC_ASSERT(
			Type == GraphType::Undirected ||
			Type == GraphType::Directed ||
			Type == GraphType::Mixed);

		typedef typename Forward<VertexData>::type 
			VertexData_Class;
		typedef typename Forward<EdgeData>::type 
			EdgeData_Class;

		// Vertices

		class Vertex;
		typedef std::list<Vertex> VertexSet;
		typedef typename VertexSet::iterator 
			Vertex_Iterator;
		typedef typename VertexSet::const_iterator 
			Vertex_ConstIterator;

		// Edges

		class Edge;
		typedef std::list<Edge> EdgeSet;
		typedef typename EdgeSet::iterator 
			Edge_Iterator;
		typedef typename EdgeSet::const_iterator 
			Edge_ConstIterator;

		// Incidences

		class Incidence_Link;
		class Incidence;
		typedef Incidence_Graph_::Incidence_Iterator<Incidence*> 
			Incidence_Iterator;
		typedef Incidence_Graph_::Incidence_Iterator<const Incidence*> 
			Incidence_ConstIterator;

	protected:
		enum IncidenceType
		{
			IncidenceTypes = (Type == GraphType::Undirected) ? 1 : ((Type == GraphType::Directed) ? 2 : 3),
			Undirected = (Type == GraphType::Directed) ? IncidenceTypes : 0,
			Incoming = (Type == GraphType::Directed) ? 0 : ((Type == GraphType::Mixed) ? 1 : IncidenceTypes),
			Outgoing = (Type == GraphType::Directed) ? 1 : ((Type == GraphType::Mixed) ? 2 : IncidenceTypes)
		};
	};

}

#endif
