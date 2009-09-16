#include "pastelgeometrytest.h"

#include "pastel/geometry/kdtree.h"
#include "pastel/geometry/kdtree_tools.h"
#include "pastel/geometry/intersect_line_sphere.h"
#include "pastel/geometry/intersect_line_alignedbox.h"

#include "pastel/sys/random_vector.h"

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
		KdTree<Real, N> kdTree(ofDimension(dimension));

		const integer points = 10000;
		std::vector<Vector<Real, N> > pointList;
		pointList.reserve(points);

		for (integer i = 0;i < points;++i)
		{
			pointList.push_back(randomVectorBall<N, Real>(dimension));
		}

		kdTree.insert(pointList.begin(), pointList.end());

		log() << "The kdTree has " << logNewLine;
		log() << kdTree.nodes() << " nodes" << logNewLine;
		log() << "Depth of " << depth(kdTree) << logNewLine;
		log() << kdTree.objects() << " object references ("
			<< (Real)kdTree.objects() / kdTree.leaves() << " per leaf on average)." << logNewLine;

		kdTree.refine(
			computeKdTreeMaxDepth(kdTree.objects()), 16, Midpoint_SplitRule());

		REPORT(!check(kdTree));

		log() << "The kdTree has " << logNewLine;
		log() << kdTree.nodes() << " nodes" << logNewLine;
		log() << "Of which " << kdTree.leaves() << " are leaf nodes." << logNewLine;
		log() << "Depth of " << depth(kdTree) << logNewLine;
		log() << kdTree.objects() << " object references ("
			<< (Real)kdTree.objects() / kdTree.leaves() << " per leaf on average)." << logNewLine;

		{
			KdTree<Real, Dynamic> bTree(ofDimension(dimension));

			std::vector<Vector<Real, Dynamic> > bPointList;
			bPointList.reserve(points);

			for (integer i = 0;i < points;++i)
			{
				Vector<Real, Dynamic> p(ofDimension(dimension));
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
				computeKdTreeMaxDepth(bTree.objects()), 16, Midpoint_SplitRule());

			REPORT(!check(bTree));

			REPORT(!equivalentKdTree(kdTree, bTree));
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

		Tuple<real, 2> bound(const Sphere3* object, integer axis) const
		{
			return Tuple<real, 2>(
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
		typedef KdTree<real, 3, SpherePolicy> Tree;
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
			new(sphere) Sphere3(Vector3(1 * randomVectorBall<3, real>()),
				0.01);

			sphereList.push_back(sphere);
		}

		kdTree.reserveBound(AlignedBox3(Vector3(-10), Vector3(10)));
		kdTree.insert(sphereList.begin(), sphereList.end());

		log() << "The kdTree has " << logNewLine;
		log() << kdTree.nodes() << " nodes" << logNewLine;
		log() << "Depth of " << depth(kdTree) << logNewLine;
		log() << kdTree.objects() << " object references ("
			<< (real)kdTree.objects() / kdTree.leaves() << " per leaf on average)." << logNewLine;

		//kdTree.refine(computeKdTreeMaxDepth(kdTree.objects()), 16, SlidingMidpoint2_SplitRule_PointKdTree());
		refineSurfaceAreaHeuristic(computeKdTreeMaxDepth(kdTree.objects()), 2, kdTree);
		//refineSurfaceAreaHeuristic(0, 2, kdTree);

		kdTree.reserveBound(AlignedBox3(kdTree.bound().min() - 1, kdTree.bound().max() + 1));

		REPORT(!check(kdTree));

		log() << "The kdTree has " << logNewLine;
		log() << kdTree.nodes() << " nodes" << logNewLine;
		log() << "Of which " << kdTree.leaves() << " are leaf nodes." << logNewLine;
		log() << "Depth of " << depth(kdTree) << logNewLine;
		log() << kdTree.objects() << " object references ("
			<< (real)kdTree.objects() / kdTree.leaves() << " per leaf on average)." << logNewLine;

		std::vector<Color> palette;
		falseColorPalette(palette, 256);

		const integer width = 800;
		const integer height = 800;

		Array<Color, 2> image(width, height, Color(0));

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
					Vector3(
					2 * ((real)(x + 0.5) / (width - 1)) - 1,
					2 * ((real)(y + 0.5) / (height - 1)) - 1,
					-2),
					Vector3(0, 0, 1));

				const ConstObjectIterator iter =
					recursiveRayTraversal(kdTree, ray, sphereIntersector);
				if (iter != kdTree.end())
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
