#ifndef PASTELSYS_TESTRUNNER_HPP
#define PASTELSYS_TESTRUNNER_HPP

#include "pastel/sys/testing/testrunner.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/logging/log.h"
#include "pastel/sys/string.h"

#include <vector>
#include <iostream>
#include <string>

namespace Pastel
{

	inline TestRunner::TestRunner()
		: functionSet_()
		, name_("Unnamed")
	{
	}

	inline TestRunner::TestRunner(const std::string& name)
		: functionSet_()
		, name_(name)
	{
	}

	inline void TestRunner::add(
		const std::string& key,
		Function function)
	{
		functionSet_.insert(std::make_pair(key, function));
	}

	inline void TestRunner::run() const
	{
		log() << "Testing " << name_ << "..."
			<< logNewLine << logNewLine;

		ConstIterator iter(functionSet_.begin());
		ConstIterator iterEnd(functionSet_.end());
		while (iter != iterEnd)
		{
			run(iter);
			++iter;
		}

		//log() << name_ << " testing complete."
		//	<< logNewLine << logNewLine;
	}

	inline void TestRunner::run(
		const ConstIterator& iter) const
	{
		if (iter == functionSet_.end())
		{
			return;
		}

		log() << iter->first << logNewLine;

		Function function = iter->second;
		if (function)
		{
			try
			{
				function();
			}
			catch(const InvariantFailure&)
			{
				log() << "An invariant failure occurred while running the test " 
					<< iter->first << "."
					<< logNewLine;
			}
			catch(...)
			{
				log() << "An exception was thrown while running the test " 
					<< iter->first << "."
					<< logNewLine;
				throw;
			};
		}
	}

	inline bool TestRunner::run(
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

	inline bool TestRunner::runPrefix(
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

	inline void TestRunner::clear()
	{
		functionSet_.clear();
	}

	inline void TestRunner::help() const
	{
		std::cout << name_ << " test suite" << std::endl;
		std::cout << std::endl;
		std::cout << "'quit' to quit." << std::endl;
		std::cout << "'help' to print this text." << std::endl;
		std::cout << "'all' to run all tests." << std::endl;
		std::cout << "'prefix' to add a prefix to typed names." << std::endl;
		std::cout << "'' to list available tests." << std::endl;
		std::cout << "Specifying a name runs all the tests that have that name as a prefix." << std::endl;
	}

	inline void TestRunner::console() const
	{
		help();

		std::string prefix;

		bool quit = false;
		while (!quit)
		{
			std::cout << prefix << "> ";
			std::string input;
			std::getline(std::cin, input);

			if (!std::cin)
			{
				break;
			}

			std::string trimmedInput(trim(input));
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
				bool result = runPrefix(prefix + trimmedInput);
				if (!result)
				{
					std::cout << "No such keyword or test(s) found." << std::endl;
				}
			}
		}
	}

	inline void TestRunner::printTests() const
	{
		ConstIterator iter(functionSet_.begin());
		ConstIterator iterEnd(functionSet_.end());
		while (iter != iterEnd)
		{
			std::cout << iter->first << std::endl;

			++iter;
		}
	}

}

#endif
