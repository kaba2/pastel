#include "pastel/sys/statreal_tools.h"

namespace Pastel
{

	PASTELSYS std::ostream& operator<<(
		std::ostream& stream,
		const StatReal::GlobalStats& stats)
	{
		stream << "Constructed " << stats.constructed() << std::endl;
		stream << "Destructed " << stats.destructed() << std::endl;

		const char* name[] = {
			"Operations",
			"Comparisons",
			"Conversions"
		};

		PASTEL_STATIC_ASSERT(sizeof(name) / sizeof(const char*) == (int)StatReal::Stat::Enum::Size);

		for (integer i = 0;i < (integer)StatReal::Stat::Enum::Size;++i)
		{
			stream << name[i] << ": " << stats.statistic(i) << std::endl;
		}

		return stream;
	}

}
