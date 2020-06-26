// Description: Testing for Anderson-Darling statistic
// DocumentationOf: random_anderson_darling.h

#include "pastel_example.h"

#include "pastel/sys/random_anderson_darling.h"
#include "pastel/sys/random_gaussian.h"
#include "pastel/sys/keyvalue.h"
#include "pastel/sys/array.h"

#include "pastel/math/distance.h"

#include "pastel/geometry/distance_point_point.h"

#include "pastel/gfx/pcx.h"

#include <iostream>
#include <set>

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
			testCorrelation();
		}

		void test()
		{
			const integer m = 100000;
			const integer n = 64;
			std::vector<dreal> input(n);

			/*
			Significance levels, when mean
			and deviation are known.
			Computed with 100000 tests each, using
			a sample size of 64, and data generated
			from a normal distribution with
			mean 0 and deviation 1.
			For the 0.01% signifance we used 1000000 
			tests.

			Significance | Reject normality if above
			-------------|--------------------------
			90%          | 0.35
			80%          | 0.45
			70%          | 0.55
			60%          | 0.7
			50%          | 0.8
			30%          | 1.1
			20%          | 1.4
			15%          | 1.610 (*) 
			10%          | 1.933 (*)
			5%           | 2.492 (*)
			2.5%         | 3.070 (*)
			1%           | 3.857 (*)
			0.5%         | 4.6
			0.1%         | 6.1
			0.01%        | 8.4

			(*) "EDF Statistics for Goodness-of-Fit and Some Comparisons",
			M.A. Stephens
			*/

			dreal minThreshold = 1.0;
			dreal maxThreshold = 2.0;
			//dreal minThreshold = 0.631;
			//dreal maxThreshold = 0.631;
			integer thresholds = (maxThreshold - minThreshold) * 20;
			if (thresholds == 0)
			{
				++thresholds;
			}

			for (integer k = 0;k < thresholds;++k)
			{
				const dreal threshold = 
					linear(minThreshold, maxThreshold, (dreal)k / thresholds);
				integer negatives = 0;
				dreal mean = 0;
				dreal deviation = 1;
				//dreal populationMean = nan<dreal>();
				dreal populationDeviation = nan<dreal>();
				dreal populationMean = 0;
				//dreal populationDeviation = 1;
				for (integer j = 0;j < m;++j)
				{
					for (integer i = 0;i < n;++i)
					{
						input[i] = mean + randomGaussian<dreal>() * deviation;
						//input[i] = mean + (2 * random<dreal>() - 1) * std::sqrt((dreal)3) * deviation;

						/*
						input[i] = randomGaussian<dreal>();
						if (i > n / 4)
						{
							input[i] += 2;
						}
						*/
					}
					/*
					dreal sum = 0;
					dreal squareSum = 0;
					for (integer i = 0;i < n;++i)
					{
						sum += input[i];
						squareSum += square(input[i]);
					}
					std::cout 
						<< "mean " << sum / n 
						<< ", stdev = " << std::sqrt((squareSum / n) - square(sum / n))
						<< std::endl;
					*/

					const dreal t = gaussianAndersonDarling<dreal>(
						range(input.begin(), input.end()), 
						populationMean, populationDeviation);
					if (t > threshold)
					{
						++negatives;
					}	
				}

				const dreal negativePercent =
					(dreal)(100 * negatives) / m;

				std::cout << threshold << ": " << negativePercent << "% negatives." << std::endl;

				TEST_ENSURE_OP(negativePercent, <, 10);
			}
		}

		void testCorrelation()
		{
			const integer k = 1024;
			const integer w = 512;
			const integer n = k + w - 1;
			
			std::vector<dreal> dataSet;
			dataSet.reserve(n);
			for (integer i = 0;i < n;++i)
			{
				dataSet.push_back(randomGaussian<dreal>());
			}

			typedef std::set<KeyValue<dreal, integer> > NearestSet;

			NearestSet aNearestSet;
			NearestSet bNearestSet;

			std::vector<dreal> differenceSet(w, 0);
			for (integer i = 0;i < n - w + 1;++i)
			{
				for (integer j = 0;j < w;++j)
				{
					differenceSet[j] = dataSet[i + j] - dataSet[j];
				}

				const dreal ad =
					gaussianAndersonDarling<dreal>(
					range(differenceSet.begin(), differenceSet.end()),
					0, 1);

				aNearestSet.insert(
					keyValue(ad, i));
				if (aNearestSet.size() > k)
				{
					NearestSet::iterator aLast = aNearestSet.end();
					--aLast;
					aNearestSet.erase(aLast);
				}

				const dreal distance =
					distance2(differenceSet.begin(), differenceSet.begin() + i,
					w, Euclidean_Distance<dreal>());

				bNearestSet.insert(
					keyValue(distance, i));
				if (bNearestSet.size() > k)
				{
					NearestSet::iterator bLast = bNearestSet.end();
					--bLast;
					bNearestSet.erase(bLast);
				}
			}

			const integer imageWidth = k;
			const integer imageHeight = k;
			Array<real32> image(Vector2i(imageWidth, imageHeight));
			NearestSet::iterator aIter = aNearestSet.begin();
			NearestSet::iterator bIter = bNearestSet.begin();
			
			ENSURE_OP(aNearestSet.size(), ==, k);
			ENSURE_OP(bNearestSet.size(), ==, k);

			for (integer i = 0;i < k;++i)
			{
				const integer x = aIter->value();
				const integer y = bIter->value();
				ENSURE_OP(x, >=, 0);
				ENSURE_OP(y, >=, 0);
				ENSURE_OP(x, <, k);
				ENSURE_OP(y, <, k);
				
				image(x, y) = 1;
				++aIter;
				++bIter;
			}

			saveGrayscalePcx(image, "andersondarling-vs-distance.pcx");
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("AndersonDarling", test);
	}

	CallFunction run(addTest);

}
