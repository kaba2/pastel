#ifndef PASTELSYS_LOG_HPP
#define PASTELSYS_LOG_HPP

#include "pastel/sys/logging/log.h"
#include "pastel/sys/ensure.h"

#include <sstream>
#include <vector>

namespace Pastel
{

	inline Log::~Log()
	{
		removeLoggers();
	}

	inline void Log::swap(Log& that)
	{
		Logger::swap(that);
		loggerSet_.swap(that.loggerSet_);
	}

	inline Log& Log::operator=(const Log& that)
	{
		Log copy(that);
		swap(copy);
		return *this;
	}

	inline void Log::addLogger(Logger* logger)
	{
		loggerSet_.insert(logger);
		logger->addLog(this);
	}

	inline void Log::removeLogger(Logger* logger)
	{
		logger->removeLog(this);
		loggerSet_.erase(logger);
	}

	inline void Log::removeLoggers()
	{
		Logger_ConstIterator iter = loggerSet_.begin();
		Logger_ConstIterator iterEnd = loggerSet_.end();
		while (iter != iterEnd)
		{

			(*iter)->removeLog(this);

			++iter;
		}
	}

	inline void Log::finalize()
	{
		Logger_ConstIterator iter = loggerSet_.begin();
		Logger_ConstIterator iterEnd = loggerSet_.end();
		while (iter != iterEnd)
		{

			(*iter)->finalize();

			++iter;
		}
	}

	inline Log& Log::operator<<(const std::string& value)
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

	inline Log& Log::operator<<(char value)
	{
		std::stringstream stream;
		stream << value;
		std::string text;
		stream >> text;
		(*this) << text;

		return *this;
	}

	inline Log& Log::operator<<(uchar value)
	{
		std::stringstream stream;
		stream << value;
		std::string text;
		stream >> text;
		(*this) << text;

		return *this;
	}

	inline Log& Log::operator<<(int value)
	{
		std::stringstream stream;
		stream << value;
		std::string text;
		stream >> text;
		(*this) << text;

		return *this;
	}

	inline Log& Log::operator<<(uint value)
	{
		std::stringstream stream;
		stream << value;
		std::string text;
		stream >> text;
		(*this) << text;

		return *this;
	}

	inline Log& Log::operator<<(long value)
	{
		std::stringstream stream;
		stream << value;
		std::string text;
		stream >> text;
		(*this) << text;

		return *this;
	}

	inline Log& Log::operator<<(ulong value)
	{
		std::stringstream stream;
		stream << value;
		std::string text;
		stream >> text;
		(*this) << text;

		return *this;
	}

	inline Log& Log::operator<<(longlong value)
	{
		std::stringstream stream;
		stream << value;
		std::string text;
		stream >> text;
		(*this) << text;

		return *this;
	}

	inline Log& Log::operator<<(ulonglong value)
	{
		std::stringstream stream;
		stream << value;
		std::string text;
		stream >> text;
		(*this) << text;

		return *this;
	}

	inline Log& Log::operator<<(float value)
	{
		std::stringstream stream;
		stream << value;
		std::string text;
		stream >> text;
		(*this) << text;

		return *this;
	}

	inline Log& Log::operator<<(double value)
	{
		std::stringstream stream;
		stream << value;
		std::string text;
		stream >> text;
		(*this) << text;

		return *this;
	}

	inline Log& Log::operator<<(void (*function)(Log&))
	{
		if (function)
		{
			(*function)(*this);
		}

		return *this;
	}

}

#endif
