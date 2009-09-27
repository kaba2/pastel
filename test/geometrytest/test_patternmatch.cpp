#include "pastelgeometrytest.h"

#include "pastel/geometry/pointkdtree_tools.h"
#include "pastel/geometry/point_pattern_matching.h"
#include "pastel/geometry/bounding_alignedbox.h"

#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/draw.h"
#include "pastel/gfx/image_gfxrenderer.h"
#include "pastel/gfx/gfxrenderer_tools.h"
#include "pastel/gfx/color_tools.h"

#include "pastel/device/timer.h"

#include "pastel/math/uniform_sampling.h"

#include "pastel/sys/random.h"
#include "pastel/sys/arrayview.h"
#include "pastel/sys/view_tools.h"
#include "pastel/sys/view_all.h"

#include "pastel/sys/vector_tools.h"

using namespace Pastel;

namespace
{

	void render(
		const std::vector<Vector2>& modelSet,
		const std::vector<Vector2>& sceneSet,
		const std::vector<Vector2>& correctSet,
		const AffineTransformation2 transform,
		const std::string& fileName)
	{
		//const real ratio = (real)4 / 3;

		const integer width = 1000;
		const integer height = 1000;
		//const integer height = 1000 / ratio;

		const integer modelPoints = modelSet.size();

		Array<Color, 2> image(width, height);

		AlignedBox2 viewWindow = 
			boundingAlignedBox<2, real>(2, sceneSet.begin(), sceneSet.end());

		/*
		if (viewWindow.extent().x() < viewWindow.extent().y())
		{
			const real xDelta = viewWindow.extent().y() * ratio - viewWindow.extent().x();
			viewWindow.min().x() -= xDelta / 2;
			viewWindow.max().x() += xDelta / 2;
		}
		else
		{
			const real yDelta = viewWindow.extent().x() / ratio - viewWindow.extent().y();
			viewWindow.min().y() -= yDelta / 2;
			viewWindow.max().y() += yDelta / 2;
		}
		*/

		Image_GfxRenderer<Color> renderer;
		renderer.setImage(&image);
		renderer.setViewWindow(viewWindow);
		renderer.setColor(Color(1));
		renderer.setFilled(true);

		const real radius = 0.01;

		const integer scenePoints = sceneSet.size();

		for (integer i = 0;i < scenePoints;++i)
		{
			drawCircle(renderer, Sphere2(sceneSet[i], radius), 20);
		}

		for (integer i = 0;i < modelPoints;++i)
		{
			const Vector2 correct = correctSet[i];
			const Vector2 transformed = transformPoint(modelSet[i], transform);

			renderer.setFilled(true);
			renderer.setColor(Color(0, 1, 0));
			drawFatSegment(renderer, Segment2(correct, transformed), radius * 0.25, radius * 0.25);

			renderer.setFilled(false);
			renderer.setColor(Color(1, 0, 0));
			drawCircle(renderer, Sphere2(correct, radius), 30);

			renderer.setColor(Color(0, 0, 1));
			drawCircle(renderer, Sphere2(transformed, radius), 30);
		}

		savePcx(image, fileName);
	}

	void testPatternMatch()
	{
		const integer extraPoints = 10;
		const integer missingPoints = 0;
		const integer modelPoints = 30;
		const real noise = 0;

		std::vector<Vector2> sceneSet;

		std::vector<Vector2> modelSet;
		modelSet.reserve(modelPoints);

		AffineTransformation2 transformation =
			similarityTransformation(2 * random<real>(),
			random<real>() * 2 * constantPi<real>(),
			evaluate(randomVectorCube<real, 2>() * 0.1));

		std::vector<Vector2> correctSet;
		correctSet.reserve(modelPoints);

		integer removedPoints = 0;

		for (integer i = 0;i < modelPoints;++i)
		{
			modelSet.push_back(
				Vector2(random<real>(), random<real>()));

			const Vector2 transformedModelPoint =
				transformPoint(modelSet.back(), transformation) +
				randomVectorSphere<real, 2>() * noise;

			correctSet.push_back(transformedModelPoint);

			if (i < modelPoints - missingPoints)
			{
				sceneSet.push_back(transformedModelPoint);
			}
		}

		for (integer i = 0;i < modelPoints;++i)
		{
			const integer j = randomInteger() % modelPoints;
			std::swap(modelSet[i],
				modelSet[j]);
			std::swap(correctSet[i],
				correctSet[j]);
		}

		for (integer i = 0;i < extraPoints;++i)
		{
			sceneSet.push_back(Vector2(2 * randomVector<real, 2>() - 1));
		}

		log() << "Computing kd-trees..." << logNewLine;

		typedef PointKdTree<real, 2> SceneTree;
		typedef SceneTree::ConstObjectIterator SceneIterator;

		typedef PointKdTree<real, 2> ModelTree;
		typedef ModelTree::ConstObjectIterator ModelIterator;

		SceneTree sceneTree;
		sceneTree.insert(sceneSet.begin(), sceneSet.end());

		ModelTree modelTree;
		modelTree.insert(modelSet.begin(), modelSet.end());

		sceneTree.refine(SlidingMidpoint2_SplitRule_PointKdTree());
		modelTree.refine(SlidingMidpoint2_SplitRule_PointKdTree());

		log() << "Computing point pattern match..." << logNewLine;

		Timer timer;
		timer.setStart();

		Tuple<real, 4> parameter;
		const bool success = pointPatternMatch(
			sceneTree, modelTree, 0.7,  0.01, PatternMatch::RelativeDistance,
			parameter);

		timer.store();

		log() << "Matching took " << timer.seconds() << " seconds." << logNewLine;

		if (success)
		{
			log() << "Found the model from the scene!" << logNewLine;
			log() << "Scaling " << parameter[0] << logNewLine;
			log() << "Ccw rotation " << radiansToDegrees<real>(parameter[1]) << logNewLine;
			log() << "Translation " << parameter[2] << ", " << parameter[3] << logNewLine;
		}
		else
		{
			log() << "Failed to find the pattern from the background" << logNewLine;
		}

		AffineTransformation2 matchedTransform = similarityTransformation(parameter);

		render(modelSet, sceneSet, correctSet, matchedTransform, "patternmatch.pcx");
	}

