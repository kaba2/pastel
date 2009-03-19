#include "pastelmathtest.h"

#include "pastel/math/uniformsampling.h"

#include "pastel/geometry/poissondiskpattern.h"

#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/drawing.h"
#include "pastel/gfx/color_tools.h"

#include "pastel/sys/random.h"
#include "pastel/sys/view_all.h"

using namespace Pastel;

namespace
{

	void draw(const std::vector<Vector2>& sample,
		integer x, integer y, const Color& color,
		Array<2, Color>& image)
	{
		const integer samples = sample.size();
		for (integer i = 0;i < samples;++i)
		{
			drawPixel(Point<2, integer>(x + sample[i].x() * 100,
				y + sample[i].y() * 100),
				color, arrayView(image));
		}
	}

	Color myRandomColor()
	{
		return hsvToRgb(randomRgbColor() * Vector3(1, 1, 0.1) + Vector3(0, 0, 0.9));
	}

	void test()
	{
		const integer Width = 600;
		const integer Height = 600;

		Array<2, Color> image(Width, Height);

		std::vector<Vector2> square;

		/*

		for (integer i = 0;i < Samples;++i)
		{
			square.push_back(Vector2(
				randomReal(),
				randomReal()));
		}
		*/

		const integer XGrids = 10;
		const integer YGrids = 10;

		const integer XSamples = 400;
		const integer YSamples = 400;

		const real xDelta = (real)1 / XSamples;
		const real yDelta = (real)1 / YSamples;

		const real xGridDelta = (real)1 / (XGrids - 1);
		const real yGridDelta = (real)1 / (YGrids - 1);

		for (integer y = 0; y < YGrids;++y)
		{
			for (integer x = 0;x < XSamples;++x)
			{
				square.push_back(Vector2(
					x * xDelta, y * yGridDelta));
			}
		}

		for (integer x = 0; x < XGrids;++x)
		{
			for (integer y = 0;y < YSamples;++y)
			{
				square.push_back(Vector2(
					x * xGridDelta, y * yDelta));
			}
		}

		draw(square, 0, 0, myRandomColor(), image);

		const integer Samples = square.size();

		{
			std::vector<Vector2> pointList;
			pointList.reserve(Samples);

			for (integer i = 0;i < Samples;++i)
			{
				pointList.push_back(
					(uniformlySampleBall(square[i]) + 1) / 2);
			}

			draw(pointList, 150, 0, myRandomColor(), image);
		}

		{
			std::vector<Vector2> pointList;
			pointList.reserve(Samples);

			for (integer i = 0;i < Samples;++i)
			{
				pointList.push_back(
					uniformlySampleSimplex(square[i]));
			}
			draw(pointList, 300, 0, myRandomColor(), image);
		}

		{
			std::vector<Vector2> pointList;
			pointList.reserve(Samples);

			for (integer i = 0;i < Samples;++i)
			{
				pointList.push_back(
					(uniformlySampleAnnulus(square[i], 0.5, 1) + 1) / 2);
			}
			draw(pointList, 300, 150, myRandomColor(), image);
		}

		{
			std::vector<Vector2> pointList;
			const integer points = 25;

			for (integer i = 0;i < points;++i)
			{
				pointList.push_back(
					(uniformlySampleSphere(Vector1((real)i / (points - 1))) + 1) / 2);
			}
			draw(pointList, 150, 150, myRandomColor(), image);
		}

		{
			std::vector<Vector2> pointList;
			const integer points = 25;

			for (integer i = 0;i < points;++i)
			{
				pointList.push_back(
					(uniformlySampleHemisphere(Vector1((real)i / (points - 1))) + 1) / 2);
			}
			draw(pointList, 0, 150, myRandomColor(), image);
		}

		{
			std::vector<Vector2> pointList;
			const integer points = 25;

			for (integer i = 0;i < points;++i)
			{
				pointList.push_back(
					(cosineSampleHemisphere(Vector1((real)i / (points - 1))) + 1) / 2);
			}
			draw(pointList, 0, 300, myRandomColor(), image);
		}

		{
			std::vector<Vector2> pointList;
			const integer points = 100;

			for (integer i = 0;i < points;++i)
			{
				pointList.push_back(
					shrink((randomVectorBall<3, real>() + 1) / 2));
			}
			draw(pointList, 150, 300, myRandomColor(), image);
		}

		{
			std::vector<Vector2> pointList;

			for (integer i = 0;i < Samples;++i)
			{
				pointList.push_back(
					shrink((uniformlySampleSphere(square[i]) + 1) / 2));
			}
			draw(pointList, 300, 300, myRandomColor(), image);
		}

		{
			std::vector<Vector2> pointList;

			for (integer i = 0;i < Samples;++i)
			{
				pointList.push_back(
					shrink((uniformlySampleHemisphere(square[i]) + 1) / 2));
			}
			draw(pointList, 0, 450, myRandomColor(), image);
		}

		{
			std::vector<Vector2> pointList;

			for (integer i = 0;i < Samples;++i)
			{
				pointList.push_back(
					shrink((cosineSampleHemisphere(square[i]) + 1) / 2));
			}
			draw(pointList, 150, 450, myRandomColor(), image);
		}

		{
			std::vector<Vector2> pointList;
			const integer points = 100;

			for (integer i = 0;i < points;++i)
			{
				pointList.push_back(
					(randomVectorBall<2, real>() + 1) / 2);
			}
			draw(pointList, 300, 450, myRandomColor(), image);
		}

		savePcx(image, "uniformsampling.pcx");
	}

	class PushBack
	{
	public:
		explicit PushBack(
			std::vector<Vector2>& data)
			: data_(data)
		{
		}

		void operator()(const Point2& that) const
		{
			data_.push_back(asVector(that));
		}

		std::vector<Vector2>& data_;
	};

	void test2()
	{
		const integer Width = 500;
		const integer Height = 250;

		Array<2, Color> image(Width, Height);

		std::vector<Vector2> square;
		PushBack pushBack(square);

		poissonDiskPattern(
			AlignedBox2(0, 0, 1, 1),
			0.05, pushBack);

		draw(square, 0, 0, myRandomColor(), image);

		const integer Samples = square.size();

		{
			std::vector<Vector2> pointList;
			pointList.reserve(Samples);

			for (integer i = 0;i < Samples;++i)
			{
				pointList.push_back(
					(uniformlySampleBall(square[i]) + 1) / 2);
			}

			draw(pointList, 150, 0, myRandomColor(), image);
		}

		{
			std::vector<Vector2> pointList;
			pointList.reserve(Samples);

			for (integer i = 0;i < Samples;++i)
			{
				pointList.push_back(
					uniformlySampleSimplex(square[i]));
			}
			draw(pointList, 300, 0, myRandomColor(), image);
		}

		{
			std::vector<Vector2> pointList;
			pointList.reserve(Samples);

			for (integer i = 0;i < Samples;++i)
			{
				pointList.push_back(
					(uniformlySampleAnnulus(square[i], 0.5, 1) + 1) / 2);
			}
			draw(pointList, 300, 150, myRandomColor(), image);
		}

		savePcx(image, "uniformsampling_distortion.pcx");
	}

	void testBegin()
	{
		test();
		test2();
	}

	void testAdd()
	{
		mathTestList().add("UniformSampling", testBegin);
	}

	CallFunction run(testAdd);

}
