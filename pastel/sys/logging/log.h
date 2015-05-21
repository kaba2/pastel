// Description: Log class
// Detail: Outputs logging information to a set of Logger's.
// Documentation: logging.txt

#ifndef PASTELSYS_LOG_H
#define PASTELSYS_LOG_H

namespace Pastel
{

	class Log;
	Log& log();

}

#include "pastel/sys/mytypes.h"
#include "pastel/sys/logging/logger_fwd.h"
#include "pastel/sys/printable/printable_concept.h"

#include <set>

namespace Pastel
{

	class Log
		: public Logger
	{
	public:
		// Using default constructor.
		// Using default copy constructor.

		~Log();

		void swap(Log& that);
		Log& operator=(const Log& that);

		void addLogger(Logger* logger);
		void removeLogger(Logger* logger);
		void removeLoggers();

		// Virtual since Log is a Logger.
		virtual Log& operator<<(const std::string& value);
		virtual void finalize();

		template <
			typename Type,
			typename = 
				Requires<
					Models<Type, Printable_Concept>
				>
			>
		Log& operator<<(const Type& value);
		
		Log& operator<<(void (*function)(Log&));

	private:
		using LoggerSet = std::set<Logger*>;
		using Logger_Iterator = LoggerSet::iterator;
		using Logger_ConstIterator = LoggerSet::const_iterator;

		LoggerSet loggerSet_;
	};

	inline void logNewLine(Log& log)
	{
		log << "\n";
	}

}

namespace Pastel
{

	inline Log& log()
	{
		static Log theLog;
		return theLog;
	}

}

#include "pastel/sys/logging/log.hpp"
#include "pastel/sys/logging/logger.h"

#endif
