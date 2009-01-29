#include "pastelgfxtest.h"

#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/loadpcx.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/noise.h"
#include "pastel/gfx/mipmap.h"
#include "pastel/gfx/summedareatable.h"
#include "pastel/gfx/imagepdf.h"
#include "pastel/gfx/filter_all.h"

#include "pastel/sys/lineararray.h"
#include "pastel/sys/view_all.h"
#include "pastel/sys/string_tools.h"
#include "pastel/sys/extendedconstview_all.h"

#include "pastel/device/timer.h"

#include <set>
#include <list>
#include <vector>
#include <algorithm>

#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

using namespace boost::lambda;

using namespace Pastel;

namespace
{

	void testTechnique1()
	{
		LinearArray<2, Color> colorImage;
		loadPcx("test_imagepdf_input.pcx", colorImage);

		LinearArray<2, real32> image(colorImage.extent());

		visit(constArrayView(colorImage), arrayView(image), _2 = bind(rgbLuma, _1));

		ImagePdf<2, real32> imagePdf(constArrayView(image));

		clear(0, arrayView(image));

		integer points = 0;

		Timer timer;
		timer.setStart();

		integer stepPoints = 10000;
		for (integer step = 0;step < 4;++step)
		{
			for (;points < stepPoints;++points)
			{
				image(imagePdf()) += 0.2;
			}

			timer.store();

			log() << "Computation took " << timer.seconds() << " seconds." << logNewLine;

			saveGrayscalePcx(image, std::string("test_imagepdf_output_") + integerToString(points) + std::string(".pcx"));

			stepPoints *= 10;
		}
	}

	void testTechnique2()
	{
		LinearArray<2, Color> colorImage;
		loadPcx("test_imagepdf2_input.pcx", colorImage);

		LinearArray<2, real32> image(colorImage.extent());

		visit(constArrayView(colorImage), arrayView(image), _2 = bind(rgbLuma, _1));

		log() << "Computing mipmaps..." << logNewLine;

		MipMap<2, real32> mipMap(constArrayView(image),
			ArrayExtender<2, real32>(), boxFilter());

		const integer images = mipMap.levels();

		for (integer i = 0;i < images;++i)
		{
			visit(arrayView(mipMap(i)), bind(clamp<real32>, _1, 0, 1));
		}

		log() << "Generating points..." << logNewLine;

		if (images > 1)
		{
			image.setExtent(mipMap.extent());
			clear(0, arrayView(image));

			integer points = 0;

			Timer timer;
			timer.setStart();

			integer stepPoints = 10000;
			for (integer step = 0;step < 4;++step)
			{
				for (;points < stepPoints;++points)
				{
					Point<2, integer> position(0, 0);

					for (integer level = images - 2;level >= 0;--level)
					{
						asVector(position) *= 2;
						const LinearArray<2, real32>& mipmap = mipMap(level);

						const real32 sum1 = mipmap(position);
						const real32 sum2 = sum1 + mipmap(position + Vector<2, integer>(1, 0));
						const real32 sum3 = sum2 + mipmap(position + Vector<2, integer>(1, 1));
						const real32 sum4 = sum3 + mipmap(position + Vector<2, integer>(0, 1));
						const real32 value = randomReal() * sum4;

						if (value < sum1)
						{
							// Do nothing.
						}
						else if (value < sum2)
						{
							++position.x();
						}
						else if (value < sum3)
						{
							++position.x();
							++position.y();
						}
						else
						{
							++position.y();
						}
					}

					image(position) += 0.2;
				}

				timer.store();

				log() << "Computation took " << timer.seconds() << " seconds." << logNewLine;
				saveGrayscalePcx(image, std::string("test_imagepdf2_output_") + integerToString(points) + std::string(".pcx"));

				stepPoints *= 10;
			}
		}
	}

	void computeProbabilityTree(
		std::vector<real32>& probabilityTree,
		const LinearArray<2, real32>& summedAreaImage,
		const Rectangle2& wholeRegion)
	{
		probabilityTree.push_back(1);

		std::list<Rectangle2 > regionQueue;

		regionQueue.push_back(wholeRegion);

		while(!regionQueue.empty())
		{
			const Rectangle2 region = regionQueue.front();
			regionQueue.pop_front();

			if (anyGreater(region.extent(), 1))
			{
				const integer axis = maxIndex(region.extent());
				const integer midPoint = (region.max()[axis] + region.min()[axis]) / 2;

				Rectangle2 aRegion = region;
				aRegion.max()[axis] = midPoint;

				Rectangle2 bRegion = region;
				bRegion.min()[axis] = midPoint;

				const real32 aSum =
					summedAreaTable(clampedConstView(arrayView(summedAreaImage)),
					AlignedBox2(aRegion.min(), aRegion.max()));
				const real32 bSum =
					summedAreaTable(clampedConstView(arrayView(summedAreaImage)),
					AlignedBox2(bRegion.min(), bRegion.max()));

				const real32 aProbability = aSum / (aSum + bSum);
				const real32 bProbability = 1 - aProbability;

				probabilityTree.push_back(aProbability);
				regionQueue.push_back(aRegion);

				probabilityTree.push_back(bProbability);
				regionQueue.push_back(bRegion);
			}
		}
	}

	void testTechnique4()
	{
		LinearArray<2, Color> colorImage;
		loadPcx("test_imagepdf4_input.pcx", colorImage);

		LinearArray<2, real32> image(colorImage.extent());

		visit(constArrayView(colorImage), arrayView(image), _2 = bind(rgbLuma, _1));

		log() << "Computing summed area table..." << logNewLine;

		LinearArray<2, real32> summedAreaImage(image.extent());

		const integer width = image.width();
		const integer height = image.height();

		computeSummedAreaTable(
			constArrayView(image), arrayView(summedAreaImage));

		saveGrayscalePcx(summedAreaImage, "test_imagepdf4_summedarea.pcx");

		// Compute a balanced binary tree.

		std::vector<real32> probabilityTree;

		computeProbabilityTree(probabilityTree,
			summedAreaImage,
			Rectangle2(0, 0, width, height));

		log() << "Generating points..." << logNewLine;

		clear(0, arrayView(image));

		integer points = 0;

		Timer timer;
		timer.setStart();

		integer stepPoints = 10000;
		for (integer step = 0;step < 4;++step)
		{
			for (;points < stepPoints;++points)
			{
				integer node = 1;
				const integer nodes = probabilityTree.size();
				Point<2, integer> position(0, 0);
				Vector<2, integer> delta(width, height);

				while(node < nodes)
				{
					const integer axis = maxIndex(delta);
					const real32 aProbability = probabilityTree[node];

					delta[axis] /= 2;

					if (randomReal() < aProbability)
					{
						node = node * 2 + 1;
					}
					else
					{
						node = (node + 1) * 2 + 1;
						position[axis] += delta[axis];
					}
				}

				drawPixel(position, 0.2, arrayView(image), _1 + _2);
			}

			timer.store();

			log() << "Computation took " << timer.seconds() << " seconds." << logNewLine;

			saveGrayscalePcx(image, std::string("test_imagepdf4_output_") + integerToString(points) + std::string(".pcx"));

			stepPoints *= 10;
		}
	}

	void testBegin()
	{
		// 9 s
		// 48 s
		// 176 s
		// 27 s

		testTechnique1();
		//testTechnique2();
		//testTechnique4();
	}

	void testAdd()
	{
		gfxTestList().add("ImagePdf", testBegin);
	}

	CallFunction run(testAdd);

}
