#include "pastel/geometry/propertyhalfmesh.h"

namespace Pastel
{

	PropertyHalfMesh_Policy::PropertyHalfMesh_Policy()
		: vertexIndices_()
		, halfIndices_()
		, edgeIndices_()
		, polygonIndices_()
		, vertexData_()
		, halfData_()
		, edgeData_()
		, polygonData_()
	{
	}

	PropertyHalfMesh_Policy::PropertyHalfMesh_Policy(
		const PropertyHalfMesh_Policy& that)
		: vertexIndices_(that.vertexIndices_)
		, halfIndices_(that.halfIndices_)
		, edgeIndices_(that.edgeIndices_)
		, polygonIndices_(that.polygonIndices_)
		, vertexData_(that.vertexData_)
		, halfData_(that.halfData_)
		, edgeData_(that.edgeData_)
		, polygonData_(that.polygonData_)
	{
	}

	PropertyHalfMesh_Policy::~PropertyHalfMesh_Policy()
	{
		clear();
	}

	PropertyHalfMesh_Policy& PropertyHalfMesh_Policy::operator=(
		const PropertyHalfMesh_Policy& that)
	{
		PropertyHalfMesh_Policy copy(that);
		swap(copy);
		return *this;
	}

	void PropertyHalfMesh_Policy::swap(PropertyHalfMesh_Policy& that)
	{
		vertexIndices_.swap(that.vertexIndices_);
		halfIndices_.swap(that.halfIndices_);
		edgeIndices_.swap(that.edgeIndices_);
		polygonIndices_.swap(that.polygonIndices_);

		vertexData_.swap(that.vertexData_);
		halfData_.swap(that.halfData_);
		edgeData_.swap(that.edgeData_);
		polygonData_.swap(that.polygonData_);
	}

	void PropertyHalfMesh_Policy::clear()
	{
		vertexIndices_.clear();
		halfIndices_.clear();
		edgeIndices_.clear();
		polygonIndices_.clear();

		vertexData_.clear();
		halfData_.clear();
		edgeData_.clear();
		polygonData_.clear();
	}

    void PropertyHalfMesh_Policy::reserveVertices(integer vertices)
    {
        vertexData_.resize(vertices);
    }

    // Private

	void PropertyHalfMesh_Policy::constructVertex(EntityData* data)
	{
		integer rollBackIndex = 0;
		integer id = 0;

		try
		{
			id = vertexIndices_.allocate();
			++rollBackIndex;

			vertexData_.resize(id + 1);
			++rollBackIndex;

			*data = id;
			++rollBackIndex;
		}
		catch(...)
		{
			switch(rollBackIndex)
			{
			case 2:
				// Can't and needn't rollback.
				// Fall-through
			case 1:
				vertexIndices_.deallocate(id);
				break;
			};

			throw;
		}
	}

	void PropertyHalfMesh_Policy::destructVertex(EntityData* data)
	{
		vertexIndices_.deallocate(*data);
		data->~EntityData();
	}

	void PropertyHalfMesh_Policy::constructHalf(EntityData* data)
	{
		integer rollBackIndex = 0;
		integer id = 0;

		try
		{
			id = halfIndices_.allocate();
			++rollBackIndex;

			halfData_.resize(id + 1);
			++rollBackIndex;

			*data = id;
			++rollBackIndex;
		}
		catch(...)
		{
			switch(rollBackIndex)
			{
			case 2:
				// Can't and needn't rollback.
				// Fall-through
			case 1:
				halfIndices_.deallocate(id);
				break;
			};

			throw;
		}
	}

	void PropertyHalfMesh_Policy::destructHalf(EntityData* data)
	{
		halfIndices_.deallocate(*data);
		data->~EntityData();
	}

	void PropertyHalfMesh_Policy::constructEdge(EntityData* data)
	{
		integer rollBackIndex = 0;
		integer id = 0;

		try
		{
			id = edgeIndices_.allocate();
			++rollBackIndex;

			edgeData_.resize(id + 1);
			++rollBackIndex;

			*data = id;
			++rollBackIndex;
		}
		catch(...)
		{
			switch(rollBackIndex)
			{
			case 2:
				// Can't and needn't rollback.
				// Fall-through
			case 1:
				edgeIndices_.deallocate(id);
				break;
			};

			throw;
		}
	}

	void PropertyHalfMesh_Policy::destructEdge(EntityData* data)
	{
		edgeIndices_.deallocate(*data);
		data->~EntityData();
	}

	void PropertyHalfMesh_Policy::constructPolygon(EntityData* data)
	{
		integer rollBackIndex = 0;
		integer id = 0;

		try
		{
			id = polygonIndices_.allocate();
			++rollBackIndex;

			polygonData_.resize(id + 1);
			++rollBackIndex;

			*data = id;
			++rollBackIndex;
		}
		catch(...)
		{
			switch(rollBackIndex)
			{
			case 2:
				// Can't and needn't rollback.
				// Fall-through
			case 1:
				polygonIndices_.deallocate(id);
				break;
			};

			throw;
		}
	}

	void PropertyHalfMesh_Policy::destructPolygon(EntityData* data)
	{
		polygonIndices_.deallocate(*data);
		data->~EntityData();
	}

}
