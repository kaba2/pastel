// Description: LuDecomposition class
// Detail: Computes the PLU-decomposition of a matrix

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
		const Tuple<integer, N>& rowPermutation() const;
		bool evenPermutation() const;
		bool singular() const;

	private:
		Matrix<N, N, Real> packedLu_;
		Tuple<integer, N> rowPermutation_;
		bool evenRowPermutation_;
		bool singular_;

		Vector<Real, N> invLargestInRow_;
	};

	typedef LuDecomposition<1, real> LuDecomposition1;
	typedef LuDecomposition<2, real> LuDecomposition2;
	typedef LuDecomposition<3, real> LuDecomposition3;
	typedef LuDecomposition<4, real> LuDecomposition4;
	typedef LuDecomposition<Dynamic, real> LuDecompositionD;

}

#include "pastel/math/ludecomposition.hpp"

#endif
