// Description: Log class
// Detail: Outputs logging information to a set of Logger's.
// Documentation: logging.txt

#ifndef PASTEL_LOG_H
#define PASTEL_LOG_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/countedptr.h"
#include "pastel/sys/logger.h"

#include <set>

namespace Pastel
{

	class PASTELSYS Log
		: public Logger
	{
	public:
		// Using default constructor.
		// Using default copy constructor.
		// Using default destructor.

		void swap(Log& that);
		Log& operator=(const Log& that);

		void addLogger(const LoggerPtr& observer);
		void removeLogger(const LoggerPtr& observer);

		virtual Log& operator<<(const std::string& value);
		virtual void finalize();

		Log& operator<<(char value);
		Log& operator<<(uchar value);
		Log& operator<<(int value);
		Log& operator<<(uint value);
		Log& operator<<(long value);
		Log& operator<<(ulong value);
		Log& operator<<(longlong value);
		Log& operator<<(ulonglong value);
		Log& operator<<(float value);
		Log& operator<<(double value);
		Log& operator<<(void (*function)(Log&));

	private:
		typedef std::set<LoggerPtr> ObserverContainer;
		typedef ObserverContainer::iterator ObserverIterator;
		typedef ObserverContainer::const_iterator ConstObserverIterator;

		ObserverContainer observer_;
	};

	PASTELSYS Log& log();

	inline void logNewLine(Log& log)
	{
		log << "\n";
	}

}

#endif
