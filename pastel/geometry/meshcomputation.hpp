#ifndef PASTELGEOMETRY_MESHCOMPUTATION_HPP
#define PASTELGEOMETRY_MESHCOMPUTATION_HPP

#include "pastel/geometry/meshcomputation.h"

#include "pastel/sys/vector_tools.h"
#include "pastel/math/coordinates.h"
#include "pastel/sys/vector_tools.h"

#include "pastel/sys/constants.h"

namespace Pastel
{

	template <
		typename VertexKey,
		typename TriangleKey,
		typename Real>
	void computeTriangleNormals(
		TriangleMesh& mesh,
		const PropertyId<VertexKey, Point<3, Real> >& vertexPositionId,
		const PropertyId<TriangleKey, Vector<3, Real> >& triangleNormalId)
	{
		ConstMemoryView<Point<3, Real> > positions;
		bool result = mesh.vertexProperty(
			vertexPositionId, positions);
		ENSURE(result);

		MemoryView<Vector<3, Real> > normals;
		result = mesh.triangleProperty(
			triangleNormalId, normals);
		if (!result)
		{
			mesh.trianglePropertyAdd(
				triangleNormalId);
			mesh.triangleProperty(
				triangleNormalId, normals);
		}

		ConstMemoryView<Integer3>
			triangles(mesh.triangleIndices());

		for (integer i = 0;i < triangles.size();++i)
		{
			const integer aIndex = triangles[i][0];
			const integer bIndex = triangles[i][1];
			const integer cIndex = triangles[i][2];

			const Vector<3, Real> bDelta(
				positions[bIndex] - positions[aIndex]);
			const Vector<3, Real> cDelta(
				positions[cIndex] - positions[aIndex]);

			normals[i] = cross(bDelta, cDelta);
			Real normalNorm = norm(normals[i]);
			if (normalNorm == 0)
			{
				// If the triangle is degenerate, use
				// a triangle centroid as the normal.
				normals[i] = asVector(positions[aIndex] +
					bDelta * (Real)0.5 +
					cDelta * (Real)0.5);
				normalNorm = norm(normals[i]);
			}

			normals[i] /= normalNorm;
		}
	}

	template <
		typename VertexKey,
		typename Real>
	void computeVertexNormals(
		TriangleMesh& mesh,
		const PropertyId<VertexKey, Point<3, Real> >& vertexPositionId,
		const PropertyId<VertexKey, Vector<3, Real> >& vertexNormalId)
	{
		bool result = false;

		ConstMemoryView<Point<3, Real> > positions;
		result =	mesh.vertexProperty(vertexPositionId, positions);
		ENSURE(result);

		MemoryView<Vector<3, Real> > normals;
		result = mesh.vertexProperty(vertexNormalId, normals);
		if (!result)
		{
			mesh.vertexPropertyAdd<Vector<3, Real> >(vertexNormalId);
			mesh.vertexProperty(vertexNormalId, normals);
		}

		ConstMemoryView<Integer3> triangles(
			mesh.triangleIndices());

		// Zero all normals

		for (integer i = 0;i < normals.size();++i)
		{
			normals[i].set(0);
		}

		// Using a shooting approach,
		// compute each triangle's unit normal and
		// add it to the triangle's each vertex.

		for (integer i = 0;i < triangles.size();++i)
		{
			integer aIndex = triangles[i][0];
			integer bIndex = triangles[i][1];
			integer cIndex = triangles[i][2];

			const Vector<3, Real> bDelta(
				positions[bIndex] - positions[aIndex]);
			const Vector<3, Real> cDelta(
				positions[cIndex] - positions[aIndex]);

			Vector<3, Real> normal(cross(bDelta, cDelta));
			const Real normalNorm(norm(normal));
			// If the triangle is degenerate,
			// do not compute its contribution.
			if (normalNorm > 0)
			{
				normal /= normalNorm;

				normals[aIndex] += normal;
				normals[bIndex] += normal;
				normals[cIndex] += normal;
			}
		}

		// Normalize normals

		for (integer i = 0;i < normals.size();++i)
		{
			Real normalNorm(norm(normals[i]));
			if (normalNorm == 0)
			{
				// If the vertex normal is degenerate,
				// use the vertex centroid.
				normals[i] = Vector3(asVector(positions[i]));
				normalNorm = norm(normals[i]);
				if (normalNorm == 0)
				{
					// If this is the origin, pick
					// some arbitrary vector.
					normals[i].set(0, 0, 1);
					normalNorm = 1;
				}
			}

			normals[i] /= normalNorm;
		}
	}

	template <
		typename VertexKey,
		typename Real>
	void computeSphereParametrization(
		TriangleMesh& mesh,
		const PropertyId<VertexKey, Point<3, Real> >& vertexPositionId,
		const PropertyId<VertexKey, Point<2, Real> >& vertexUvId)
	{
		bool result = false;

		ConstMemoryView<Point<3, Real> > positions;
		result = mesh.vertexProperty(vertexPositionId, positions);
		ENSURE(result);

		MemoryView<Point<2, Real> > uvs;
		result = mesh.vertexProperty(vertexUvId, uvs);
		if (!result)
		{
			mesh.vertexPropertyAdd<Point<2, Real> >(vertexUvId);
			mesh.vertexProperty(vertexUvId, uvs);
		}

		Vector<2, Real> normalization(1 / (2 * constantPi<Real>()),
			1 / constantPi<Real>());
		for (integer i = 0;i < positions.size();++i)
		{
			uvs[i] = Point<2, Real>(cartesianToDirection(
				asVector(positions[i])) * normalization);
		}
	}

}

#endif
