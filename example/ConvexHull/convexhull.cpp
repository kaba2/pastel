#include "pastel/device/devicesystem.h"
#include "pastel/device/gfxdevice.h"

#include "pastel/sys/log_all.h"
#include "pastel/sys/vector_tools.h"

#include "pastel/math/matrix_tools.h"
#include "pastel/math/affinetransformation_tools.h"

#include "pastel/gl/glgfxrenderer.h"

#include "pastel/gfx/gfxrenderer_tools.h"

#include "pastel/gfxui/gfx_ui.h"

#include "pastel/geometry/convexhull.h"

#include <iostream>

using namespace Pastel;

const integer ScreenWidth = 640;
const integer ScreenHeight = 480;

class ConvexHull_Gfx_Ui
	: public Gfx_Ui
{
public:
	ConvexHull_Gfx_Ui();
	~ConvexHull_Gfx_Ui();

private:
	virtual void onGfxLogic();
	virtual void onKey(bool pressed, SDLKey key);
	virtual void onRender();

	void drawDiagonal(integer i, integer j);
	void redrawPointSet();
	void redrawPolygon();
	void redrawConvexHull();

	std::vector<Vector2> pointSet_;
	std::vector<Vector2> convexHull_;

	bool mouseLeftPressed_;
};

ConvexHull_Gfx_Ui::ConvexHull_Gfx_Ui()
: Gfx_Ui(new GlGfxRenderer())
, pointSet_()
, convexHull_()
, mouseLeftPressed_(false)
{
}

ConvexHull_Gfx_Ui::~ConvexHull_Gfx_Ui()
{
	delete &renderer();
}

void ConvexHull_Gfx_Ui::onKey(bool pressed, SDLKey key)
{
	if (pressed)
	{
		if (key == SDLK_SPACE)
		{
			pointSet_.clear();
			convexHull_.clear();
		}
	}
}

void ConvexHull_Gfx_Ui::drawDiagonal(integer i, integer j)
{
	renderer().setColor(Color(0, 0, 1));
	drawFatSegment(renderer(),
		Segment2(pointSet_[i],
		pointSet_[j]), 0.005, 0.005);
}

void ConvexHull_Gfx_Ui::redrawPointSet()
{
	renderer().setColor(Color(1));
	const integer points = pointSet_.size();
	for (integer i = 0;i < points;++i)
	{
		drawCircle(renderer(), Sphere2(pointSet_[i], 0.01));
	}
}

void ConvexHull_Gfx_Ui::redrawPolygon()
{
	renderer().setColor(Color(1));
	const integer points = pointSet_.size();
	for (integer i = 0;i < points;++i)
	{
		integer iNext = i + 1;
		if (iNext == points)
		{
			iNext = 0;
		}
		drawFatSegment(renderer(), Segment2(pointSet_[i], pointSet_[iNext]), 0.005, 0.005);
	}
}

void ConvexHull_Gfx_Ui::redrawConvexHull()
{
	renderer().setColor(Color(0, 1, 0));
	const integer points = convexHull_.size();
	for (integer i = 0;i < points;++i)
	{
		integer next = i + 1;
		if (next == points)
		{
			next = 0;
		}

		drawFatSegment(renderer(),
			Segment2(convexHull_[i],
			convexHull_[next]), 0.005, 0.005);
	}
}

void ConvexHull_Gfx_Ui::onRender()
{
	renderer().setColor(Color(0));
	renderer().clear();

	//redrawPolygon();
	redrawConvexHull();
	redrawPointSet();

	gfxDevice().swapBuffers();
}

void ConvexHull_Gfx_Ui::onGfxLogic()
{
	Integer2 iMouse;
	bool leftButton = false;
	iMouse = deviceSystem().mouse(&leftButton);

	const Vector2 currentMouse(
		2 * ((real)iMouse[0] / ScreenWidth) - 1,
		-(2 * ((real)iMouse[1] / ScreenHeight) - 1));

	const Vector2 normMouse(
		(currentMouse + 1) / 2);

	const Vector2 worldMouse(
		transformPoint(
		renderer().viewWindow().at(normMouse),
		renderer().viewTransformation()));

	if (!mouseLeftPressed_)
	{
		if (leftButton)
		{
			//cout << worldMouse[0] << ", " << worldMouse[1] << endl;

			pointSet_.push_back(
				Vector2(worldMouse[0], worldMouse[1]));
			convexHullGrahamsScan(
				pointSet_, convexHull_);

			//cout << "Diameter = " << convexPolygonDiameter(convexHull_) << endl;

			mouseLeftPressed_ = true;
		}
	}
	else
	{
		if (!leftButton)
		{
			mouseLeftPressed_ = false;
		}
	}
}

int myMain()
{
	log().addLogger(streamLogger(&std::cout));
	log().addLogger(fileLogger("log.txt"));

	deviceSystem().initialize();
	gfxDevice().initialize(ScreenWidth, ScreenHeight, 32, false);
	deviceSystem().setCaption("Pastel's convex hull example");

	ConvexHull_Gfx_Ui convexHullUi;
	deviceSystem().setUi(&convexHullUi);

	deviceSystem().startEventLoop();

	gfxDevice().deInitialize();
	deviceSystem().deInitialize();
	return 0;
}
