// Description: Least-squares affine transformation between point-sets.
// Documentation: ls_transformations.txt

#ifndef PASTELGEOMETRY_LS_AFFINE_H
#define PASTELGEOMETRY_LS_AFFINE_H

#include "pastel/sys/math/sign.h"

#include "pastel/math/matrix.h"

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

	Output
	------

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

	W (Matrix<Real> : Matrix<Real>()): 
	A non-negative (m x n) matrix, which contains the weights 
	for the least-squares error metric. If W is not given, or is
	the empty matrix, then it is required that m = n, and it 
	is assumed that W is the (n x n) identity matrix. 
	*/
	template <
		typename Real,
		typename... ArgumentSet>
	void lsAffine(
		const MatrixView<Real>& fromSet,
		const MatrixView<Real>& toSet,
		const MatrixView<Real>& Qs,
		const MatrixView<Real>& Ss,
		const MatrixView<Real>& ts,
		ArgumentSet&&... argumentSet)
	{
		// Least-Squares Transformations between Point-Sets_,
		// Kalle Rutanen, Germán Gómez-Herrero, 
		// Sirkka-Liisa Eriksson, Karen Egiazarian, 
		// 18th Scandinavian Conference on Image Analysis, 
		// pp.501-511, June 17-20, 2013.

		ENSURE_OP(fromSet.rows(), ==, toSet.rows());

		MapMatrix<Real> P(fromSet.data(), fromSet.rows(), fromSet.cols());
		MapMatrix<Real> R(toSet.data(), toSet.rows(), toSet.cols());

		integer d = P.rows();
		integer m = P.cols();
		integer n = R.cols();

		ENSURE_OP(d, >, 0);
		ENSURE_OP(m, >, 0);
		ENSURE_OP(n, >, 0);

        ENSURE_OP(Qs.rows(), ==, d);
        ENSURE_OP(Qs.cols(), ==, d);
        ENSURE_OP(Ss.rows(), ==, d);
        ENSURE_OP(Ss.cols(), ==, d);
        ENSURE_OP(ts.rows(), ==, d);
        ENSURE_OP(ts.cols(), ==, 1);

		LsAffine_Matrix matrix =
			PASTEL_ARG_ENUM(matrix, LsAffine_Matrix::Free);
		LsAffine_Scaling scaling =
			PASTEL_ARG_ENUM(scaling, LsAffine_Scaling::Free);
		LsAffine_Translation translation =
			PASTEL_ARG_ENUM(translation, LsAffine_Translation::Free);
		integer orientation =
			PASTEL_ARG_S(orientation, (integer)1);
		MatrixView<Real> Ws = 
			PASTEL_ARG_S(W, MatrixView<Real>());

        MapMatrix<Real> Q(Qs.data(), d, d);
        MapMatrix<Real> S(Ss.data(), d, d);
        MapColMatrix<Real> t(ts.data(), d, 1);
        MapMatrix<Real> W(Ws.data(), Ws.rows(), Ws.cols());

		// Initialize Q, S, and t.
		Q = Matrix<Real>::Identity(d, d);
		S = Matrix<Real>::Identity(d, d);
		t = ColMatrix<Real>::Zero(d, 1);

		bool wSpecified = !Ws.isEmpty();

		// When W is not specified, we require
		// the point-sets to have an equal number
		// of points.
		ENSURE2(
			wSpecified || 
			(P.cols() == R.cols()), 
			P.cols(), R.cols());

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

		// This case is not implemented, because
		// Eigen does not have a Sylvester-equation solver.
		ENSURE(!(scaling == LsAffine_Scaling::Free && 
			matrix == LsAffine_Matrix::Identity));

		// When Q = I and S = +/- I, a negative
		// det(QS) is only possible in odd dimensions.
		ENSURE(!(orientation < 0 &&
			matrix == LsAffine_Matrix::Identity &&
			scaling == LsAffine_Scaling::Rigid &&
			even(d)));

		Real totalWeight = n;
		if (wSpecified)
		{
			totalWeight = W.sum();
		}

		ENSURE(!negative(totalWeight));

		ColMatrix<Real> fromCentroid(d, 1);
		ColMatrix<Real> toCentroid(d, 1);

		if (translation == LsAffine_Translation::Free)
		{
			if (wSpecified)
			{
				// With weighting.
				fromCentroid = 
					(P * W).rowwise().sum() / totalWeight;
		        toCentroid = 
		        	(R * W.transpose()).rowwise().sum() / totalWeight;
			}
	    	else
	    	{
	        	// Without weighting.
	        	fromCentroid = P.rowwise().sum() / m;
	        	toCentroid = R.rowwise().sum() / n;
			}
		 
		    // Form the centered point-sets. The optimal transformation
		    // will map fromCentroid to toCentroid. After this the problem
		    // has been reduced from affine to linear.
		    P.colwise() -= fromCentroid;
		    R.colwise() -= toCentroid;
		}

		Matrix<Real> PP(d, d);
		Matrix<Real> RP(d, d);

		if (wSpecified)
		{
		    // With weighting.
		    PP = P * diagonalMatrix(asVector(W.rowwise().sum())) * P.transpose();
		    RP = R * W.transpose() * P.transpose();
		}
		else
		{
		    // Without weighting.
		    PP = P * P.transpose();
		    RP = R * P.transpose();
		}

		if (scaling == LsAffine_Scaling::Free && 
			matrix == LsAffine_Matrix::Identity)
		{
		    // f(x) = Sx
		 
		    // Find the optimal scaling; solve:
			// PP^T S + S PP^T = RP^T + PR^T

			// Not implemented, since Eigen is missing
			// a solver for Sylvester equations.

			// Forced oriented solution would have det(QS) < 0;
			// oriented solution is not implemented.
			ASSERT_OP(orientation, ==, 0);
		}

		if (scaling == LsAffine_Scaling::Free && 
			matrix == LsAffine_Matrix::Free)
		{

		    // f(x) = Ax
		 
		    // Compute the optimal linear transformation.
		    // [UP, UR, X, DP, DR] = gsvd(PP, RP);
		    // A = UR * (DR * pinv(DP)) * UP.transpose();
		    Matrix<Real> pinvPP = PP.completeOrthogonalDecomposition().pseudoInverse();
			Matrix<Real> A = RP * pinvPP;

			// Compute Q and S from A such that
			// A = QS and S is symmetric positive semi-definite.
			Eigen::JacobiSVD<Matrix<Real>> svd(A, Eigen::ComputeThinU | Eigen::ComputeThinV);
			const auto& U = svd.matrixU();
			const auto& V = svd.matrixV();
			const auto& s = svd.singularValues();
			Q = U * V.transpose();
			S = V * diagonalMatrix(s) * V.transpose();

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

			Eigen::JacobiSVD<Matrix<Real>> svd(RP, Eigen::ComputeThinU | Eigen::ComputeThinV);
			const auto& U = svd.matrixU();
			const auto& V = svd.matrixV();

		    // Compute the optimal non-oriented orthogonal Q.
			Q = U * V.transpose();

		    if (orientation != 0 &&
		    	sign(determinant(Q)) != sign(orientation))
		    {
		        // If orientation is to be forced at det(A) = g, where g = +- 1,
		        // then A is given by:
		        //
		        //    Q = UDV^T, where
		        //    D = [1, ..., 1, g det(UV^T)].

		        ColMatrix<Real> s = ColMatrix<Real>::Ones(d, 1);
		        s(d - 1) = -1;

		        // Compute the optimal oriented orthogonal Q.
		        Q = U * diagonalMatrix(s) * V.transpose();
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
			S.diagonal() = RP.diagonal().array() / PP.diagonal().array();

			// Compute det(QS) = det(S).
			Real sDet = S.diagonal().prod();

			if (orientation != 0 &&
				sign(sDet) != sign(orientation))
			{
				// From the form of the error functional
				// we see that we can obtain the solution
				// of the oriented problem by negating that
				// diagonal element of S for which 
				// S_{ii} (RP^T)_{ii} is the smallest.
				
				ColMatrix<Real> product = S.diagonal() * RP.diagonal();

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
			Real s = (Q.transpose() * RP).trace() / PP.trace();
			S *= s;

			if (matrix == LsAffine_Matrix::Free)
			{
				if (orientation != 0)
				{
					// The orientation has already been handled
					// in the selection of Q.
					ASSERT(sign(determinant(Q * S)) == sign(orientation));
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
	}

}

#endif
