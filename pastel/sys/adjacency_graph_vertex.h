#ifndef PASTEL_ADJACENCY_GRAPH_VERTEX_H
#define PASTEL_ADJACENCY_GRAPH_VERTEX_H

#include "pastel/sys/adjacency_graph.h"

#include <array>

namespace Pastel
{

	template <GraphType::Enum Type, typename VertexData, typename EdgeData>
	class Adjacency_Graph_Fwd<Type, VertexData, EdgeData>::Vertex
		: private PossiblyEmptyMember<VertexData>
	{
	public:
		typedef Adjacency_Graph_Fwd<Type, VertexData, EdgeData> Graph;
		typedef PossiblyEmptyMember<VertexData> Base;
		friend class Adjacency_Graph<Type, VertexData, EdgeData>;

		Vertex(const Vertex& that)
			: partitionSet_()
			, sentinel_()
			, incidencesSet_()
		{
			partitionSet_.fill((Incidence*)&sentinel_);
			incidencesSet_.fill(0);

			if (Base::data())
			{
				new(Base::data()) VertexData(that.data());
			}
		}

		Vertex(Vertex&& that)
			: partitionSet_()
			, sentinel_()
			, incidencesSet_()
		{
			partitionSet_.fill((Incidence*)&sentinel_);
			incidencesSet_.fill(0);

			if (Base::data())
			{
				new(Base::data()) VertexData(std::move(that.data()));
			}
		}

		explicit Vertex(VertexData data)
			: partitionSet_()
			, sentinel_()
			, incidencesSet_()
		{
			partitionSet_.fill((Incidence*)&sentinel_);
			incidencesSet_.fill(0);

			if (Base::data())
			{
				new(Base::data()) VertexData(std::move(data));
			}
		}

		~Vertex()
		{
			clear();

			if (Base::data())
			{
				Base::data()->~VertexData();
			}
		}

		// Undirected incidences

		Incidence_Iterator undirectedBegin()
		{
			return begin_<Graph::Undirected>();
		}

		Incidence_Iterator undirectedEnd()
		{
			return end_<Graph::Undirected>();
		}

		Incidence_ConstIterator cUndirectedBegin() const
		{
			return cbegin_<Graph::Undirected>();
		}

		Incidence_ConstIterator cUndirectedEnd() const
		{
			return cend_<Graph::Undirected>();
		}

		integer undirectedEdges() const
		{
			return incidences_<Graph::Undirected>();
		}

		// Outgoing incidences

		Incidence_Iterator outgoingBegin()
		{
			return begin_<Graph::Outgoing>();
		}

		Incidence_Iterator outgoingEnd()
		{
			return end_<Graph::Outgoing>();
		}

		Incidence_ConstIterator cOutgoingBegin() const
		{
			return cbegin_<Graph::Outgoing>();
		}

		Incidence_ConstIterator cOutgoingEnd() const
		{
			return cend_<Graph::Outgoing>();
		}

		integer outgoingEdges() const
		{
			return incidences_<Graph::Outgoing>();
		}

		// Incoming incidences

		Incidence_Iterator incomingBegin()
		{
			return begin_<Graph::Incoming>();
		}

		Incidence_Iterator incomingEnd()
		{
			return end_<Graph::Incoming>();
		}

		Incidence_ConstIterator cIncomingBegin() const
		{
			return cbegin_<Graph::Incoming>();
		}

		Incidence_ConstIterator cIncomingEnd() const
		{
			return cend_<Graph::Incoming>();
		}

		integer incomingEdges() const
		{
			return incidences_<Graph::Incoming>();
		}

		// All incidences

		Incidence_Iterator begin() const
		{
			return sentinel_.next_;
		}

		Incidence_Iterator end() const
		{
			return sentinel_;
		}

		Incidence_ConstIterator cbegin() const
		{
			return sentinel_.next_;
		}

		Incidence_ConstIterator cend() const
		{
			return sentinel_;
		}

		integer incidentEdges() const
		{
			integer result = 0;
			for (integer i = 0;i < IncidenceTypes;++i)
			{
				result += incidencesSet_[i];
			}
			return result;
		}

		// Label data

		VertexData& data()
		{
			PENSURE(Base::data());
			return *Base::data();
		}

		const VertexData& data() const
		{
			PENSURE(Base::data());
			return *Base::data();
		}

	private:
		// Deleted
		Vertex();
		// Deleted
		Vertex& operator=(Vertex that);

		template <IncidenceType I>
		void insert(Incidence* incidence)
		{
			// Find a proper place to insert the
			// incidence.
			Incidence* position = 
				partitionSet_[I];
			if (incidencesSet_[I] == 0)
			{
				integer i = I + 1;
				while(i < IncidenceTypes && incidencesSet_[i] == 0)
				{
					++i;
				}

				if (i < IncidenceTypes)
				{
					position = partitionSet_[i];
				}
				else
				{
					position = (Incidence*)&sentinel_;				
				}
			}

			Incidence* prev = position->prev_;
			
			// Link at the front of the list.
			prev->next_ = incidence;
			incidence->prev_ = prev;

			incidence->next_ = position;
			position->prev_ = incidence;

			// Make this incidence the new head of the list.
			partitionSet_[I] = incidence;
			++incidencesSet_[I];
		}

		template <IncidenceType I>
		void erase(Incidence* incidence)
		{
			ASSERT_OP(I, <, IncidenceTypes);
			ASSERT_OP(incidencesSet_[I], >, 0);
			ASSERT(incidence != &sentinel_);

			// Link the incidence off the list.
			Incidence* next = incidence->next_;
			Incidence* prev = incidence->prev_;

			next->prev_ = prev;
			prev->next_ = next;

			--incidencesSet_[I];

			// Make sure the partitionSet does not
			// reference 'incidence'.
			if (incidencesSet_[I] == 0)
			{
				partitionSet_[I] = (Incidence*)&sentinel_;
			}
			else if (partitionSet_[I] == incidence)
			{
				partitionSet_[I] = next;
			}

			// Delete the incidence.
			delete incidence;
		}

		template <IncidenceType I>
		void erase()
		{
			Incidence* incidence = partitionSet_[I];
			while(incidence != &sentinel_)
			{
				Incidence* next = incidence->next_;
				erase<I>(incidence);
				incidence = next;
			}
		}

		void clear()
		{
			Incidence* sentinel = (Incidence*)&sentinel_;

			// Delete all incidences.
			Incidence* incidence = sentinel->next_;
			while(incidence != sentinel)
			{
				Incidence* next = incidence->next_;
				delete incidence;
				incidence = next;
			}
			
			// Forget all incidences.
			partitionSet_.fill(sentinel);
			incidencesSet_.fill(0);

			sentinel->next_ = sentinel;
			sentinel->prev_ = sentinel;
		}

		template <int I>
		PASTEL_ENABLE_IF_C(I < IncidenceTypes, Incidence*) 
			begin_() const
		{
			return partitionSet_[I];
		}

		template <int I>
		PASTEL_ENABLE_IF_C(I < IncidenceTypes, Incidence*) 
			end_() const
		{
			if (incidencesSet_[I] > 0)
			{
				for (integer i = I + 1;i < IncidenceTypes;++i)
				{
					if (incidencesSet_[i] > 0)
					{
						return partitionSet_[i];
					}
				}
			}
			
			return (Incidence*)&sentinel_;
		}

		template <int I>
		PASTEL_ENABLE_IF_C(I < IncidenceTypes, integer) 
			incidences_() const
		{
			return incidencesSet_[I];
		}

		//! Partition of incidences into lists of given incidence type.
		/*!
		Depending on the type of the graph, the 'partitionSet'
		contains either 1 (undirected graph), 2 (directed graph),
		or 3 (mixed graph) components. 	In any case, the partitionSet
		denotes three intervals in a looped doubly-linked list of
		incidences (that the Incidences form by their 'next' and 
		'prev' member variables). If incidencesSet[I] > 0, the 
		partitionSet[I] contains the first element of the incidences
		of type I (Incoming, Outgoing, Undirected). The other incidences
		of the same type can be found by following the 'next' fields
		of 'Incidence'. The one-past-end element of the incidences of 
		type I is contained in the partitionSet[J] where J > I is the first 
		incidence type with incidenceSet[J] > 0. If there is no such J,
		the one-past-end element is given by &sentinel_. If 
		incidenceSet[I] = 0, then partitionSet[I] = &sentinel.
		*/
		std::array<Incidence*, IncidenceTypes> partitionSet_;
		Incidence_Link sentinel_;

		//! The number of incidences of type I for each I.
		std::array<integer, IncidenceTypes> incidencesSet_;
	};

}

#endif
