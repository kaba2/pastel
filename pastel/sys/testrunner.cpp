#include "pastel/sys/testrunner.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/log.h"
#include "pastel/sys/string_tools.h"

#include <vector>
#include <iostream>
#include <string>

using namespace std;

namespace Pastel
{

	void TestRunner::add(
		const std::string& key,
		Function function)
	{
		functions_.insert(std::make_pair(key, function));
	}

	void TestRunner::run()
	{
		log() << "TestRunner: Starting a test suite"
			<< logNewLine << logNewLine;

		ConstIterator iter(functions_.begin());
		ConstIterator iterEnd(functions_.end());
		while (iter != iterEnd)
		{
			log() << iter->first << logNewLine;

			Function function = iter->second;
			if (!REPORT(!function))
			{
				function();
			}
			++iter;
		}

		log() << "TestRunner: Test suite complete."
			<< logNewLine << logNewLine;
	}

	bool TestRunner::run(
		const std::string& key)
	{
		bool result = false;

		//ConstIterator iter(functions_.find(key));
		ConstIterator iter(functions_.lower_bound(key));
		if (iter != functions_.end())
		{
			log() << iter->first << logNewLine;

			Function function = iter->second;
			if (!REPORT(!function))
			{
				function();
				result = true;
			}
		}

		return result;
	}

	void TestRunner::clear()
	{
		functions_.clear();
	}

	void TestRunner::console()
	{
		cout << "TestRunner console" << endl;
		cout << "'quit' to quit." << endl;
		cout << "'all' to run all tests." << endl;
		cout << "'' to list available tests." << endl;

		bool quit = false;
		while (!quit)
		{
			cout << "> ";
			string input;
			getline(cin, input);

			if (!cin)
			{
				break;
			}

			string trimmedInput(trim(input));
			if (trimmedInput == "quit")
			{
				break;
			}
			else if (trimmedInput == "all")
			{
				run();
			}
			else if (trimmedInput.empty())
			{
				printTests();
			}
			else
			{
				bool result = run(trimmedInput);
				if (!result)
				{
					cout << "No such test found." << endl;
					printTests();
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
