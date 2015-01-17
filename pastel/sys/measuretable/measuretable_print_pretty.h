// Description: Ascii pretty-printing for measure-table

#ifndef PASTELSYS_MEASURETABLE_PRINT_PRETTY_H
#define PASTELSYS_MEASURETABLE_PRINT_PRETTY_H

#include "pastel/sys/measuretable.h"

#include <iostream>

namespace Pastel
{

	void printPretty(
		const MeasureTable& measureTable,
		std::ostream& stream);

}

#include "pastel/sys/measuretable_print_pretty.hpp"

#endif
