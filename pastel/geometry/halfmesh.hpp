#ifndef PASTEL_HALFMESH_HPP
#define PASTEL_HALFMESH_HPP

#include "pastel/geometry/halfmesh.h"

#include "pastel/sys/unorderedmap.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <typename DataPolicy>
	HalfMesh<DataPolicy>::HalfMesh()
		: DataPolicy()
		, vertexSet_()
		, halfSet_()
		, edgeSet_()
		, polygonSet_()
		, vertexAllocator_(sizeof(VertexBody))
		, halfAllocator_(sizeof(HalfBody))
		, edgeAllocator_(sizeof(EdgeBody))
		, polygonAllocator_(sizeof(PolygonBody))
	{
	}

	template <typename DataPolicy>
	HalfMesh<DataPolicy>::HalfMesh(
		const HalfMesh& that)
		: DataPolicy()
		, vertexSet_()
		, halfSet_()
		, edgeSet_()
		, polygonSet_()
		, vertexAllocator_(sizeof(VertexBody))
		, halfAllocator_(sizeof(HalfBody))
		, edgeAllocator_(sizeof(EdgeBody))
		, polygonAllocator_(sizeof(PolygonBody))
	{
		UnorderedMap<Vertex, Vertex, VertexHash> vertexMap;

		// Construct vertices along with a vertex map.
		{
			ConstVertexIterator iter = that.vertexBegin();
			const ConstVertexIterator iterEnd = that.vertexEnd();
			while(iter != iterEnd)
			{
				const Vertex thatVertex = *iter;
				const Vertex vertex = addVertex();

				vertexMap.insert(std::make_pair(thatVertex, vertex));
				*vertex = *thatVertex;

				++iter;
			}
		}

		// Construct edges.
		{
			ConstEdgeIterator iter = that.edgeBegin();
			const ConstEdgeIterator iterEnd = that.edgeEnd();
			while(iter != iterEnd)
			{
				const Edge thatEdge = *iter;

				const Edge edge = addEdge(
					vertexMap[thatEdge.half().origin()],
					vertexMap[thatEdge.half().destination()]);
				*edge = *thatEdge;

				++iter;
			}
		}

		// Construct polygons.
		{
			ConstPolygonIterator iter = that.polygonBegin();
			const ConstPolygonIterator iterEnd = that.polygonEnd();
			while(iter != iterEnd)
			{
				std::vector<Vertex> vertexLoop;

				const Polygon thatPolygon = *iter;
				const Half halfBegin = thatPolygon.half();
				Half half = halfBegin;
				do
				{
					vertexLoop.push_back(vertexMap[half.origin()]);

					half = half.next();
				}
				while(half != halfBegin);

				const Polygon polygon = addPolygon(vertexLoop);
				*polygon = *thatPolygon;

				++iter;
			}
		}
	}

	template <typename DataPolicy>
	HalfMesh<DataPolicy>::~HalfMesh()
	{
		clear();
	}

	template <typename DataPolicy>
	HalfMesh<DataPolicy>&
		HalfMesh<DataPolicy>::operator=(const HalfMesh& that)
	{
		HalfMesh copy(that);
		swap(copy);
		return *this;
	}

	template <typename DataPolicy>
	void HalfMesh<DataPolicy>::swap(HalfMesh& that)
	{
		DataPolicy::swap(that);
		vertexSet_.swap(that.vertexSet_);
		halfSet_.swap(that.halfSet_);
		edgeSet_.swap(that.edgeSet_);
		polygonSet_.swap(that.polygonSet_);
		vertexAllocator_.swap(that.vertexAllocator_);
		halfAllocator_.swap(that.halfAllocator_);
		edgeAllocator_.swap(that.edgeAllocator_);
		polygonAllocator_.swap(that.polygonAllocator_);
	}

	template <typename DataPolicy>
	void HalfMesh<DataPolicy>::clear()
	{
		while (!polygonSet_.empty())
		{
			removePolygon(*polygonSet_.begin());
		}

		while (!edgeSet_.empty())
		{
			removeEdge(*edgeSet_.begin());
		}

		while (!vertexSet_.empty())
		{
			removeVertex(*vertexSet_.begin());
		}

		vertexSet_.clear();
		halfSet_.clear();
		edgeSet_.clear();
		polygonSet_.clear();
		vertexAllocator_.clear();
		halfAllocator_.clear();
		edgeAllocator_.clear();
		polygonAllocator_.clear();

		DataPolicy::clear();
	}

	// Iterators

	template <typename DataPolicy>
	typename HalfMesh<DataPolicy>::ConstVertexIterator
		HalfMesh<DataPolicy>::vertexBegin() const
	{
		return vertexSet_.begin();
	}

	template <typename DataPolicy>
	typename HalfMesh<DataPolicy>::ConstVertexIterator
		HalfMesh<DataPolicy>::vertexEnd() const
	{
		return vertexSet_.end();
	}

	template <typename DataPolicy>
	typename HalfMesh<DataPolicy>::ConstHalfIterator
		HalfMesh<DataPolicy>::halfBegin() const
	{
		return halfSet_.begin();
	}

	template <typename DataPolicy>
	typename HalfMesh<DataPolicy>::ConstHalfIterator
		HalfMesh<DataPolicy>::halfEnd() const
	{
		return halfSet_.end();
	}

	template <typename DataPolicy>
	typename HalfMesh<DataPolicy>::ConstEdgeIterator
		HalfMesh<DataPolicy>::edgeBegin() const
	{
		return edgeSet_.begin();
	}

	template <typename DataPolicy>
	typename HalfMesh<DataPolicy>::ConstEdgeIterator
		HalfMesh<DataPolicy>::edgeEnd() const
	{
		return edgeSet_.end();
	}

	template <typename DataPolicy>
	typename HalfMesh<DataPolicy>::ConstPolygonIterator
		HalfMesh<DataPolicy>::polygonBegin() const
	{
		return polygonSet_.begin();
	}

	template <typename DataPolicy>
	typename HalfMesh<DataPolicy>::ConstPolygonIterator
		HalfMesh<DataPolicy>::polygonEnd() const
	{
		return polygonSet_.end();
	}

	template <typename DataPolicy>
	typename HalfMesh<DataPolicy>::Half
		HalfMesh<DataPolicy>::findHalf(
		Vertex fromVertex, Vertex toVertex) const
	{
		PENSURE(!fromVertex.empty());
		PENSURE(!toVertex.empty());

		if (fromVertex.isolated() ||
			toVertex.isolated())
		{
			return Half();
		}

		Half begin(fromVertex.half());
		Half current(begin);

		do
		{
			if (current.destination() == toVertex)
			{
				return current;
			}

			current = current.rotateNext();
		}
		while (current != begin);

		return Half();
	}

	// Modification

	template <typename DataPolicy>
	bool HalfMesh<DataPolicy>::makeAdjacent(Half in, Half out)
	{
		PENSURE(!in.empty());
		PENSURE(!out.empty());
		PENSURE(in.destination() == out.origin());
		PENSURE(in.left().empty());
		PENSURE(out.left().empty());

		if (in.next() == out)
		{
			// Adjacency is already correct.

			return true;
		}

		Half b(in.next());
		Half d(out.previous());

		// Find a free incident half edge
		// after 'd' and before 'in'.
		Half g(findFreeIncident(out.origin(),
			out.pair(), in));
		if (g.empty())
		{
			// There is no such half-edge.
			return false;
		}
		Half h(g.next());

		in.half_->next_ = out.half_;
		out.half_->previous_ = in.half_;

		g.half_->next_ = b.half_;
		b.half_->previous_ = g.half_;

		d.half_->next_ = h.half_;
		h.half_->previous_ = d.half_;

		return true;
	}

	template <typename DataPolicy>
	typename HalfMesh<DataPolicy>::Vertex
		HalfMesh<DataPolicy>::addVertex()
	{
		return allocateVertex();
	}

	template <typename DataPolicy>
	typename HalfMesh<DataPolicy>::Edge
		HalfMesh<DataPolicy>::addEdge(
		Vertex fromVertex, Vertex toVertex,
		bool* outAlreadyExisted)
	{
		ENSURE(!fromVertex.empty());
		ENSURE(!toVertex.empty());

		if (outAlreadyExisted)
		{
			*outAlreadyExisted = false;
		}

		// We have decided to leave out
		// edge loops.

		const bool edgeLoop = (fromVertex == toVertex);
		if (REPORT(edgeLoop))
		{
			return Edge();
		}

		// We have also decided to leave out
		// parallel edges. In this case
		// we return the existing edge.

		Half existingHalf(findHalf(fromVertex,
			toVertex));

		if (!existingHalf.empty())
		{
			if (outAlreadyExisted)
			{
				*outAlreadyExisted = true;
			}

			return existingHalf.edge();
		}

		// Check that the vertices are free.
		// If not, the edge addition is not
		// possible.

		// This will be reused later..
		Half fromFreeHalf;

		if (!fromVertex.isolated())
		{
			fromFreeHalf = findFreeIncident(fromVertex);
			if (fromFreeHalf.empty())
			{
				return Edge();
			}
		}

		if (!toVertex.isolated())
		{
			Half toFreeHalf(findFreeIncident(toVertex));
			if (toFreeHalf.empty())
			{
				return Edge();
			}
		}

		// Allocate data

		integer rollBackIndex = 0;

		Edge edge;
		Half fromToHalf;
		Half toFromHalf;

		try
		{
			edge = allocateEdge();
			++rollBackIndex;

			fromToHalf = allocateHalf();
			++rollBackIndex;

			toFromHalf = allocateHalf();
			++rollBackIndex;
		}
		catch(...)
		{
			switch(rollBackIndex)
			{
			case 2:
				deallocateHalf(fromToHalf);
				// Fall-through
			case 1:
				deallocateEdge(edge);
				break;
			};

			throw;
		};

		// Initialize data

		{
			EdgeBody* edgeBody = edge.edge_;
			edgeBody->half_ = fromToHalf.half_;
		}

		{
			HalfBody* halfBody = fromToHalf.half_;

			halfBody->next_ = toFromHalf.half_;
			halfBody->previous_ = toFromHalf.half_;

			halfBody->pair_ = toFromHalf.half_;
			halfBody->origin_ = fromVertex.vertex_;
			halfBody->edge_ = edge.edge_;
			halfBody->left_ = 0;
		}

		{
			HalfBody* halfBody = toFromHalf.half_;

			halfBody->next_ = fromToHalf.half_;
			halfBody->previous_ = fromToHalf.half_;

			halfBody->pair_ = fromToHalf.half_;
			halfBody->origin_ = toVertex.vertex_;
			halfBody->edge_ = edge.edge_;
			halfBody->left_ = 0;
		}

		// Link the from-side of the edge.

		if (fromVertex.isolated())
		{
			VertexBody* vertexBody = fromVertex.vertex_;
			vertexBody->half_ = fromToHalf.half_;
		}
		else
		{
			Half fromIn(fromFreeHalf);

			ASSERT(!fromIn.empty());

			Half fromOut(fromIn.next());

			fromIn.half_->next_ = fromToHalf.half_;
			fromToHalf.half_->previous_ = fromIn.half_;

			toFromHalf.half_->next_ = fromOut.half_;
			fromOut.half_->previous_ = toFromHalf.half_;
		}

		// Link the to-side of the edge.

		if (toVertex.isolated())
		{
			VertexBody* vertexBody = toVertex.vertex_;
			vertexBody->half_ = toFromHalf.half_;
		}
		else
		{
			// 'toFreeHalf' can't be reused here,
			// because the perimeter has changed
			// due to the linking of the from-side.
			Half toIn(findFreeIncident(toVertex));

			ASSERT(!toIn.empty());

			Half toOut(toIn.next());

			toIn.half_->next_ = toFromHalf.half_;
			toFromHalf.half_->previous_ = toIn.half_;

			fromToHalf.half_->next_ = toOut.half_;
			toOut.half_->previous_ = fromToHalf.half_;
		}

		return edge;
	}

	template <typename DataPolicy>
	typename HalfMesh<DataPolicy>::Polygon
		HalfMesh<DataPolicy>::addPolygon(
		const std::vector<Half>& halfLoop)
	{
		ENSURE(!halfLoop.empty());

		const integer halfCount = halfLoop.size();

		// Check that the half-edges are free
		// and form a chain.

		for (integer i = 0;i < halfCount;++i)
		{
			integer nextIndex = i + 1;
			if (nextIndex == halfCount)
			{
				nextIndex = 0;
			}

			const Half current(halfLoop[i]);
			const Half next(halfLoop[nextIndex]);

			ENSURE(!current.empty());
			ENSURE(!next.empty());

			const bool notLegalLoop =
				current.destination() != next.origin();

			if (REPORT(notLegalLoop))
			{
				// The half-edges do not form a chain.
				return Polygon();
			}

			const bool nonManifoldCondition =
				!current.free();

			if (REPORT(nonManifoldCondition))
			{
				// The polygon would introduce
				// a non-manifold condition.
				return Polygon();
			}
		}

		// Try to reorder the links to get
		// a proper orientation.

		for (integer i = 0;i < halfCount;++i)
		{
			integer nextIndex = i + 1;
			if (nextIndex == halfCount)
			{
				nextIndex = 0;
			}

			const bool nonManifoldCondition =
				!makeAdjacent(halfLoop[i],
				halfLoop[nextIndex]);

			if (REPORT(nonManifoldCondition))
			{
				// The polygon would introduce
				// a non-manifold condition.
				return Polygon();
			}
		}

		// Create the polygon

		Polygon newPolygon(allocatePolygon());

		// Link the polygon to a half-edge

		PolygonBody* polygonBody = newPolygon.polygon_;
		polygonBody->half_ = halfLoop[0].half_;

		// Link half-edges to the polygon.

		for (integer i = 0;i < halfCount;++i)
		{
			Half current(halfLoop[i]);

			HalfBody* halfBody = current.half_;
			halfBody->left_ = newPolygon.polygon_;
		}

		return newPolygon;
	}

	template <typename DataPolicy>
	typename HalfMesh<DataPolicy>::Polygon
		HalfMesh<DataPolicy>::addPolygon(
		Vertex aVertex,
		Vertex bVertex,
		Vertex cVertex)
	{
		std::vector<Vertex> vertexList;
		vertexList.reserve(3);

		vertexList.push_back(aVertex);
		vertexList.push_back(bVertex);
		vertexList.push_back(cVertex);

		return addPolygon(vertexList);
	}

	template <typename DataPolicy>
	typename HalfMesh<DataPolicy>::Polygon
		HalfMesh<DataPolicy>::addPolygon(
		Vertex aVertex,
		Vertex bVertex,
		Vertex cVertex,
		Vertex dVertex)
	{
		std::vector<Vertex> vertexList;
		vertexList.reserve(4);

		vertexList.push_back(aVertex);
		vertexList.push_back(bVertex);
		vertexList.push_back(cVertex);
		vertexList.push_back(dVertex);

		return addPolygon(vertexList);
	}

	template <typename DataPolicy>
	typename HalfMesh<DataPolicy>::Polygon
		HalfMesh<DataPolicy>::addPolygon(
		const std::vector<Vertex>& vertexLoop)
	{
		ENSURE(!vertexLoop.empty());

		const integer vertexCount = vertexLoop.size();

		// Check that the vertices are free and non-empty.

		for (integer i = 0;i < vertexCount;++i)
		{
			ENSURE(!vertexLoop[i].empty());

			const bool nonManifoldCondition =
				!vertexLoop[i].free();

			if (REPORT(nonManifoldCondition))
			{
				return Polygon();
			}
		}

		// Find the existing half-edges.
		// Check that they are free.

		std::vector<Half> halfLoop;
		halfLoop.reserve(vertexCount);

		for (integer i = 0;i < vertexCount;++i)
		{
			integer nextIndex = i + 1;
			if (nextIndex == vertexCount)
			{
				nextIndex = 0;
			}

			Half half(findHalf(
				vertexLoop[i], vertexLoop[nextIndex]));

			if (!half.empty())
			{
				const bool nonManifoldCondition =
					!half.free();
				if (REPORT(nonManifoldCondition))
				{
					return Polygon();
				}
			}

			halfLoop.push_back(half);
		}

		// This array will contain the
		// newly created edges so that
		// we can roll back in case of exceptions.

		std::vector<Edge> newEdges;
		newEdges.reserve(vertexCount);

		Polygon polygon;

		try
		{
			for (integer i = 0;i < vertexCount;++i)
			{
				integer nextIndex = i + 1;
				if (nextIndex == vertexCount)
				{
					nextIndex = 0;
				}

				if (halfLoop[i].empty())
				{
					bool alreadyExisted = false;
					const Edge newEdge(addEdge(
						vertexLoop[i], vertexLoop[nextIndex],
						&alreadyExisted));
					ASSERT(!alreadyExisted);

					Half newHalf(newEdge.half());
					if (newHalf.origin() != vertexLoop[i])
					{
						newHalf = newHalf.pair();
						ASSERT(newHalf.origin() == vertexLoop[i]);
					}

					halfLoop[i] = newHalf;

					try
					{
						newEdges.push_back(newEdge);
					}
					catch(...)
					{
						// Roll back the edge addition.
						// Continue handling in the outer
						// level...
						removeEdge(newEdge);
						throw;
					}
				}
			}

			polygon = addPolygon(halfLoop);
			if (polygon.empty())
			{
				// Note this case should not throw
				// an exception.

				// Roll back edge additions.

				const integer newEdgeCount = newEdges.size();
				for (integer i = 0;i < newEdgeCount;++i)
				{
					removeEdge(newEdges[i]);
				}

				return Polygon();
			}
		}
		catch(...)
		{
			// Roll back edge additions.

			const integer newEdgeCount = newEdges.size();
			for (integer i = 0;i < newEdgeCount;++i)
			{
				removeEdge(newEdges[i]);
			}

			throw;
		}

		return polygon;
	}

	template <typename DataPolicy>
	void HalfMesh<DataPolicy>::removeVertex(Vertex vertex)
	{
		ENSURE(!vertex.empty());

		if (!vertex.isolated())
		{
			// Remove every edge that is connected
			// to this vertex

			Half current;
			Half next(vertex.half());

			do
			{
				current = next;
				next = next.rotateNext();
				if (next.edge() == current.edge())
				{
					next = next.rotateNext();
				}
				removeEdge(current.edge());
			}
			while (current != next);
		}

		deallocateVertex(vertex);
	}

	template <typename DataPolicy>
	void HalfMesh<DataPolicy>::removeEdge(Edge edge)
	{
		ENSURE(!edge.empty());

		Half fromToHalf(edge.half());
		ASSERT(!fromToHalf.empty());
		Half toFromHalf(fromToHalf.pair());

		// Remove the neighbouring polygons.

		if (!fromToHalf.left().empty())
		{
			removePolygon(fromToHalf.left());
		}

		if (!toFromHalf.left().empty())
		{
			removePolygon(toFromHalf.left());
		}

		// Link the from-side of the edge
		// off the model.

		Vertex fromVertex(fromToHalf.origin());

		Half fromIn(fromToHalf.previous());
		Half fromOut(fromToHalf.rotateNext());

		if (fromVertex.half() == fromToHalf)
		{
			if (fromOut == fromToHalf)
			{
				fromVertex.vertex_->half_ = 0;
			}
			else
			{
				fromVertex.vertex_->half_ = fromOut.half_;
			}
		}

		fromIn.half_->next_ = fromOut.half_;
		fromOut.half_->previous_ = fromIn.half_;

		// Link the to-side of the edge
		// off the model.

		Vertex toVertex(toFromHalf.origin());

		Half toIn(toFromHalf.previous());
		Half toOut(toFromHalf.rotateNext());

		if (toVertex.half() == toFromHalf)
		{
			if (toOut == toFromHalf)
			{
				toVertex.vertex_->half_ = 0;
			}
			else
			{
				toVertex.vertex_->half_ = toOut.half_;
			}
		}

		toIn.half_->next_ = toOut.half_;
		toOut.half_->previous_ = toIn.half_;

		// 3) Deallocate data

		deallocateHalf(fromToHalf);
		deallocateHalf(toFromHalf);
		deallocateEdge(edge);
	}

	template <typename DataPolicy>
	void HalfMesh<DataPolicy>::removePolygon(Polygon polygon)
	{
		ENSURE(!polygon.empty());

		Half begin(polygon.half());
		ASSERT(!begin.empty());
		Half current(begin);

		do
		{
			HalfBody* halfBody = current.half_;
			halfBody->left_ = 0;
			current = current.next();
		}
		while (current != begin);

		deallocatePolygon(polygon);
	}

	// Private

	template <typename DataPolicy>
	typename HalfMesh<DataPolicy>::Half
		HalfMesh<DataPolicy>::findFreeIncident(
		Vertex vertex) const
	{
		ASSERT(!vertex.empty());
		ASSERT(!vertex.isolated());

		Half begin(vertex.half().pair());
		Half current(begin);

		do
		{
			if (current.left().empty())
			{
				return current;
			}
			current = current.next().pair();
		}
		while (current != begin);

		return Half();
	}

	template <typename DataPolicy>
	typename HalfMesh<DataPolicy>::Half
		HalfMesh<DataPolicy>::findFreeIncident(
		Vertex vertex, Half startingFrom,
		Half andBefore) const
	{
		ASSERT(!vertex.empty());
		ASSERT(!vertex.isolated());
		ASSERT(!startingFrom.empty());
		ASSERT(startingFrom.destination() == vertex);
		ASSERT(!andBefore.empty());
		ASSERT(andBefore.destination() == vertex);

		if (andBefore == startingFrom)
		{
			return Half();
		}

		Half current(startingFrom);

		do
		{
			if (current.left().empty())
			{
				return current;
			}
			current = current.next().pair();
		}
		while (current != andBefore);

		return Half();
	}

	// Allocation

	template <typename DataPolicy>
	typename HalfMesh<DataPolicy>::Vertex
		HalfMesh<DataPolicy>::allocateVertex()
	{
		integer rollBackIndex = 0;
		VertexBody* vertexBody = 0;

		try
		{
			vertexBody = (VertexBody*)vertexAllocator_.allocate();
			++rollBackIndex;

			new(vertexBody) VertexBody;
			++rollBackIndex;

			DataPolicy::constructVertex(vertexBody->data());
			++rollBackIndex;

			if (vertexBody->data())
			{
				new(vertexBody->data()) VertexData;
			}
			++rollBackIndex;

			vertexSet_.insert(Vertex(vertexBody));
			++rollBackIndex;
		}
		catch(...)
		{
			switch(rollBackIndex)
			{
			case 4:
				DataPolicy::destructVertex(vertexBody->data());
				// Fall-through
			case 3:
				if (vertexBody->data())
				{
					StdExt::destruct(vertexBody->data());
				}
				// Fall-through
			case 2:
				StdExt::destruct(vertexBody);
				// Fall-through
			case 1:
				vertexAllocator_.deallocate(vertexBody);
				break;
			};

			throw;
		}

		return Vertex(vertexBody);
	}

	template <typename DataPolicy>
	typename HalfMesh<DataPolicy>::Half
		HalfMesh<DataPolicy>::allocateHalf()
	{
		integer rollBackIndex = 0;
		HalfBody* halfBody = 0;

		try
		{
			halfBody = (HalfBody*)halfAllocator_.allocate();
			++rollBackIndex;

			new(halfBody) HalfBody;
			++rollBackIndex;

			DataPolicy::constructHalf(halfBody->data());
			++rollBackIndex;

			if (halfBody->data())
			{
				new(halfBody->data()) HalfData;
			}
			++rollBackIndex;

			halfSet_.insert(Half(halfBody));
			++rollBackIndex;
		}
		catch(...)
		{
			switch(rollBackIndex)
			{
			case 4:
				DataPolicy::destructHalf(halfBody->data());
				// Fall-through
			case 3:
				if (halfBody->data())
				{
					StdExt::destruct(halfBody->data());
				}
				// Fall-through
			case 2:
				StdExt::destruct(halfBody);
				// Fall through.
			case 1:
				halfAllocator_.deallocate(halfBody);
				break;
			};

			throw;
		}

		return Half(halfBody);
	}

	template <typename DataPolicy>
	typename HalfMesh<DataPolicy>::Edge
		HalfMesh<DataPolicy>::allocateEdge()
	{
		integer rollBackIndex = 0;
		EdgeBody* edgeBody = 0;

		try
		{
			edgeBody = (EdgeBody*)edgeAllocator_.allocate();
			++rollBackIndex;

			new(edgeBody) EdgeBody;
			++rollBackIndex;

			DataPolicy::constructEdge(edgeBody->data());
			++rollBackIndex;

			if (edgeBody->data())
			{
				new(edgeBody->data()) EdgeData;
			}
			++rollBackIndex;

			edgeSet_.insert(Edge(edgeBody));
			++rollBackIndex;
		}
		catch(...)
		{
			// Roll back.

			switch(rollBackIndex)
			{
			case 4:
				DataPolicy::destructEdge(edgeBody->data());
				// Fall-through
			case 3:
				if (edgeBody->data())
				{
					StdExt::destruct(edgeBody->data());
				}
				// Fall-through
			case 2:
				StdExt::destruct(edgeBody);
				// Fall-through
			case 1:
				edgeAllocator_.deallocate(edgeBody);
				break;
			};

			throw;
		}

		return Edge(edgeBody);
	}

	template <typename DataPolicy>
	typename HalfMesh<DataPolicy>::Polygon
		HalfMesh<DataPolicy>::allocatePolygon()
	{
		integer rollBackIndex = 0;
		PolygonBody* polygonBody = 0;

		try
		{
			polygonBody = (PolygonBody*)polygonAllocator_.allocate();
			++rollBackIndex;

			new(polygonBody) PolygonBody;
			++rollBackIndex;

			if (polygonBody->data())
			{
				new(polygonBody->data()) PolygonData;
			}
			++rollBackIndex;

			DataPolicy::constructPolygon(polygonBody->data());
			++rollBackIndex;

			polygonSet_.insert(Polygon(polygonBody));
			++rollBackIndex;
		}
		catch(...)
		{
			// Roll back.

			switch(rollBackIndex)
			{
			case 4:
				DataPolicy::destructPolygon(polygonBody->data());
				// Fall-through
			case 3:
				if (polygonBody->data())
				{
					StdExt::destruct(polygonBody->data());
				}
				// Fall-through
			case 2:
				StdExt::destruct(polygonBody);
				// Fall-through
			case 1:
				polygonAllocator_.deallocate(polygonBody);
				break;
			};

			throw;
		}

		return Polygon(polygonBody);
	}

	// Deallocation

	template <typename DataPolicy>
	void HalfMesh<DataPolicy>::deallocateVertex(Vertex vertex)
	{
		ASSERT(!vertex.empty());

		VertexBody* vertexBody = vertex.vertex_;
		vertexSet_.erase(vertex);
		DataPolicy::destructVertex(vertexBody->data());
		if (vertexBody->data())
		{
			StdExt::destruct(vertexBody->data());
		}
		StdExt::destruct(vertexBody);
		vertexAllocator_.deallocate(vertexBody);
	}

	template <typename DataPolicy>
	void HalfMesh<DataPolicy>::deallocateHalf(Half half)
	{
		ASSERT(!half.empty());

		HalfBody* halfBody = half.half_;
		halfSet_.erase(half);
		DataPolicy::destructHalf(halfBody->data());
		if (halfBody->data())
		{
			StdExt::destruct(halfBody->data());
		}
		StdExt::destruct(halfBody);
		halfAllocator_.deallocate(halfBody);
	}

	template <typename DataPolicy>
	void HalfMesh<DataPolicy>::deallocateEdge(Edge edge)
	{
		ASSERT(!edge.empty());

		EdgeBody* edgeBody = edge.edge_;
		edgeSet_.erase(edge);
		DataPolicy::destructEdge(edgeBody->data());
		if (edgeBody->data())
		{
			StdExt::destruct(edgeBody->data());
		}
		StdExt::destruct(edgeBody);
		edgeAllocator_.deallocate(edgeBody);
	}

	template <typename DataPolicy>
	void HalfMesh<DataPolicy>::deallocatePolygon(Polygon polygon)
	{
		ASSERT(!polygon.empty());

		PolygonBody* polygonBody = polygon.polygon_;
		polygonSet_.erase(polygon);
		DataPolicy::destructPolygon(polygonBody->data());
		if (polygonBody->data())
		{
			StdExt::destruct(polygonBody->data());
		}
		StdExt::destruct(polygonBody);
		polygonAllocator_.deallocate(polygonBody);
	}

	template <typename DataPolicy>
	integer HalfMesh<DataPolicy>::vertices() const
	{
		return vertexSet_.size();
	}

	template <typename DataPolicy>
	integer HalfMesh<DataPolicy>::edges() const
	{
		return edgeSet_.size();
	}

	template <typename DataPolicy>
	integer HalfMesh<DataPolicy>::polygons() const
	{
		return polygonSet_.size();
	}

}

#endif
