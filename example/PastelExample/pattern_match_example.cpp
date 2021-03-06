// Description: Point-pattern matching example
// Documentation: match_points.txt

#include "pastel_example.h"

#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/pattern_matching/match_points_vw.h"
#include "pastel/geometry/pattern_matching/match_points_kr.h"
#include "pastel/geometry/bounding/bounding_alignedbox.h"
#include "pastel/geometry/bounding/bounding_sphere.h"

#include "pastel/gfx/image_file/pcx/savepcx.h"
#include "pastel/gfx/drawing.h"
#include "pastel/gfx/image_gfxrenderer.h"
#include "pastel/gfx/gfxrenderer_tools.h"
#include "pastel/gfx/color/color_tools.h"

#include "pastel/math/sampling/uniform_sampling.h"
#include "pastel/math/conformalaffine2d/conformalaffine2d_tools.h"

#include "pastel/sys/locator/array_locator.h"
#include "pastel/sys/random.h"
#include "pastel/sys/view/arrayview.h"
#include "pastel/sys/view/view_tools.h"
#include "pastel/sys/view/concrete_views.h"

#include "pastel/sys/vector/vector_tools.h"

using namespace Pastel;

namespace
{
	
	void render(
		const std::vector<Vector2>& modelSet,
		const std::vector<Vector2>& sceneSet,
		const std::vector<Vector2>& correctSet,
		const AffineTransformation<dreal, 2, 2> transform,
		const std::string& fileName)
	{
		//const dreal ratio = (dreal)4 / 3;

		const integer width = 1000;
		const integer height = 1000;
		//const integer height = 1000 / ratio;

		const integer modelPoints = modelSet.size();

		Array<Color, 2> image(Vector2i(width, height));

		const Sphere2 sceneSphere =
			boundingSphere(pointSetRange(sceneSet));

		const AlignedBox2 viewWindow = 
			boundingAlignedBox(sceneSphere) * 1.5;

		/*
		if (viewWindow.extent().x() < viewWindow.extent().y())
		{
			const dreal xDelta = viewWindow.extent().y() * ratio - viewWindow.extent().x();
			viewWindow.min().x() -= xDelta / 2;
			viewWindow.max().x() += xDelta / 2;
		}
		else
		{
			const dreal yDelta = viewWindow.extent().x() / ratio - viewWindow.extent().y();
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

		const dreal radius = 0.005;

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
				transformPoint(transform, modelSet[i]);

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

	class Test
		: public TestSuite
	{
	public:
		Test()
			: TestSuite(&testReport())
		{
		}

		virtual void run()
		{
			testPatternMatch();
			testBoxPatternMatch(100, 0, "box_edge");
			testBoxPatternMatch(0, 100, "box_uniform");
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
			const dreal minMatchRatio = 0.5;
			// What norm-distance to use as a matching
			// criterion between points?
			const dreal matchingDistance = 0.1;

			const dreal noise = 0;
			const dreal confidence = 1;

			// Storage for scene points.
			std::vector<Vector2> sceneSet;

			// Storage for model points.
			std::vector<Vector2> modelSet;
			modelSet.reserve(modelPoints);

			// Generate a random conformal affine transformation.
			ConformalAffine2D<dreal> transformation(
				// Scaling in the range [0, 2].
				2 * random<dreal>(),
				// Rotation in the range [0, 2pi].
				random<dreal>() * 2 * constantPi<dreal>(),
				// Translation in the range [0, 0.1]^2.
				evaluate(randomVectorCube<dreal, 2>() * 0.1));

			// Storage for the distorted model points.
			std::vector<Vector2> correctSet;
			correctSet.reserve(modelPoints);

			// Generate model points.
			for (integer i = 0;i < modelPoints;++i)
			{
				// A random point in the [0, 1]^2 range.
				// Store the model point.
				modelSet.push_back(
					Vector2(random<dreal>(), random<dreal>()));

				// Transform the point by the conformal affine
				// transformation and add some noise.
				const Vector2 transformedModelPoint =
					transformPoint(transformation, modelSet.back()) +
					randomVectorSphere<dreal, 2>() * noise;

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
					randomInteger(modelPoints);
				std::swap(modelSet[i],
					modelSet[j]);
				std::swap(correctSet[i],
					correctSet[j]);
			}

			// Generate the extra points into the scene set.
			for (integer i = 0;i < extraPoints;++i)
			{
				sceneSet.push_back(Vector2(
					2 * randomVector<dreal, 2>() - 1));
			}

			std::cout << "Computing kd-trees..." << std::endl;

			// Compute the kd-tree for the scene set.

			typedef PointKdTree<dreal> SceneTree;
			typedef SceneTree::Point_ConstIterator SceneIterator;

			SceneTree sceneTree;
			sceneTree.insertRange(
				range(sceneSet.begin(), sceneSet.end()));
			sceneTree.refine(SlidingMidpoint_SplitRule());

			// Compute the kd-tree for the model set.

			typedef PointKdTree<dreal> ModelTree;
			typedef ModelTree::Point_ConstIterator ModelIterator;

			ModelTree modelTree;
			modelTree.insertRange(
				range(modelSet.begin(), modelSet.end()));
			modelTree.refine(SlidingMidpoint_SplitRule());

			// Compute the pattern matching.

			std::cout << "Computing point pattern match..." << std::endl;

			ConformalAffine2D<dreal> similarity;
			const bool success = pointPatternMatch(
				sceneTree, modelTree, 
				minMatchRatio,  matchingDistance,
				confidence,
				similarity);

			if (success)
			{
				std::cout << "Found the model from the scene!" << std::endl;
				std::cout << "Scaling " << similarity.scaling() << std::endl;
				std::cout << "Ccw rotation " << radiansToDegrees<dreal>(similarity.rotation()) << std::endl;
				std::cout << "Translation " << similarity.translation()[0] << ", " << similarity.translation()[1] << std::endl;
			}
			else
			{
				std::cout << "Failed to find the pattern from the background." << std::endl;
			}

			AffineTransformation<dreal> matchedTransform = 
				toAffine(similarity);

			// Render the matching to an image file.
			render(modelSet, sceneSet, correctSet, matchedTransform, "patternmatch.pcx");
		}

		void testBoxPatternMatch(
			integer edgePoints, integer randomPoints,
			const std::string& name)
		{
			const integer RemovePoints = 0;
			const dreal minDistance = (dreal)1 / 100;

			std::vector<Vector2> modelSet;

			for (integer i = 0;i < edgePoints;++i)
			{
				const dreal x =
					(dreal)i / (edgePoints - 1);

				modelSet.push_back(
					Vector2(x, 0));
				modelSet.push_back(
					Vector2(x, 1));

				if (i > 0 && i < edgePoints - 1)
				{
					const dreal y = x;

					modelSet.push_back(
						Vector2(0, y));
					modelSet.push_back(
						Vector2(1, y));
				}
			}

			for (integer i = 0;i < randomPoints;++i)
			{
				modelSet.push_back(randomVector<dreal, 2>());
			}

			std::random_shuffle(
				modelSet.begin(), modelSet.end());

			const integer modelPoints = modelSet.size();

			const dreal scaling = random<dreal>(1, 2);
			const dreal angle = random<dreal>(0, constantPi<dreal>());
			const Vector2 translation = randomVectorSphere<dreal, 2>() * random<dreal>(0, 3);

			std::cout << "Scaling = " << scaling << std::endl;
			std::cout << "Angle = " << radiansToDegrees<dreal>(angle) << " degrees." << std::endl;
			std::cout << "Translation = (" << translation.x() << ", " << translation.y() << ")" << std::endl;

			const ConformalAffine2D<dreal> transform(scaling, angle, translation);

			std::vector<Vector2> sceneSet;
			std::vector<Vector2> correctSet;

			for (integer i = 0;i < modelPoints;++i)
			{
				sceneSet.push_back(transformPoint(transform, modelSet[i]));
				correctSet.push_back(sceneSet.back());
			}

			for (integer i = 0;i < RemovePoints;++i)
			{
				sceneSet.pop_back();
			}

			ConformalAffine2D<dreal> similarity;
			const bool success = pointPatternMatch(
				range(sceneSet.begin(), sceneSet.end()),
				range(modelSet.begin(), modelSet.end()),
				1,  minDistance,
				0.99,
				similarity);

			if (success)
			{
				std::cout << "Found the model from the scene!" << std::endl;
				std::cout << "Scaling " << similarity.scaling() << std::endl;
				std::cout << "Ccw rotation " << radiansToDegrees<dreal>(similarity.rotation()) << std::endl;
				std::cout << "Translation " << similarity.translation()[0] << ", " << similarity.translation()[1] << std::endl;
			}
			else
			{
				std::cout << "Failed to find the pattern from the background" << std::endl;
			}

			AffineTransformation<dreal> matchedTransform = toAffine(similarity);

			render(modelSet, sceneSet, correctSet, matchedTransform, "patternmatch_" + name + ".pcx");
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("PatternMatch", test);
	}

	CallFunction run(addTest);

}
