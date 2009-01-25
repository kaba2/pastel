/*!
\file
\brief A class for accumulating calls to simple functions and running them.
*/

#ifndef PASTELSYS_TESTRUNNER_H
#define PASTELSYS_TESTRUNNER_H

#include "pastel/sys/syslibrary.h"

#include <map>
#include <string>

namespace Pastel
{

	//! Accumulate calls to simple functions and run them.

	class PASTELSYS TestRunner
	{
	public:
		typedef void (*Function)();

	private:
		typedef std::map<std::string, Function> Container;
		typedef Container::iterator Iterator;

	public:
		typedef Container::const_iterator ConstIterator;

		// Using default constructor.
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

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

		void console() const;
		void printTests() const;

	private:
		Container functions_;
	};

}

#endif
