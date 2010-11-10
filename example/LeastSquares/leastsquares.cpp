#include "pastel/device/devicesystem.h"
#include "pastel/device/gfxdevice.h"

#include "pastel/sys/log_all.h"
#include "pastel/sys/vector_tools.h"
#include "pastel/math/uniform_sampling.h"

#include "pastel/math/matrix_tools.h"
#include "pastel/math/affinetransformation_tools.h"
#include "pastel/geometry/pointset_eigen.h"
#include "pastel/math/orthonormal.h"

#include "pastel/gl/glgfxrenderer.h"
#include "pastel/gfx/gfxrenderer_tools.h"

#include "pastel/gfxui/gfx_ui.h"

#include "pastel/geometry/convexhull.h"
#include "pastel/geometry/diameter.h"
#include "pastel/geometry/intersect_alignedbox_plane.h"

#include <iostream>

using namespace Pastel;
using namespace std;

const integer ScreenWidth = 800;
const integer ScreenHeight = 600;

class LeastSquares_Gfx_Ui
	: public Gfx_Ui
{
public:
	LeastSquares_Gfx_Ui();
	~LeastSquares_Gfx_Ui();

private:
	virtual void onRender();
	virtual void onGfxLogic();

	void redrawPointSet(const std::vector<Vector2>& pointSet);
	void applyTransformation(
		const AffineTransformation2& transformation);
	void generatePoints(
		std::vector<Vector2>& pointSet, integer points);

	std::vector<Vector2> sourceSet_;
	std::vector<Vector2> targetSet_;
	std::vector<Vector2> fittedSet_;
	integer points_;
};

LeastSquares_Gfx_Ui::LeastSquares_Gfx_Ui()
	: Gfx_Ui(new GlGfxRenderer())
	, points_(1000)
{
	generatePoints(sourceSet_, points_);
	targetSet_ = sourceSet_;
	fittedSet_ = sourceSet_;
}

LeastSquares_Gfx_Ui::~LeastSquares_Gfx_Ui()
{
	delete &renderer();
}

void LeastSquares_Gfx_Ui::redrawPointSet(const std::vector<Vector2>& pointSet)
{
	const integer points = pointSet.size();
	for (integer i = 0;i < points;++i)
	{
		drawCircle(renderer(), Sphere2(pointSet[i], 0.01), 20);
	}
}

void LeastSquares_Gfx_Ui::onRender()
{
	renderer().setColor(Color(0));
	renderer().clear();

	renderer().setColor(Color(1, 0, 0));
	renderer().setFilled(true);
	redrawPointSet(targetSet_);

	AffineTransformation2 transformation = 
		lsAffine(
		sourceSet_, targetSet_);

	const integer points = sourceSet_.size();
	for (integer i = 0;i < points;++i)
	{
		fittedSet_[i] = transformPoint(sourceSet_[i], transformation);
	}

	renderer().setColor(Color(0, 1, 0));
	renderer().setFilled(false);
	redrawPointSet(fittedSet_);

	for (integer i = 0;i < points;++i)
	{
		drawSegment(renderer(), Segment2(targetSet_[i], fittedSet_[i]));
	}

	renderer().setColor(Color(1));
	const Vector2 meanPoint = pointSetMean(
		forwardRange(targetSet_.begin(), targetSet_.end()), 
		Vector_PointPolicy2());
	const Vector2 maximalVariance = 
		largestEigenVector(
		forwardRange(targetSet_.begin(), targetSet_.end()), 
		Vector_PointPolicy2()) * -0.4;

	renderer().setFilled(true);
	renderer().setColor(Color(1, 0, 0));
	drawFatSegment(renderer(), 
		Segment2(meanPoint - cross(maximalVariance), 
		meanPoint + cross(maximalVariance)), 0.01, 0.01);

	MatrixD eigenVectorSet;
	VectorD eigenValueSet;

	approximateEigenstructure(
		forwardRange(targetSet_.begin(), targetSet_.end()),
		Vector_PointPolicy2(),
		2,
		eigenVectorSet,
		eigenValueSet);

	orthonormalize(eigenVectorSet);

	drawFatSegment(renderer(), 
		Segment2(meanPoint - Vector2(eigenVectorSet[0]), 
		meanPoint + Vector2(eigenVectorSet[0])), 0.01, 0.01);

	drawFatSegment(renderer(), 
		Segment2(meanPoint - Vector2(eigenVectorSet[1]), 
		meanPoint + Vector2(eigenVectorSet[1])), 0.01, 0.01);
	
	AlignedBox2 box(-0.3, -0.5, 0.3, 0.5);
	box += Vector2(0.2);

	renderer().setFilled(false);
	drawBox(renderer(), box);

	const Plane2 plane(meanPoint, maximalVariance);

	integer clipDimension = 0;
	real minBoxMax = 0;
	real maxBoxMin = 0;

	if (intersect(box, plane, clipDimension, minBoxMax, maxBoxMin))
	{
		renderer().setColor(Color(1, 1, 0));
		AlignedBox2 minBox(box);
		minBox.min() += 0.02;
		minBox.max() -= 0.02;
		minBox.max()[clipDimension] = minBoxMax;
		drawBox(renderer(), minBox);

		renderer().setColor(Color(1, 0, 1));
		AlignedBox2 maxBox(box);
		maxBox.min() += 0.04;
		maxBox.max() -= 0.04;
		maxBox.min()[clipDimension] = maxBoxMin;
		drawBox(renderer(), maxBox);
	}

	gfxDevice().swapBuffers();
}

