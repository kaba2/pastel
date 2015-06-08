// Description: Least-squares affine transformation between point-sets.
// Documentation: ls_transformations.txt

#ifndef PASTELGEOMETRY_LS_AFFINE_H
#define PASTELGEOMETRY_LS_AFFINE_H

#include "pastel/sys/math/sign.h"

#include <armadillo>

namespace Pastel
{

	enum class LsAffine_Matrix
	{
		Free,
		Identity
	};

	enum class LsAffine_Scaling
	{
		Free,
		Conformal,
		Rigid
	};

	enum class LsAffine_Translation
	{
		Free,
		Identity
	};

	template <typename Real>
	struct LsAffine_Return
	{
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

	W ((m x n)-matrix): 
	A non-negative matrix, which contains the weights for the 
	least-squares error metric. If it is not given, then 
	it is required that m = n, and it is assumed that 
	W = eye(m, n). 
	*/
	template <
		typename Real,
		typename... ArgumentSet>
	LsAffine_Return<Real> lsAffine(
		const arma::Mat<Real>& fromSet,
		const arma::Mat<Real>& toSet,
		ArgumentSet&&... argumentSet)
	{
		// Least-Squares Transformations between Point-Sets_,
		// Kalle Rutanen, Germán Gómez-Herrero, 
		// Sirkka-Liisa Eriksson, Karen Egiazarian, 
		// 18th Scandinavian Conference on Image Analysis, 
		// pp.501-511, June 17-20, 2013.

		// Check equal dimension of point-sets.
		ENSURE_OP(fromSet.n_rows, ==, toSet.n_rows);

		integer d = fromSet.n_rows;
		integer m = fromSet.n_cols;
		integer n = toSet.n_cols;

		LsAffine_Translation translation =
			PASTEL_ARG_S(translation, LsAffine_Translation::Free);
		LsAffine_Scaling scaling =
			PASTEL_ARG_S(scaling, LsAffine_Scaling::Free);
		LsAffine_Matrix matrix =
			PASTEL_ARG_S(matrix, LsAffine_Matrix::Free);
		integer orientation =
			PASTEL_ARG_S(orientation, (integer)0);
		auto&& W = 
			PASTEL_ARG_S(W, arma::eye(m, n));

		enum : bool 
		{
			wSpecified =
				PASTEL_ARG_S_MATCHES(W)
		};

		// Defaults for Q and S.
		arma::Mat<Real> Q = arma::eye(d, d);
		arma::Mat<Real> S = arma::eye(d, d);
		arma::Mat<Real> t = arma::zeros(d, 1);

		arma::Mat<Real> P = fromSet;
		arma::Mat<Real> R = toSet;

		// When W is not specified, we required
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

		Real totalWeight = n;
		if (wSpecified)
		{
			totalWeight = arma::accu(W);
		}

		ENSURE(!negative(totalWeight));

		arma::Mat<Real> fromCentroid(n, 1);
		arma::Mat<Real> toCentroid(m, 1);

		if (translation == LsAffine_Translation::Free)
		{
			if (wSpecified)
			{
				// With weighting.
				fromCentroid = 
					P * W * arma::ones(n, 1) / totalWeight;
		        toCentroid = 
		        	Q * W.t() * (arma::ones(m, 1) / totalWeight);
			}
	    	else
	    	{
	        	// Without weighting.
	        	fromCentroid = arma::sum(P, 1) / m;
	        	toCentroid = arma::sum(Q, 1) / n;
			}
		    
		    // Form the centered point-sets. The optimal transformation
		    // will map fromCentroid to toCentroid. After this the problem
		    // has been reduced from affine to linear.
		    P -= fromCentroid * arma::ones(1, m);
		    R -= toCentroid * arma::ones(1, n);
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
		    S = arma::syl(PP, PP.t(), -(RP + RP.t()));
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
		    arma::svd(U, s, V, RP);

		    unused(s);

		    if (orientation != 0 &&
		    	sign(arma::det(Q)) != sign(orientation))
		    {
		        // If orientation is to be forced at det(A) = g, where g = +- 1,
		        // then A is given by:
		        //
		        //    Q = UDV^T, where
		        //    D = [1, ..., 1, g det(UV^T)].

		        arma::Col<Real> s = arma::ones<arma::Col<Real>>(d);
		        s(d - 1) = -1;

		        // Compute the optimal oriented orthogonal Q.
		        Q = U * arma::diagmat(s) * V.t();
		    }
		    else
		    {
		    	// Either orientation is correct or it is 
		    	// not enforced.
			    Q = U * V.t();
		    }
		}

		if (scaling == LsAffine_Scaling::Conformal)
		{
		    // f(x) = sQx
		    
		    // Compute the optimal scaling parameter.
		    Real s = arma::trace(Q.t() * RP) / arma::trace(PP);
		    S = s * arma::eye(d, d);
		}

		if (translation == LsAffine_Translation::Free)
		{
		    // Compute the optimal translation.
		    t = toCentroid - Q * S * fromCentroid;
		}

		return {Q, S, t};
	}

}


#endif
