// Description: FileLogObserver class
// Detail: Outputs logging information to a file
// Documentation: log.txt

#ifndef PASTEL_FILELOGOBSERVER_H
#define PASTEL_FILELOGOBSERVER_H

#include "pastel/sys/logobserver.h"

#include <fstream>

namespace Pastel
{

	class PASTELSYS FileLogObserver
		: public LogObserver
	{
	public:
		explicit FileLogObserver(const std::string& fileName);

		virtual ~FileLogObserver();

		virtual FileLogObserver& operator<<(const std::string& value);

	private:
		std::ofstream file_;
	};

	typedef CountedPtr<FileLogObserver> FileLogObserverPtr;

	PASTELSYS FileLogObserverPtr fileLogObserver(const std::string& fileName);

}

#endif
