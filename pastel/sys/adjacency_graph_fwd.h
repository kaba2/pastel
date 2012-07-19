#ifndef PASTEL_ADJACENCY_GRAPH_FWD_H
#define PASTEL_ADJACENCY_GRAPH_FWD_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/iterator_range.h"
#include "pastel/sys/possiblyemptymember.h"

#include <list>

namespace Pastel
{

	template <typename VertexData, typename EdgeData>
	class Adjacency_Graph;

	template <typename VertexData, typename EdgeData>
	class Adjacency_Graph_Fwd
	{
	public:
		class Vertex;
		typedef std::list<Vertex> VertexSet;
		typedef typename VertexSet::iterator Vertex_Iterator;
		typedef typename VertexSet::const_iterator Vertex_ConstIterator;

		class Edge;
		typedef std::list<Edge> ExidentSet;
		typedef typename ExidentSet::iterator Edge_Iterator;
		typedef typename ExidentSet::const_iterator Edge_ConstIterator;

		typedef typename IteratorToRange<Vertex_Iterator>::type 
			Vertex_Range;
		typedef typename IteratorToRange<Vertex_ConstIterator>::type
			Vertex_ConstRange;

		typedef typename IteratorToRange<Edge_Iterator>::type 
			Edge_Range;
		typedef typename IteratorToRange<Edge_ConstIterator>::type
			Edge_ConstRange;

	protected:
		class Vertex
			: private PossiblyEmptyMember<VertexData>
		{
		public:
			typedef PossiblyEmptyMember<VertexData> Base;
			friend class Adjacency_Graph<VertexData, EdgeData>;

			Vertex()
				: exidentSet_()
			{
				if (Base::data())
				{
					new(Base::data()) VertexData();
				}
			}

			Vertex(const Vertex& that)
				: exidentSet_(that.exidentSet_)
			{
				if (Base::data())
				{
					new(Base::data()) VertexData(that());
				}
			}

			Vertex(Vertex&& that)
				: exidentSet_()
			{
				if (Base::data())
				{
					new(Base::data()) VertexData;
				}

				swap(that);
			}

			explicit Vertex(const VertexData& data)
				: exidentSet_()
			{
				if (Base::data())
				{
					new(Base::data()) VertexData(data);
				}
			}

			~Vertex()
			{
				if (Base::data())
				{
					Base::data()->~VertexData();
				}
			}

			Vertex& operator=(const Vertex& that)
			{
				Vertex thatCopy(that);
				swap(thatCopy);
				return *this;
			}

			Vertex& operator=(Vertex&& that)
			{
				swap(that);
				return *this;
			}

			void swap(Vertex& that)
			{
				exidentSet_.swap(that.exidentSet_);
				if (Base::data() && that.Base::data())
				{
					using std::swap;
					swap(*Base::data(), *that.Base::data());
				}
			}

			Edge_Range exidentRange()
			{
				return range(exidentSet_.begin(), exidentSet_.end());
			}

			Edge_ConstRange exidentRange() const
			{
				return range(exidentSet_.begin(), exidentSet_.end());
			}

			VertexData& operator()()
			{
				PENSURE(Base::data());
				return *Base::data();
			}

			const VertexData& operator()() const
			{
				PENSURE(Base::data());
				return *Base::data();
			}

		private:
			ExidentSet exidentSet_;
		};

		class Edge
			: private PossiblyEmptyMember<EdgeData>
		{
		public:
			typedef PossiblyEmptyMember<EdgeData> Base;

			explicit Edge(
				const Vertex_Iterator& to,
				const EdgeData& data = EdgeData())
				: to_(to)
			{
				if (Base::data())
				{
					new(Base::data()) EdgeData(data);
				}
			}

			~Edge()
			{
				if (Base::data())
				{
					Base::data()->~EdgeData();
				}
			}

			const Vertex_Iterator& to()
			{
				return to_;
			}

			const Vertex_ConstIterator& to() const
			{
				return to_;
			}

			const EdgeData& operator()() const
			{
				PENSURE(Base::data());
				return *Base::data();
			}

			EdgeData& operator()()
			{
				PENSURE(Base::data());
				return *Base::data();
			}

		private:
			Vertex_Iterator to_;
		};
	};

}

#endif
