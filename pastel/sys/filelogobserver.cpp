#include "pastel/sys/filelogobserver.h"

namespace Pastel
{

	PASTELSYS FileLogObserverPtr fileLogObserver(const std::string& fileName)
	{
		return FileLogObserverPtr(new FileLogObserver(fileName));
	}

	FileLogObserver::FileLogObserver(const std::string& fileName)
		: file_(fileName.c_str())
	{
	}

	FileLogObserver::~FileLogObserver()
	{
	}

	FileLogObserver& FileLogObserver::operator<<(const std::string& value)
	{
		file_ << value;

		return *this;
	}

}
