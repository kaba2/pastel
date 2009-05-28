#ifndef PASTEL_LUDECOMPOSITION_H
#define PASTEL_LUDECOMPOSITION_H

#include "pastel/math/matrix_tools.h"
#include "pastel/math/mathlibrary.h"

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	/*!
	Let M be a nxn matrix. Then M
	can be decomposed into
	M = PLU
	where
	P is an nxn permutation matrix
	L is an nxn lower triangular matrix
	U is an nxn upper triangular matrix
	*/

	template <int N, typename Real>
	class LuDecomposition
	{
	public:
		// Using default copy constructor.
		// Using default destructor.

		LuDecomposition();
		explicit LuDecomposition(integer dimension);
		explicit LuDecomposition(const Matrix<N, N, Real>& matrix);

		void swap(LuDecomposition& that);

		LuDecomposition<N, Real>& operator=(const LuDecomposition& that);

		bool decompose(const Matrix<N, N, Real>& matrix);

		const Matrix<N, N, Real>& packedLu() const;
		const Tuple<N, integer>& rowPermutation() const;
		bool evenPermutation() const;
		bool singular() const;

	private:
		Matrix<N, N, Real> packedLu_;
		Tuple<N, integer> rowPermutation_;
		bool evenRowPermutation_;
		bool singular_;

		Vector<N, Real> invLargestInRow_;
	};

}

#include "pastel/math/ludecomposition.hpp"

#endif
