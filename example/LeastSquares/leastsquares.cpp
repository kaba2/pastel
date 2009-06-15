#include "pastel/device/devicesystem.h"
#include "pastel/device/gfxdevice.h"

#include "pastel/sys/log_all.h"
#include "pastel/sys/point_tools.h"

#include "pastel/math/matrix_tools.h"
#include "pastel/math/affinetransformation_tools.h"
#include "pastel/sys/random_vector.h"
#include "pastel/math/largest_eigenvector.h"

#include "pastel/gl/glgfxrenderer.h"
#include "pastel/gfx/gfxrenderer_tools.h"

#include "pastel/geometry/convexhull.h"
#include "pastel/geometry/diameter.h"
#include "pastel/geometry/monotonizepolygon.h"
#include "pastel/geometry/diagonal_axis.h"
#include "pastel/geometry/intersect_alignedbox_plane.h"

#include <iostream>

using namespace Pastel;
using namespace std;

const integer ScreenWidth = 640;
const integer ScreenHeight = 480;

GfxRenderer<Color>* renderer__;

std::vector<Point2> sourceSet__;
std::vector<Point2> targetSet__;
std::vector<Point2> fittedSet__;
const integer points__ = 1000;

bool mouseLeftPressed__ = false;

void redraw();

void keyHandler(bool pressed, SDLKey key)
{
	if (pressed)
	{
		if (key == SDLK_ESCAPE)
		{
			deviceSystem().stopEventLoop();
		}
	}
}

void redrawPointSet(const std::vector<Point2>& pointSet)
{
	const integer points = pointSet.size();
	for (integer i = 0;i < points;++i)
	{
		drawCircle(*renderer__, Sphere2(pointSet[i], 0.01), 20);
	}
}

void redraw()
{
	renderer__->setColor(Color(0, 0, 0));
	renderer__->clear();

	renderer__->setColor(Color(1, 0, 0));
	renderer__->setFilled(true);
	redrawPointSet(targetSet__);

	AffineTransformation2 transformation = leastSquaresAffineTransformation(
		sourceSet__, targetSet__);

	const integer points = sourceSet__.size();
	for (integer i = 0;i < points;++i)
	{
		fittedSet__[i] = sourceSet__[i] * transformation;
	}

	renderer__->setColor(Color(0, 1, 0));
	renderer__->setFilled(false);
	redrawPointSet(fittedSet__);

	for (integer i = 0;i < points;++i)
	{
		drawSegment(*renderer__, Segment2(targetSet__[i], fittedSet__[i]));
	}

	renderer__->setColor(Color(1));
	const Point2 meanPoint = mean(targetSet__);
	const Vector2 maximalVariance = largestEigenVector(targetSet__) * -0.4;
	const Vector2 diagonalVariance = 
		diagonalAxis<2, real>(2, maximalDiagonalVariance(targetSet__)) * 0.4;
	/*
	const Vector2 diagonalVariance = 
		diagonalAxis<2, real>(2, nearestDiagonalAxis(maximalVariance)) * 0.4;
	*/
	renderer__->setFilled(true);
	/*
	drawFatSegment(*renderer__, 
		Segment2(meanPoint - cross(diagonalVariance), 
		meanPoint + cross(diagonalVariance)), 0.01, 0.01);
	*/
	renderer__->setColor(Color(1, 0, 0));
	drawFatSegment(*renderer__, 
		Segment2(meanPoint - cross(maximalVariance), 
		meanPoint + cross(maximalVariance)), 0.01, 0.01);

	AlignedBox2 box(-0.3, -0.5, 0.3, 0.5);
	box += Vector2(0.2);

	renderer__->setFilled(false);
	drawBox(*renderer__, box);

	const Plane2 plane(meanPoint, maximalVariance);

	integer clipDimension = 0;
	real minBoxMax = 0;
	real maxBoxMin = 0;

	if (intersect(box, plane, clipDimension, minBoxMax, maxBoxMin))
	{
		renderer__->setColor(Color(1, 1, 0));
		AlignedBox2 minBox(box);
		minBox.min() += 0.02;
		minBox.max() -= 0.02;
		minBox.max()[clipDimension] = minBoxMax;
		drawBox(*renderer__, minBox);

		renderer__->setColor(Color(1, 0, 1));
		AlignedBox2 maxBox(box);
		maxBox.min() += 0.04;
		maxBox.max() -= 0.04;
		maxBox.min()[clipDimension] = maxBoxMin;
		drawBox(*renderer__, maxBox);
	}

	gfxDevice().swapBuffers();
}

