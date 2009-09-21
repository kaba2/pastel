#include "pastelmathtest.h"

#include "pastel/math/uniform_distortion.h"

#include "pastel/geometry/poissondiskpattern.h"

#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/draw.h"
#include "pastel/gfx/color_space.h"
#include "pastel/gfx/color_tools.h"

#include "pastel/sys/random.h"
#include "pastel/sys/view_all.h"
#include "pastel/sys/string_tools.h"

using namespace Pastel;

namespace
{

	void draw(const std::vector<Vector2>& sample,
		integer x, integer y, const Color& color,
		Array<Color, 2>& image)
	{
		const integer samples = sample.size();
		for (integer i = 0;i < samples;++i)
		{
			drawPixel(Vector<integer, 2>(x + sample[i].x() * 100,
				y + sample[i].y() * 100),
				color, arrayView(image));
		}
	}

	Color myRandoColor()
	{
		return hsvToRgb(randomRgbColor() * Color(1, 1, 0.1) + Color(0, 0, 0.9));
	}

	void test()
	{
		const integer Width = 600;
		const integer Height = 600;

		Array<Color, 2> image(Width, Height);

		std::vector<Vector2> square;

		/*

		for (integer i = 0;i < Samples;++i)
		{
			square.push_back(Vector2(
				random<real>(),
				random<real>()));
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

		draw(square, 0, 0, myRandoColor(), image);

		const integer Samples = square.size();

		{
			std::vector<Vector2> pointList;
			pointList.reserve(Samples);

			for (integer i = 0;i < Samples;++i)
			{
				pointList.push_back(
					(uniformlySampleBall(square[i]) + 1) / 2);
			}

			draw(pointList, 150, 0, myRandoColor(), image);
		}

		{
			std::vector<Vector2> pointList;
			pointList.reserve(Samples);

			for (integer i = 0;i < Samples;++i)
			{
				pointList.push_back(
					uniformlySampleSimplex(square[i]));
			}
			draw(pointList, 300, 0, myRandoColor(), image);
		}

		{
			std::vector<Vector2> pointList;
			pointList.reserve(Samples);

			for (integer i = 0;i < Samples;++i)
			{
				pointList.push_back(
					(uniformlySampleAnnulus(square[i], 0.5, 1) + 1) / 2);
			}
			draw(pointList, 300, 150, myRandoColor(), image);
		}

		{
			std::vector<Vector2> pointList;
			const integer points = 25;

			for (integer i = 0;i < points;++i)
			{
				pointList.push_back(
					(uniformlySampleSphere(Vector1((real)i / (points - 1))) + 1) / 2);
			}
			draw(pointList, 150, 150, myRandoColor(), image);
		}

		{
			std::vector<Vector2> pointList;
			const integer points = 25;

			for (integer i = 0;i < points;++i)
			{
				pointList.push_back(
					(uniformlySampleHemisphere(Vector1((real)i / (points - 1))) + 1) / 2);
			}
			draw(pointList, 0, 150, myRandoColor(), image);
		}

		{
			std::vector<Vector2> pointList;
			const integer points = 25;

			for (integer i = 0;i < points;++i)
			{
				pointList.push_back(
					(cosineSampleHemisphere(Vector1((real)i / (points - 1))) + 1) / 2);
			}
			draw(pointList, 0, 300, myRandoColor(), image);
		}

		{
			std::vector<Vector2> pointList;
			const integer points = 100;

			for (integer i = 0;i < points;++i)
			{
				pointList.push_back(
					shrink((randomVectorBall<3, real>() + 1) / 2));
			}
			draw(pointList, 150, 300, myRandoColor(), image);
		}

		{
			std::vector<Vector2> pointList;

			for (integer i = 0;i < Samples;++i)
			{
				pointList.push_back(
					shrink((uniformlySampleSphere(square[i]) + 1) / 2));
			}
			draw(pointList, 300, 300, myRandoColor(), image);
		}

		{
			std::vector<Vector2> pointList;

			for (integer i = 0;i < Samples;++i)
			{
				pointList.push_back(
					shrink((uniformlySampleHemisphere(square[i]) + 1) / 2));
			}
			draw(pointList, 0, 450, myRandoColor(), image);
		}

		{
			std::vector<Vector2> pointList;

			for (integer i = 0;i < Samples;++i)
			{
				pointList.push_back(
					shrink((cosineSampleHemisphere(square[i]) + 1) / 2));
			}
			draw(pointList, 150, 450, myRandoColor(), image);
		}

		{
			std::vector<Vector2> pointList;
			const integer points = 100;

			for (integer i = 0;i < points;++i)
			{
				pointList.push_back(
					(randomVectorBall<2, real>() + 1) / 2);
			}
			draw(pointList, 300, 450, myRandoColor(), image);
		}

		savePcx(image, "uniform_distortion.pcx");
	}

	class PushBack
	{
	public:
		explicit PushBack(
			std::vector<Vector2>& data)
			: data_(data)
		{
		}

		void operator()(const Vector2& that) const
		{
			data_.push_back(that);
		}

		std::vector<Vector2>& data_;
	};

	void test2()
	{
		const integer Width = 500;
		const integer Height = 250;

		Array<Color, 2> image(Width, Height);

		std::vector<Vector2> square;
		PushBack pushBack(square);

		poissonDiskPattern(
			AlignedBox2(0, 0, 1, 1),
			0.05, pushBack);

		draw(square, 0, 0, myRandoColor(), image);

		const integer Samples = square.size();

		{
			std::vector<Vector2> pointList;
			pointList.reserve(Samples);

			for (integer i = 0;i < Samples;++i)
			{
				pointList.push_back(
					(uniformlySampleBall(square[i]) + 1) / 2);
			}

			draw(pointList, 150, 0, myRandoColor(), image);
		}

		{
			std::vector<Vector2> pointList;
			pointList.reserve(Samples);

			for (integer i = 0;i < Samples;++i)
			{
				pointList.push_back(
					uniformlySampleSimplex(square[i]));
			}
			draw(pointList, 300, 0, myRandoColor(), image);
		}

		{
			std::vector<Vector2> pointList;
			pointList.reserve(Samples);

			for (integer i = 0;i < Samples;++i)
			{
				pointList.push_back(
					(uniformlySampleAnnulus(square[i], 0.5, 1) + 1) / 2);
			}
			draw(pointList, 300, 150, myRandoColor(), image);
		}

		savePcx(image, "uniform_sampling_distortion.pcx");
	}

	template <int N, typename Real>
	void testDistance()
	{
		const integer Samples = 100000;
		std::vector<Vector<Real, N> > pointList;
		pointList.reserve(Samples);

		const integer Width = 512;
		const integer Height = 512;

		std::vector<integer> histogram(Width, 0);

		for (integer i = 0;i < Samples;++i)
		{
			const Vector<Real, N> sample
				= randomVectorBall<N, Real>();
			const Real distance = 
				norm(sample);
			++histogram[quantizeUnsigned(distance, Width)];
		}

		integer maxBin = 0;
		for (integer i = 0;i < Width;++i)
		{
			if (histogram[i] > maxBin)
			{
				maxBin = histogram[i];
			}
		}

		Array<Color, 2> image(Width, Height, Color(1, 1, 1));
		for (integer x = 0;x < Width;++x)
		{
			const real t = dequantizeUnsigned(x, Width);
			drawVerticalLine(x, 0, 
				dequantizeUnsigned(histogram[x], maxBin + 1) * Height,
				linear(Color(0, 0, 0), Color(0, 0, 0.2), t), 
				arrayView(image));
		}

		savePcx(image, "test_uniform_sampling_distance_" + integerToString(N, 2) + ".pcx");
	}

	void testAdd()
	{
		mathTestList().add("UniformSampling_1", test);
		mathTestList().add("UniformSampling_2", test2);
		mathTestList().add("UniformSampling_Distance_2", testDistance<2, real>);
		mathTestList().add("UniformSampling_Distance_8", testDistance<8, real>);
	}

	CallFunction run(testAdd);

}
