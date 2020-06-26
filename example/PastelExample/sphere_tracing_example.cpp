// Description: Sphere tracing example
// DocumentationOf: kdtree.h

#include "pastel_example.h"

#include "pastel/geometry/kdtree.h"
#include "pastel/geometry/kdtree_tools.h"
#include "pastel/geometry/intersect_line_sphere.h"
#include "pastel/geometry/intersect_line_alignedbox.h"

#include "pastel/math/uniform_sampling.h"
#include "pastel/math/smoothstep.h"

#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/color_palette.h"

#include "pastel/sys/array.h"
#include "pastel/sys/tristate.h"
#include "pastel/sys/pool_allocator.h"

#include <iostream>
#include <string>

using namespace Pastel;

namespace
{

	class SpherePolicy
	{
	public:
		typedef Sphere3* Object;

		AlignedBox3 bound(const Sphere3* object) const
		{
			Sphere3 temp = *object;
			temp.setRadius(temp.radius() * 1.1);
			return boundingAlignedBox(temp);

			return boundingAlignedBox(*object);
		}

		Tuple<dreal, 2> bound(const Sphere3* object, integer axis) const
		{
			return Tuple<dreal, 2>(
				object->position()[axis] - object->radius(),
				object->position()[axis] + object->radius());
		}
	};

	bool sphereIntersector(const Sphere3* object,
		const Line3& ray, dreal& t)
	{
		const AlignedBox3 box = boundingAlignedBox(*object);

		/*
		Vector2 hitList;
		if (!intersect(ray, box, hitList) ||
			hitList[1] < 0)
		{
			return false;
		}
		*/

		Vector2 hitList;
		if (!intersect(ray, *object, hitList) ||
			hitList[1] < 0)
		{
			return false;
		}

		if (hitList[0] > 0)
		{
			t = hitList[0];
		}
		else
		{
			t = hitList[1];
		}

		return true;
	}

	void testSphereTracing()
	{
		typedef KdTree<dreal, 3, SpherePolicy> Tree;
		typedef Tree::ConstObjectIterator ConstObjectIterator;

		SpherePolicy spherePolicy;
		Tree kdTree(3, spherePolicy);
		const integer spheres = 10000;

		PoolAllocator allocator(sizeof(Sphere3));

		std::vector<Sphere3*> sphereList;
		sphereList.reserve(spheres);

		for (integer i = 0;i < spheres;++i)
		{
			Sphere3* sphere = (Sphere3*)allocator.allocate();
			new(sphere) Sphere3(Vector3(1 * randomVectorBall<dreal, 3>()),
				0.01);

			sphereList.push_back(sphere);
		}

		kdTree.reserveBound(AlignedBox3(Vector3(-10), Vector3(10)));
		kdTree.insert(sphereList.begin(), sphereList.end());

		std::cout << "The kdTree has " << std::endl;
		std::cout << kdTree.nodes() << " nodes" << std::endl;
		std::cout << "Depth of " << depth(kdTree) << std::endl;
		std::cout << kdTree.objects() << " object references ("
			<< (dreal)kdTree.objects() / kdTree.leaves() << " per leaf on average)." << std::endl;

		refineSurfaceAreaHeuristic(computeKdTreeMaxDepth(kdTree.objects()), 2, kdTree);
		//refineSurfaceAreaHeuristic(0, 2, kdTree);

		kdTree.reserveBound(AlignedBox3(kdTree.bound().min() - 1, kdTree.bound().max() + 1));

		REPORT(!check(kdTree));

		std::cout << "The kdTree has " << std::endl;
		std::cout << kdTree.nodes() << " nodes" << std::endl;
		std::cout << "Of which " << kdTree.leaves() << " are leaf nodes." << std::endl;
		std::cout << "Depth of " << depth(kdTree) << std::endl;
		std::cout << kdTree.objects() << " object references ("
			<< (dreal)kdTree.objects() / kdTree.leaves() << " per leaf on average)." << std::endl;

		std::vector<Color> palette;
		falseColorPalette(palette, 256);

		const integer width = 800;
		const integer height = 800;

		Array<Color, 2> image(Vector2i(width, height), Color(0));

		integer reportedProgress = -1;

		for (integer y = 0;y < height;++y)
		{
			const integer progress = 100 * ((dreal)y / (height - 1));
			if (progress != reportedProgress)
			{
				std::cout << progress << "% ";
				reportedProgress = progress;
			}
			for (integer x = 0;x < width;++x)
			{
				const Line3 ray(
					Vector3(
					2 * ((dreal)(x + 0.5) / (width - 1)) - 1,
					2 * ((dreal)(y + 0.5) / (height - 1)) - 1,
					-2),
					Vector3(0, 0, 1));

				const ConstObjectIterator iter =
					recursiveRayTraversal(kdTree, ray, sphereIntersector);
				if (iter != kdTree.end())
				{
					image(x, y) = linear(Color(1), evaluate(0.5 * Color(0.5, 0, 1)), cubicSmoothStep(norm((*iter)->position())));
					//image(x, y) = palette[(uint8)((pointer_integer)(*iter) % 256)];
					//image(x, y) = Color(1);
				}
			}
		}

		std::cout << std::endl;

		allocator.clear();

		savePcx(image, "spheretracing.pcx");
	}

	void addTest()
	{
		testRunner().add("Sphere tracing", testSphereTracing);
	}

	CallFunction run(addTest);

}
