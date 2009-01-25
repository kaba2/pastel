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

	typedef UnorderedSet<int, hash<int>, std::equal_to<int>,
		PoolAllocator> Container;

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
		REPORT2(hashtable.size() != values.size(),
			hashtable.size(),
			values.size());

		Container::const_iterator hashIter;
		std::vector<int>::const_iterator valuesIter;

		valuesIter = values.begin();
		while (valuesIter != values.end())
		{
			REPORT(hashtable.find(*valuesIter) == hashtable.end());
			++valuesIter;
		}

		/*
		hashIter = hashtable.begin();
		while (hashIter != hashtable.end())
		{
			valuesIter = std::find(values.begin(), values.end(), *hashIter);
			REPORT(valuesIter == values.end());
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
			REPORT(
				(hashIter == hashtable.end()) !=
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

			REPORT(*result.first != value);
			REPORT(result.second == true);
		}

		for(int i = 0; i < 2000; ++i)
		{
			int value = randomInteger();
			std::pair<Container::iterator, bool> result =
				hashtable.insert(value);

			valuesIter = std::find(values.begin(), values.end(), value);
			REPORT(
				(result.second == true) !=
				(valuesIter == values.end()));

			hashIter = hashtable.find(value);
			REPORT(hashIter == hashtable.end());

			hashIter = hashtable.erase(hashIter);
			REPORT(hashtable.erase(value) != 0);
			REPORT(hashtable.find(value) != hashtable.end());
		}
	}

	// Test copy constructor, assignment, swap
	void testCopyEtc(
		Container& hashtable,
		const std::vector<int>& values)
	{
		Container that(hashtable);
		REPORT2(that.size() != values.size(), that.size(), values.size());
		REPORT2(that.size() != hashtable.size(), that.size(), hashtable.size());
		REPORT2(that.max_load_factor() != hashtable.max_load_factor(),
			that.max_load_factor(), hashtable.max_load_factor());
		testSameValues(that, values);
		std::swap(hashtable, that);

		REPORT2(that.size() != values.size(), that.size(), values.size());
		REPORT2(that.size() != hashtable.size(), that.size(), hashtable.size());
		REPORT2(that.max_load_factor() != hashtable.max_load_factor(),
			that.max_load_factor(), hashtable.max_load_factor());
		testSameValues(that, values);
		testSameValues(hashtable, values);

		hashtable = that;
		REPORT2(that.size() != values.size(), that.size(), values.size());
		REPORT2(that.size() != hashtable.size(), that.size(), hashtable.size());
		REPORT2(that.max_load_factor() != hashtable.max_load_factor(),
			that.max_load_factor(), hashtable.max_load_factor());
		testSameValues(hashtable, values);

		Container that2;
		that2 = hashtable;
		REPORT2(that2.size() != values.size(), that2.size(), values.size());
		REPORT2(that2.size() != hashtable.size(), that2.size(), hashtable.size());
		REPORT2(that2.max_load_factor() != hashtable.max_load_factor(),
			that2.max_load_factor(), hashtable.max_load_factor());
		testSameValues(hashtable, values);
	}

	// Test rehashing
	void testRehash(
		Container& hashtable,
		const std::vector<int>& values)
	{
		REPORT(hashtable.load_factor() > hashtable.max_load_factor());

		for(float loadFactor = 0.5; loadFactor <= 3.0; loadFactor += 0.5)
		{
			hashtable.max_load_factor(loadFactor);
			for(integer i = 1; i < 100000; i += 10000)
			{
				hashtable.rehash(i);
				REPORT((integer)hashtable.bucket_count() < i);
				REPORT(hashtable.max_load_factor() != loadFactor);
				REPORT(hashtable.load_factor() > loadFactor);
				testSameValues(hashtable, values);
				testCopyEtc(hashtable, values);
			}
		}
	}

	void testBegin()
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

	void testAdd()
	{
		sysTestList().add("UnorderedSet", testBegin);
	}

	CallFunction run(testAdd);
}
