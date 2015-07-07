// Description: Testing for least-squares transformations
// DocumentationOf: ls_affine.h

#include "test/test_init.h"

#include <pastel/geometry/pattern_matching/ls_affine.h>
#include <pastel/math/sampling/random_orthogonal.h>
#include <pastel/sys/random.h>

namespace
{

	class Test
	{
	public:
		virtual void run()
		{
			testRandom<float>();
			testRandom<double>();
		}

		template <typename Real>
		void testRandom()
		{
			integer trials = 400;
			Real threshold = 
				std::is_same<Real, float>::value ? 1e-4 : 1e-11;

			// Randomly chosen cases.

			integer fails = 0;
			for (integer k = 0; k < trials;++k)
			{
				integer d = randomInteger(10) + 1;
				integer n = randomInteger(100) + 10;
				arma::Mat<Real> W(n, n, arma::fill::eye);
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

				arma::Mat<Real> Q(d, d, arma::fill::eye);

				if (matrix == LsAffine_Matrix::Free)
				{
					Q = randomOrthogonal<Real>(d, 
						PASTEL_TAG(orientation), orientation);
				}

				arma::Mat<Real> S(d, d, arma::fill::eye);
				if (scaling == LsAffine_Scaling::Free)
				{
					S = arma::randn<arma::Mat<Real>>(d, d);
					S = S + S.t();
					if (matrix == LsAffine_Matrix::Free)
					{
						arma::Mat<Real> U, V;
						arma::Col<Real> D;
						arma::svd(U, D, V, Q * S);
						Q = U * V.t();
						S = V * arma::diagmat(D) * V.t();
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

				arma::Col<Real> t(d, arma::fill::zeros);
				if (translation == LsAffine_Translation::Free)
				{
					t = arma::randn<arma::Col<Real>>(d, 1) * 10;
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
				arma::Mat<Real> P = arma::randn<arma::Mat<Real>>(d, n);
				arma::Mat<Real> R = Q * S * P + t * arma::ones<arma::Mat<Real>>(1, n);

				arma::Mat<Real> QE(d, d);
				arma::Mat<Real> SE(d, d);
				arma::Col<Real> tE(d);

				const Real* qePointer = QE.memptr();
				const Real* sePointer = SE.memptr();
				const Real* tePointer = tE.memptr();

				// Compute the transformation back by least-squares.
				auto lsMatch = lsAffine(
					P, R,
					PASTEL_TAG(orientation), orientation,
					matrix,
					scaling,
					translation,
					PASTEL_TAG(W), W,
					PASTEL_TAG(Q0), std::move(QE),
					PASTEL_TAG(S0), std::move(SE),
					PASTEL_TAG(t0), std::move(tE));

				QE = std::move(lsMatch.Q);
				SE = std::move(lsMatch.S);
				tE = std::move(lsMatch.t);

				REQUIRE(qePointer == QE.memptr());
				REQUIRE(sePointer == SE.memptr());
				REQUIRE(tePointer == tE.memptr());

				// Check that the errors are small.
				Real qError = arma::norm(QE - Q, "inf");
				Real sError = arma::norm(SE - S, "inf");
				Real tError = arma::norm(tE - t, "inf");

				if (std::max(std::max(qError, sError), tError) > threshold ||
				   (orientation != 0 && sign(arma::det(QE * SE)) != sign(orientation)))
				{
					/*
					std::cout << orientation << " " 
						<< (integer)matrix << " "
						<< (integer)scaling << " " 
						<< (integer)translation << std::endl;

					std::cout << qError << " " 
						<< sError << " "
						<< tError << std::endl;

					std::cout << "Q" << Q << std::endl;
					std::cout << "S" << S << std::endl;
					std::cout << "t" << t << std::endl;

					std::cout << "QE" << QE << std::endl;
					std::cout << "SE" << SE << std::endl;
					std::cout << "tE" << tE << std::endl;
					*/

					fails = fails + 1;
				}
			}

			REQUIRE(fails == 0);
		}
	};

	TEST_CASE("lsAffine", "[lsAffine]")
	{
	}

}
