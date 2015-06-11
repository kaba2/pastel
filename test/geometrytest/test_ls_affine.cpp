// Description: Testing for least-squares transformations
// DocumentationOf: ls_affine.h

#include "test_pastelgeometry.h"

#include <pastel/geometry/pattern_matching/ls_affine.h>
#include <pastel/math/sampling/random_orthogonal.h>
#include <pastel/sys/random.h>

using namespace Pastel;

namespace
{

	class Test
		: public TestSuite
	{
	public:
		Test()
			: TestSuite(&testReport())
		{
		}

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
				std::is_same<Real, float>::value ? 1e-4 : 1e-12;

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
					LsAffine_Scaling::Conformal, 
					LsAffine_Scaling::Rigid});

				LsAffine_Translation translation = randomElement({
					LsAffine_Translation::Free, 
					LsAffine_Translation::Identity});

				integer orientation = randomElement({-1, 0, 1});
    
				if (scaling == LsAffine_Scaling::Free || 
					matrix == LsAffine_Matrix::Identity)
				{
					// Orientation can not be forced when scaling is free or
					// the matrix Q is identity.
					orientation = 0;
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
					S *= random<Real>() * 5;
				}
    
				arma::Col<Real> t(d, arma::fill::zeros);
				if (translation == LsAffine_Translation::Free)
				{
					t = arma::randn<arma::Col<Real>>(d, 1) * 10;
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

				TEST_ENSURE(qePointer == QE.memptr());
				TEST_ENSURE(sePointer == SE.memptr());
				TEST_ENSURE(tePointer == tE.memptr());

				// Check that the errors are small.
				Real qError = arma::norm(QE - Q, "inf");
				Real sError = arma::norm(SE - S, "inf");
				Real tError = arma::norm(tE - t, "inf");

				if (std::max(std::max(qError, sError), tError) > threshold ||
				   (orientation != 0 && sign(arma::det(QE * SE)) != sign(orientation)))
				{
					//std::cout << orientation << " " << (integer)matrix << " " << (integer)scaling << " " << (integer)translation << std::endl;

					fails = fails + 1;
				}
			}

			TEST_ENSURE_OP(fails, ==, 0);
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("lsAffine", test);
	}

	CallFunction run(addTest);

}
