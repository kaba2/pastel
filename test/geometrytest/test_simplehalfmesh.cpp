// Description: Some testing for HalfMesh
// DocumentationOf: halfmesh.h

#include "pastelgeometrytest.h"
#include "pastel/geometry/simplehalfmesh.h"

using namespace Pastel;

namespace
{

	typedef SimpleHalfMesh<integer, integer, integer, integer>
		Mesh;

	typedef Mesh::Vertex Vertex;
	typedef Mesh::Edge Edge;

	class Test
		: public TestSuite
	{
	public:
		Test()
			: TestSuite(&testReport())
		{
		}

		virtual void run()
		{
			test();
		}

		void test()
		{
			Mesh mesh;

			Vertex aVertex(mesh.addVertex());
			Vertex bVertex(mesh.addVertex());
			Vertex cVertex(mesh.addVertex());
			Vertex dVertex(mesh.addVertex());

			aVertex() = 0;
			bVertex() = 1;
			cVertex() = 2;
			dVertex() = 3;

			log() << aVertex() << ", "
				<< bVertex() << ", "
				<< cVertex() << ", "
				<< dVertex() << logNewLine;

			mesh.addEdge(aVertex, bVertex);

			log() << aVertex() << ", "
				<< bVertex() << ", "
				<< cVertex() << ", "
				<< dVertex() << logNewLine;

			mesh.addEdge(bVertex, cVertex);

			log() << aVertex() << ", "
				<< bVertex() << ", "
				<< cVertex() << ", "
				<< dVertex() << logNewLine;

			mesh.addEdge(cVertex, dVertex);

			log() << aVertex() << ", "
				<< bVertex() << ", "
				<< cVertex() << ", "
				<< dVertex() << logNewLine;

			mesh.addEdge(dVertex, aVertex);

			log() << aVertex() << ", "
				<< bVertex() << ", "
				<< cVertex() << ", "
				<< dVertex() << logNewLine;

			mesh.addPolygon(aVertex, cVertex, bVertex);

			log() << aVertex() << ", "
				<< bVertex() << ", "
				<< cVertex() << ", "
				<< dVertex() << logNewLine;

			mesh.addPolygon(aVertex, dVertex, cVertex);

			log() << aVertex() << ", "
				<< bVertex() << ", "
				<< cVertex() << ", "
				<< dVertex() << logNewLine;

			mesh.removeVertex(aVertex);

			log() << "removed" << ", "
				<< bVertex() << ", "
				<< cVertex() << ", "
				<< dVertex() << logNewLine;
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("SimpleHalfMesh", test);
	}

	CallFunction run(addTest);

}
