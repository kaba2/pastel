// Description: Element-wise binary vector expressions
// Documentation: vectorexpression.txt

#ifndef PASTEL_BINARY_VECTOREXPRESSION_H
#define PASTEL_BINARY_VECTOREXPRESSION_H

#include "pastel/sys/vectorexpression.h"
#include "pastel/sys/binary_vectorexpression_detail.h"

#include <cmath>

namespace Pastel
{

	// Here are listed all predefined binary vector expressions.

	PASTEL_BINARY_VECTOREXPRESSION_RIGHTCONSTANT(VectorPow, std::pow, pow);
	PASTEL_BINARY_VECTOREXPRESSION(VectorAtan2, std::atan2, atan2);
	PASTEL_BINARY_VECTOREXPRESSION(VectorMin, std::min, min);
	PASTEL_BINARY_VECTOREXPRESSION(VectorMax, std::max, max);

}

#endif
