#ifndef PASTEL_LIST_GRAPH_H
#define PASTEL_LIST_GRAPH_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/iterator_range.h"
#include "pastel/sys/possiblyemptymember.h"

#include <list>

namespace Pastel
{

	template <typename VertexData = EmptyClass, typename EdgeData = EmptyClass>
	class List_Graph
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

		List_Graph()
			: vertexSet_()
			, edges_(0)
		{
		}

		List_Graph(const List_Graph& that)
			: vertexSet_(that.vertexSet_)
			, edges_(that.edges_)
		{
		}

		List_Graph(List_Graph&& that)
			: vertexSet_()
			, edges_(0)
		{
			swap(that);
		}

		List_Graph& operator=(List_Graph&& that)
		{
			swap(that);
			return *this;
		}

		List_Graph& operator=(const List_Graph& that)
		{
			List_Graph copy(that);
			swap(copy);
			return *this;
		}

		void swap(List_Graph& that)
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
		class Vertex
			: private PossiblyEmptyMember<VertexData>
		{
		public:
			typedef PossiblyEmptyMember<VertexData> Base;
			friend class List_Graph<VertexData, EdgeData>;

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
				return data_;
			}

			EdgeData& operator()()
			{
				return data_;
			}

		private:
			Vertex_Iterator to_;
			EdgeData data_;
		};

		VertexSet vertexSet_;
		integer edges_;
	};

}

#endif
