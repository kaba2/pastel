// Description: Testing for Anderson-Darling statistic
// DocumentationOf: random_anderson_darling.h

#include "pastelsystest.h"

#include "pastel/sys/random_anderson_darling.h"

#include <iostream>

using namespace Pastel;

namespace
{

	class Test
		: public TestSuite
	{
	public:
		Test()
			: TestSuite(&sysTestReport())
		{
		}

		virtual void run()
		{
			test();
		}

		void test()
		{
			const integer m = 100000;
			const integer n = 64;
			std::vector<real> input(n);

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
			30%          | 1.1
			20%          | 1.4
			10%          | 2.0
			5%           | 2.6
			2.5%         | 3.3
			1%           | 4.0
			0.5%         | 4.6
			0.1%         | 6.1
			0.01%        | 8.4
			*/

			//real minThreshold = 1.0;
			//real maxThreshold = 2.0;
			real minThreshold = 0.631;
			real maxThreshold = 0.631;
			integer thresholds = (maxThreshold - minThreshold) * 10;
			if (thresholds == 0)
			{
				++thresholds;
			}

			for (integer k = 0;k < thresholds;++k)
			{
				const real threshold = 
					linear(minThreshold, maxThreshold, (real)k / thresholds);
				integer negatives = 0;
				real mean = 0;
				real deviation = 1;
				real populationMean = nan<real>();
				real populationDeviation = nan<real>();
				//real populationMean = 0;
				//real populationDeviation = 1;
				for (integer j = 0;j < m;++j)
				{
					for (integer i = 0;i < n;++i)
					{
						input[i] = mean + randomGaussian<real>() * deviation;
						//input[i] = mean + (2 * random<real>() - 1) * std::sqrt((real)3) * deviation;

						/*
						input[i] = randomGaussian<real>();
						if (i > n / 4)
						{
							input[i] += 2;
						}
						*/
					}
					/*
					real sum = 0;
					real squareSum = 0;
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

					const real t = gaussianAndersonDarling<real>(
						range(input.begin(), input.end()), 
						populationMean, populationDeviation);
					if (t > threshold)
					{
						++negatives;
					}	
				}

				const real negativePercent =
					(real)(100 * negatives) / m;

				std::cout << threshold << ": " << negativePercent << "% negatives." << std::endl;

				TEST_ENSURE_OP(negativePercent, <, 10);
			}
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		sysTestList().add("AndersonDarling", test);
	}

	CallFunction run(addTest);

}
