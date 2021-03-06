#ifndef PASTELSYS_INCIDENCE_GRAPH_VERTEX_H
#define PASTELSYS_INCIDENCE_GRAPH_VERTEX_H

#include "pastel/sys/incidence_graph.h"

#include <array>

namespace Pastel
{

	template <typename Settings>
	class IncidenceGraph_Fwd<Settings>::Vertex
	{
	public:
		using Graph = IncidenceGraph_Fwd<Settings>;
		template <typename, template <typename> class>
		friend class IncidenceGraph;

		//! Move-constructs from another vertex.
		/*!
		Time complexity: constant
		Exception safety: strong

		FIX: This function is needed solely because Visual Studio 2010
		does not support the emplace function properly. Remove this 
		function when support for emplace becomes available.
		*/
		Vertex(Vertex&& that)
			: data_(std::move((VertexData&&)that))
			, partitionSet_(std::move(that.partitionSet_))
			, sentinel_()
			, incidencesSet_(std::move(that.incidencesSet_))
		{
			Incidence* next = that.sentinel_.next_;
			Incidence* prev = that.sentinel_.prev_;

			if (next != &that.sentinel_)
			{
				sentinel_.next_ = next;
				next->prev_ = (Incidence*)&sentinel_;
				
				sentinel_.prev_ = prev;
				prev->next_ = (Incidence*)&sentinel_;
			}

			for (integer i = 0;i < IncidenceTypes;++i)
			{
				if (incidencesSet_[i] == 0)
				{
					partitionSet_[i] = (Incidence*)&sentinel_;
				}
			}

			that.forget();
		}

		//! Assigns to the contained data.
		template <typename Type>
		Vertex& operator=(Type&& that)
		{
			data() = std::forward<Type>(that);
			return *this;
		}

		~Vertex()
		{
			clear();
		}

		VertexData& data() {
			return data_;
		}

		const VertexData& data() const {
			return data_;
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
			return begin_<Graph::Undirected>();
		}

		Incidence_ConstIterator cUndirectedEnd() const
		{
			return end_<Graph::Undirected>();
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
			return begin_<Graph::Outgoing>();
		}

		Incidence_ConstIterator cOutgoingEnd() const
		{
			return end_<Graph::Outgoing>();
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
			return begin_<Graph::Incoming>();
		}

		Incidence_ConstIterator cIncomingEnd() const
		{
			return end_<Graph::Incoming>();
		}

		integer incomingEdges() const
		{
			return incidences_<Graph::Incoming>();
		}

		// All incidences

		Incidence_Iterator allBegin() const
		{
			return sentinel_.next_;
		}

		Incidence_Iterator allEnd() const
		{
			return (Incidence*)&sentinel_;
		}

		Incidence_ConstIterator cAllBegin() const
		{
			return sentinel_.next_;
		}

		Incidence_ConstIterator cAllEnd() const
		{
			return (const Incidence*)&sentinel_;
		}

		integer allEdges() const
		{
			integer result = 0;
			for (integer i = 0;i < IncidenceTypes;++i)
			{
				result += incidencesSet_[i];
			}
			return result;
		}

	private:
		Vertex() = delete;
		Vertex(const Vertex& that) = delete;
		Vertex& operator=(const Vertex& that) = delete;

	private:
		explicit Vertex(VertexData data)
			: data_(std::move(data))
			, partitionSet_()
			, sentinel_()
			, incidencesSet_()
		{
			partitionSet_.fill((Incidence*)&sentinel_);
			incidencesSet_.fill(0);
		}

		void insert(integer I, Incidence* incidence)
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

		void release(integer I, Incidence* incidence)
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
		}

		void erase(integer I, Incidence* incidence)
		{
			release(I, incidence);

			// Delete the incidence.
			delete incidence;
		}

		void move(integer From, integer To,
			Incidence* incidence)
		{
			release(From, incidence);
			insert(To, incidence);
		}

		void erase(integer I)
		{
			Incidence* incidence = partitionSet_[I];
			while(incidence != &sentinel_)
			{
				Incidence* next = incidence->next_;
				erase(I, incidence);
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

			forget();
		}

		void forget()
		{
			// Forget all incidences.
			Incidence* sentinel = (Incidence*)&sentinel_;

			partitionSet_.fill(sentinel);
			incidencesSet_.fill(0);

			sentinel->next_ = sentinel;
			sentinel->prev_ = sentinel;
		}

		template <integer I>
		requires (I < IncidenceTypes)
		Incidence* begin_() const
		{
			return partitionSet_[I];
		}

		template <integer I>
		requires (I < IncidenceTypes)
		Incidence* end_() const
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

		template <integer I>
		requires (I < IncidenceTypes)
		integer incidences_() const
		{
			return incidencesSet_[I];
		}

		BOOST_ATTRIBUTE_NO_UNIQUE_ADDRESS
		VertexData data_;

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
