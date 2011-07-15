// Description: Logger class
// Detail: An abstract base class for reporting logging information
// Documentation: loggers.txt

#ifndef PASTEL_LOGGER_H
#define PASTEL_LOGGER_H

#include "pastel/sys/syslibrary.h"
#include "pastel/sys/unorderedset.h"

#include <string>

namespace Pastel
{

	class PASTELSYS Log;

	class PASTELSYS Logger
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
		//! Remove a log from the set of observed logs.
		void removeLog(Log* log);

		typedef UnorderedSet<Log*> LogSet;
		typedef LogSet::iterator Log_Iterator;
		typedef LogSet::const_iterator Log_ConstIterator;

		//! The set of observer logs.
		LogSet logSet_;
	};

}

#endif
