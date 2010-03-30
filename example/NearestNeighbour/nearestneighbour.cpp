#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/pointkdtree_tools.h"
#include "pastel/geometry/distance_point_point.h"
#include "pastel/geometry/intersect_segment_halfspace.h"
#include "pastel/geometry/point_patterns.h"
#include "pastel/geometry/search_depth_first_pointkdtree.h"
#include "pastel/geometry/search_all_neighbors_pointkdtree.h"
#include "pastel/geometry/search_all_neighbors_bruteforce.h"
#include "pastel/geometry/search_nearest_pointkdtree.h"
#include "pastel/geometry/search_range_pointkdtree.h"
#include "pastel/geometry/count_range_pointkdtree.h"
#include "pastel/geometry/alignedbox_tools.h"

#include "pastel/gfx/gfxrenderer_tools.h"
#include "pastel/gfx/color_space.h"

#include "pastel/gfxui/gfx_ui.h"

#include "pastel/device/devicesystem.h"
#include "pastel/device/gfxdevice.h"
#include "pastel/device/timedevice.h"
#include "pastel/device/timer.h"

#include "pastel/math/matrix_tools.h"
#include "pastel/math/affinetransformation_tools.h"

#include "pastel/sys/random.h"
#include "pastel/sys/randomdistribution.h"
#include "pastel/sys/log_all.h"
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

//typedef MinimumVolume_SplitRule_PointKdTree SplitRule;
typedef SlidingMidpoint_SplitRule_PointKdTree SplitRule;
//typedef LongestMedian_SplitRule_PointKdTree SplitRule;
//typedef Midpoint_SplitRule_PointKdTree SplitRule;

const integer ScreenWidth = 800;
const integer ScreenHeight = 600;
const integer LogicFps = 100;
const real SearchRadius = 0.1;
const integer NearestPoints = 15;
const real TranslationSpeed = 0.02;
const real ZoomFactor = 1.05;
const real RotationSpeed = 0.02;
const integer SprayPoints = 50;
const real SprayRadius = 0.05;
const real PointRange = 0.9;

class NearestNeighbor_Gfx_Ui
	: public Gfx_Ui
{
public:
	NearestNeighbor_Gfx_Ui();
	virtual ~NearestNeighbor_Gfx_Ui();

private:
	typedef PointKdTree<real, 2> MyTree;
	typedef std::vector<MyTree::ConstObjectIterator> NearestPointSet;

	virtual void onRender();
	virtual void onKey(bool pressed, SDLKey key);

	void drawKdTree(const MyTree& tree);
	void drawKdTree(
		MyTree::Cursor cursor, 
		const AlignedBox2& bound,
		integer depth = 0,
		integer bucketSize = 1);
	/*
	void drawBspTree(const MyTree& tree);
	void drawBspTree(
		MyTree::Cursor cursor, 
		std::vector<Plane2>& planeSet, 
		const AlignedBox2& treeBound,
		const AlignedBox2& bound,
		integer depth,
		integer bucketSize);
	*/

	void redrawPointSet();
	void redrawNearest();
	void redrawRange();
	void handleKeyboard();
	void onGfxLogic();
	void sprayPoints(
		const Vector2& center, real radius, integer points);
	void erasePoints(const Vector2& center, real radius);
	void computeTree(integer maxDepth);
	void timing();

	MyTree tree_;
	Euclidean_NormBijection<real> normBijection_;

	std::vector<MyTree::ConstObjectIterator> rangePointSet_;
	NearestPointSet nearestPointSet_;

	bool drawTree_;
	bool drawPoints_;
	bool drawNearest_;

	bool mouseLeftPressed_;

	integer nearestPoints_;
	real searchRadius_;
	real scaling_;
	integer treeDrawDepth_;
};

