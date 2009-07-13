// Description: MeasureTable class
// Detail: Stores values and the visual design of a table.

#ifndef PASTEL_MEASURETABLE_H
#define PASTEL_MEASURETABLE_H

#include "pastel/sys/array.h"
#include "pastel/sys/smallset.h"

namespace Pastel
{

	class PASTELSYS MeasureTable
	{
	public:
		class Alignment
		{
		public:
			enum Enum
			{
				Left,
				Right,
				Center
			};
		};

		class Entry
		{
		public:
			Entry()
				: alignment_(Alignment::Left)
				, text_()
			{
			}

			void setAlignment(
				Alignment::Enum alignment)
			{
				alignment_ = alignment;
			}

			Alignment::Enum alignment() const
			{
				return alignment_;
			}

			std::string& text()
			{
				return text_;
			}

			const std::string& text() const
			{
				return text_;
			}

		private:
			Alignment::Enum alignment_;
			std::string text_;
		};

		void setCaption(const std::string& caption);
		const std::string& caption() const;

		void setSize(integer width, integer height);

		integer width() const;
		integer height() const;

		Entry& operator()(integer x, integer y);
		const Entry& operator()(integer x, integer y) const;

		void addHorizontalSeparator(integer y);
		void addVerticalSeparator(integer x);

		bool hasHorizontalSeparator(integer y) const;
		bool hasVerticalSeparator(integer x) const;

	private:
		Array<2, Entry> data_;
		SmallSet<integer> hSeparatorSet_;
		SmallSet<integer> vSeparatorSet_;
		std::string caption_;
	};

}

#endif
