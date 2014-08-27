// Description: Latex printing for measure-table

#ifndef PASTELSYS_MEASURETABLE_PRINT_LATEX_H
#define PASTELSYS_MEASURETABLE_PRINT_LATEX_H

#include "pastel/sys/measuretable.h"

#include <iostream>

namespace Pastel
{

	void printLatex(
		const MeasureTable& measureTable,
		std::ostream& stream);

}

#include "pastel/sys/measuretable_print_latex.hpp"

#endif
