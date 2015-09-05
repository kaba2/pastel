// Description: Least-squares affine transformation between point-sets.
// Documentation: ls_transformations.txt

#ifndef PASTELGEOMETRY_LS_AFFINE_H
#define PASTELGEOMETRY_LS_AFFINE_H

#include "pastel/sys/math/sign.h"

#include <armadillo>

namespace Pastel
{

	enum class LsAffine_Matrix : integer
	{
		Free,
		Identity
	};

	enum class LsAffine_Scaling : integer
	{
		Free,
		Diagonal,
		Conformal,
		Rigid
	};

	enum class LsAffine_Translation : integer
	{
		Free,
		Identity
	};

	template <typename Real>
	struct LsAffine_Return
	{
		arma::Mat<Real> Q;
		arma::Mat<Real> S;
		arma::Col<Real> t;
	};

	//! Least-squares affine transformation between point-sets
	/*!
	Preconditions:
	d > 0
	m > 0
	n > 0

	The Q, S, and t are chosen such that they minimize the error metric

	  sum_{i = 1}^m sum_{j = 1}^n w_{ij} ||(QS p_i + t) - r_j||^2

	subject to the given constraints. If m = n, and W is diagonal,
	then the error metric simplifies to

	  sum_{i = 1}^m w_{ii} ||(QS p_i + t) - r_i||^2.

	Input
	-----

	fromSet ((d x m)-matrix):
	Each column contains the coordinates of a d-dimensional point.

	toSet ((d x n)-matrix):
	Each column contains the coordinates of a d-dimensional point.
	If W is not specified, it must hold that m = n.

	Returns
	-------

	Q ((d x d)-matrix):
	An orthogonal matrix, representing a rotation or a reflection.
	Initialized with Q0 (e.g. avoid reallocation using std::move).

	S ((d x d)-matrix):
	A symmetric matrix, representing a scaling. By the eigenvalue 
	decomposition, a symmetric matrix is the composition of a 
	rotation, axis-aligned scaling, and a reverse rotation. 
	Initialized with S0 (e.g. avoid reallocation using std::move).

	t ((d x 1)-matrix):
	A vector, representing a translation. 
	Initialized with t0 (e.g. avoid reallocation using std::move).

	Optional arguments
	------------------

	matrix (LsAffine_Matrix : Free):
	Constraint for the matrix Q.
		Free: Q^T Q = I
	 	Identity: Q = I

	scaling (LsAffine_Scaling : Free):
	Constraint for the scaling S.
		Free: S^T = S
		Diagonal: S is diagonal
		Conformal: S = sI
		Rigid: S = +/- I

	translation (LsAffine_Translation : Free):
	Constraint for the translation t.
		Free: no constraint
		Identity: t = 0

	orientation (integer : 1):
	Constraint for the determinant of QS.
		<0: det(QS) < 0
		 0: no constraint
		>0: det(QS) > 0
	Orientation cannot be constrained when 
	scaling == Free; this would result in solutions 
	with det(QS) = 0.

	W (arma::Mat<Real> : arma::Mat<Real>()): 
	A non-negative (m x n) matrix, which contains the weights 
	for the least-squares error metric. If W is not given, or is
	the empty matrix, then it is required that m = n, and it 
	is assumed that W is the (n x n) identity matrix. 

	Q0 (arma::Mat<Real> : arma::Mat<ReaL>()):
	A (d, d) matrix by which to initialize the returned Q.
	If empty, then Q is initialized with the identity matrix,
	with fresh memory.

	S0 (arma::Mat<Real> : arma::Mat<ReaL>()):
	A (d, d) matrix by which to initialize the returned S.
	If empty, then S is initialized with the identity matrix,
	with fresh memory.

	t0 (arma::Col<Real> : arma::Col<ReaL>()):
	A (d, 1) matrix by which to initialize the returned t.
	If empty, then t is initialized with the zero matrix,
	with fresh memory.
	*/
	template <
		typename Real,
		typename... ArgumentSet>
	LsAffine_Return<Real> lsAffine(
		arma::Mat<Real> fromSet,
		arma::Mat<Real> toSet,
		ArgumentSet&&... argumentSet)
	{
		// Least-Squares Transformations between Point-Sets_,
		// Kalle Rutanen, Germán Gómez-Herrero, 
		// Sirkka-Liisa Eriksson, Karen Egiazarian, 
		// 18th Scandinavian Conference on Image Analysis, 
		// pp.501-511, June 17-20, 2013.

		ENSURE_OP(fromSet.n_rows, ==, toSet.n_rows);

		arma::Mat<Real>& P = fromSet;
		arma::Mat<Real>& R = toSet;

		integer d = fromSet.n_rows;
		integer m = fromSet.n_cols;
		integer n = toSet.n_cols;

		ENSURE_OP(d, >, 0);
		ENSURE_OP(m, >, 0);
		ENSURE_OP(n, >, 0);

		LsAffine_Matrix matrix =
			PASTEL_ARG_ENUM(matrix, LsAffine_Matrix::Free);
		LsAffine_Scaling scaling =
			PASTEL_ARG_ENUM(scaling, LsAffine_Scaling::Free);
		LsAffine_Translation translation =
			PASTEL_ARG_ENUM(translation, LsAffine_Translation::Free);
		integer orientation =
			PASTEL_ARG_S(orientation, (integer)1);
		arma::Mat<Real> W = 
			PASTEL_ARG_S(W, arma::Mat<Real>());
		arma::Mat<Real> Q = 
			PASTEL_ARG_S(Q0, arma::Mat<Real>());
		arma::Mat<Real> S = 
			PASTEL_ARG_S(S0, arma::Mat<Real>());
		arma::Col<Real> t= 
			PASTEL_ARG_S(t0, arma::Col<Real>());

		// Initialize Q, S, and t.
		Q.eye(d, d);
		S.eye(d, d);
		t.zeros(d);

        // We wish to preserve the memory storage
        // of Q, S, and t. Store the memory addresses
        // to check the preservation later.
        const Real* qPointer = Q.memptr();
        const Real* sPointer = S.memptr();
        const Real* tPointer = t.memptr();

		auto result = [&]()
		{
	        // Make sure that memory was not reallocated.
	        ASSERT(Q.memptr() == qPointer);
	        unused(qPointer);

	        ASSERT(S.memptr() == sPointer);
	        unused(sPointer);

	        ASSERT(t.memptr() == tPointer);
	        unused(tPointer);

			return LsAffine_Return<Real>{std::move(Q), std::move(S), std::move(t)};
		};

		bool wSpecified = !W.is_empty();

		// When W is not specified, we require
		// the point-sets to have an equal number
		// of points.
		ENSURE2(
			wSpecified || 
			(fromSet.n_cols == toSet.n_cols), 
			fromSet.n_cols, toSet.n_cols);

		// When Q = I, and S is not rigid, forcing the
		// orientation results in solutions for which
		// det(QS) = 0.
		ENSURE(!(matrix == LsAffine_Matrix::Identity &&
			scaling != LsAffine_Scaling::Rigid &&
			orientation != 0));

		// When S is free, forcing the orientation
		// results in solutions for which det(QS) = 0.
		ENSURE(!(scaling == LsAffine_Scaling::Free &&
			orientation != 0));

		// This case is not implemented; I do not know
		// the solution to this problem.
		ENSURE(!(matrix == LsAffine_Matrix::Free &&
			scaling == LsAffine_Scaling::Diagonal));

		// When Q = I and S = +/- I, a negative
		// det(QS) is only possible in odd dimensions.
		ENSURE(!(orientation < 0 &&
			matrix == LsAffine_Matrix::Identity &&
			scaling == LsAffine_Scaling::Rigid &&
			even(d)));

		Real totalWeight = n;
		if (wSpecified)
		{
			totalWeight = arma::accu(W);
		}

		ENSURE(!negative(totalWeight));

		arma::Col<Real> fromCentroid(d);
		arma::Col<Real> toCentroid(d);

		if (translation == LsAffine_Translation::Free)
		{
			if (wSpecified)
			{
				// With weighting.
				fromCentroid = 
					P * W * arma::ones<arma::Mat<Real>>(n, 1) / totalWeight;
		        toCentroid = 
		        	R * W.t() * (arma::ones<arma::Mat<Real>>(m, 1) / totalWeight);
			}
	    	else
	    	{
	        	// Without weighting.
	        	fromCentroid = arma::sum(P, 1) / m;
	        	toCentroid = arma::sum(R, 1) / n;
			}
		 
		    // Form the centered point-sets. The optimal transformation
		    // will map fromCentroid to toCentroid. After this the problem
		    // has been reduced from affine to linear.
		    P.each_col() -= fromCentroid;
		    R.each_col() -= toCentroid;
		}

		arma::Mat<Real> PP(d, d);
		arma::Mat<Real> RP(d, d);

		if (wSpecified)
		{
		    // With weighting.
		    PP = P * arma::diagmat(arma::sum(W, 1)) * P.t();
		    RP = R * W.t() * P.t();
		}
		else
		{
		    // Without weighting.
		    PP = P * P.t();
		    RP = R * P.t();
		}

		if (scaling == LsAffine_Scaling::Free && 
			matrix == LsAffine_Matrix::Identity)
		{
		    // f(x) = Sx
		 
		    // Find the optimal scaling.
		    arma::Mat<Real> S_;
		    if (!arma::syl(S_, PP, PP.t(), -(RP + RP.t())))
		    {
			    // In failure, the output matrix is resetted,
			    // so we cannot use S as the output matrix.
		    	return result();
		    }

		    S = S_;

			// Forced oriented solution would have det(QS) < 0;
			// oriented solution is not implemented.
			ASSERT_OP(orientation, ==, 0);
		}

		if (scaling == LsAffine_Scaling::Free && 
			matrix == LsAffine_Matrix::Free)
		{
		    // f(x) = Ax
		 
		    // Compute the optimal linear transformation.
		    //[UP, UR, X, DP, DR] = arma::gsvd(PP, RP);
		    //A = UR * (DR * arma::pinv(DP)) * UP.t();
		    arma::Mat<Real> pinvPP;
		    if (!arma::pinv(pinvPP, PP))
			{
				return result();
			}

			arma::Mat<Real> A = RP * pinvPP;

			// Compute Q and S from A such that
			// A = QS and S is symmetric positive semi-definite.
			arma::Mat<Real> U;
			arma::Mat<Real> V;
			arma::Col<Real> s;
			if (!arma::svd(U, s, V, A))
			{
				return result();
			}

			Q = U * V.t();
			S = V * arma::diagmat(s) * V.t();

			// Forced oriented solution would have det(QS) < 0;
			// oriented solution is not implemented.
			ASSERT_OP(orientation, ==, 0);
		}

		if (matrix == LsAffine_Matrix::Free &&
			(scaling == LsAffine_Scaling::Rigid ||
			scaling == LsAffine_Scaling::Conformal))
		{
		    // f(x) = sQx

		    // Compute the optimal orthogonal transformation.
		    arma::Mat<Real> U;
			arma::Mat<Real> V;
			arma::Col<Real> s;
		    unused(s);

		    if (!arma::svd(U, s, V, RP))
			{
				return result();
			}

		    // Compute the optimal non-oriented orthogonal Q.
		    Q = U * V.t();

		    if (orientation != 0 &&
		    	sign(arma::det(Q)) != sign(orientation))
		    {
		        // If orientation is to be forced at det(A) = g, where g = +- 1,
		        // then A is given by:
		        //
		        //    Q = UDV^T, where
		        //    D = [1, ..., 1, g det(UV^T)].

		        arma::Col<Real> s(d, arma::fill::ones);
		        s(d - 1) = -1;

		        // Compute the optimal oriented orthogonal Q.
		        Q = U * arma::diagmat(s) * V.t();
		    }
		}

		if (matrix == LsAffine_Matrix::Identity &&
			scaling == LsAffine_Scaling::Diagonal)
		{
			// f(x) = Dx

			// The error is given by
			// sum_{i = 1}^d S_{ii}^2 (PP^T)_{ii} - 
			// 2 sum_{i = 1}^d S_{ii}  (RP^T)_{ii}

			// Compute the optimal diagonal scaling S.
			// FIX: Make this orthogonality-maximizing.
			S.diag() = RP.diag() / PP.diag();

			// Compute det(QS) = det(S).
			Real sDet = arma::prod(S.diag());

			if (orientation != 0 &&
				sign(sDet) != sign(orientation))
			{
				// From the form of the error functional
				// we see that we can obtain the solution
				// of the oriented problem by negating that
				// diagonal element of S for which 
				// S_{ii} (RP^T)_{ii} is the smallest.
				
				arma::Col<Real> product = S.diag() * RP.diag();

				// Find smallest S_{ii} (RP^T)_{ii}.
				integer iMin = 0;
				Real minValue = Infinity();
				for (integer i = 1;i < d;++i)
				{
					// S_{ii} (RP^T)_{ii} >= 0; otherwise
					// the non-oriented solution would not
					// have minimum error.
					ASSERT_OP(product(i), >=, 0);
					if (product(i) < minValue)
					{
						iMin = i;
						minValue = product(i);
					}
				}

				// Negate that diagonal element which causes
				// the least error.
				S(iMin, iMin) = -S(iMin, iMin);
			}
		}

		if (scaling == LsAffine_Scaling::Conformal)
		{
			// f(x) = sQx

			// Compute the optimal scaling parameter.
			Real s = arma::trace(Q.t() * RP) / arma::trace(PP);
			S *= s;

			if (matrix == LsAffine_Matrix::Free)
			{
				if (orientation != 0)
				{
					// The orientation has already been handled
					// in the selection of Q.
					ASSERT(sign(arma::det(Q * S)) == sign(orientation));
				}
			}
			else
			{
				// det(sQ) < 0 is possible only when d is odd.
				// In addition, forced oriented solutions would 
				// have det(sQ) = 0; oriented solution is not 
				// implemented.
				ASSERT_OP(orientation, ==, 0);
			}
		}

		if (matrix == LsAffine_Matrix::Identity &&
			scaling == LsAffine_Scaling::Rigid &&
			orientation < 0)
		{
			ASSERT1(odd(d), d);

			// S = -I is the only possible choice, since
			// det(QS) = det(S) = det(-I) = (-1)^d = -1;
			// here we require d to be odd.
			S = -S;
		}

		if (translation == LsAffine_Translation::Free)
		{
		    // Compute the optimal translation.
		    t = toCentroid - Q * S * fromCentroid;
		}

		return result();
	}

}

#endif
