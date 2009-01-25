#ifndef PASTELMATH_LUDECOMPOSITION_H
#define PASTELMATH_LUDECOMPOSITION_H

#include "pastel/math/matrix_tools.h"
#include "pastel/math/mathlibrary.h"

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	template <int N, typename Real>
	class LuDecomposition
	{
	public:
		// Using default copy constructor.
		// Using default destructor.

		LuDecomposition();
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
