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
				asPoint(randomVectorSphere<N, Real>()));
		}

		pointSet.swap(result);
	}

	template <int N, typename Real>
	void test()
	{
		const integer points = 7500;
		const integer kNearest = 1;

		log() << "Creating point set with " << points << " points." << logNewLine;
		std::vector<Point<N, Real> > pointSet;
		generateSpherePointSet(points, pointSet);

		Matrix<Unbounded, Unbounded, integer> neighborSet;
		neighborSet.setSize(points, kNearest);

		log() << "Starting algorithm..." << logNewLine;

		allNearestNeighbors(
			pointSet,
			kNearest,
			neighborSet);

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
