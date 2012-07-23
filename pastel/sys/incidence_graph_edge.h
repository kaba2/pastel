#ifndef PASTEL_INCIDENCE_GRAPH_EDGE_H
#define PASTEL_INCIDENCE_GRAPH_EDGE_H

#include "pastel/sys/incidence_graph.h"

namespace Pastel
{

	namespace Incidence_Graph_
	{

		template <typename EdgeData, GraphType::Enum Type>
		class Optional_EdgeData;

		template <typename EdgeData>
		class Optional_EdgeData<EdgeData, GraphType::Directed>
			: protected PossiblyEmptyMember<EdgeData>
		{
		public:
			explicit Optional_EdgeData(bool directed)
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

		template <typename EdgeData>
		class Optional_EdgeData<EdgeData, GraphType::Undirected>
			: protected PossiblyEmptyMember<EdgeData>
		{
		public:
			explicit Optional_EdgeData(bool directed)
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

		template <typename EdgeData>
		class Optional_EdgeData<EdgeData, GraphType::Mixed>
			: protected PossiblyEmptyMember<EdgeData>
		{
		public:
			explicit Optional_EdgeData(bool directed)
				: directed_(directed)
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

			bool directed_;
		};

	}

	template <GraphType::Enum Type, typename VertexData, typename EdgeData>
	class Incidence_Graph_Fwd<Type, VertexData, EdgeData>::Edge
		: public Incidence_Graph_::Optional_EdgeData<EdgeData, Type>
	{
	public:
		friend class Incidence_Graph<Type, VertexData, EdgeData>;

		typedef Incidence_Graph_::Optional_EdgeData<EdgeData, Type>
			Base;

		typedef PossiblyEmptyMember<EdgeData> Data;

		Edge(const Edge& that)
			: Base(that)
			, from_(that.from_)
			, to_(that.to_)
		{
			if (Data::data())
			{
				new(Data::data()) EdgeData(that.data());
			}
		}

		Edge(Edge&& that)
			: Base(that)
			, from_(that.from_)
			, to_(that.to_)
		{
			if (Data::data())
			{
				new(Data::data()) EdgeData(std::move(that.data()));
			}
		}

		explicit Edge(EdgeData data, bool directed)
			: Base(directed)
			, from_(0)
			, to_(0)
		{
			if (Data::data())
			{
				new(Data::data()) EdgeData(std::move(data));
			}
		}

		~Edge()
		{
			if (Data::data())
			{
				Data::data()->~EdgeData();
			}
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

		const EdgeData& data() const
		{
			PENSURE(Data::data());
			return *Data::data();
		}

		EdgeData& data()
		{
			PENSURE(Data::data());
			return *Data::data();
		}

	private:
		// Deleted
		Edge();
		// Deleted
		Edge& operator=(Edge that);

		Incidence* from_;
		Incidence* to_;
	};

}

#endif
