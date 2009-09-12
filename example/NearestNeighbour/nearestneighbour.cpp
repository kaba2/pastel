#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/pointkdtree_tools.h"
#include "pastel/geometry/distance_point_point.h"
#include "pastel/geometry/intersect_segment_halfspace.h"
#include "pastel/geometry/point_patterns.h"
#include "pastel/geometry/search_nearest_pointkdtree.h"

#include "pastel/gfx/gfxrenderer_tools.h"

#include "pastel/device/devicesystem.h"
#include "pastel/device/gfxdevice.h"
#include "pastel/device/timedevice.h"
#include "pastel/device/timer.h"

#include "pastel/math/matrix_tools.h"
#include "pastel/math/affinetransformation_tools.h"

#include "pastel/sys/random.h"
#include "pastel/sys/randomdistribution.h"
#include "pastel/sys/log_all.h"
#include "pastel/sys/point_tools.h"
#include "pastel/sys/vector_tools.h"
#include "pastel/sys/nulliterator.h"

#include "pastel/gl/glgfxrenderer.h"

#include "pastel/geometry/overlaps_segments.h"

#include <vector>

#include <iostream>
using std::cout;
using std::endl;

#include <set>

using namespace Pastel;

const integer ScreenWidth = 800;
const integer ScreenHeight = 600;
const integer LogicFps = 100;
const real SearchRadius = 0.2;
const integer NearestPoints = 15;
const real TranslationSpeed = 0.02;
const real ZoomFactor = 1.05;
const real RotationSpeed = 0.02;
const integer SprayPoints = 50;
const real SprayRadius = 0.05;
const real PointRange = 0.9;

integer nearestPoints__ = NearestPoints;
real searchRadius__ = SearchRadius;

GlGfxRenderer* renderer__;

typedef PointKdTree<real, 2> MyTree;

MyTree tree__;
Euclidean_NormBijection<real> normBijection__;

typedef std::vector<MyTree::ConstObjectIterator> NearestPointSet;
std::vector<MyTree::ConstObjectIterator> rangePointSet__;

NearestPointSet nearestPointSet__;
real renderTimeAccumulator = 0;
real secondAccumulator = 0;
real lastUpdateTime = 0;
integer fps = 0;

bool drawTree__ = true;
bool drawPoints__ = true;
bool drawNearest__ = true;

bool firstPoint__ = true;
integer firstPointIndex__ = 0;

bool mouseLeftPressed__ = false;

void computeTree(integer maxDepth);
void sprayPoints(const Point2& center, real radius, integer points);
void erasePoints(const Point2& center, real radius);