NearestNeighbor_Gfx_Ui::NearestNeighbor_Gfx_Ui()
	: Gfx_Ui(new GlGfxRenderer())
	, tree_()
	, normBijection_()
	, rangePointSet_()
	, nearestPointSet_()
	, drawTree_(true)
	, drawPoints_(true)
	, drawNearest_(true)
	, mouseLeftPressed_(false)
	, nearestPoints_(NearestPoints)
	, searchRadius_(infinity<real>())
	, scaling_(1)
	, treeDrawDepth_(0)
{
	renderer().setColor(Color(0));
	renderer().clear();
	gfxDevice().swapBuffers();

	//generateClusteredPointSet(10000, 2, 10, pointSet_);
	//generateUniformBallPointSet(10000, 2, pointSet_);
	//generateUniformCubePointSet(10000, 2, pointSet_);

	CountedPtr<Clustered_RandomDistribution<real, 2> >
		clusteredDistribution = clusteredRandomDistribution<real, 2>();

	const integer clusters = 10;
	for (integer i = 0;i < clusters;++i)
	{
		Matrix2 rotation;
		setRandomRotation(rotation);

		clusteredDistribution->add(
			translate(
			transform(
			scale(
			gaussianRandomDistribution<real, 2>(), 
			evaluate(randomVector<real, 2>() * 0.05)),
			rotation),
			randomVector<real, 2>()));
	}

	CountedPtr<RandomDistribution<real, 2> >
		randomDistribution = clusteredDistribution;

	/*
	CountedPtr<RandomDistribution<real, 2> >
		randomDistribution = 
		scale(
		gaussianDistribution<2, real>(), 
		randomVector<real, 2>());
	*/

	std::vector<Vector2> pointSet;

	AlignedBox2 bound;

	for (integer i = 0;i < 10000;++i)
	{
		pointSet.push_back(
			randomDistribution->sample());
		extendToCover(pointSet.back(), bound);
	}

	tree_.insert(pointSet.begin(), pointSet.end());

	/*
	generateGaussianPointSet(10000, 2, pointSet_);
	scale(randomVector<real, 2>(), pointSet_);
	randomlyReduceDimensionality(1, pointSet_);
	randomlyRotate(pointSet_);
	*/

	computeTree(1);
	//computeTree(128);
	//timing();

	concentrate(renderer(), bound);
}

NearestNeighbor_Gfx_Ui::~NearestNeighbor_Gfx_Ui()
{
	delete &renderer();
}

void NearestNeighbor_Gfx_Ui::onRender()
{
	renderer().setColor(Color(0));
	renderer().clear();

	if (drawPoints_)
	{
		redrawPointSet();
	}

	if (drawTree_)
	{
		drawKdTree(tree_);
	}
	renderer().setColor(Color(0, 1, 0));
	drawBox(renderer(), tree_.bound());

	redrawRange();
	redrawNearest();

	gfxDevice().swapBuffers();
}

void NearestNeighbor_Gfx_Ui::onKey(bool pressed, SDLKey key)
{
	if (pressed)
	{
		if (key == SDLK_ESCAPE)
		{
			deviceSystem().stopEventLoop();
		}

		if (key == SDLK_z)
		{
			if (check(tree_))
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
			tree_.refine(SplitRule());
		}

		if (key == SDLK_c)
		{
			tree_.clear();
		}

		if (key == SDLK_m)
		{
			tree_.merge();
		}

		if (key == SDLK_x)
		{
			tree_.eraseObjects();
		}

		if (key == SDLK_F5)
		{
			if (searchRadius_ == infinity<real>())
			{
				searchRadius_ = SearchRadius;
			}
			else
			{
				if (nearestPoints_ > NearestPoints)
				{
					nearestPoints_ = NearestPoints;
					log() << "Automatically set nearest point count to " << nearestPoints_ << logNewLine;
					log() << "(Otherwise every point would have matched)" << logNewLine;
				}
				searchRadius_ = infinity<real>();
			}
			log() << "Search radius set to " << searchRadius_ << logNewLine;
		}
		if (key == SDLK_F6)
		{
			if (nearestPoints_ > NearestPoints)
			{
				nearestPoints_ = NearestPoints;
			}
			else
			{
				nearestPoints_ = tree_.objects();
				if (searchRadius_ == infinity<real>())
				{
					searchRadius_ = SearchRadius;
					log() << "Automatically set search radius to " << searchRadius_ << logNewLine;
					log() << "(Otherwise every point would have matched)" << logNewLine;
				}
			}

			log() << "Nearest point count set to " << nearestPoints_ << logNewLine;
		}

		if (key == SDLK_t)
		{
			drawTree_ = !drawTree_;
		}
		if (key == SDLK_p)
		{
			drawPoints_ = !drawPoints_;
		}
		if (key == SDLK_n)
		{
			drawNearest_ = !drawNearest_;
		}
		if (key == SDLK_KP_PLUS)
		{
			++treeDrawDepth_;
		}
		if (key == SDLK_KP_MINUS)
		{
			if (treeDrawDepth_ > 0)
			{
				--treeDrawDepth_;
			}
		}
		if (key == SDLK_0)
		{
			computeTree(64);
		}
	}
}