	void testBoxPatternMatch()
	{
		const integer EdgePoints = 0;
		const integer RemovePoints = 0;
		const integer RandomPoints = 20000;
		const real minDistance = (real)1 / 1000;

		std::vector<Vector2> modelSet;

		for (integer i = 0;i < EdgePoints;++i)
		{
			const real x =
				(real)i / (EdgePoints - 1);

			modelSet.push_back(
				Vector2(x, 0));
			modelSet.push_back(
				Vector2(x, 1));

			if (i > 0 && i < EdgePoints - 1)
			{
				const real y = x;

				modelSet.push_back(
					Vector2(0, y));
				modelSet.push_back(
					Vector2(1, y));
			}
		}

		for (integer i = 0;i < RandomPoints;++i)
		{
			modelSet.push_back(randomVector<real, 2>());
		}

		std::random_shuffle(
			modelSet.begin(), modelSet.end());

		const integer modelPoints = modelSet.size();

		const real scaling = random<real>(1, 2);
		const real angle = random<real>(0, constantPi<real>());
		const Vector2 translation = randomVectorSphere<real, 2>() * random<real>(0, 3);

		log() << "Scaling = " << scaling << logNewLine;
		log() << "Angle = " << radiansToDegrees<real>(angle) << " degrees." << logNewLine;
		log() << "Translation = (" << translation.x() << ", " << translation.y() << ")" << logNewLine;

		const AffineTransformation2 transform =
			similarityTransformation(scaling, angle, translation);

		std::vector<Vector2> sceneSet;
		std::vector<Vector2> correctSet;

		for (integer i = 0;i < modelPoints;++i)
		{
			sceneSet.push_back(transformPoint(modelSet[i], transform));
			correctSet.push_back(sceneSet.back());
		}

		for (integer i = 0;i < RemovePoints;++i)
		{
			sceneSet.pop_back();
		}

		Timer timer;
		timer.setStart();

		Tuple<real, 4> parameter;
		const bool success = pointPatternMatch(
			sceneSet.begin(), sceneSet.end(),
			modelSet.begin(), modelSet.end(),
			1,  minDistance, PatternMatch::AbsoluteDistance,
			parameter);

		timer.store();

		log() << "Matching took " << timer.seconds() << " seconds." << logNewLine;

		if (success)
		{
			log() << "Found the model from the scene!" << logNewLine;
			log() << "Scaling " << parameter[0] << logNewLine;
			log() << "Ccw rotation " << radiansToDegrees<real>(parameter[1]) << logNewLine;
			log() << "Translation " << parameter[2] << ", " << parameter[3] << logNewLine;
		}
		else
		{
			log() << "Failed to find the pattern from the background" << logNewLine;
		}

		AffineTransformation2 matchedTransform = similarityTransformation(parameter);

		render(modelSet, sceneSet, correctSet, matchedTransform, "boxpatternmatch.pcx");
	}

	void testBegin()
	{
		//testBoxPatternMatch();
		testPatternMatch();
	}

	void testAdd()
	{
		geometryTestList().add("PatternMatch", testBegin);
	}

	CallFunction run(testAdd);

}