void applyTransformation(const AffineTransformation2& transformation)
{
	const integer points = targetSet__.size();
	for (integer i = 0;i < points;++i)
	{
		targetSet__[i] = targetSet__[i] * transformation;
	}
}

void logicHandler()
{
	Integer2 iMouse;
	bool leftButton = false;
	iMouse = deviceSystem().mouse(&leftButton);

	const Vector2 currentMouse = Vector2(
		2 * ((real)iMouse[0] / ScreenWidth) - 1,
		-(2 * ((real)iMouse[1] / ScreenHeight) - 1));

	const Vector2 normMouse(
		(currentMouse + 1) / 2);

	const Point2 worldMouse(
		renderer__->viewWindow().at(normMouse) *
		renderer__->viewTransformation());

	if (deviceSystem().keyDown(SDLK_a))
	{
		applyTransformation(translation2(Vector2(-0.05, 0)));
	}

	if (deviceSystem().keyDown(SDLK_d))
	{
		applyTransformation(translation2(Vector2(0.05, 0)));
	}

	if (deviceSystem().keyDown(SDLK_w))
	{
		applyTransformation(translation2(Vector2(0, 0.05)));
	}

	if (deviceSystem().keyDown(SDLK_s))
	{
		applyTransformation(translation2(Vector2(0, -0.05)));
	}

	if (deviceSystem().keyDown(SDLK_1))
	{
		applyTransformation(AffineTransformation2(
			Matrix2(1 / 1.1, 0, 0, 1), Vector2(0, 0)));
	}
	if (deviceSystem().keyDown(SDLK_3))
	{
		applyTransformation(AffineTransformation2(
			Matrix2(1.1, 0, 0, 1), Vector2(0, 0)));
	}
	if (deviceSystem().keyDown(SDLK_r))
	{
		const integer points = targetSet__.size();
		for (integer i = 0;i < points;++i)
		{
			targetSet__[i] += randomVectorSphere<2, real>() * 0.01;
		}
	}
	if (deviceSystem().keyDown(SDLK_q))
	{
		applyTransformation(rotation2<real>(degreesToRadians<real>(5)));
	}
	if (deviceSystem().keyDown(SDLK_e))
	{
		applyTransformation(rotation2<real>(degreesToRadians<real>(-5)));
	}

	redraw();
}

void generatePoints(std::vector<Point2>& pointSet, integer points)
{
	std::vector<Point2> result;
	for (integer i = 0;i < points;++i)
	{
		result.push_back(Point2(2 * random<real>() - 1, 2 * random<real>() - 1));
	}
	result.swap(pointSet);
}

int myMain()
{
	log().addObserver(streamLogObserver(&std::cout));
	log().addObserver(fileLogObserver("log.txt"));

	deviceSystem().initialize();
	deviceSystem().setKeyHandler(keyHandler);
	deviceSystem().setLogicHandler(logicHandler);

	gfxDevice().initialize(ScreenWidth, ScreenHeight, 32, false);
	deviceSystem().setCaption("Pastel's least squares example");

	renderer__ = new GlGfxRenderer();
	renderer__->setColor(Color(0, 0, 0));
	renderer__->clear();
	gfxDevice().swapBuffers();

	generatePoints(sourceSet__, points__);
	targetSet__ = sourceSet__;
	fittedSet__ = sourceSet__;

	deviceSystem().startEventLoop();

	delete renderer__;

	gfxDevice().deInitialize();
	deviceSystem().deInitialize();
	return 0;
}
