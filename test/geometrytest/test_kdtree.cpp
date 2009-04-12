#include "pastelgeometrytest.h"

#include "pastel/geometry/kdtree.h"
#include "pastel/geometry/kdtree_tools.h"
#include "pastel/geometry/intersect_line_sphere.h"
#include "pastel/geometry/intersect_line_alignedbox.h"

#include "pastel/math/uniformsampling.h"

#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/palette.h"

#include "pastel/sys/array.h"
#include "pastel/sys/tristate.h"
#include "pastel/sys/poolallocator.h"

#include <iostream>
#include <string>

using namespace Pastel;

namespace
{

	void testPointConstruction()
	{
		KdTree<2, real> tree;

		const integer points = 100000;
		std::vector<Point2> pointList;
		pointList.reserve(points);

		for (integer i = 0;i < points;++i)
		{
			pointList.push_back(Point2(randomVectorBall<2, real>()));
		}

		tree.insert(pointList.begin(), pointList.end());

		log() << "The tree has " << logNewLine;
		log() << tree.nodes() << " nodes" << logNewLine;
		log() << "Depth of " << depth(tree) << logNewLine;
		log() << tree.objects() << " object references ("
			<< (real)tree.objects() / tree.leaves() << " per leaf on average)." << logNewLine;

		refineMidpoint(computeKdTreeMaxDepth(tree.objects()), 4, tree);

		REPORT(!check(tree));

		log() << "The tree has " << logNewLine;
		log() << tree.nodes() << " nodes" << logNewLine;
		log() << "Of which " << tree.leaves() << " are leaf nodes." << logNewLine;
		log() << "Depth of " << depth(tree) << logNewLine;
		log() << tree.objects() << " object references ("
			<< (real)tree.objects() / tree.leaves() << " per leaf on average)." << logNewLine;
	}

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

		Tuple<2, real> bound(const Sphere3* object, integer axis) const
		{
			return Tuple<2, real>(
				object->position()[axis] - object->radius(),
				object->position()[axis] + object->radius());
		}
	};

	bool sphereIntersector(const Sphere3* object,
		const Line3& ray, real& t)
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
		typedef KdTree<3, real, SpherePolicy> Tree;
		typedef Tree::ConstObjectIterator ConstObjectIterator;

		SpherePolicy spherePolicy;
		Tree tree(spherePolicy);
		const integer spheres = 10000;

		PoolAllocator allocator(sizeof(Sphere3));

		std::vector<Sphere3*> sphereList;
		sphereList.reserve(spheres);

		for (integer i = 0;i < spheres;++i)
		{
			Sphere3* sphere = (Sphere3*)allocator.allocate();
			new(sphere) Sphere3(Point3(1 * randomVectorBall<3, real>()),
				0.01);

			sphereList.push_back(sphere);
		}

		tree.reserveBound(AlignedBox3(Point3(-10), Point3(10)));
		tree.insert(sphereList.begin(), sphereList.end());

		log() << "The tree has " << logNewLine;
		log() << tree.nodes() << " nodes" << logNewLine;
		log() << "Depth of " << depth(tree) << logNewLine;
		log() << tree.objects() << " object references ("
			<< (real)tree.objects() / tree.leaves() << " per leaf on average)." << logNewLine;

		//refineSlidingMidpoint(computeKdTreeMaxDepth(tree.objects()), 4, tree);
		refineSurfaceAreaHeuristic(computeKdTreeMaxDepth(tree.objects()), 2, tree);
		//refineSurfaceAreaHeuristic(0, 2, tree);

		tree.reserveBound(AlignedBox3(tree.bound().min() - 1, tree.bound().max() + 1));

		REPORT(!check(tree));

		log() << "The tree has " << logNewLine;
		log() << tree.nodes() << " nodes" << logNewLine;
		log() << "Of which " << tree.leaves() << " are leaf nodes." << logNewLine;
		log() << "Depth of " << depth(tree) << logNewLine;
		log() << tree.objects() << " object references ("
			<< (real)tree.objects() / tree.leaves() << " per leaf on average)." << logNewLine;

		std::vector<Color> palette;
		falseColorPalette(palette, 256);

		const integer width = 800;
		const integer height = 800;

		Array<2, Color> image(width, height, Color(0));

		integer reportedProgress = -1;

		for (integer y = 0;y < height;++y)
		{
			const integer progress = 100 * ((real)y / (height - 1));
			if (progress != reportedProgress)
			{
				log() << progress << "% ";
				reportedProgress = progress;
			}
			for (integer x = 0;x < width;++x)
			{
				const Line3 ray(
					Point3(
					2 * ((real)(x + 0.5) / (width - 1)) - 1,
					2 * ((real)(y + 0.5) / (height - 1)) - 1,
					-2),
					Vector3(0, 0, 1));

				const ConstObjectIterator iter =
					recursiveRayTraversal(tree, ray, sphereIntersector);
				if (iter != tree.end())
				{
					image(x, y) = palette[(uint8)((pointer_integer)(*iter) % 256)];
					//image(x, y) = Color(1);
				}
			}
		}

		log() << logNewLine;

		allocator.clear();

		savePcx(image, "spheretracing.pcx");
	}

	void testBegin()
	{
		//testPointConstruction();
		testSphereTracing();
	}

	void testAdd()
	{
		geometryTestList().add("KdTree", testBegin);
	}

	CallFunction run(testAdd);

}
