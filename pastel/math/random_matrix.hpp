#ifndef PASTELMATH_RANDOM_MATRIX_HPP
#define PASTELMATH_RANDOM_MATRIX_HPP

#include "pastel/math/random_matrix.h"

#include "pastel/sys/random_uniform.h"

namespace Pastel
{

	template <typename Real>
	void setRandomMatrix(
		Matrix<Real>& matrix)
	{
		typedef typename Matrix<Real>::Iterator
			Iterator;

		Iterator iter = matrix.begin();
		Iterator iterEnd = matrix.end();
		while(iter != iterEnd)
		{
			*iter = random<Real>() * 2 - 1;
			++iter;
		}
	}

}


#endif
