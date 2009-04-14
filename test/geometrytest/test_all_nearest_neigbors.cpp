#include "pastelgeometrytest.h"

#include "pastel/geometry/all_nearest_neighbors.h"
#include "pastel/math/uniformsampling.h"

#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/drawing.h"
#include "pastel/gfx/imagegfxrenderer.h"
#include "pastel/gfx/gfxrenderer_tools.h"
#include "pastel/gfx/color_tools.h"

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
		integer points,
		std::vector<Point<N, Real> >& pointSet)
	{
		ENSURE1(points >= 0, points);

		std::vector<Point<N, Real> > result;
		result.reserve(points);
		
		for (integer i = 0;i < points;++i)
		{
			result.push_back(
				asPoint(randomVectorBall<N, Real>()));
		}

		pointSet.swap(result);
	}

	template <int N, typename Real>
	void test()
	{
		const integer points = 5000;
		const integer kNearest = 1;

		log() << "Creating point set with " << points << " points." << logNewLine;
		std::vector<Point<N, Real> > pointSet;
		generateSpherePointSet(points, pointSet);

		Array<2, integer> neighborSet(kNearest, points);
		Array<2, integer> naiveNeighborSet(kNearest, points);

		Timer timer;

		log() << "Computing with a brute force algorithm..." << logNewLine;

		timer.setStart();

		allNearestNeighborsNaive(
			pointSet,
			kNearest,
			infinity<Real>(),
			naiveNeighborSet);

		/*
		allNearestNeighbors(
			pointSet,
			kNearest,
			naiveNeighborSet);
		*/

		timer.store();

		log() << "Computation took " << timer.seconds() 
			<< " seconds." << logNewLine;

		drawNearest("brute", pointSet, naiveNeighborSet);

		log() << "Computing with a more complex algorithm..." << logNewLine;

		timer.setStart();

		/*
		allNearestNeighbors(
			pointSet,
			kNearest,
			neighborSet);
		*/

		/*
		allNearestNeighborsOwn(
			pointSet,
			kNearest,
			neighborSet);
		*/

		allNearestNeighborsKdTree(
			pointSet,
			kNearest,
			infinity<Real>(),
			norm<N, Real>,
			neighborSet);

		timer.store();

		log() << "Computation took " << timer.seconds() 
			<< " seconds." << logNewLine;

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
		const std::vector<Point<N, Real> >& pointSet,
		const Array<2, integer>& neighborSet)
	{
	}

	template <int N, typename Real>
	typename boost::enable_if_c<(N == 2)>::type
		drawNearest(const std::string& name,
		const std::vector<Point<N, Real> >& pointSet,
		const Array<2, integer>& neighborSet)
	{
		const integer points = neighborSet.height();
		const integer kNearest = neighborSet.width();

		Array<2, Color> image(768, 768);
		ImageGfxRenderer<Color> renderer;
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
		test<12, float>();
	}

	void testAdd()
	{
		geometryTestList().add("AllNearest", testAllNearest);
	}

	CallFunction run(testAdd);

}
