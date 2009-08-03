#include "pastelgeometrytest.h"

#include "pastel/geometry/search_all_neighbors.h"
#include "pastel/sys/random_vector.h"
#include "pastel/math/normbijection.h"

#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/drawing.h"
#include "pastel/gfx/imagegfxrenderer.h"
#include "pastel/gfx/gfxrenderer_tools.h"
#include "pastel/gfx/color_tools.h"

#include "pastel/sys/countingiterator.h"
#include "pastel/sys/random.h"
#include "pastel/sys/arrayview.h"
#include "pastel/sys/view_tools.h"
#include "pastel/sys/view_all.h"
#include "pastel/sys/vector_tools.h"

#include "pastel/device/timer.h"

using namespace Pastel;

namespace
{

	template <int N, typename Real>
	void generateSpherePointSet(
		integer dimension,
		integer points,
		std::vector<Point<Real, N> >& pointSet)
	{
		ENSURE_OP(points, >=, 0);

		std::vector<Point<Real, N> > result;
		result.reserve(points);
		
		for (integer i = 0;i < points;++i)
		{
			result.push_back(
				asPoint(randomVectorBall<N, Real>(dimension)));
		}

		pointSet.swap(result);
	}

	template <int N, typename Real>
	void test(integer dimension, integer points, integer kNearest)
	{
		log() << dimension << "-D, " << points << " points, " 
			<< kNearest << " neighbors." << logNewLine;

		std::vector<Point<Real, N> > pointSet;
		generateSpherePointSet(dimension, points, pointSet);

		Array<integer, 2> neighborSet(kNearest, points);
		Array<integer, 2> naiveNeighborSet(kNearest, points);

		Timer timer;

		log() << "Brute force..." << logNewLine;

		timer.setStart();

		searchAllNeighborsBruteForce(
			pointSet,
			CountingIterator<integer>(0),
			CountingIterator<integer>(points),
			kNearest,
			infinity<Real>(),
			Euclidean_NormBijection<Real>(),
			naiveNeighborSet);

		timer.store();

		log() << timer.seconds() << " seconds." << logNewLine;

		drawNearest("brute", pointSet, naiveNeighborSet);

		/*
		searchAllNeighborsVaidya(
			pointSet,
			kNearest,
			naiveNeighborSet);

		searchAllNeighbors(
			pointSet,
			kNearest,
			neighborSet);
	
		searchAllNeighborsOwn(
			pointSet,
			kNearest,
			neighborSet);
		*/


		log() << "Kd-tree..." << logNewLine;

		timer.setStart();

		searchAllNeighborsKdTree(
			pointSet,
			CountingIterator<integer>(0),
			CountingIterator<integer>(points),
			0,
			kNearest,
			infinity<Real>(),
			0,
			Euclidean_NormBijection<Real>(),
			16,
			SlidingMidpoint2_SplitRule(),
			&neighborSet);

		timer.store();

		log() << timer.seconds() << " seconds." << logNewLine;

		drawNearest("kdtree", pointSet, neighborSet);

		integer differ = 0;
		for (integer i = 0;i < points;++i)
		{
			for (integer j = 0;j < kNearest;++j)
			{
				if (naiveNeighborSet(j, i) != neighborSet(j, i))
				{
					++differ;
				}
			}
		}

		log() << "Differ = " << differ << logNewLine;

		log() << "Done." << logNewLine;
	}

	template <int N, typename Real>
	typename boost::disable_if_c<(N == 2)>::type
		drawNearest(const std::string& name,
		const std::vector<Point<Real, N> >& pointSet,
		const Array<integer, 2>& neighborSet)
	{
	}

	template <int N, typename Real>
	typename boost::enable_if_c<(N == 2)>::type
		drawNearest(const std::string& name,
		const std::vector<Point<Real, N> >& pointSet,
		const Array<integer, 2>& neighborSet)
	{
		const integer points = neighborSet.height();
		const integer kNearest = neighborSet.width();

		Array<Color, 2> image(768, 768);
		Image_GfxRenderer<Color> renderer;
		renderer.setImage(&image);
		renderer.setColor(Color(1));
		renderer.setViewWindow(AlignedBox2(-1, -1, 1, 1));
		renderer.setFilled(false);

		for (integer i = 0;i < points;++i)
		{
			if (neighborSet(0, i) >= 0 && neighborSet(0, i) < pointSet.size())
			{
				renderer.setColor(Color(1));
			}
			else
			{
				renderer.setColor(Color(0, 1, 0));
			}
			drawCircle(renderer, Sphere2(pointSet[i], 0.02));
			for (integer j = 0;j < kNearest;++j)
			{
				const integer neighbor = neighborSet(j, i);
				if (neighbor >= 0)
				{
					renderer.setColor(hsvToRgb(Color((real)j / kNearest, 1, 1)));
					drawArrow(renderer, Segment2(pointSet[i], pointSet[neighbor]), 0.04 / (j + 1));
				}
			}
		}

		savePcx(image, "test_all_nearest_" + name + ".pcx");
	}

	void testAllNearest()
	{
		//test<8, real>(8, 20000, 1);
		test<Dynamic, real>(8, 20000, 1);
		/*
		test<8, real>(8, 5000, 1);
		test<8, real>(8, 10000, 1);
		test<8, real>(8, 20000, 1);
		test<Dynamic, real>(8, 5000, 1);
		test<Dynamic, real>(8, 10000, 1);
		*/
		//test<8, real>(8, 10000, 1);
		//test<Dynamic, real>(8, 1000, 1);
		//test<8, real>(8, 20000, 1);
		//test<Dynamic, real>(8, 20000, 1);

		/*
		test<Dynamic, real>(20, 5000, 1);

		test<5, real>(5, 10000, 1);
		test<Dynamic, real>(10, 10000, 1);
		test<Dynamic, real>(20, 10000, 1);

		test<5, real>(5, 20000, 1);
		test<Dynamic, real>(10, 20000, 1);
		test<Dynamic, real>(20, 20000, 1);

		test<5, real>(5, 10000, 4);
		test<Dynamic, real>(10, 10000, 4);
		test<Dynamic, real>(20, 10000, 4);

		test<5, real>(5, 10000, 8);
		test<Dynamic, real>(10, 10000, 8);
		test<Dynamic, real>(20, 10000, 8);
		*/
	}

	void testAdd()
	{
		geometryTestList().add("AllNearest", testAllNearest);
	}

	CallFunction run(testAdd);

}
