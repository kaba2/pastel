// Description: Testing for tag-hashing
// DocumentationOf: tag.h

#include "test/test_init.h"

#include <pastel/sys/ensure.h>

#include <fstream>
#include <map>
#include <iostream>

namespace
{

	void test(std::string filename)
	{
		std::map<int, std::string> wordSet;
		std::ifstream file(filename);
		ENSURE(file.is_open());

		std::string word;
		while (file >> word)
		{
			auto result = 
				wordSet.insert(std::make_pair(tagHash(word.c_str(), word.length()), word));
			if (!result.second)
			{
				std::cout << word << " collides with " << result.first->second << std::endl;
			}
		}
	}

}

TEST_CASE("Tag (Tag)")
{
	test("corncob_lowercase.txt");
}
