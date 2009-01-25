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
		// Using default constructor.
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		typedef void (*Function)();

		//! Adds a function to the queue.
		void add(const std::string& key,
			Function function);

		//! Runs all function calls in the queue.
		void run();

		//! Runs one specified function in the queue.
		bool run(const std::string& key);

		//! Removes all functions calls from the queue.
		void clear();

		void console();
		void printTests() const;

	private:
		typedef std::map<std::string, Function> Container;
		typedef Container::iterator Iterator;
		typedef Container::const_iterator ConstIterator;

		Container functions_;
	};

}

#endif
