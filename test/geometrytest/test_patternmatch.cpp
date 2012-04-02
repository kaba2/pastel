#include "pastelgeometrytest.h"

#include "pastel/geometry/pointkdtree_tools.h"
#include "pastel/geometry/point_pattern_matching.h"
#include "pastel/geometry/point_pattern_match_gmo.h"
#include "pastel/geometry/bounding_alignedbox.h"
#include "pastel/sys/array_pointpolicy.h"

#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/draw.h"
#include "pastel/gfx/image_gfxrenderer.h"
#include "pastel/gfx/gfxrenderer_tools.h"
#include "pastel/gfx/color_tools.h"

#include "pastel/device/timer.h"

#include "pastel/math/uniform_sampling.h"
#include "pastel/math/conformalaffine2d_tools.h"

#include "pastel/sys/random.h"
#include "pastel/sys/arrayview.h"
#include "pastel/sys/view_tools.h"
#include "pastel/sys/views.h"

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

		Array<Color, 2> image(Vector2i(width, height));

		const Sphere2 sceneSphere =
			boundingSphere(
			range(sceneSet.begin(), sceneSet.end()),
			Vector_PointPolicy2());

		const AlignedBox2 viewWindow = 
			boundingAlignedBox(sceneSphere) * 1.5;

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

		// In white filled circles, draw the
		// scene point set.

		renderer.setColor(Color(1));
		renderer.setFilled(true);

		const real radius = 0.005;

		const integer scenePoints = sceneSet.size();
		for (integer i = 0;i < scenePoints;++i)
		{
			drawCircle(renderer, Sphere2(sceneSet[i], radius), 20);
		}

		for (integer i = 0;i < modelPoints;++i)
		{
			const Vector2 correct = correctSet[i];

			// Transform a model point with the found transformation.
			const Vector2 transformed = 
				transformPoint(modelSet[i], transform);

			// Connect the distorted model point with the
			// transformed model point with a green line segment.
			renderer.setFilled(true);
			renderer.setColor(Color(0, 1, 0));
			drawFatSegment(renderer, Segment2(correct, transformed), radius * 0.25, radius * 0.25);

			// With a red hollow circle, draw the distorted model point.
			renderer.setFilled(false);
			renderer.setColor(Color(1, 0, 0));
			drawCircle(renderer, Sphere2(correct, radius), 30);

			// With a blue hollow circle, draw the transformed model point.
			renderer.setColor(Color(0, 0, 1));
			drawCircle(renderer, Sphere2(transformed, radius), 30);
		}

		// With a green hollow circle, draw the bounding sphere for the scene set.
		renderer.setFilled(false);
		renderer.setColor(Color(0, 1, 0));
		drawCircle(renderer, sceneSphere, 30);

		savePcx(image, fileName);
	}

	void testPatternMatch()
	{
		// How many extra random points to generate to 
		// the scene set?
		const integer extraPoints = 0;
		// How many of the model points to remove in
		// the scene set?
		const integer missingPoints = 0;
		// How many points in the model set?
		const integer modelPoints = 30;
		// What percentage of the points must be
		// matched to accept a model match?
		const real minMatchRatio = 0.5;
		// What norm-distance to use as a matching
		// criterion between points?
		const real matchingDistance = 0.1;

		const real noise = 0;
		const real confidence = 1;

		// Storage for scene points.
		std::vector<Vector2> sceneSet;

		// Storage for model points.
		std::vector<Vector2> modelSet;
		modelSet.reserve(modelPoints);

		// Generate a random conformal affine transformation.
		ConformalAffine2 transformation(
			// Scaling in the range [0, 2].
			2 * random<real>(),
			// Rotation in the range [0, 2pi].
			random<real>() * 2 * constantPi<real>(),
			// Translation in the range [0, 0.1]^2.
			evaluate(randomVectorCube<real, 2>() * 0.1));

		// Storage for the distorted model points.
		std::vector<Vector2> correctSet;
		correctSet.reserve(modelPoints);

		// Generate model points.
		for (integer i = 0;i < modelPoints;++i)
		{
			// A random point in the [0, 1]^2 range.
			// Store the model point.
			modelSet.push_back(
				Vector2(random<real>(), random<real>()));

			// Transform the point by the conformal affine
			// transformation and add some noise.
			const Vector2 transformedModelPoint =
				transformPoint(modelSet.back(), transformation) +
				randomVectorSphere<real, 2>() * noise;

			// Store the distorted model point.
			correctSet.push_back(transformedModelPoint);

			if (i < modelPoints - missingPoints)
			{
				// Store the distorted model point into
				// the scene set.
				sceneSet.push_back(transformedModelPoint);
			}
		}

		// Shuffle the model set and the distorted model set
		// so that the pattern matching algorithm can not
		// possibly take advantage of the known order.
		for (integer i = 0;i < modelPoints;++i)
		{
			const integer j = 
				randomInteger() % modelPoints;
			std::swap(modelSet[i],
				modelSet[j]);
			std::swap(correctSet[i],
				correctSet[j]);
		}

		// Generate the extra points into the scene set.
		for (integer i = 0;i < extraPoints;++i)
		{
			sceneSet.push_back(Vector2(
				2 * randomVector<real, 2>() - 1));
		}

		log() << "Computing kd-trees..." << logNewLine;

		// Compute the kd-tree for the scene set.

		typedef PointKdTree<real, 2> SceneTree;
		typedef SceneTree::Point_ConstIterator SceneIterator;

		SceneTree sceneTree;
		sceneTree.insert(
			range(sceneSet.begin(), sceneSet.end()));
		sceneTree.refine(SlidingMidpoint_SplitRule_PointKdTree());

		// Compute the kd-tree for the model set.

		typedef PointKdTree<real, 2> ModelTree;
		typedef ModelTree::Point_ConstIterator ModelIterator;

		ModelTree modelTree;
		modelTree.insert(
			range(modelSet.begin(), modelSet.end()));
		modelTree.refine(SlidingMidpoint_SplitRule_PointKdTree());

		// Compute the pattern matching.

		log() << "Computing point pattern match..." << logNewLine;

		Timer timer;
		timer.setStart();

		ConformalAffine2 similarity;
		const bool success = pointPatternMatch(
			sceneTree, modelTree, 
			minMatchRatio,  matchingDistance,
			confidence,
			similarity);

		timer.store();

		log() << "Matching took " << timer.seconds() << " seconds." << logNewLine;

		if (success)
		{
			log() << "Found the model from the scene!" << logNewLine;
			log() << "Scaling " << similarity.scaling() << logNewLine;
			log() << "Ccw rotation " << radiansToDegrees<real>(similarity.rotation()) << logNewLine;
			log() << "Translation " << similarity.translation()[0] << ", " << similarity.translation()[1] << logNewLine;
		}
		else
		{
			log() << "Failed to find the pattern from the background." << logNewLine;
		}

		AffineTransformation2 matchedTransform = 
			toAffine(similarity);

		// Render the matching to an image file.
		render(modelSet, sceneSet, correctSet, matchedTransform, "patternmatch.pcx");
	}

	void testBoxPatternMatch(
		integer edgePoints, integer randomPoints,
		const std::string& name)
	{
		const integer RemovePoints = 0;
		const real minDistance = (real)1 / 100;

		std::vector<Vector2> modelSet;

		for (integer i = 0;i < edgePoints;++i)
		{
			const real x =
				(real)i / (edgePoints - 1);

			modelSet.push_back(
				Vector2(x, 0));
			modelSet.push_back(
				Vector2(x, 1));

			if (i > 0 && i < edgePoints - 1)
			{
				const real y = x;

				modelSet.push_back(
					Vector2(0, y));
				modelSet.push_back(
					Vector2(1, y));
			}
		}

		for (integer i = 0;i < randomPoints;++i)
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

		const ConformalAffine2 transform(scaling, angle, translation);

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

		ConformalAffine2 similarity;
		const bool success = pointPatternMatch(
			range(sceneSet.begin(), sceneSet.end()),
			range(modelSet.begin(), modelSet.end()),
			1,  minDistance,
			0.99,
			similarity);

		timer.store();

		log() << "Matching took " << timer.seconds() << " seconds." << logNewLine;

		if (success)
		{
			log() << "Found the model from the scene!" << logNewLine;
			log() << "Scaling " << similarity.scaling() << logNewLine;
			log() << "Ccw rotation " << radiansToDegrees<real>(similarity.rotation()) << logNewLine;
			log() << "Translation " << similarity.translation()[0] << ", " << similarity.translation()[1] << logNewLine;
		}
		else
		{
			log() << "Failed to find the pattern from the background" << logNewLine;
		}

		AffineTransformation2 matchedTransform = toAffine(similarity);

		render(modelSet, sceneSet, correctSet, matchedTransform, "patternmatch_" + name + ".pcx");
	}

	void testPekka()
	{
		const real M[] = 
		{
			184,   124,
			236,   127,
			145,   132,
			201,   144,
			197,   146,
			309,   149,
			126,   150,
			240,   150,
			165,   152,
			319,   152,
			322,   159,
			142,   161,
			196,   161,
			302,   161,
			144,   169,
			328,   177,
			304,   178,
			313,   194,
			382,   194,
			158,   199,
			175,   202,
			192,   204,
			380,   208,
			402,   210,
			375,   218,
			254,   223,
			203,   225,
			183,   226,
			193,   227,
			404,   229,
			236,   237,
			403,   244,
			249,   248,
			360,   259,
			412,   270,
			430,   275,
			311,   277,
			335,   278,
			387,   281,
			347,   282,
			405,   283,
			327,   284,
			385,   289,
			394,   290,
			357,   295,
			364,   297,
			208,   569,
			178,   585
		};

		const real S[] = 
		{
			183,   124,
			200,   145,
			126,   152,
			163,   155,
			142,   160,
			183,   162,
			176,   163,
			230,   163,
			142,   170,
			304,   172,
			370,   174,
			302,   178,
			283,   181,
			383,   191,
			315,   196,
			158,   198,
			365,   201,
			175,   203,
			192,   203,
			376,   209,
			382,   210,
			401,   210,
			203,   225,
			192,   226,
			393,   226,
			267,   228,
			404,   230,
			399,   234,
			409,   240,
			248,   248,
			288,   250,
			374,   255,
			282,   263,
			326,   269,
			280,   271,
			311,   274,
			409,   274,
			430,   275,
			385,   281,
			327,   283,
			346,   283,
			412,   283,
			404,   284,
			334,   285,
			385,   290,
			 99,   364,
			 88,   411,
			193,   445,
			223,   497,
			207,   568,
			238,   569,
			380,   611,
			444,   611,
			290,   629,
			388,   633,
			479,   633,
			499,   646,
			289,   668,
			441,   674,
			473,   675
		};

		const integer sPoints = (sizeof(S) / sizeof(real)) / 2;
		const integer mPoints = (sizeof(M) / sizeof(real)) / 2;

		ConformalAffine2 similarity;
		const bool success = pointPatternMatch(
			range(sparseIterator(countingIterator(&M[0]), 2), mPoints),
			range(sparseIterator(countingIterator(&S[0]), 2), sPoints),
			0.6, 0.03, 0.95, similarity,
			Array_PointPolicy2(),
			Array_PointPolicy2());

		if (success)
		{
			log() << "Success." << logNewLine;
		}
		else
		{
			log() << "Fail." << logNewLine;
		}
	}

	void testPekka2()
	{
		const real modelData[] = 
		{
			184,   124,
			236,   127,
			145,   132,
			201,   144,
			197,   146,
			309,   149,
			126,   150,
			240,   150,
			165,   152,
			319,   152,
			322,   159,
			142,   161,
			196,   161,
			302,   161,
			144,   169,
			328,   177,
			304,   178,
			313,   194,
			382,   194,
			158,   199,
			175,   202,
			192,   204,
			380,   208,
			402,   210,
			375,   218,
			254,   223,
			203,   225,
			183,   226,
			193,   227,
			404,   229,
			236,   237,
			403,   244,
			249,   248,
			360,   259,
			412,   270,
			430,   275,
			311,   277,
			335,   278,
			387,   281,
			347,   282,
			405,   283,
			327,   284,
			385,   289,
			394,   290,
			357,   295,
			364,   297,
			208,   569,
			178,   585
		};

		const real sceneData[] = 
		{
			183,   124,
			200,   145,
			126,   152,
			163,   155,
			142,   160,
			183,   162,
			176,   163,
			230,   163,
			142,   170,
			304,   172,
			370,   174,
			302,   178,
			283,   181,
			383,   191,
			315,   196,
			158,   198,
			365,   201,
			175,   203,
			192,   203,
			376,   209,
			382,   210,
			401,   210,
			203,   225,
			192,   226,
			393,   226,
			267,   228,
			404,   230,
			399,   234,
			409,   240,
			248,   248,
			288,   250,
			374,   255,
			282,   263,
			326,   269,
			280,   271,
			311,   274,
			409,   274,
			430,   275,
			385,   281,
			327,   283,
			346,   283,
			412,   283,
			404,   284,
			334,   285,
			385,   290,
			 99,   364,
			 88,   411,
			193,   445,
			223,   497,
			207,   568,
			238,   569,
			380,   611,
			444,   611,
			290,   629,
			388,   633,
			479,   633,
			499,   646,
			289,   668,
			441,   674,
			473,   675
		};

		const integer n = 2;

		const integer scenePoints = (sizeof(sceneData) / sizeof(real)) / n;
		const integer modelPoints = (sizeof(modelData) / sizeof(real)) / n;

		typedef Array_PointPolicy<real> PointPolicy;

		typedef PointKdTree<real, Dynamic, PointPolicy> SceneTree;
		typedef SceneTree::Point_ConstIterator SceneIterator;

		typedef PointKdTree<real, Dynamic, PointPolicy> ModelTree;
		typedef ModelTree::Point_ConstIterator ModelIterator;

		PointPolicy pointPolicy(n);

		SceneTree sceneTree(pointPolicy);
		sceneTree.insert(
			constSparseRange(
			countingIterator(&sceneData[0]),
			countingIterator(&sceneData[0] + scenePoints * n),
			n));

		//constSparseIterator(countingIterator(&sceneData[0]), n), 
		//constSparseIterator(countingIterator(&sceneData[0]), n) + scenePoints);

		ModelTree modelTree(pointPolicy);
		modelTree.insert(
			constSparseRange(
			countingIterator(&modelData[0]),
			countingIterator(&modelData[0] + modelPoints * n),
			n));

		//constSparseIterator(countingIterator(&modelData[0]), n),
		//constSparseIterator(countingIterator(&modelData[0]), n) + modelPoints);

		sceneTree.refine(SlidingMidpoint_SplitRule_PointKdTree());
		modelTree.refine(SlidingMidpoint_SplitRule_PointKdTree());

		// Here's where we store the results.

		std::vector<std::pair<SceneIterator, ModelIterator> > pairSet;
		Vector<real> translation(ofDimension(n));
		real stability = 0;

		// Compute the point pattern match.

		Euclidean_NormBijection<real> normBijection;

		const real minMatchRatio = 0.50;
		const real matchingDistance = 10;
		const real maxBias = 1;

		const bool success = Pastel::pointPatternMatchGmo(
			sceneTree, modelTree, 
			minMatchRatio, matchingDistance, maxBias,
			MatchingMode::MaximumMatch,
			normBijection,
			translation, 
			stability,
			std::back_inserter(pairSet));

		if (success)
		{
			log() << "Success." << logNewLine;
		}
		else
		{
			log() << "Fail." << logNewLine;
		}
	}

	void test()
	{
		testPekka2();
		//testPekka();
		//testBoxPatternMatch(100, 0, "box_edge");
		//testBoxPatternMatch(0, 100, "box_uniform");
		//testPatternMatch();
	}

	void addTest()
	{
		testRunner().add("PatternMatch", test);
	}

	CallFunction run(addTest);

}
