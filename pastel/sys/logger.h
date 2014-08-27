#ifndef PASTELSYS_LOGGER_H
#define PASTELSYS_LOGGER_H

#include "pastel/sys/logger_fwd.h"
#include "pastel/sys/log.h"

#include <string>
#include <unordered_set>

namespace Pastel
{

	Logger::~Logger()
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

	void Logger::swap(Logger& that)
	{
		logSet_.swap(that.logSet_);
	}

	void Logger::addLog(Log* log)
	{
		ASSERT(log);
		logSet_.insert(log);
	}

	void Logger::removeLog(Log* log)
	{
		ASSERT(log);
		logSet_.erase(log);
	}

}

#endif
