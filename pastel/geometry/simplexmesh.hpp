#ifndef PASTEL_SIMPLEXMESH_HPP
#define PASTEL_SIMPLEXMESH_HPP

#include "pastel/geometry/simplexmesh.h"

namespace Pastel
{

	template <int N, typename DataPolicy>
	SimplexMesh<N, DataPolicy>::SimplexMesh()
		: vertexSet_()
		, simplexSet_()
		, vertexAllocator_(sizeof(VertexBody))
		, simplexAllocator_(sizeof(SimplexBody))
	{
	}

	template <int N, typename DataPolicy>
	SimplexMesh<N, DataPolicy>::SimplexMesh(const SimplexMesh& that)
		: vertexSet_()
		, simplexSet_()
		, vertexAllocator_(sizeof(VertexBody))
		, simplexAllocator_(sizeof(SimplexBody))
	{
		// TODO
		ENSURE(false);
	}

	template <int N, typename DataPolicy>
	SimplexMesh<N, DataPolicy>::~SimplexMesh()
	{
		clear();
	}

	template <int N, typename DataPolicy>
	SimplexMesh<N, DataPolicy>& SimplexMesh<N, DataPolicy>::operator=(
		const SimplexMesh& that)
	{
		SimplexMesh copy(that);
		swap(copy);
		return *this;
	}

	template <int N, typename DataPolicy>
	void SimplexMesh<N, DataPolicy>::swap(SimplexMesh& that)
	{
		DataPolicy::swap(that);
		vertexSet_.swap(that.vertexSet_);
		simplexSet_.swap(that.simplexSet_);
		vertexAllocator_.swap(that.vertexAllocator_);
		simplexAllocator_.swap(that.simplexAllocator_);
	}

	template <int N, typename DataPolicy>
	void SimplexMesh<N, DataPolicy>::clear()
	{
		vertexSet_.clear();
		simplexSet_.clear();
		vertexAllocator_.clear();
		simplexAllocator_.clear();
		DataPolicy::clear();
	}

	template <int N, typename DataPolicy>
	typename SimplexMesh<N, DataPolicy>::Vertex
		SimplexMesh<N, DataPolicy>::addVertex()
	{
		return allocateVertex();
	}

	template <int N, typename DataPolicy>
	typename SimplexMesh<N, DataPolicy>::Simplex
		SimplexMesh<N, DataPolicy>::addSimplex(
			const Tuple<Vertex, PASTEL_ADD_N(N, 1)>& vertexList)
	{
		Simplex simplex = allocateSimplex();

		SimplexBody* data = simplex.data_;
		data->vertexList_ = vertexList;

		return simplex;
	}

	template <int N, typename DataPolicy>
	typename SimplexMesh<N, DataPolicy>::ConstVertexIterator
		SimplexMesh<N, DataPolicy>::vertexBegin() const
	{
		return vertexSet_.begin();
	}

	template <int N, typename DataPolicy>
	typename SimplexMesh<N, DataPolicy>::ConstVertexIterator
		SimplexMesh<N, DataPolicy>::vertexEnd() const
	{
		return vertexSet_.end();
	}

	template <int N, typename DataPolicy>
	typename SimplexMesh<N, DataPolicy>::ConstSimplexIterator
		SimplexMesh<N, DataPolicy>::simplexBegin() const
	{
		return simplexSet_.begin();
	}

	template <int N, typename DataPolicy>
	typename SimplexMesh<N, DataPolicy>::ConstSimplexIterator
		SimplexMesh<N, DataPolicy>::simplexEnd() const
	{
		return simplexSet_.end();
	}

	template <int N, typename DataPolicy>
	integer SimplexMesh<N, DataPolicy>::vertices() const
	{
		return vertexSet_.size();
	}

	template <int N, typename DataPolicy>
	integer SimplexMesh<N, DataPolicy>::simplices() const
	{
		return simplexSet_.size();
	}

	// Private

	template <int N, typename DataPolicy>
	typename SimplexMesh<N, DataPolicy>::Vertex
		SimplexMesh<N, DataPolicy>::allocateVertex()
	{
		integer rollBackIndex = 0;

		VertexBody* vertexData = (VertexBody*)vertexAllocator_.allocate();
		++rollBackIndex;

		try
		{
			new(vertexData) VertexBody;
			++rollBackIndex;

			vertexSet_.insert(Vertex(vertexData));
			++rollBackIndex;

			if (vertexData->data())
			{
				new(vertexData->data()) VertexData;
			}
			++rollBackIndex;

			DataPolicy::constructVertex(vertexData->data());
			++rollBackIndex;
		}
		catch(...)
		{
			switch(rollBackIndex)
			{
			case 4:
				if (vertexData->data())
				{
					vertexData->data()->~VertexData();
				}
				// Fall-through
			case 3:
				vertexSet_.erase(Vertex(vertexData));
				// Fall-through
			case 2:
				vertexData->~VertexBody();
				// Fall-through
			case 1:
				vertexAllocator_.deallocate(vertexData);
				break;
			};

			throw;
		}

		return Vertex(vertexData);
	}

	template <int N, typename DataPolicy>
	typename SimplexMesh<N, DataPolicy>::Simplex
		SimplexMesh<N, DataPolicy>::allocateSimplex()
	{
		integer rollBackIndex = 0;

		SimplexBody* simplexData = (SimplexBody*)simplexAllocator_.allocate();
		++rollBackIndex;

		try
		{
			new(simplexData) SimplexBody;
			++rollBackIndex;

			simplexSet_.insert(Simplex(simplexData));
			++rollBackIndex;

			if (simplexData->data())
			{
				new(simplexData->data()) SimplexData;
			}
			++rollBackIndex;

			DataPolicy::constructSimplex(simplexData->data());
			++rollBackIndex;
		}
		catch(...)
		{
			switch(rollBackIndex)
			{
			case 4:
				if (simplexData->data())
				{
					simplexData->data()->~SimplexData();
				}
				// Fall-through
			case 3:
				simplexSet_.erase(Simplex(simplexData));
				// Fall-through
			case 2:
				simplexData->~SimplexBody();
				// Fall-through
			case 1:
				simplexAllocator_.deallocate(simplexData);
				break;
			};

			throw;
		}

		return Simplex(simplexData);
	}

}

#endif
