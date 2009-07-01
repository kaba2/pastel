#ifndef PASTEL_MEASURETABLE_TOOLS_H
#define PASTEL_MEASURETABLE_TOOLS_H

#include "pastel/sys/measuretable.h"

#include <iostream>

namespace Pastel
{

	PASTELSYS void printPretty(
		const MeasureTable& measureTable,
		std::ostream& stream);

	PASTELSYS void printLatex(
		const MeasureTable& measureTable,
		std::ostream& stream);

}

#endif