void keyHandler(bool pressed, SDLKey key)
{
	if (pressed)
	{
		if (key == SDLK_ESCAPE)
		{
			deviceSystem().stopEventLoop();
		}

		if (key == SDLK_z)
		{
			if (check(tree__))
			{
				log() << "Tree checked ok." << logNewLine;
			}
			else
			{
				log() << "Tree had errors!" << logNewLine;
			}
		}

		if (key == SDLK_o)
		{
			tree__.refine(SlidingMidpoint_SplitRule_PointKdTree());
		}

		if (key == SDLK_c)
		{
			tree__.clear();
		}

		if (key == SDLK_m)
		{
			tree__.merge();
		}

		if (key == SDLK_x)
		{
			tree__.eraseObjects();
		}

		if (key == SDLK_F5)
		{
			if (searchRadius__ == infinity<real>())
			{
				searchRadius__ = SearchRadius;
			}
			else
			{
				if (nearestPoints__ > NearestPoints)
				{
					nearestPoints__ = NearestPoints;
					log() << "Automatically set nearest point count to " << nearestPoints__ << logNewLine;
					log() << "(Otherwise every point would have matched)" << logNewLine;
				}
				searchRadius__ = infinity<real>();
			}
			log() << "Search radius set to " << searchRadius__ << logNewLine;
		}
		if (key == SDLK_F6)
		{
			if (nearestPoints__ > NearestPoints)
			{
				nearestPoints__ = NearestPoints;
			}
			else
			{
				nearestPoints__ = tree__.objects();
				if (searchRadius__ == infinity<real>())
				{
					searchRadius__ = SearchRadius;
					log() << "Automatically set search radius to " << searchRadius__ << logNewLine;
					log() << "(Otherwise every point would have matched)" << logNewLine;
				}
			}

			log() << "Nearest point count set to " << nearestPoints__ << logNewLine;
		}

		if (key == SDLK_t)
		{
			drawTree__ = !drawTree__;
		}
		if (key == SDLK_p)
		{
			drawPoints__ = !drawPoints__;
		}
		if (key == SDLK_n)
		{
			drawNearest__ = !drawNearest__;
		}
		if (key == SDLK_0)
		{
			computeTree(24);
		}
		if (key == SDLK_1)
		{
			computeTree(1);
		}
		if (key == SDLK_2)
		{
			computeTree(2);
		}
		if (key == SDLK_3)
		{
			computeTree(3);
		}
		if (key == SDLK_4)
		{
			computeTree(4);
		}
		if (key == SDLK_5)
		{
			computeTree(5);
		}
		if (key == SDLK_6)
		{
			computeTree(6);
		}
		if (key == SDLK_7)
		{
			computeTree(7);
		}
		if (key == SDLK_8)
		{
			computeTree(8);
		}
		if (key == SDLK_9)
		{
			computeTree(9);
		}
	}
}

void drawBspTree(MyTree::Cursor cursor, 
				 std::vector<Plane2>& planeSet, 
				 const AlignedBox2& treeBound,
				 const AlignedBox2& bound,
				 integer depth,
				 integer bucketSize)
{
	if (cursor.isBucket())
	{
		if (!cursor.empty())
		{
			MyTree::Cursor bucket = cursor;

			if (bucket.parent().exists() && !bucket.empty() && 
				bucket.parent().objects() == bucket.objects())
			{
				renderer__->setColor(Color(1, 0, 0) / std::pow((real)(depth + 1), (real)0.5));
				drawSegment(*renderer__, Segment2(bound.min(), bound.max()));
			}
		}

		return;
	}
	if (!cursor.leaf() && drawTree__ && !cursor.empty())
	{
		const integer splitAxis = cursor.splitAxis();
		
		Vector2 basisDirection = unitAxis<real, 2>(splitAxis);
		const Vector2* splitDirection = &basisDirection;
		
		const Line2 line(
			asPoint(*splitDirection * cursor.splitPosition()),
			cross(*splitDirection));
		
		planeSet.push_back(
			Plane2(line.position(), *splitDirection));

		AlignedBox2 rightBound(bound);
		rightBound.min()[splitAxis] = cursor.splitPosition();

		drawBspTree(
			cursor.right(), 
			planeSet,
			treeBound,
			rightBound,
			depth + 1,
			bucketSize);

		planeSet.back().setNormal(-(*splitDirection));

		AlignedBox2 leftBound(bound);
		leftBound.max()[splitAxis] = cursor.splitPosition();

		drawBspTree(
			cursor.left(), 
			planeSet,
			treeBound,
			leftBound,
			depth + 1,
			bucketSize);

		planeSet.pop_back();

		Vector2 hitList;
		if (intersect(line, bound, hitList))
		{
			// Clip the segment against the planeSet.

			Segment2 segment(
				line.at(hitList[0]),
				line.at(hitList[1]));

			const integer planes = planeSet.size();
			for (integer i = 0;i < planes;++i)
			{
				const Plane2& plane = planeSet[i];

				if (intersect(segment, planeSet[i], hitList))
				{
					segment.set(
						segment.at(hitList[0]),
						segment.at(hitList[1]));
				}
				else
				{
					return;
				}
			}

			renderer__->setColor(Color(0, 1, 0) / std::pow((real)(depth + 1), (real)0.5));
			renderer__->setFilled(false);
			drawSegment(*renderer__, segment);
		}
	}

	renderer__->setColor(Color(1, 0, 1) / std::pow((real)(depth + 1), (real)0.5));
	//drawBox(*renderer__, bound);
}

