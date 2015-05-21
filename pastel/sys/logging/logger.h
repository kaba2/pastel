#ifndef PASTELSYS_LOGGER_H
#define PASTELSYS_LOGGER_H

#include "pastel/sys/logging/logger_fwd.h"
#include "pastel/sys/logging/log.h"

#include <string>
#include <unordered_set>

namespace Pastel
{

	inline Logger::~Logger()
	{
		Log_ConstIterator iter = logSet_.begin();
		Log_ConstIterator iterEnd = logSet_.end();
		while(iter != iterEnd)
		{
			Log_ConstIterator next = iter;
			++next;

			// The current iterator 'iter'
			// will be erased.

			Log* log = *iter;
			log->removeLogger(this);

			iter = next;
		}
	}

	inline void Logger::swap(Logger& that)
	{
		logSet_.swap(that.logSet_);
	}

	inline void Logger::addLog(Log* log)
	{
		if (log)
		{
			logSet_.insert(log);
		}
	}

	inline void Logger::removeLog(Log* log)
	{
		if (log)
		{
			logSet_.erase(log);
		}
	}

}

#endif
