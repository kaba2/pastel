#include "pastel/sys/measuretable_tools.h"
#include "pastel/sys/string_algorithms.h"

#include <vector>

namespace Pastel
{

	PASTELSYS void printPretty(
		const MeasureTable& measureTable,
		std::ostream& stream)
	{
		integer columns = measureTable.width();
		const integer rows = measureTable.height();

		if (columns == 0 || rows == 0)
		{
			return;
		}

		std::vector<integer> columnWidth;
		columnWidth.reserve(columns);

		for (integer x = 0;x < columns;++x)
		{
			integer maxWidth = 0;
			for (integer y = 0;y < rows;++y)
			{
				const integer entryWidth = 
					measureTable(x, y).text().length();
				if (entryWidth > maxWidth)
				{
					maxWidth = entryWidth;
				}
			}
			columnWidth.push_back(maxWidth + 2);
		}

		for (integer y = 0;y < rows;++y)
		{
			if (measureTable.hasHorizontalSeparator(y))
			{
				for (integer x = 0;x < columns;++x)
				{
					if (measureTable.hasVerticalSeparator(x))
					{
						stream << "+";
					}
					for (integer i = 0;i < columnWidth[x];++i)
					{
						stream << "-";
					}
				}
				if (measureTable.hasVerticalSeparator(columns))
				{
					stream << "+";
				}
				stream << std::endl;
			}

			for (integer x = 0;x < columns;++x)
			{

				const MeasureTable::Entry& entry = measureTable(x, y);
				
				integer paddingLength = 
					(columnWidth[x] - entry.text().length()) - 2;
				std::string prePadding;
				std::string postPadding;

				switch(entry.alignment())
				{
				case MeasureTable::Alignment::Left:
					postPadding = repeat(" ", paddingLength);
					break;
				case MeasureTable::Alignment::Right:
					postPadding = repeat(" ", paddingLength);
					break;
				case MeasureTable::Alignment::Center:
					prePadding = repeat(" ", paddingLength / 2);
					postPadding = repeat(" ", paddingLength - prePadding.length());
					break;
				};

				if (measureTable.hasVerticalSeparator(x))
				{
					stream << "|";
				}
				stream << " " << prePadding << measureTable(x, y).text() 
					<< postPadding << " ";
			}
			if (measureTable.hasVerticalSeparator(columns))
			{
				stream << "|";
			}
			stream << std::endl;
		}

		if (measureTable.hasHorizontalSeparator(rows))
		{
			for (integer x = 0;x < columns;++x)
			{
				if (measureTable.hasVerticalSeparator(x))
				{
					stream << "+";
				}
				for (integer i = 0;i < columnWidth[x];++i)
				{
					stream << "-";
				}
			}
			if (measureTable.hasVerticalSeparator(columns))
			{
				stream << "+";
			}
			stream << std::endl;
		}

		stream << measureTable.caption() << std::endl;
	}

	PASTELSYS void printLatex(

		const MeasureTable& measureTable,
		std::ostream& stream)
	{
		integer columns = measureTable.width();
		const integer rows = measureTable.height();

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
