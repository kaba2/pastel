#include "pastel/sys/logger.h"
#include "pastel/sys/log.h"

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