void LeastSquares_Gfx_Ui::applyTransformation(
	const AffineTransformation2& transformation)
{
	const integer points = targetSet_.size();
	for (integer i = 0;i < points;++i)
	{
		targetSet_[i] = transformPoint(targetSet_[i], transformation);
	}
}

void LeastSquares_Gfx_Ui::onGfxLogic()
{
	if (deviceSystem().keyDown(SDLK_j))
	{
		applyTransformation(translation2(Vector2(-0.05, 0)));
	}

	if (deviceSystem().keyDown(SDLK_l))
	{
		applyTransformation(translation2(Vector2(0.05, 0)));
	}

	if (deviceSystem().keyDown(SDLK_i))
	{
		applyTransformation(translation2(Vector2(0, 0.05)));
	}

	if (deviceSystem().keyDown(SDLK_k))
	{
		applyTransformation(translation2(Vector2(0, -0.05)));
	}

	if (deviceSystem().keyDown(SDLK_7))
	{
		applyTransformation(AffineTransformation2(
			Matrix2(1 / 1.1, 0, 0, 1), Vector2(0, 0)));
	}
	if (deviceSystem().keyDown(SDLK_9))
	{
		applyTransformation(AffineTransformation2(
			Matrix2(1.1, 0, 0, 1), Vector2(0, 0)));
	}
	if (deviceSystem().keyDown(SDLK_p))
	{
		const integer points = targetSet_.size();
		for (integer i = 0;i < points;++i)
		{
			targetSet_[i] += randomVectorSphere<real, 2>() * 0.01;
		}
	}
	if (deviceSystem().keyDown(SDLK_u))
	{
		applyTransformation(rotation2<real>(degreesToRadians<real>(5)));
	}
	if (deviceSystem().keyDown(SDLK_o))
	{
		applyTransformation(rotation2<real>(degreesToRadians<real>(-5)));
	}
}

void LeastSquares_Gfx_Ui::generatePoints(
	std::vector<Vector2>& pointSet, integer points)
{
	std::vector<Vector2> result;
	for (integer i = 0;i < points;++i)
	{
		result.push_back(Vector2(2 * random<real>() - 1, 2 * random<real>() - 1));
	}
	result.swap(pointSet);
}

int myMain()
{
	log().addLogger(streamLogger(&std::cout));
	log().addLogger(fileLogger("log.txt"));

	deviceSystem().initialize();
	gfxDevice().initialize(ScreenWidth, ScreenHeight, 32, false);
	deviceSystem().setCaption("Pastel's least squares example");

	LeastSquares_Gfx_Ui leastSquaresUi;
	deviceSystem().setUi(&leastSquaresUi);
	
	deviceSystem().startEventLoop();

	gfxDevice().deInitialize();
	deviceSystem().deInitialize();
	return 0;
}
