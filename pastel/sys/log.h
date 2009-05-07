/*!
\file
\brief A singleton class for logging.
*/

#ifndef PASTELSYS_LOG_H
#define PASTELSYS_LOG_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/countedptr.h"
#include "pastel/sys/logobserver.h"

#include <set>

namespace Pastel
{

	class PASTELSYS Log
	{
	public:
		// Using default constructor.
		// Using default copy constructor.
		// Using default destructor.

		void swap(Log& that);
		Log& operator=(const Log& that);

		void addObserver(const LogObserverPtr& observer);
		void removeObserver(const LogObserverPtr& observer);

		Log& operator<<(const std::string& value);
		Log& operator<<(char value);
		Log& operator<<(uchar value);
		Log& operator<<(int value);
		Log& operator<<(uint value);
		Log& operator<<(long value);
		Log& operator<<(ulong value);
		Log& operator<<(float value);
		Log& operator<<(double value);
		Log& operator<<(void (*function)(Log&));

	private:
		typedef std::set<LogObserverPtr> ObserverContainer;
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
