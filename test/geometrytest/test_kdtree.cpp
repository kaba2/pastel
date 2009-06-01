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

	template <int N, typename Real>
	void testNearest2(integer dimension)
	{
		KdTree<N, Real> tree(dimension);

		const integer points = 10000;
		std::vector<Point<N, Real> > pointList;
		pointList.reserve(points);

		for (integer i = 0;i < points;++i)
		{
			pointList.push_back(asPoint(randomVectorBall<N, Real>(dimension)));
		}

		tree.insert(pointList.begin(), pointList.end());

		log() << "The tree has " << logNewLine;
		log() << tree.nodes() << " nodes" << logNewLine;
		log() << "Depth of " << depth(tree) << logNewLine;
		log() << tree.objects() << " object references ("
			<< (Real)tree.objects() / tree.leaves() << " per leaf on average)." << logNewLine;

		tree.refine(
			computeKdTreeMaxDepth(tree.objects()), 4, MidpointRule());

		REPORT(!check(tree));

		log() << "The tree has " << logNewLine;
		log() << tree.nodes() << " nodes" << logNewLine;
		log() << "Of which " << tree.leaves() << " are leaf nodes." << logNewLine;
		log() << "Depth of " << depth(tree) << logNewLine;
		log() << tree.objects() << " object references ("
			<< (Real)tree.objects() / tree.leaves() << " per leaf on average)." << logNewLine;

		{
			KdTree<Dynamic, Real> bTree(dimension);

			std::vector<Point<Dynamic, Real> > bPointList;
			bPointList.reserve(points);

			for (integer i = 0;i < points;++i)
			{
				Point<Dynamic, Real> p(ofDimension(dimension));
				for (integer j = 0;j < dimension;++j)
				{
					p[j] = pointList[i][j];
				}

				bPointList.push_back(p);
				ENSURE(allEqual(bPointList.back(),
					p));
			}

			bTree.insert(bPointList.begin(), bPointList.end());

			bTree.refine(
				computeKdTreeMaxDepth(bTree.objects()), 4, MidpointRule());

			REPORT(!check(bTree));

			REPORT(!equivalentKdTree(tree, bTree));
		}
	}

	void testNearest()
	{
		testNearest2<2, float>(2);
		//testNearest2<Dynamic, float>(10);
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
		Tree tree(3, spherePolicy);
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

		//tree.refine(computeKdTreeMaxDepth(tree.objects()), 4, SlidingMidpointRule());
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

	void testAdd()
	{
		geometryTestList().add("KdTree.RayTracing", testSphereTracing);
		geometryTestList().add("KdTree.Nearest", testNearest);
	}

	CallFunction run(testAdd);

}