void drawBspTree(const MyTree& tree)
{
	std::vector<Plane2> planeSet;
	drawBspTree(tree.root(), planeSet, tree.bound(), tree.bound(), 0, tree.bucketSize());

	renderer__->setColor(Color(0, 1, 0));
	drawBox(*renderer__, tree.bound());
}

/*
void redrawTree(MyTree::Cursor cursor, const AlignedBox2& bound, integer depth)
{
	if (!cursor.leaf() && drawTree__)
	{
		{
			AlignedBox2 nextBound(bound);
			nextBound.max()[cursor.splitAxis()] = cursor.splitPosition();
			redrawTree(cursor.left(), nextBound, depth + 1);
		}
		{
			AlignedBox2 nextBound(bound);
			nextBound.min()[cursor.splitAxis()] = cursor.splitPosition();
			redrawTree(cursor.right(), nextBound, depth + 1);
		}
	}
	renderer__->setColor(Color(0, 1, 0) / std::pow((real)(depth + 1), (real)0.5));
	renderer__->setFilled(false);
	drawBox(*renderer__, bound);
}
*/

void redrawPointSet()
{
	Color red = Color(1, 0, 0);
	Color yellow = Color(0, 0, 1);

	{
		integer i = 0;
		const integer points = tree__.objects();
		MyTree::ConstObjectDataIterator iter(tree__.objectBegin());
		MyTree::ConstObjectDataIterator iterEnd(tree__.objectEnd());
		while (iter != iterEnd)
		{
			ENSURE_OP(points, >=, 0);
			real alpha = (real)i / points;

			Color color = red * (1 - alpha) + yellow * alpha;

			renderer__->setColor(color);
			renderer__->drawPoint(
				*iter);
			++i;
			++iter;
		}
	}
}

void redrawNearest()
{
	Color aColor = Color(1, 0, 0);
	Color bColor = Color(1, 1, 0);

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

	if (drawNearest__)
	{
		const integer points = nearestPointSet__.size();
		for (integer i = 0;i < points;++i)
		{
			const real alpha = (real)i / points;
			Color color = aColor * (1 - alpha) + bColor * alpha;

			MyTree::ConstObjectIterator dataIter = 
				nearestPointSet__[i];
			renderer__->setColor(color);
			renderer__->setFilled(false);
			//drawCircle(renderer__, Sphere2(dataIter->key(), 0.01), 20);
			drawSegment(*renderer__, Segment2(worldMouse, dataIter->object()));
		}
	}

	renderer__->setColor(Color(1));
	renderer__->setFilled(false);

	if (searchRadius__ != infinity<real>())
	{
		drawCircle(*renderer__, Sphere2(worldMouse, searchRadius__), 20);
	}

	drawCircle(*renderer__, Sphere2(worldMouse, SprayRadius), 20);
}

void redrawRange()
{
	if (searchRadius__ == infinity<real>() ||
		!drawNearest__)
	{
		return;
	}

	const integer points = rangePointSet__.size();
	for (integer i = 0;i < points;++i)
	{
		Color color(1, 1, 0);

		MyTree::ConstObjectIterator dataIter(
			rangePointSet__[i]);
		renderer__->setColor(color);
		renderer__->setFilled(false);
		drawCircle(*renderer__, Sphere2(dataIter->object(), 0.01), 20);
		//drawSegment(*renderer__, Segment2(worldMouse, dataIter->object()));
	}
}

