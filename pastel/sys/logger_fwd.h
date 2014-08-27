// Description: Logger class
// Detail: An abstract base class for reporting logging information
// Documentation: loggers.txt

#ifndef PASTELSYS_LOGGER_FWD_H
#define PASTELSYS_LOGGER_FWD_H

#include "pastel/sys/syslibrary.h"

#include <string>
#include <unordered_set>

namespace Pastel
{

	class Log;

	class Logger
	{
	public:
		// Using default constructor.
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		virtual ~Logger();

		void swap(Logger& that);

		//! Write a string.
		virtual Logger& operator<<(
			const std::string& value) = 0;

		//! Frees any resources.
		/*!
		This is called, for example, before aborting
		program execution via an assertion. With this
		function the logger has the chance to close 
		a file or flush the contents of a stream.
		*/
		virtual void finalize() = 0;

	private:
		friend class Log;
		
		//! Add a log to the set of observer logs.
		void addLog(Log* log);

		//! Remove a log from the set of observer logs.
		void removeLog(Log* log);

		using LogSet = std::unordered_set<Log*>;
		using Log_Iterator = LogSet::iterator;
		using Log_ConstIterator = LogSet::const_iterator;

		//! The set of observer logs.
		LogSet logSet_;
	};

}

#endif
