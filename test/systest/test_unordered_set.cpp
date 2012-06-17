// Description: Testing for UnorderedSet
// DocumentationOf: unorderedset.h

#include "pastelsystest.h"

#include "pastel/sys/callfunction.h"
#include "pastel/sys/mytypes.h"
#include "pastel/sys/unorderedset.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/nativeallocator.h"

#include "pastel/sys/random.h"

#include <vector>
#include <set>
#include <algorithm>
#include <ctime>

namespace
{
	using namespace Pastel;

	typedef UnorderedSet<int, Hash<int>, std::equal_to<int>,
		PoolAllocator> Container;

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

		void generateRandomUniqueValues(std::vector<int>& values)
		{
			int numValues = 1 + randomInteger() % 30000;

			// Generate unique values
			std::set<int> valueSet;
			for(int i = 0; i < numValues; ++i)
			{
				valueSet.insert(randomInteger());
			}

			std::vector<int> orderedValues(valueSet.begin(), valueSet.end());
			while (!orderedValues.empty())
			{
				int index = randomInteger() % orderedValues.size();
				values.push_back(orderedValues[index]);
				orderedValues.erase(orderedValues.begin() + index);
			}
		}


		// Check that both containers hold the same values
		void testSameValues(
			const Container& hashtable,
			const std::vector<int>& values)
		{
			TEST_ENSURE_OP(hashtable.size(), ==, values.size());

			Container::const_iterator hashIter;
			std::vector<int>::const_iterator valuesIter;

			valuesIter = values.begin();
			while (valuesIter != values.end())
			{
				TEST_ENSURE(hashtable.find(*valuesIter) != hashtable.end());
				++valuesIter;
			}

			/*
			hashIter = hashtable.begin();
			while (hashIter != hashtable.end())
			{
				valuesIter = std::find(values.begin(), values.end(), *hashIter);
				TEST_ENSURE(valuesIter != values.end());
				++hashIter;
			}
			*/
		}


		// Check that find works properly
		void testFind(
			const Container& hashtable,
			const std::vector<int>& values)
		{
			Container::const_iterator hashIter;
			std::vector<int>::const_iterator valuesIter;

			for(int i = 0; i < 2000; ++i)
			{
				int value = randomInteger();
				hashIter = hashtable.find(value);
				valuesIter = std::find(values.begin(), values.end(), value);
				TEST_ENSURE(
					(hashIter == hashtable.end()) ==
					(valuesIter == values.end()));
			}
		}

		// Check that insert works properly
		void testInsertErase(
			Container& hashtable,
			const std::vector<int>& values)
		{
			Container::iterator hashIter;
			std::vector<int>::const_iterator valuesIter;

			for(int i = 0; i < 2000; ++i)
			{
				int value = values[randomInteger() % values.size()];
				std::pair<Container::iterator, bool> result =
					hashtable.insert(value);

				TEST_ENSURE(*result.first == value);
				TEST_ENSURE(result.second != true);
			}

			for(int i = 0; i < 2000; ++i)
			{
				int value = randomInteger();
				std::pair<Container::iterator, bool> result =
					hashtable.insert(value);

				valuesIter = std::find(values.begin(), values.end(), value);
				TEST_ENSURE(
					(result.second == true) ==
					(valuesIter == values.end()));

				hashIter = hashtable.find(value);
				TEST_ENSURE(hashIter != hashtable.end());

				hashIter = hashtable.erase(hashIter);
				TEST_ENSURE(hashtable.erase(value) == 0);
				TEST_ENSURE(hashtable.find(value) == hashtable.end());
			}
		}

		// Test copy constructor, assignment, swap
		void testCopyEtc(
			Container& hashtable,
			const std::vector<int>& values)
		{
			Container that(hashtable);
			TEST_ENSURE_OP(that.size(), ==, values.size());
			TEST_ENSURE_OP(that.size(), ==, hashtable.size());
			TEST_ENSURE_OP(that.max_load_factor(), ==, hashtable.max_load_factor());
			testSameValues(that, values);
			std::swap(hashtable, that);

			TEST_ENSURE_OP(that.size(), ==, values.size());
			TEST_ENSURE_OP(that.size(), ==, hashtable.size());
			TEST_ENSURE_OP(that.max_load_factor(), ==, hashtable.max_load_factor());
			testSameValues(that, values);
			testSameValues(hashtable, values);

			hashtable = that;
			TEST_ENSURE_OP(that.size(), ==, values.size());
			TEST_ENSURE_OP(that.size(), ==, hashtable.size());
			TEST_ENSURE_OP(that.max_load_factor(), ==, hashtable.max_load_factor());
			testSameValues(hashtable, values);

			Container that2;
			that2 = hashtable;
			TEST_ENSURE_OP(that2.size(), ==, values.size());
			TEST_ENSURE_OP(that2.size(), ==, hashtable.size());
			TEST_ENSURE_OP(that2.max_load_factor(), ==, hashtable.max_load_factor());
			testSameValues(hashtable, values);
		}

		// Test rehashing
		void testRehash(
			Container& hashtable,
			const std::vector<int>& values)
		{
			TEST_ENSURE_OP(hashtable.load_factor(), <=, hashtable.max_load_factor());

			for(float loadFactor = 0.5; loadFactor <= 3.0; loadFactor += 0.5)
			{
				hashtable.max_load_factor(loadFactor);
				for(integer i = 1; i < 100000; i += 10000)
				{
					hashtable.rehash(i);
					TEST_ENSURE_OP((integer)hashtable.bucket_count(), >=, i);
					TEST_ENSURE_OP(hashtable.max_load_factor(), ==, loadFactor);
					TEST_ENSURE_OP(hashtable.load_factor(), <=, loadFactor);
					testSameValues(hashtable, values);
					testCopyEtc(hashtable, values);
				}
			}
		}

		void test()
		{
			//int seed = time(NULL);
			int seed = 1175532569;
			srand(seed);

			std::vector<int> values;
			generateRandomUniqueValues(values);

			log() << "seed: " << seed << logNewLine;
			log() << "values: " << values.size() << logNewLine;

			Container hashtable;
			Container hashIter;

			hashtable.insert(values.begin(), values.end());

			testSameValues(hashtable, values);
			testFind(hashtable, values);
			testCopyEtc(hashtable, values);
			testInsertErase(hashtable, values);
			testRehash(hashtable, values);

			log() << logNewLine;
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("UnorderedSet", test);
	}

	CallFunction run(addTest);
}