void redraw()
{
	renderer__->setColor(Color(0));
	renderer__->clear();

	if (drawPoints__)
	{
		redrawPointSet();
	}

	drawBspTree(tree__);
	//redrawRange();
	redrawNearest();

	gfxDevice().swapBuffers();
}

void handleKeyboard()
{
	if (deviceSystem().keyDown(SDLK_q))
	{
		const AffineTransformation2 transformation(renderer__->viewTransformation());
		renderer__->setViewTransformation(
			rotation2<real>(RotationSpeed) * transformation);
	}
	if (deviceSystem().keyDown(SDLK_e))
	{
		const AffineTransformation2 transformation(renderer__->viewTransformation());
		renderer__->setViewTransformation(
			rotation2<real>(-RotationSpeed) * transformation);
	}
	if (deviceSystem().keyDown(SDLK_a))
	{
		const AffineTransformation2 transformation(renderer__->viewTransformation());
		renderer__->setViewTransformation(
			transformation *
			translation2<real>(-TranslationSpeed * transformation.transformation()[0]));

	}
	if (deviceSystem().keyDown(SDLK_d))
	{
		const AffineTransformation2 transformation(renderer__->viewTransformation());
		renderer__->setViewTransformation(
			transformation *
			translation2<real>(TranslationSpeed * transformation.transformation()[0]));
	}
	if (deviceSystem().keyDown(SDLK_w))
	{
		const AffineTransformation2 transformation(renderer__->viewTransformation());
		renderer__->setViewTransformation(
			transformation *
			translation2<real>(TranslationSpeed * transformation.transformation()[1]));
	}
	if (deviceSystem().keyDown(SDLK_s))
	{
		const AffineTransformation2 transformation(renderer__->viewTransformation());
		renderer__->setViewTransformation(
			transformation *
			translation2<real>(-TranslationSpeed * transformation.transformation()[1]));
	}
	if (deviceSystem().keyDown(SDLK_r))
	{
		AffineTransformation2 transformation(renderer__->viewTransformation());
		transformation.transformation() /= ZoomFactor;
		renderer__->setViewTransformation(transformation);
	}

	if (deviceSystem().keyDown(SDLK_f))
	{
		AffineTransformation2 transformation(renderer__->viewTransformation());
		transformation.transformation() *= ZoomFactor;
		renderer__->setViewTransformation(transformation);
	}
}

void logicHandler()
{
	handleKeyboard();

	const integer currentTicks =
		timeDevice().ticks();
	const real currentTime =
		(real)currentTicks /
		timeDevice().tickFrequency();

	real deltaTime =
		currentTime - lastUpdateTime;

	lastUpdateTime = currentTime;

	// Bound above by a second.
	if (deltaTime > 1)
	{
		deltaTime = 1;
	}

	renderTimeAccumulator += deltaTime;
	secondAccumulator += deltaTime;

	real keyboardDelay = (real)1 / 100;

	if (secondAccumulator >= 3)
	{
		log() << "Fps: " << fps / secondAccumulator << logNewLine;
		secondAccumulator = 0;
		fps = 0;
	}

	Integer2 iMouse;
	bool leftButton = false;
	bool rightButton = false;
	iMouse = deviceSystem().mouse(&leftButton, &rightButton);

	const Vector2 currentMouse = Vector2(
		2 * ((real)iMouse[0] / ScreenWidth) - 1,
		-(2 * ((real)iMouse[1] / ScreenHeight) - 1));

	const Vector2 normMouse(
		(currentMouse + 1) / 2);

	const Point2 worldMouse(
		renderer__->viewWindow().at(normMouse) *
		renderer__->viewTransformation());

	if (leftButton)
	{
		sprayPoints(worldMouse, SprayRadius, SprayPoints);
	}

	if (rightButton)
	{
		erasePoints(worldMouse, SprayRadius);
	}

	nearestPointSet__.clear();
	nearestPointSet__.reserve(nearestPoints__);

	integer count = countNearest(
		tree__,
		worldMouse, 
		normBijection__.toBijection(searchRadius__),
		normBijection__);

	if (count > 0)
	{
		log() << count << ", ";
	}

	searchNearest(
		tree__, 
		worldMouse, 
		DepthFirst_SearchAlgorithm_PointKdTree(),
		Always_Accept_PointKdTree(), 
		normBijection__.toBijection(searchRadius__), 0,
		normBijection__, nearestPoints__, std::back_inserter(nearestPointSet__), 
		NullIterator());
	if (searchRadius__ != infinity<real>())
	{
		searchRange(tree__, 
			AlignedBox2(worldMouse - searchRadius__, 
			worldMouse + searchRadius__),
			rangePointSet__);
	}

	{
		const integer Fps = 100;
		const real FrameDelay =
			(real)1 / Fps;

		if (renderTimeAccumulator > FrameDelay)
		{
			redraw();

			renderTimeAccumulator -= (real)(
				(integer)(renderTimeAccumulator / FrameDelay) *
				FrameDelay);
			++fps;
		}
	}
}

