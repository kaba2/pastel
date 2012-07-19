#ifndef PASTEL_ADJACENCY_GRAPH_FWD_H
#define PASTEL_ADJACENCY_GRAPH_FWD_H

#include "pastel/sys/mytypes.h"
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
		typedef std::list<Edge> EdgeSet;
		typedef typename EdgeSet::iterator Edge_Iterator;
		typedef typename EdgeSet::const_iterator Edge_ConstIterator;

	protected:
		class Vertex
			: private PossiblyEmptyMember<VertexData>
		{
		public:
			typedef PossiblyEmptyMember<VertexData> Base;
			friend class Adjacency_Graph<VertexData, EdgeData>;

			Vertex(const Vertex& that)
				: first_(that.first_)
				, last_(that.last_)
				, edges_(that.edges_)
			{
				if (Base::data())
				{
					new(Base::data()) VertexData(that.data());
				}
			}

			Vertex(Vertex&& that)
				: first_(that.first_)
				, last_(that.last_)
				, edges_(that.edges_)
			{
				if (Base::data())
				{
					new(Base::data()) VertexData(std::move(that.data()));
				}
			}

			explicit Vertex(
				VertexData data,
				Edge_Iterator end)
				: first_(end)
				, last_(end)
				, edges_(0)
			{
				if (Base::data())
				{
					new(Base::data()) VertexData(std::move(data));
				}
			}

			~Vertex()
			{
				if (Base::data())
				{
					Base::data()->~VertexData();
				}
			}

			Edge_Iterator begin()
			{
				return first_;
			}

			Edge_Iterator end()
			{
				Edge_Iterator result = last_;
				if (edges_ > 0)
				{
					++result;
				}
				return result;
			}

			Edge_ConstIterator cbegin() const
			{
				return ((Vertex&)*this).begin();
			}

			Edge_ConstIterator cend() const
			{
				return ((Vertex&)*this).end();
			}

			integer edges() const
			{
				return edges_;
			}

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

			void insert(
				const Edge_Iterator& edge)
			{
				if (edges_ == 0)
				{
					first_ = edge;
				}
				last_ = edge;
				++edges_;
			}

			void erase(
				const Edge_Iterator& edge,
				const Edge_Iterator& end)
			{
				ASSERT_OP(edges_, >, 0);
				--edges_;
				if (edges_ == 0)
				{
					first_ = end;
					last_ = end;
				}
				else
				{
					if (first_ == edge)
					{
						++first_;
					}
					else if (last_ == edge)
					{
						--last_;
					}
				}
			}

			void erase(
				const Edge_Iterator& end)
			{
				first_ = end;
				last_ = end;
				edges_ = 0;
			}

			Edge_Iterator first_;
			Edge_Iterator last_;
			integer edges_;
		};

		class Edge
			: private PossiblyEmptyMember<EdgeData>
		{
		public:
			typedef PossiblyEmptyMember<EdgeData> Base;

			Edge(const Edge& that)
				: from_(that.from_)
				, to_(that.to_)
			{
				if (Base::data())
				{
					new(Base::data()) EdgeData(that.data());
				}
			}

			Edge(Edge&& that)
				: from_(that.from_)
				, to_(that.to_)
			{
				if (Base::data())
				{
					new(Base::data()) EdgeData(std::move(that.data()));
				}
			}

			explicit Edge(
				Vertex_Iterator from,
				Vertex_Iterator to,
				EdgeData data = EdgeData())
				: from_(from)
				, to_(to)
			{
				if (Base::data())
				{
					new(Base::data()) EdgeData(std::move(data));
				}
			}

			~Edge()
			{
				if (Base::data())
				{
					Base::data()->~EdgeData();
				}
			}

			const Vertex_Iterator& from()
			{
				return from_;
			}

			const Vertex_ConstIterator& from() const
			{
				return from_;
			}

			const Vertex_Iterator& to()
			{
				return to_;
			}

			const Vertex_ConstIterator& to() const
			{
				return to_;
			}

			const EdgeData& data() const
			{
				PENSURE(Base::data());
				return *Base::data();
			}

			EdgeData& data()
			{
				PENSURE(Base::data());
				return *Base::data();
			}

		private:
			// Deleted
			Edge();
			// Deleted
			Edge& operator=(Edge that);

			Vertex_Iterator from_;
			Vertex_Iterator to_;
		};
	};

}

#endif
