#include "pastel/sys/log.h"
#include "pastel/sys/ensure.h"

#include <sstream>

namespace Pastel
{

	void Log::swap(Log& that)
	{
		observer_.swap(that.observer_);
	}

	Log& Log::operator=(const Log& that)
	{
		Log copy(that);
		swap(copy);
		return *this;
	}

	void Log::addObserver(const LogObserverPtr& observer)
	{
		observer_.insert(observer);
	}

	void Log::removeObserver(const LogObserverPtr& observer)
	{
		observer_.erase(observer);
	}

	Log& Log::operator<<(const std::string& value)
	{
		ConstObserverIterator iter(observer_.begin());
		ConstObserverIterator iterEnd(observer_.end());

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

	PASTELSYS Log& log()
	{
		static Log theLog;
		return theLog;
	}

}