void sprayPoints(const Point2& center, real radius, integer points)
{
	std::vector<Point2> newPointSet;
	for (integer i = 0;i < points;++i)
	{
		const real randomAngle = random<real>() * 2 * constantPi<real>();
		const real randomRadius = (randomGaussian<real>() / 2) * radius;

		if (std::abs(randomRadius) <= radius)
		{
			const Point2 point(
				center + 
				Vector2(
				cos(randomAngle) * randomRadius,
				sin(randomAngle) * randomRadius));

			newPointSet.push_back(point);
		}
	}

	//tree__.insert(newPointSet.begin(), newPointSet.end());

	for (integer i = 0;i < newPointSet.size();++i)
	{
		tree__.insert(newPointSet[i]);
	}
}

void erasePoints(const Point2& center, real radius)
{
	NearestPointSet nearestSet;
	searchNearest(
		tree__,
		center,
		DepthFirst_SearchAlgorithm_PointKdTree(),
		Always_Accept_PointKdTree(),
		normBijection__.toBijection(radius),
		0, normBijection__,
		tree__.objects(),
		std::back_inserter(nearestSet),
		NullIterator());

	for (integer i = 0;i < nearestSet.size();i += 16)
	{
		tree__.erase(nearestSet[i]);
	}
}

void computeTree(integer maxDepth)
{
	MyTree newTree;

	Timer timer;

	timer.setStart();

	timer.store();

	cout << "Destruction took " << timer.seconds() << " seconds." << endl;

	timer.setStart();

	log() << "Constructing a new kd-tree with max-depth " << maxDepth << "." << logNewLine;

	newTree.insert(tree__.objectBegin(), tree__.objectEnd());

	ENSURE(check(newTree));

	newTree.refine(SlidingMidpoint_SplitRule_PointKdTree(), maxDepth);
	//newTree.refine(Midpoint_SplitRule_PointKdTree(), 2);

	ENSURE(check(newTree));

	MyTree copyTree(newTree);
	newTree.swap(copyTree);

	ENSURE(check(newTree));

	log() << "The constructed kd-tree has " << logNewLine;
	log() << "depth " << depth(newTree) << "." << logNewLine;
	log() << newTree.nodes() << " nodes." << logNewLine;

	timer.store();

	cout << "Construction took " << timer.seconds() << " seconds." << endl;

	newTree.swap(tree__);
}

void timing()
{
	Timer timer;

	timer.setStart();

	MyTree::ConstObjectIterator iter(tree__.begin());
	MyTree::ConstObjectIterator iterEnd(tree__.end());
	while (iter != iterEnd)
	{
		nearestPointSet__.clear();
		nearestPointSet__.reserve(NearestPoints);

		searchNearest(
			tree__, 
			iter->object(),
			DepthFirst_SearchAlgorithm_PointKdTree(),
			Always_Accept_PointKdTree(),
			infinity<real>(),
			0,
			Euclidean_NormBijection<real>(),
			NearestPoints, 
			std::back_inserter(nearestPointSet__),
			NullIterator());
		++iter;
	}

	timer.store();

	cout << "Finding " << NearestPoints << " nearest neighbours for "
		<< tree__.objects() << " points took " << timer.seconds() << " seconds." << endl;
}

