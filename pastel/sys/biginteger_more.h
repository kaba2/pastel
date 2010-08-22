// Description: More BigInteger class

#ifndef PASTEL_BIGINTEGER_MORE_H
#define PASTEL_BIGINTEGER_MORE_H

#include "pastel/sys/biginteger.h"

namespace Pastel
{

	void swap(BigInteger& left, BigInteger& right);

	bool zero(const BigInteger& that);
	bool negative(const BigInteger& that);
	bool positive(const BigInteger& that);

}

#include "pastel/sys/biginteger_more.hpp"

#endif
