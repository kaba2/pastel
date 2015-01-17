#ifndef PASTELSYS_MEASURETABLE_PRINT_LATEX_HPP
#define PASTELSYS_MEASURETABLE_PRINT_LATEX_HPP

#include "pastel/sys/measuretable_print_latex.h"
#include "pastel/sys/string.h"

#include <vector>

namespace Pastel
{

	inline void printLatex(
		const MeasureTable& measureTable,
		std::ostream& stream)
	{
		integer columns = measureTable.width();
		integer rows = measureTable.height();

		if (columns == 0 || rows == 0)
		{
			return;
		}

		stream << "\\begin{table}" << std::endl;
		stream << "\\small" << std::endl;
		stream << "\\center" << std::endl;
		stream << "\\begin{tabular}{";
		for (integer x = 0;x < columns;++x)
		{
			if (measureTable.hasVerticalSeparator(x))
			{
				stream << "|";
			}
			switch(measureTable(x, 0).alignment())
			{
			case MeasureTable::Alignment::Left:
				stream << "l";
				break;
			case MeasureTable::Alignment::Right:
				stream << "r";
				break;
			case MeasureTable::Alignment::Center:
				stream << "c";
				break;
			};
		}
		if (measureTable.hasVerticalSeparator(columns))
		{
			stream << "|";
		}
		stream << "}" << std::endl;

		for (integer y = 0;y < rows;++y)
		{
			if (measureTable.hasHorizontalSeparator(y))
			{
				stream << "\\hline";
				stream << std::endl;
			}

			for (integer x = 0;x < columns;++x)
			{
				stream << measureTable(x, y).text();
				if (x < columns - 1)
				{

					stream << " & ";
				}
			}
			stream << " \\\\";
			stream << std::endl;
		}

		if (measureTable.hasHorizontalSeparator(rows))
		{
			stream << "\\hline";
			stream << std::endl;
		}

		stream << "\\end{tabular}" << std::endl;
		if (!measureTable.caption().empty())
		{
			stream << "\\caption{" << measureTable.caption() 
				<< "}" << std::endl;
		}
		stream << "\\end{table}" << std::endl;
	}

}

#endif
