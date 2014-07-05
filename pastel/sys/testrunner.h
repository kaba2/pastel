// Description: TestRunner class
// Detail: Stores test functions to be run as a batch or from a console.
// Documentation: unit_testing.txt

#ifndef PASTELSYS_TESTRUNNER_H
#define PASTELSYS_TESTRUNNER_H

#include "pastel/sys/syslibrary.h"

#include <map>
#include <string>

namespace Pastel
{

	//! Stores test functions to be run as a batch or from a console.

	class PASTELSYS TestRunner
	{
	public:
		typedef void (*Function)();

	private:
		using Container = std::map<std::string, Function>;
		using Iterator = Container::iterator;

	public:
		using ConstIterator = Container::const_iterator;

		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		TestRunner();
		explicit TestRunner(const std::string& name);

		//! Adds a function to the queue.
		void add(const std::string& key,
			Function function);

		//! Runs all function calls in the queue.
		void run() const;

		//! Runs a specified function in the queue.
		void run(const ConstIterator& iter) const;

		//! Runs one specified function in the queue.
		bool run(const std::string& key) const;

		//! Runs all functions that have 'key' as prefix.
		bool runPrefix(const std::string& key) const;

		//! Removes all functions calls from the queue.
		void clear();

		void help() const;
		void console() const;
		void printTests() const;

	private:
		Container functionSet_;
		std::string name_;
	};

}

#endif
