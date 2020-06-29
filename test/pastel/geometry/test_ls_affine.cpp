// Description: Testing for least-squares transformations
// DocumentationOf: ls_affine.h

#include "test/test_init.h"

#include <pastel/geometry/pattern_matching/ls_affine.h>
#include <pastel/math/sampling/random_orthogonal.h>
#include <pastel/sys/random.h>
#include <pastel/math/matrix/random_matrix.h>

namespace
{

	template <typename Real>
	void testRandom()
	{
		integer trials = 400;
		Real threshold = 
			std::is_same<Real, float>::value ? 1e-3 : 1e-11;

		// Randomly chosen cases.

		integer fails = 0;
		for (integer k = 0; k < trials;++k)
		{
			integer d = randomInteger(10) + 1;
			integer n = randomInteger(100) + 10;
			Matrix<Real> W = Matrix<Real>::Identity(n, n);
			MatrixView<Real> Ws(W.data(), W.rows(), W.cols());

			// Note that this is a multiple of the
			// identity matrix, and so should not affect
			// the solution at all. Note that if you
			// use other types of W matrices, then
			// you don't recover Q, S, and t, because
			// the distance-measure is different.
			W *= random<Real>() * 10;

			LsAffine_Matrix matrix = randomElement({
				LsAffine_Matrix::Free, 
				LsAffine_Matrix::Identity});

			LsAffine_Scaling scaling = randomElement({
				LsAffine_Scaling::Free, 
				LsAffine_Scaling::Diagonal,
				LsAffine_Scaling::Conformal, 
				LsAffine_Scaling::Rigid});

			LsAffine_Translation translation = randomElement({
				LsAffine_Translation::Free, 
				LsAffine_Translation::Identity});

			integer orientation = randomElement({-1, 0, 1});

			if (scaling == LsAffine_Scaling::Free || 
				(matrix == LsAffine_Matrix::Identity &&
				scaling != LsAffine_Scaling::Rigid))
			{
				// Orientation can not be forced when scaling is free or
				// the matrix Q is identity and scaling is not rigid.
				orientation = 0;
			}

			if (matrix == LsAffine_Matrix::Free &&
				scaling == LsAffine_Scaling::Diagonal)
			{
				// This is not implemented.
				continue;
			}

			if (orientation < 0 &&
				matrix == LsAffine_Matrix::Identity &&
				scaling == LsAffine_Scaling::Rigid &&
				even(d))
			{
				// When Q = I and S = +/- I, a negative
				// det(QS) is only possible in odd dimensions.
				orientation = randomElement({0, 1});
			}

			Matrix<Real> Q = Matrix<Real>::Identity(d, d);

			if (matrix == LsAffine_Matrix::Free)
			{
				Q = randomOrthogonal<Real>(d, 
					PASTEL_TAG(orientation), orientation);
			}

			Matrix<Real> S = Matrix<Real>::Identity(d, d);
			if (scaling == LsAffine_Scaling::Free)
			{
				if (matrix == LsAffine_Matrix::Identity) {
					// This case is not implemented.
					continue;
				}

				S = randomGaussianMatrix<Real>(d, d);
				S = S + S.transpose();
				if (matrix == LsAffine_Matrix::Free)
				{
					Eigen::JacobiSVD<Matrix<Real>> svd(Q * S, Eigen::ComputeThinU | Eigen::ComputeThinV);
					const auto& U = svd.matrixU();
					const auto& V = svd.matrixV();
					const auto& D = svd.singularValues();
					Q = U * V.transpose();
					S = V * diagonalMatrix(D) * V.transpose();
				}
			}

			if (scaling == LsAffine_Scaling::Conformal)
			{
				S *= random<Real>() * 10 + 1;
			}

			if (scaling == LsAffine_Scaling::Diagonal)
			{
				for (integer i = 0; i < d; ++i)
				{
					S(i, i) *= random<Real>() * 10 + 1;
				}	
			}

			ColMatrix<Real> t = ColMatrix<Real>::Zero(d, 1);
			if (translation == LsAffine_Translation::Free)
			{
				t = randomGaussianMatrix<Real, Dynamic, 1>(d, 1) * 10;
			}

			if (orientation < 0)
			{
				if (matrix == LsAffine_Matrix::Identity)
				{
					ASSERT(scaling != LsAffine_Scaling::Diagonal);
					ASSERT(scaling != LsAffine_Scaling::Free);

					if (scaling == LsAffine_Scaling::Rigid)
					{
						S = -S;
					}
				}
				else
				{
					// The orientation has already been
					// handled for the free case in the
					// generation of Q.
				}
			}

			// Generate test point-sets.
			Matrix<Real> P = randomGaussianMatrix<Real>(d, n);
			Matrix<Real> R = (Q * S * P).colwise() + t;

			Matrix<Real> QE(d, d);
			Matrix<Real> SE(d, d);
			ColMatrix<Real> tE(d);

			// Compute the transformation back by least-squares.
			lsAffine(
				view(P), view(R),
				view(QE), view(SE), view(tE),
				PASTEL_TAG(orientation), orientation,
				PASTEL_TAG(matrix), matrix,
				PASTEL_TAG(scaling), scaling,
				PASTEL_TAG(translation), translation,
				PASTEL_TAG(W), Ws);

			// Check that the errors are small.
			Real qError = maxNorm(QE - Q);
			Real sError = maxNorm(SE - S);
			Real tError = maxNorm(tE - t);

			if (std::max(std::max(qError, sError), tError) > threshold ||
				(orientation != 0 && sign(determinant(QE * SE)) != sign(orientation)))
			{
				std::cout << orientation << " " 
					<< (integer)matrix << " "
					<< (integer)scaling << " " 
					<< (integer)translation << std::endl;

				std::cout 
					<< "Q-error " << qError << std::endl 
					<< "S-error " << sError << std::endl
					<< "t-error " << tError << std::endl
					<< "threshold " << threshold << std::endl;

				std::cout << "Q" << Q << std::endl;
				std::cout << "S" << S << std::endl;
				std::cout << "t" << t << std::endl;

				std::cout << "QE" << QE << std::endl;
				std::cout << "SE" << SE << std::endl;
				std::cout << "tE" << tE << std::endl;

				fails = fails + 1;
			}
		}

		REQUIRE(fails == 0);
	}

}

TEST_CASE("lsAffine (lsAffine)")
{
	testRandom<float>();
	testRandom<double>();
}