/*
void NearestNeighbor_Gfx_Ui::drawBspTree(
	MyTree::Cursor cursor, 
	std::vector<Plane2>& planeSet, 
	const AlignedBox2& treeBound,
	const AlignedBox2& bound,
	integer depth,
	integer bucketSize)
{
	if (cursor.objects() <= bucketSize)
	{
		return;
	}
	if (!cursor.leaf() && drawTree_ && !cursor.empty())
	{
		const integer splitAxis = cursor.splitAxis();
		
		Vector2 basisDirection = unitAxis<real, 2>(splitAxis);
		const Vector2* splitDirection = &basisDirection;
		
		const Line2 line(
			*splitDirection * cursor.splitPosition(),
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

			renderer().setColor(Color(0, 1, 0) / std::pow((real)(depth + 1), (real)0.5));
			renderer().setFilled(false);
			drawSegment(renderer(), segment);
		}
	}

	renderer().setColor(Color(1, 0, 1) / std::pow((real)(depth + 1), (real)0.5));
	//drawBox(renderer(), bound);
}

void NearestNeighbor_Gfx_Ui::drawBspTree(const MyTree& tree)
{
	std::vector<Plane2> planeSet;
	drawBspTree(tree.root(), planeSet, tree.bound(), tree.bound(), 0, tree.bucketSize());

	renderer().setColor(Color(0, 1, 0));
	drawBox(renderer(), tree.bound());
}
*/

void NearestNeighbor_Gfx_Ui::drawKdTree(const MyTree& tree)
{
	drawKdTree(tree.root(), tree.bound(), 0);
}

void NearestNeighbor_Gfx_Ui::drawKdTree(
	MyTree::Cursor cursor, 
	const AlignedBox2& bound, 
	integer depth,
	integer bucketSize)
{
	//Color color(hsvToRgb(Color((real32)(depth % 8) / 8, 1, 1)));
	Color color(1);

	if (cursor.leaf() || 
		cursor.objects() <= bucketSize ||
		depth == treeDrawDepth_)
	{
		//renderer().setColor(color / std::pow((real)(depth + 1), (real)0.5));
		renderer().setColor(color);
		renderer().setFilled(false);
		drawBox(renderer(), bound);
	}
	else
	{
		{
			AlignedBox2 nextBound(bound);
			nextBound.min()[cursor.splitAxis()] = 
				cursor.left().min();
			nextBound.max()[cursor.splitAxis()] = 
				cursor.left().max();
			drawKdTree(cursor.left(), nextBound, depth + 1);
		}
		{
			AlignedBox2 nextBound(bound);
			nextBound.min()[cursor.splitAxis()] = 
				cursor.right().min();
			nextBound.max()[cursor.splitAxis()] = 
				cursor.right().max();
			drawKdTree(cursor.right(), nextBound, depth + 1);
		}
		/*
		renderer().setColor(Color(1, 1, 1));
		if (cursor.splitAxis() == 0)
		{
			const real x = cursor.splitPosition();
			const real y1 = std::max(bound.min().y(), (real)-1000);
			const real y2 = std::min(bound.max().y(), (real)1000);

			drawSegment(renderer(),
				Segment2(Vector2(x, y1), Vector2(x, y2)));
		}
		else
		{
			const real y = cursor.splitPosition();
			const real x1 = std::max(bound.min().x(), (real)-1000);
			const real x2 = std::min(bound.max().x(), (real)1000);

			drawSegment(renderer(),
				Segment2(Vector2(x1, y), Vector2(x2, y)));
		}
		*/
	}
}

