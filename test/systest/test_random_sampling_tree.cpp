// Description: Testing for random sampling tree
// DocumentationOf: random_sampling_tree.h

#include "pastelsystest.h"

#include "pastel/sys/random_sampling_tree.h"
#include "pastel/sys/random_uniform.h"
#include "pastel/sys/math_functions.h"

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
			typedef RandomSamplingTree<real, integer>
				SamplingTree;
			typedef SamplingTree::Iterator Iterator;

			SamplingTree samplingTree;

			typedef SamplingTree::Tree Tree;

			Tree& tree = samplingTree.tree();

			const integer n = 10;
			std::vector<real> massSet;
			massSet.reserve(n);
			real totalMass = 0;
			for (integer i = 0;i < n;++i)
			{
				const real mass = randomOpen0<real>();
				massSet[i] = mass;
				totalMass += mass;

				Iterator iter = tree.insert(keyValue(mass, i));
			}

			std::vector<real> frequencySet(n, 0);
			const integer samples = 1000000;
			for (integer i = 0;i < samples;++i)
			{
				++frequencySet[randomlySample(samplingTree)->key().value()];
			}
			
			for (integer i = 0;i < n;++i)
			{
				const real error =
					relativeError<real>(frequencySet[i] / samples, massSet[i] / totalMass);
				TEST_ENSURE_OP(error, <, 0.3);
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
		sysTestList().add("RandomSamplingTree", test);
	}

	CallFunction run(addTest);

}
