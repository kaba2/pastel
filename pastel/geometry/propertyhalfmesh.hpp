#ifndef PASTELGEOMETRY_PROPERTYHALFMESH_HPP
#define PASTELGEOMETRY_PROPERTYHALFMESH_HPP

#include "pastel/geometry/propertyhalfmesh.h"

namespace Pastel
{

	// Vertices

	template <typename Type>
	bool PropertyHalfMesh_Policy::vertexPropertyAdd(
		const PropertyId<integer, Type>& key,
			const PASTEL_NO_DEDUCTION(Type)& defaultValue)
	{
		return vertexData_.add(key, defaultValue);
	}

	template <typename Type>
	bool PropertyHalfMesh_Policy::vertexPropertyRemove(
		const PropertyId<integer, Type>& key)
	{
		return vertexData_.remove(key);
	}

	template <typename Type>
	bool PropertyHalfMesh_Policy::vertexProperty(
		const PropertyId<integer, Type>& key,
		MemoryView<Type>& view)
	{
		return vertexData_.get(key, view);
	}

	template <typename Type>
	bool PropertyHalfMesh_Policy::vertexProperty(
		const PropertyId<integer, Type>& key,
		ConstMemoryView<Type>& view)
	{
		return vertexData_.get(key, view);
	}

	template <typename Type>
	bool PropertyHalfMesh_Policy::vertexProperty(
		const PropertyId<integer, Type>& key,
		ConstMemoryView<Type>& view) const
	{
		return vertexData_.get(key, view);
	}

	// Half-edges

	template <typename Type>
	bool PropertyHalfMesh_Policy::halfPropertyAdd(
		const PropertyId<integer, Type>& key,
			const PASTEL_NO_DEDUCTION(Type)& defaultValue)
	{
		return halfData_.add(key, defaultValue);
	}

	template <typename Type>
	bool PropertyHalfMesh_Policy::halfPropertyRemove(
		const PropertyId<integer, Type>& key)
	{
		return halfData_.remove(key);
	}

	template <typename Type>
	bool PropertyHalfMesh_Policy::halfProperty(
		const PropertyId<integer, Type>& key,
		MemoryView<Type>& view)
	{
		return halfData_.get(key, view);
	}

	template <typename Type>
	bool PropertyHalfMesh_Policy::halfProperty(
		const PropertyId<integer, Type>& key,
		ConstMemoryView<Type>& view)
	{
		return halfData_.get(key, view);
	}

	template <typename Type>
	bool PropertyHalfMesh_Policy::halfProperty(
		const PropertyId<integer, Type>& key,
		ConstMemoryView<Type>& view) const
	{
		return halfData_.get(key, view);
	}

	// Edges

	template <typename Type>
	bool PropertyHalfMesh_Policy::edgePropertyAdd(
		const PropertyId<integer, Type>& key,
			const PASTEL_NO_DEDUCTION(Type)& defaultValue)
	{
		return edgeData_.add(key, defaultValue);
	}

	template <typename Type>
	bool PropertyHalfMesh_Policy::edgePropertyRemove(
		const PropertyId<integer, Type>& key)
	{
		return edgeData_.remove(key);
	}

	template <typename Type>
	bool PropertyHalfMesh_Policy::edgeProperty(
		const PropertyId<integer, Type>& key,
		MemoryView<Type>& view)
	{
		return edgeData_.get(key, view);
	}

	template <typename Type>
	bool PropertyHalfMesh_Policy::edgeProperty(
		const PropertyId<integer, Type>& key,
		ConstMemoryView<Type>& view)
	{
		return edgeData_.get(key, view);
	}

	template <typename Type>
	bool PropertyHalfMesh_Policy::edgeProperty(
		const PropertyId<integer, Type>& key,
		ConstMemoryView<Type>& view) const
	{
		return edgeData_.get(key, view);
	}

	// Polygons

	template <typename Type>
	bool PropertyHalfMesh_Policy::polygonPropertyAdd(
		const PropertyId<integer, Type>& key,
			const PASTEL_NO_DEDUCTION(Type)& defaultValue)
	{
		return polygonData_.add(key, defaultValue);
	}

	template <typename Type>
	bool PropertyHalfMesh_Policy::polygonPropertyRemove(
		const PropertyId<integer, Type>& key)
	{
		return polygonData_.remove(key);
	}

	template <typename Type>
	bool PropertyHalfMesh_Policy::polygonProperty(
		const PropertyId<integer, Type>& key,
		MemoryView<Type>& view)
	{
		return polygonData_.get(key, view);
	}

	template <typename Type>
	bool PropertyHalfMesh_Policy::polygonProperty(
		const PropertyId<integer, Type>& key,
		ConstMemoryView<Type>& view)
	{
		return polygonData_.get(key, view);
	}

	template <typename Type>
	bool PropertyHalfMesh_Policy::polygonProperty(
		const PropertyId<integer, Type>& key,
		ConstMemoryView<Type>& view) const
	{
		return polygonData_.get(key, view);
	}

}

#endif