class PointDrawer
{
public:
	PointDrawer(
		GfxRenderer<Color>& renderer)
		: renderer_(renderer)
	{
	}

	template <typename Iterator>
	void report(
		const Iterator& iter) const
	{
		Iterator next = iter;
		++next;
		report(iter, next, 1);
	}

	template <typename Iterator>
	void report(
		const Iterator& begin,
		const Iterator& end,
		integer count) const
	{
		renderer_.setColor(Color(1, 0, 0));

		Iterator iter(begin);
		while (iter != end)
		{
			renderer_.drawPoint(iter->object());
			++iter;
		}
	}

	GfxRenderer<Color>& renderer_;
};

void NearestNeighbor_Gfx_Ui::redrawPointSet()
{
	PointDrawer pointDrawer(renderer());

	const AlignedBox2 viewBound =
		boundingAlignedBox(
		renderer().viewWindow(),
		renderer().viewTransformation());
	
	searchRangeAlgorithm(
		tree_,
		viewBound,
		pointDrawer);
}

void NearestNeighbor_Gfx_Ui::redrawNearest()
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

	const Vector2 worldMouse(
		transformPoint(
		renderer().viewWindow().at(normMouse),
		renderer().viewTransformation()));

	if (drawNearest_)
	{
		const integer points = nearestPointSet_.size();
		for (integer i = 0;i < points;++i)
		{
			const real alpha = (real)i / points;
			Color color = aColor * (1 - alpha) + bColor * alpha;

			MyTree::ConstObjectIterator dataIter = 
				nearestPointSet_[i];
			renderer().setColor(color);
			renderer().setFilled(false);
			//drawCircle(renderer_, Sphere2(dataIter->key(), 0.01), 20);
			drawSegment(renderer(), Segment2(worldMouse, dataIter->object()));
		}
	}

	renderer().setColor(Color(1));
	renderer().setFilled(false);

	if (searchRadius_ != infinity<real>())
	{
		drawCircle(renderer(), Sphere2(worldMouse, searchRadius_ * scaling_), 20);
	}

	drawCircle(renderer(), Sphere2(worldMouse, SprayRadius * scaling_), 20);
}

void NearestNeighbor_Gfx_Ui::redrawRange()
{
	if (searchRadius_ == infinity<real>() ||
		!drawNearest_)
	{
		return;
	}

	Color color(1, 1, 0);
	renderer().setColor(color);
	renderer().setFilled(false);

	const integer points = rangePointSet_.size();
	for (integer i = 0;i < points;++i)
	{
		MyTree::ConstObjectIterator dataIter(
			rangePointSet_[i]);
		drawCircle(renderer(), Sphere2(dataIter->object(), 0.01 * scaling_),  20);
		//drawSegment(renderer(), Segment2(worldMouse, dataIter->object()));
	}
}

void NearestNeighbor_Gfx_Ui::handleKeyboard()
{
	if (deviceSystem().keyDown(SDLK_r))
	{
		scaling_ /= ZoomFactor;
	}

	if (deviceSystem().keyDown(SDLK_f))
	{
		scaling_ *= ZoomFactor;
	}
}