int myMain()
{
	log().addObserver(streamLogObserver(&std::cout));
	log().addObserver(fileLogObserver("log.txt"));

	deviceSystem().initialize();
	deviceSystem().setKeyHandler(keyHandler);
	deviceSystem().setLogicHandler(logicHandler);

	gfxDevice().initialize(ScreenWidth, ScreenHeight, 0, false);
	deviceSystem().setCaption("Pastel's nearest neighbours example");

	//generateClusteredPointSet(10000, 2, 10, pointSet__);
	//generateUniformBallPointSet(10000, 2, pointSet__);
	//generateUniformCubePointSet(10000, 2, pointSet__);

	CountedPtr<Clustered_RandomDistribution<2, real> >
		clusteredDistribution = clusteredRandomDistribution<2, real>();

	const integer clusters = 10;
	for (integer i = 0;i < clusters;++i)
	{
		Matrix2 rotation;
		setRandomRotation(rotation);

		clusteredDistribution->add(
			translate(
			transform(
			scale(
			gaussianRandomDistribution<2, real>(), 
			evaluate(randomVector<2, real>() * 0.05)),
			rotation),
			randomVector<2, real>()));
	}

	CountedPtr<RandomDistribution<2, real> >
		randomDistribution = clusteredDistribution;

	/*
	CountedPtr<RandomDistribution<2, real> >
		randomDistribution = 
		scale(
		gaussianDistribution<2, real>(), 
		randomVector<2, real>());
	*/

	std::vector<Point2> pointSet;

	for (integer i = 0;i < 10000;++i)
	{
		pointSet.push_back(
			randomDistribution->sample());
	}

	tree__.insert(pointSet.begin(), pointSet.end());

	/*
	generateGaussianPointSet(10000, 2, pointSet__);
	scale(randomVector<2, real>(), pointSet__);
	randomlyReduceDimensionality(1, pointSet__);
	randomlyRotate(pointSet__);
	*/

	computeTree(24);
	//timing();

	AlignedBox2 viewWindow(tree__.bound());

	const Point2 cameraCenter = 
		linear(viewWindow.min(), viewWindow.max(), 0.5);

	viewWindow -= asVector(cameraCenter);

	viewWindow.min() -= viewWindow.extent() * 0.05;
	viewWindow.max() += viewWindow.extent() * 0.05;
	const Vector2 viewExtent = viewWindow.extent();

	const real aspectRatio = (real)4 / 3;
	if (viewExtent.x() < aspectRatio * viewExtent.y())
	{
		const real xExtentDelta = 
			aspectRatio * viewExtent.y() - viewExtent.x();

		viewWindow.min().x() -= xExtentDelta / 2;
		viewWindow.max().x() += xExtentDelta / 2;
	}
	else
	{
		const real yExtentDelta = 
			viewExtent.x() - aspectRatio * viewExtent.y();

		viewWindow.min().y() -= yExtentDelta / 2;
		viewWindow.max().y() += yExtentDelta / 2;
	}

	renderer__ = new GlGfxRenderer();
	renderer__->setViewTransformation(
		translation2<real>(asVector(cameraCenter)));
	renderer__->setViewWindow(viewWindow);
	renderer__->setColor(Color(0));
	renderer__->clear();
	gfxDevice().swapBuffers();

	deviceSystem().startEventLoop(LogicFps);

	delete renderer__;

	gfxDevice().deInitialize();
	deviceSystem().deInitialize();
	return 0;
}
