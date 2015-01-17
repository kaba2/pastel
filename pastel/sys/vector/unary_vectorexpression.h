// Description: Element-wise unary vector expressions
// Documentation: vectorexpression.txt

#ifndef PASTELSYS_UNARY_VECTOREXPRESSION_H
#define PASTELSYS_UNARY_VECTOREXPRESSION_H

#include "pastel/sys/vector/vectorexpression.h"
#include "pastel/sys/vector/unary_vectorexpression_detail.h"

#include <cmath>

namespace Pastel
{

	PASTEL_UNARY_VECTOREXPRESSION(VectorInverse, inverse, inverse);
	PASTEL_UNARY_VECTOREXPRESSION(VectorAbs, mabs, mabs);
	PASTEL_UNARY_VECTOREXPRESSION(VectorSquare, square, squarev);
	PASTEL_UNARY_VECTOREXPRESSION(VectorExp, std::exp, exp);
	PASTEL_UNARY_VECTOREXPRESSION(VectorLog, std::log, log);
	PASTEL_UNARY_VECTOREXPRESSION(VectorSqrt, std::sqrt, sqrt);
	PASTEL_UNARY_VECTOREXPRESSION(VectorFloor, std::floor, floor);
	PASTEL_UNARY_VECTOREXPRESSION(VectorCeil, std::ceil, ceil);
	PASTEL_UNARY_VECTOREXPRESSION(VectorMod, mod, mod);
	PASTEL_UNARY_VECTOREXPRESSION(VectorSin, std::sin, sin);
	PASTEL_UNARY_VECTOREXPRESSION(VectorCos, std::cos, cos);
	PASTEL_UNARY_VECTOREXPRESSION(VectorTan, std::tan, tan);
	PASTEL_UNARY_VECTOREXPRESSION(VectorAsin, std::asin, asin);
	PASTEL_UNARY_VECTOREXPRESSION(VectorAcos, std::acos, acos);
	PASTEL_UNARY_VECTOREXPRESSION(VectorAtan, std::atan, atan);

}

#endif
