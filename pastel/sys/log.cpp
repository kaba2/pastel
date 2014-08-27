#include "pastel/sys/log.h"
#include "pastel/sys/ensure.h"

#include <sstream>
#include <vector>

namespace Pastel
{

	Log::~Log()
	{
		removeLoggers();
	}

	void Log::swap(Log& that)
	{
		Logger::swap(that);
		loggerSet_.swap(that.loggerSet_);
	}

	Log& Log::operator=(const Log& that)
	{
		Log copy(that);
		swap(copy);
		return *this;
	}

	void Log::addLogger(Logger* logger)
	{
		loggerSet_.insert(logger);
		logger->addLog(this);
	}

	void Log::removeLogger(Logger* logger)
	{
		logger->removeLog(this);
		loggerSet_.erase(logger);
	}

	void Log::removeLoggers()
	{
		Logger_ConstIterator iter = loggerSet_.begin();
		Logger_ConstIterator iterEnd = loggerSet_.end();
		while (iter != iterEnd)
		{

			(*iter)->removeLog(this);

			++iter;
		}
	}

	void Log::finalize()
	{
		Logger_ConstIterator iter = loggerSet_.begin();
		Logger_ConstIterator iterEnd = loggerSet_.end();
		while (iter != iterEnd)
		{

			(*iter)->finalize();

			++iter;
		}
	}

	Log& Log::operator<<(const std::string& value)
	{
		Logger_ConstIterator iter = loggerSet_.begin();
		Logger_ConstIterator iterEnd = loggerSet_.end();

		while (iter != iterEnd)
		{

			*(*iter) << value;

			++iter;
		}

		return *this;
	}

	Log& Log::operator<<(char value)
	{
		std::stringstream stream;
		stream << value;
		std::string text;
		stream >> text;
		(*this) << text;

		return *this;
	}

	Log& Log::operator<<(uchar value)
	{
		std::stringstream stream;
		stream << value;
		std::string text;
		stream >> text;
		(*this) << text;

		return *this;
	}

	Log& Log::operator<<(int value)
	{
		std::stringstream stream;
		stream << value;
		std::string text;
		stream >> text;
		(*this) << text;

		return *this;
	}

	Log& Log::operator<<(uint value)
	{
		std::stringstream stream;
		stream << value;
		std::string text;
		stream >> text;
		(*this) << text;

		return *this;
	}

	Log& Log::operator<<(long value)
	{
		std::stringstream stream;
		stream << value;
		std::string text;
		stream >> text;
		(*this) << text;

		return *this;
	}

	Log& Log::operator<<(ulong value)
	{
		std::stringstream stream;
		stream << value;
		std::string text;
		stream >> text;
		(*this) << text;

		return *this;
	}

	Log& Log::operator<<(longlong value)
	{
		std::stringstream stream;
		stream << value;
		std::string text;
		stream >> text;
		(*this) << text;

		return *this;
	}

	Log& Log::operator<<(ulonglong value)
	{
		std::stringstream stream;
		stream << value;
		std::string text;
		stream >> text;
		(*this) << text;

		return *this;
	}

	Log& Log::operator<<(float value)
	{
		std::stringstream stream;
		stream << value;
		std::string text;
		stream >> text;
		(*this) << text;

		return *this;
	}

	Log& Log::operator<<(double value)
	{
		std::stringstream stream;
		stream << value;
		std::string text;
		stream >> text;
		(*this) << text;

		return *this;
	}

	Log& Log::operator<<(void (*function)(Log&))
	{
		if (function)
		{
			(*function)(*this);
		}

		return *this;
	}

	Log& log()
	{
		static Log theLog;
		return theLog;
	}

}

