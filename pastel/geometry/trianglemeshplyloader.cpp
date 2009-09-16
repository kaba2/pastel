#include "pastel/geometry/trianglemeshplyloader.h"

#include "pastel/geometry/plyloader.h"

namespace Pastel
{

	namespace
	{

		class PlyVertexCallback
			: public PlyLoaderCallback
		{
		public:
			PlyVertexCallback(
				const MemoryView<Vector3>& positions,
				integer index)
				: positions_(positions)
				, index_(index)
			{
			}

			virtual bool receive(
				integer element,
				const std::vector<double>& values)
			{
				ENSURE(!values.empty());

				positions_[element][index_] = values[0];

				return true;
			}

		private:
			MemoryView<Vector3> positions_;
			const integer index_;
		};

		class PlyTriangleCallback
			: public PlyLoaderCallback
		{
		public:
			explicit PlyTriangleCallback(
				TriangleMesh& mesh)
				: mesh_(mesh)
			{
			}

			virtual bool receive(
				integer element,
				const std::vector<double>& values)
			{
				ENSURE_OP(values.size(), >=, 3);

				mesh_.addTriangle(
					(integer)values[0],
					(integer)values[1],
					(integer)values[2]);

				return true;
			}

		private:
			TriangleMesh& mesh_;
		};

	}

	PASTELGEOMETRY void loadPlyMesh(const std::string& fileName,
		TriangleMesh& mesh)
	{
		PlyLoaderInfo meshInfo;
		bool success = plyLoader().loadInfo(
			fileName, meshInfo);
		ENSURE(success);

		integer elementIndex = meshInfo.elements().find("vertex");
		const PlyLoaderElement& element =
			meshInfo.elements().valueAt(elementIndex);

		mesh.reserveVertices(
			element.count());

		MemoryView<Vector3> positions;
		success = mesh.vertexProperty<Vector3>(
			TriangleMesh::VertexPositionId, positions);
		ENSURE(success);

		PlyVertexCallback pointCallback0(positions, 0);

		plyLoader().loadElement(
			fileName,
			"vertex", "x",
			pointCallback0);

		PlyVertexCallback pointCallback1(positions, 1);

		plyLoader().loadElement(
			fileName,
			"vertex", "y",
			pointCallback1);

		PlyVertexCallback pointCallback2(positions, 2);

		plyLoader().loadElement(
			fileName,
			"vertex", "z",
			pointCallback2);

		PlyTriangleCallback triangleCallback(mesh);

		plyLoader().loadElement(
			fileName,
			"face",
			"vertex_indices",
			triangleCallback);
	}

}