void NearestNeighbor_Gfx_Ui::onGfxLogic()
{
	handleKeyboard();

	Integer2 iMouse;
	bool leftButton = false;
	bool rightButton = false;
	iMouse = deviceSystem().mouse(&leftButton, &rightButton);

	const Vector2 normMouse = Vector2(
		2 * ((real)iMouse[0] / ScreenWidth) - 1,
		-(2 * ((real)iMouse[1] / ScreenHeight) - 1));

	const Vector2 screenMouse(
		(normMouse + 1) / 2);

	const Vector2 worldMouse = screenPointToWorld(screenMouse);

	if (leftButton)
	{
		sprayPoints(worldMouse, SprayRadius, SprayPoints);
	}

	if (rightButton)
	{
		erasePoints(worldMouse, SprayRadius);
	}

	nearestPointSet_.clear();
	nearestPointSet_.reserve(nearestPoints_);

	searchNearest(
		tree_, 
		worldMouse, 
		nearestPoints_,
		std::back_inserter(nearestPointSet_), 
		NullIterator(),
		normBijection_.toBijection(searchRadius_ * scaling_), 0,
		Always_AcceptPoint<MyTree::ConstObjectIterator>(),
		8,
		normBijection_);

	rangePointSet_.clear();

	if (searchRadius_ != infinity<real>())
	{
		const integer count = countRange(tree_, 
			AlignedBox2(worldMouse - searchRadius_ * scaling_, 
			worldMouse + searchRadius_ * scaling_));
		const integer count2 = countNearest(tree_,
			worldMouse,
			searchRadius_ * scaling_,
			Always_AcceptPoint<MyTree::ConstObjectIterator>(),
			8,
			Maximum_NormBijection<real>());
		searchRange(tree_, 
			AlignedBox2(worldMouse - searchRadius_ * scaling_, 
			worldMouse + searchRadius_ * scaling_),
			std::back_inserter(rangePointSet_));
		ENSURE_OP(count, ==, rangePointSet_.size());
		ENSURE_OP(count, ==, count2);
	}
}

void NearestNeighbor_Gfx_Ui::sprayPoints(
	const Vector2& center, real radius, integer points)
{
	std::vector<Vector2> newPointSet;
	for (integer i = 0;i < points;++i)
	{
		const real randomAngle = random<real>() * 2 * constantPi<real>();
		const real randomRadius = (randomGaussian<real>() / 2) * radius * scaling_;

		if (std::abs(randomRadius) <= radius * scaling_)
		{
			const Vector2 point(
				center + 
				Vector2(
				cos(randomAngle) * randomRadius,
				sin(randomAngle) * randomRadius));

			newPointSet.push_back(point);
		}
	}

	//tree_.insert(newPointSet.begin(), newPointSet.end());

	for (integer i = 0;i < newPointSet.size();++i)
	{
		tree_.insert(newPointSet[i]);
	}
}

void NearestNeighbor_Gfx_Ui::erasePoints(const Vector2& center, real radius)
{
	NearestPointSet nearestSet;
	searchNearest(
		tree_,
		center,
		tree_.objects(),
		std::back_inserter(nearestSet),
		NullIterator(),
		normBijection_.toBijection(radius), 0,
		Always_AcceptPoint<MyTree::ConstObjectIterator>(),
		8,
		normBijection_);

	for (integer i = 0;i < nearestSet.size();i += 16)
	{
		tree_.erase(nearestSet[i]);
	}
}

void NearestNeighbor_Gfx_Ui::computeTree(integer maxDepth)
{
	MyTree newTree;

	Timer timer;

	timer.setStart();

	timer.store();

	cout << "Destruction took " << timer.seconds() << " seconds." << endl;

	timer.setStart();

	log() << "Constructing a new kd-tree with max-depth " << maxDepth << "." << logNewLine;

	newTree.insert(tree_.objectBegin(), tree_.objectEnd());

	ENSURE(check(newTree));

	newTree.refine(SplitRule());

	ENSURE(check(newTree));

	//MyTree copyTree(newTree);
	//newTree.swap(copyTree);

	ENSURE(check(newTree));

	log() << "The constructed kd-tree has " << logNewLine;
	log() << "depth " << depth(newTree) << "." << logNewLine;
	log() << newTree.nodes() << " nodes." << logNewLine;

	timer.store();

	cout << "Construction took " << timer.seconds() << " seconds." << endl;

	newTree.swap(tree_);
}

void NearestNeighbor_Gfx_Ui::timing()
{
	Timer timer;

	timer.setStart();

	MyTree::ConstObjectIterator iter(tree_.begin());
	MyTree::ConstObjectIterator iterEnd(tree_.end());
	while (iter != iterEnd)
	{
		nearestPointSet_.clear();
		nearestPointSet_.reserve(NearestPoints);

		searchNearest(
			tree_, 
			iter->object(),
			NearestPoints, 
			std::back_inserter(nearestPointSet_),
			NullIterator());
		++iter;
	}

	timer.store();

	cout << "Finding " << NearestPoints << " nearest neighbours for "
		<< tree_.objects() << " points took " << timer.seconds() << " seconds." << endl;
}

