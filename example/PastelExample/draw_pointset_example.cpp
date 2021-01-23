// Description: Draw point-set example

#include "pastel_example.h"

#include "pastel/gfx/drawing.h"
#include "pastel/gfx/color/color_space.h"
#include "pastel/gfx/image_file/pcx.h"
#include "pastel/gfx/image_gfxrenderer.h"

#include <fstream>
#include <iostream>
#include <list>

using namespace Pastel;

namespace
{

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
			test();
		}

		void test()
		{
			std::vector<Vector3> pointSet;
			
			std::ifstream file("pointset.txt");

			Vector3 minBound(infinity<dreal>());
			Vector3 maxBound(-infinity<dreal>());
			while(true)
			{
				Vector3 p;
				file >> p[0];
				file >> p[1];
				file >> p[2];
				if (!file)
				{
					break;
				}

				minBound = min(p, minBound);
				maxBound = max(p, maxBound);

				pointSet.push_back(p);
			}
			
			dreal ratio = (maxBound[1] - minBound[1]) / (maxBound[0] - minBound[0]);

			integer width = 2048;
			integer height = width * ratio;

			Array<Color> image(Vector2i(width, height), Color(1));

			Image_GfxRenderer<Color> renderer;
			renderer.setImage(&image);
			renderer.setViewWindow(AlignedBox2(shrink(minBound), shrink(maxBound)));

			for (integer i = 0;i < pointSet.size();++i)
			{
				Vector2 p = shrink(pointSet[i]);
				dreal z = pointSet[i][2];

				dreal hue = (z - minBound[2]) / (maxBound[2] - minBound[2]) * 0.75;
				renderer.setColor(hsvToRgb(Color(hue, 1, 1)));
				renderer.drawPoint(p);
			}

			savePcx(image, "pointset.pcx");
		}
	};

	void test()
	{
		Test theTest;
		theTest.run();
	}

	void addTest()
	{
		testRunner().add("DrawPointset", test);
	}

	CallFunction run(addTest);

}
