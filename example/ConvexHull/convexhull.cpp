#include "pastel/device/devicesystem.h"
#include "pastel/device/gfxdevice.h"

#include "pastel/sys/log.h"
#include "pastel/sys/streamlogobserver.h"
#include "pastel/sys/filelogobserver.h"

#include "pastel/sys/point_tools.h"
#include "pastel/math/matrix_tools.h"
#include "pastel/math/affinetransformation_tools.h"

#include "pastel/gl/glgfxrenderer.h"
#include "pastel/gfx/gfxrenderer_tools.h"

#include "pastel/geometry/convexhull.h"
#include "pastel/geometry/diameter.h"
#include "pastel/geometry/monotonizepolygon.h"

#include <iostream>

using namespace Pastel;
using namespace std;

const integer ScreenWidth = 640;
const integer ScreenHeight = 480;

GlGfxRenderer* renderer__;

std::vector<Point2> pointSet__;
std::vector<Point2> convexHull__;

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
		if (key == SDLK_SPACE)
		{
			pointSet__.clear();
			convexHull__.clear();
			redraw();
		}
	}
}

void drawDiagonal(integer i, integer j)
{
	renderer__->setColor(Color(0, 0, 1));
	drawFatSegment(*renderer__,
		Segment2(pointSet__[i],
		pointSet__[j]), 0.005, 0.005);
}

void redrawPointSet()
{
	renderer__->setColor(Color(1));
	const integer points = pointSet__.size();
	for (integer i = 0;i < points;++i)
	{
		drawCircle(*renderer__, Sphere2(pointSet__[i], 0.01));
	}
}

void redrawPolygon()
{
	renderer__->setColor(Color(1));
	const integer points = pointSet__.size();
	for (integer i = 0;i < points;++i)
	{
		integer iNext = i + 1;
		if (iNext == points)
		{
			iNext = 0;
		}
		drawFatSegment(*renderer__, Segment2(pointSet__[i], pointSet__[iNext]), 0.005, 0.005);
	}
}

void redrawConvexHull()
{
	renderer__->setColor(Color(0, 1, 0));
	const integer points = convexHull__.size();
	for (integer i = 0;i < points;++i)
	{
		integer next = i + 1;
		if (next == points)
		{
			next = 0;
		}

		drawFatSegment(*renderer__,
			Segment2(convexHull__[i],
			convexHull__[next]), 0.005, 0.005);
	}
}

void redraw()
{
	renderer__->setColor(Color(0));
	renderer__->clear();

	redrawPolygon();
	redrawConvexHull();
	redrawPointSet();

	monotonizePolygon(pointSet__, drawDiagonal);

	gfxDevice().swapBuffers();
}

void logicHandler()
{
	Integer2 iMouse;
	bool leftButton = false;
	iMouse = deviceSystem().mouse(&leftButton);

	const Vector2 currentMouse(
		2 * ((real)iMouse[0] / ScreenWidth) - 1,
		-(2 * ((real)iMouse[1] / ScreenHeight) - 1));

	const Vector2 normMouse(
		(currentMouse + 1) / 2);

	const Point2 worldMouse(
		renderer__->viewWindow().at(normMouse) *
		renderer__->viewTransformation());

	if (!mouseLeftPressed__)
	{
		if (leftButton)
		{
			//cout << worldMouse[0] << ", " << worldMouse[1] << endl;

			pointSet__.push_back(
				Point2(worldMouse[0], worldMouse[1]));
			convexHullGrahamsScan(
				pointSet__, convexHull__);
			redraw();

			//cout << "Diameter = " << convexPolygonDiameter(convexHull__) << endl;

			mouseLeftPressed__ = true;
		}
	}
	else
	{
		if (!leftButton)
		{
			mouseLeftPressed__ = false;
		}
	}
}

int myMain()
{
	log().addObserver(LogObserverPtr(new StreamLogObserver(&cout)));
	log().addObserver(LogObserverPtr(new FileLogObserver("log.txt")));

	deviceSystem().initialize();
	deviceSystem().setKeyHandler(keyHandler);
	deviceSystem().setLogicHandler(logicHandler);

	gfxDevice().initialize(ScreenWidth, ScreenHeight, 32, false);
	deviceSystem().setCaption("Pastel's convex hull example");

	renderer__ = new GlGfxRenderer();
	renderer__->setColor(Color(0));
	renderer__->clear();
	gfxDevice().swapBuffers();

	deviceSystem().startEventLoop();

	delete renderer__;

	gfxDevice().deInitialize();
	deviceSystem().deInitialize();
	return 0;
}
