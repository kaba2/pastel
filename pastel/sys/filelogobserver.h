#ifndef PASTELSYS_FILELOGOBSERVER_H
#define PASTELSYS_FILELOGOBSERVER_H

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

}

#endif
