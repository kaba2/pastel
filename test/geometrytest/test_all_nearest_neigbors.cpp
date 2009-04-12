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
		const integer points = 100;
		const integer kNearest = 1;

		log() << "Creating point set with " << points << " points." << logNewLine;
		std::vector<Point<N, Real> > pointSet;
		generateSpherePointSet(points, pointSet);

		Array<2, integer> neighborSet(kNearest, points);

		Timer timer;

		log() << "Computing with a brute force algorithm..." << logNewLine;

		timer.setStart();

		/*
		allNearestNeighborsNaive(
			pointSet,
			kNearest,
			neighborSet);
		*/

		timer.store();

		log() << "Computation took " << timer.seconds() 
			<< " seconds." << logNewLine;

		log() << "Computing with a more complex algorithm..." << logNewLine;

		timer.setStart();

		allNearestNeighborsKdTree(
			pointSet,
			kNearest,
			norm<N, Real>,
			neighborSet);

		timer.store();

		log() << "Computation took " << timer.seconds() 
			<< " seconds." << logNewLine;

		Array<N, Color> image(768, 768);
		ImageGfxRenderer<Color> renderer;
		renderer.setImage(&image);
		renderer.setColor(Color(1));
		renderer.setViewWindow(AlignedBox2(-1, -1, 1, 1));
		renderer.setFilled(false);

		for (integer i = 0;i < points;++i)
		{
			renderer.setColor(Color(1));
			drawCircle(renderer, Sphere2(pointSet[i], 0.02));
			for (integer j = 0;j < kNearest;++j)
			{
				const integer neighbor = neighborSet(i, j);
				if (neighbor >= 0)
				{
					renderer.setColor(hsvToRgb(Color((real)j / kNearest, 1, 1)));
					drawArrow(renderer, Segment2(pointSet[i], pointSet[neighbor]), 0.04 / (j + 1));
				}
			}
		}

		savePcx(image, "all_nearest.pcx");

		log() << "Done." << logNewLine;
	}

	void testAllNearest()
	{
		test<2, float>();
	}

	void testAdd()
	{
		geometryTestList().add("AllNearest", testAllNearest);
	}

	CallFunction run(testAdd);

}
