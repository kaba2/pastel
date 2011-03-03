#include "pastel/sys/testrunner.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/log.h"
#include "pastel/sys/string_algorithms.h"

#include <vector>
#include <iostream>
using std::cout;
using std::endl;

#include <string>

namespace Pastel
{

	TestRunner::TestRunner()
		: functionSet_()
		, name_("Unnamed")
	{
	}

	TestRunner::TestRunner(const std::string& name)
		: functionSet_()
		, name_(name)
	{
	}

	void TestRunner::add(
		const std::string& key,
		Function function)
	{
		functionSet_.insert(std::make_pair(key, function));
	}

	void TestRunner::run() const
	{
		log() << "Running " << name_ << " test suite..."
			<< logNewLine << logNewLine;

		ConstIterator iter(functionSet_.begin());
		ConstIterator iterEnd(functionSet_.end());
		while (iter != iterEnd)
		{
			run(iter);
			++iter;
		}

		log() << name_ << " test suite complete."
			<< logNewLine << logNewLine;
	}

	void TestRunner::run(
		const ConstIterator& iter) const
	{
		if (iter == functionSet_.end())
		{
			return;
		}

		log() << iter->first << logNewLine;

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

		ConstIterator iter(functionSet_.find(key));
		if (iter != functionSet_.end())
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

		ConstIterator iter(functionSet_.begin());
		while (iter != functionSet_.end())
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
		functionSet_.clear();
	}

	void TestRunner::help() const
	{
		cout << name_ << " test suite" << endl;
		cout << endl;
		cout << "'quit' to quit." << endl;
		cout << "'help' to print this text." << endl;
		cout << "'all' to run all tests." << endl;
		cout << "'prefix' to add a prefix to typed names." << endl;
		cout << "'' to list available tests." << endl;
		cout << "Specifying a name runs all the tests that have that name as a prefix." << endl;
	}

	void TestRunner::console() const
	{
		help();

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
			else if (trimmedInput == "help")
			{
				help();
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
		ConstIterator iter(functionSet_.begin());
		ConstIterator iterEnd(functionSet_.end());
		while (iter != iterEnd)
		{
			cout << iter->first << endl;

			++iter;
		}
	}

}
