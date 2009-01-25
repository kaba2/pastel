#ifndef PASTELGEOMETRY_TRIANGLEMESH_HPP
#define PASTELGEOMETRY_TRIANGLEMESH_HPP

#include "pastel/geometry/trianglemesh.h"

namespace Pastel
{

	template <typename Type>
	bool TriangleMesh::vertexPropertyAdd(
		const PropertyId<integer, Type>& key)
	{
		return vertexData_.add<Type>(key);
	}

	template <typename Type>
	bool TriangleMesh::vertexPropertyRemove(
		const PropertyId<integer, Type>& key)
	{
		return vertexData_.remove(key);
	}

	template <typename Type>
	bool TriangleMesh::vertexProperty(
		const PropertyId<integer, Type>& key,
		MemoryView<Type>& view)
	{
		return vertexData_.get<Type>(key, view);
	}

	template <typename Type>
	bool TriangleMesh::vertexProperty(
		const PropertyId<integer, Type>& key,
		ConstMemoryView<Type>& view)
	{
		return vertexData_.get<Type>(key, view);
	}

	template <typename Type>
	bool TriangleMesh::vertexProperty(
		const PropertyId<integer, Type>& key,
		ConstMemoryView<Type>& view) const
	{
		return vertexData_.get<Type>(key, view);
	}

	template <typename Type>
	bool TriangleMesh::trianglePropertyAdd(
		const PropertyId<integer, Type>& key)
	{
		return triangleData_.add<Type>(key);
	}

	template <typename Type>
	bool TriangleMesh::trianglePropertyRemove(
		const PropertyId<integer, Type>& key)
	{
		return triangleData_.remove(key);
	}

	template <typename Type>
	bool TriangleMesh::triangleProperty(
		const PropertyId<integer, Type>& key,
		MemoryView<Type>& view)
	{
		return triangleData_.get<Type>(key, view);
	}

	template <typename Type>
	bool TriangleMesh::triangleProperty(
		const PropertyId<integer, Type>& key,
		ConstMemoryView<Type>& view)
	{
		return triangleData_.get<Type>(key, view);
	}

	template <typename Type>
	bool TriangleMesh::triangleProperty(
		const PropertyId<integer, Type>& key,
		ConstMemoryView<Type>& view) const
	{
		return triangleData_.get<Type>(key, view);
	}

}

#endif
