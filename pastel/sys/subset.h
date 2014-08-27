// Description: Subset class
// Detail: Iterates through all fixed-sized subsets of an integer interval.
// Documentation: miscellaneous.txt

#ifndef PASTELSYS_SUBSET_H
#define PASTELSYS_SUBSET_H

#include "pastel/sys/syslibrary.h"
#include "pastel/sys/mytypes.h"

#include <vector>

namespace Pastel
{

	//! Iterates through all fixed-sized subsets of an integer interval.
	class Subset
	{
	public:
		Subset();
		Subset(integer size, integer elements);

		void swap(Subset& that);

		void first();
		bool next();

		void setSize(integer size);
		void setSize(integer size, integer elements);

		integer size() const;
		integer elements() const;

		integer operator[](integer index) const;

	private:
		std::vector<integer> data_;
		integer elements_;
	};

}

#include "pastel/sys/subset.hpp"

#endif