int myMain()
{
	log().addObserver(streamLogObserver(&std::cout));
	log().addObserver(fileLogObserver("log.txt"));

	deviceSystem().initialize();
	gfxDevice().initialize(ScreenWidth, ScreenHeight, 0, false);
	deviceSystem().setCaption("Nearest neighbors example");

	NearestNeighbor_Gfx_Ui nearestNeighborUi;
	deviceSystem().setUi(&nearestNeighborUi);

	Timer timer;

	const real maxRelativeError = 0;
	const integer k = 4;
	const integer d = 10;
	const integer n = 100000;
	enum
	{
		N = Dynamic
	};
	PointKdTree<real, N, Array_ObjectPolicy_PointKdTree<real> > tree(ofDimension(d));
	std::vector<Vector<real, N> > pointSet;
	pointSet.reserve(n);

	CountedPtr<Clustered_RandomDistribution<real, N> >
			clusteredDistribution = clusteredRandomDistribution<real, N>(d);

	const integer clusters = 100;
	for (integer i = 0;i < clusters;++i)
	{
		clusteredDistribution->add(
			translate(
			scale(
			gaussianRandomDistribution<real, N>(d), 
			evaluate(randomVector<real, N>(d) * 0.05)),
			randomVector<real, N>(d)));
	}

	CountedPtr<RandomDistribution<real, N> >
		randomDistribution = clusteredDistribution;

	for (integer i = 0;i < n;++i)
	{
		//pointSet.push_back(randomGaussianVector<real, N>(d));
		pointSet.push_back(
			randomDistribution->sample());
	}

	std::vector<PointKdTree<real, N, Array_ObjectPolicy_PointKdTree<real> >::ConstObjectIterator> querySet;
	for (integer i = 0;i < n;++i)
	{
		querySet.push_back(tree.insert(pointSet[i].rawBegin()));
	}
	tree.refine(SplitRule(), 1);

	log() << "Bounding search" << logNewLine;

	Euclidean_NormBijection<real> normBijection;
	Array<PointKdTree<real, N, Array_ObjectPolicy_PointKdTree<real> >::ConstObjectIterator> nearestSet(k, n);

	timer.setStart();

	searchAllNeighbors(tree,
		randomAccessRange(querySet.begin(), querySet.end()),
		0, k,
		&nearestSet,
		0, constantRange(infinity<real>(), querySet.size()),
		maxRelativeError,
		normBijection,
		DepthFirst_SearchAlgorithm_PointKdTree());

	timer.store();

	cout << "Finding " << k << " nearest neighbours for "
		<< tree.objects() << " points took " << timer.seconds() << " seconds." << endl;

	/*
	log() << "Brute-force search" << logNewLine;

	Array<integer> bruteSet(k, n);

	timer.setStart();

	searchAllNeighborsBruteForce(
		pointSet,
		countingIterator(0),
		countingIterator(n),
		k,
		infinity<real>(),
		normBijection,
		bruteSet);
	
	timer.store();

	integer fuckedUp = 0;
	for (integer i = 0;i < n;++i)
	{
		for (integer j = 0;j < k;++j)
		{
			const real* brute = pointSet[bruteSet(j, i)].rawBegin();
			const real* nearest = nearestSet(j, i)->object();
			const real bruteDistance = distance2(pointSet[bruteSet(j, i)], pointSet[i]);
			const real nearestDistance = 
				distance2(nearestSet(j, i)->object(), pointSet[i].rawBegin(), d, Euclidean_NormBijection<real>());
			
			if (relativeError<real>(nearestDistance, bruteDistance) > maxRelativeError)
			{
				++fuckedUp;
			}
		}
	}

	log() << "Wrong results = " << fuckedUp << logNewLine;
	cout << "Finding " << k << " nearest neighbours for "
		<< tree.objects() << " points took " << timer.seconds() << " seconds." << endl;
	*/

	deviceSystem().startEventLoop(LogicFps);

	gfxDevice().deInitialize();
	deviceSystem().deInitialize();
	return 0;
}
