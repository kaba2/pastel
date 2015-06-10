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
		LsAffine_Return() = default;
		LsAffine_Return(const LsAffine_Return& that) = default;
		LsAffine_Return(LsAffine_Return&& that) = default;
		LsAffine_Return& operator=(const LsAffine_Return& that) = default;
		LsAffine_Return& operator=(LsAffine_Return&& that) = default;

		arma::Mat<Real> Q;
		arma::Mat<Real> S;
		arma::Mat<Real> t;
	};

	//! Least-squares affine transformation between point-sets
	/*!
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

	S ((d x d)-matrix):
	A symmetric matrix, representing a scaling. By the eigenvalue 
	decomposition, a symmetric matrix is the composition of a 
	rotation, axis-aligned scaling, and a reverse rotation. 

	t ((d x 1)-matrix):
	A vector, representing a translation.

	Optional arguments
	------------------

	matrix (LsAffine_Matrix : Free):
	Constraint for the matrix Q.
		Free: Q^T Q = I
	 	Identity: Q = I

	scaling (LsAffine_Scaling : Free):
	Constraint for the scaling S.
		Free: S^T = S
		Conformal: S = sI
		Rigid: S = I

	translation (LsAffine_Translation : Free):
	Constraint for the translation t.
		Free: -
		Identity: t = 0

	orientation (integer : 0):
	Constraint for the determinant of QS.
		<0: det(QS) < 0
		 0: det(QS)
		>0: det(QS) > 0

	W (arma::Mat<Real> : arma::Mat<Real>()): 
	A non-negative (m x n) matrix, which contains the weights 
	for the least-squares error metric. If W is not given, or is
	the empty matrix, then it is required that m = n, and it 
	is assumed that W is the (n x n) identity matrix. 

	Q (arma::Mat<Real> : arma::Mat<ReaL>()):
	A (d, d) matrix by which to initialize the returned Q.
	If empty, then Q is initialized with the identity matrix,
	with fresh memory.

	S (arma::Mat<Real> : arma::Mat<ReaL>()):
	A (d, d) matrix by which to initialize the returned S.
	If empty, then S is initialized with the identity matrix,
	with fresh memory.

	t (arma::Col<Real> : arma::Col<ReaL>()):
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

		LsAffine_Matrix matrix =
			PASTEL_ARG_ENUM(matrix, LsAffine_Matrix::Free);
		LsAffine_Scaling scaling =
			PASTEL_ARG_ENUM(scaling, LsAffine_Scaling::Free);
		LsAffine_Translation translation =
			PASTEL_ARG_ENUM(translation, LsAffine_Translation::Free);
		integer orientation =
			PASTEL_ARG_S(orientation, (integer)0);
		arma::Mat<Real> W = 
			PASTEL_ARG_S(W, arma::Mat<Real>());
		arma::Mat<Real> Q = 
			PASTEL_ARG_S(Q, arma::Mat<Real>());
		arma::Mat<Real> S = 
			PASTEL_ARG_S(S, arma::Mat<Real>());
		arma::Col<Real> t= 
			PASTEL_ARG_S(t, arma::Col<Real>());

		// Initialize Q, S, and t.
		Q.eye(d, d);
		S.eye(d, d);
		t.zeros(d);

		auto* qPointer = Q.memptr();

		bool wSpecified = !W.is_empty();

		// When W is not specified, we require
		// the point-sets to have an equal number
		// of points.
		ENSURE2(
			wSpecified || 
			(fromSet.n_cols == toSet.n_cols), 
			fromSet.n_cols, toSet.n_cols);

		// Orientation can not be forced when 'scaling' is Free.
		ENSURE(!(scaling == LsAffine_Scaling::Free && orientation != 0));

		// Orientation can not be forced when 'matrix' is Identity.
		ENSURE(
			orientation == 0 ||
			matrix != LsAffine_Matrix::Identity ||
			(scaling == LsAffine_Scaling::Rigid && orientation == 1));

		if (d == 0 || m == 0 || n == 0)
		{
			return {};
		}

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
					P * W * arma::ones(n, 1) / totalWeight;
		        toCentroid = 
		        	R * W.t() * (arma::ones(m, 1) / totalWeight);
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
		    arma::syl(S, PP, PP.t(), -(RP + RP.t()));
		}

		if (scaling == LsAffine_Scaling::Free && 
			matrix == LsAffine_Matrix::Free)
		{
		    // f(x) = Ax
		    
		    // Compute the optimal linear transformation.
		    //[UP, UR, X, DP, DR] = arma::gsvd(PP, RP);
		    //A = UR * (DR * arma::pinv(DP)) * UP.t();
		    arma::Mat<Real> A = RP * arma::pinv(PP);
		    
		    // Compute Q and S from A such that
		    // A = QS and S is symmetric positive semi-definite.
		    arma::Mat<Real> U, V;
			arma::Col<Real> s;
		    arma::svd(U, s, V, A);
		    
		    Q = U * V.t();
		    S = V * arma::diagmat(s) * V.t();
		}

		if (scaling != LsAffine_Scaling::Free && 
			matrix == LsAffine_Matrix::Free)
		{
		    // f(x) = sQx
		    
		    // Compute the optimal orthogonal transformation.
		    arma::Mat<Real> U, V;
			arma::Col<Real> s;
		    unused(s);

		    arma::svd(U, s, V, RP);

		    // This is optimal if orientation is not
		    // restricted.
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

		if (scaling == LsAffine_Scaling::Conformal)
		{
		    // f(x) = sQx
		    
		    // Compute the optimal scaling parameter.
		    S *= arma::trace(Q.t() * RP) / arma::trace(PP);
		}

		if (translation == LsAffine_Translation::Free)
		{
		    // Compute the optimal translation.
		    t = toCentroid - Q * S * fromCentroid;
		}

		ENSURE(qPointer == Q.memptr());

		return {std::move(Q), std::move(S), std::move(t)};
	}

}


#endif
