#include "pastel/sys/testrunner.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/log.h"
#include "pastel/sys/string_tools.h"

#include <vector>
#include <iostream>
using std::cout;
using std::endl;

#include <string>

namespace Pastel
{

	void TestRunner::add(
		const std::string& key,
		Function function)
	{
		functions_.insert(std::make_pair(key, function));
	}

	void TestRunner::run() const
	{
		log() << "TestRunner: Starting a test suite"
			<< logNewLine << logNewLine;

		ConstIterator iter(functions_.begin());
		ConstIterator iterEnd(functions_.end());
		while (iter != iterEnd)
		{
			run(iter);
			++iter;
		}

		log() << "TestRunner: Test suite complete."
			<< logNewLine << logNewLine;
	}

	void TestRunner::run(
		const ConstIterator& iter) const
	{
		if (iter == functions_.end())
		{
			return;
		}

		log() << logNewLine;
		log() << iter->first << logNewLine;

		std::string underline;
		for (integer i = 0;i < iter->first.size();++i)
		{
			underline += '-';
		}

		log() << underline << logNewLine;
		log() << logNewLine;

		Function function = iter->second;
		if (!REPORT(!function))
		{
			function();
		}
	}

	bool TestRunner::run(
		const std::string& key) const
	{
		bool result = false;

		ConstIterator iter(functions_.find(key));
		if (iter != functions_.end())
		{
			run(iter);
			result = true;
		}

		return result;
	}

	bool TestRunner::runPrefix(
		const std::string& key) const
	{
		bool result = false;

		ConstIterator iter(functions_.begin());
		while (iter != functions_.end())
		{
			if (iter->first.substr(0, key.size()) == key)
			{
				run(iter);
				result = true;
			}

			++iter;
		}

		return result;
	}

	void TestRunner::clear()
	{
		functions_.clear();
	}

	void TestRunner::console() const
	{
		cout << "Pastel TestRunner console" << endl;
		cout << "'quit' to quit." << endl;
		cout << "'all' to run all tests." << endl;
		cout << "'prefix' to add a prefix to typed names." << endl;
		cout << "'' to list available tests." << endl;
		cout << "Specifying a name runs all the tests that have that name as a prefix." << endl;

		std::string prefix;

		bool quit = false;
		while (!quit)
		{
			cout << prefix << "> ";
			std::string input;
			std::getline(std::cin, input);

			if (!std::cin)
			{
				break;
			}

			const std::string trimmedInput(trim(input));
			if (trimmedInput == "quit")
			{
				break;
			}
			else if (trimmedInput == "all")
			{
				runPrefix(prefix);
			}
			else if (trimmedInput.substr(0, 6) == "prefix")
			{
				prefix = trim(trimmedInput.substr(6));
			}
			else if (trimmedInput.empty())
			{
				printTests();
			}
			else
			{
				const bool result = runPrefix(prefix + trimmedInput);
				if (!result)
				{
					cout << "No such keyword or test(s) found." << endl;
				}
			}
		}
	}

	void TestRunner::printTests() const
	{
		ConstIterator iter(functions_.begin());
		ConstIterator iterEnd(functions_.end());
		while (iter != iterEnd)
		{
			cout << iter->first << endl;

			++iter;
		}
	}

}
