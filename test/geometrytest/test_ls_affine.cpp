// Description: Testing for least-squares transformatins
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
			test();
		}

		void test()
		{
			integer trials = 400;
			real threshold = 1e-11;

			// Randomly chosen cases.

			integer fails = 0;
			for (integer k = 0; k < trials;++k)
			{
				integer d = randomInteger(10) + 1;
				integer n = randomInteger(100) + 10;
				arma::Mat<real> W(n, n, arma::fill::eye);
				W *= random<real>() * 10;

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
       
				arma::Mat<real> Q(d, d, arma::fill::eye);

				if (matrix == LsAffine_Matrix::Free)
				{
					Q = randomOrthogonal<real>(d, 
						PASTEL_TAG(orientation), orientation);
				}
    
				arma::Mat<real> S(d, d, arma::fill::eye);
				if (scaling == LsAffine_Scaling::Free)
				{
					S = arma::randn<arma::Mat<real>>(d, d);
					S = S + S.t();
					if (matrix == LsAffine_Matrix::Free)
					{
						arma::Mat<real> U, V;
						arma::Col<real> D;
						arma::svd(U, D, V, Q * S);
						Q = U * V.t();
						S = V * arma::diagmat(D) * V.t();
					}
				}

				if (scaling == LsAffine_Scaling::Conformal)
				{
					S *= random<real>() * 5;
				}
    
				arma::Col<real> t(d, arma::fill::zeros);
				if (translation == LsAffine_Translation::Free)
				{
					t = arma::randn(d, 1) * 10;
				}

				// Generate test point-sets.
				arma::Mat<real> P = arma::randn<arma::Mat<real>>(d, n);
				arma::Mat<real> R = Q * S * P + t * arma::ones<arma::Mat<real>>(1, n);

				// Compute the transformation back by least-squares.
				auto lsMatch = lsAffine(
					P, R,
					PASTEL_TAG(orientation), orientation,
					PASTEL_TAG(matrix), matrix,
					PASTEL_TAG(scaling), scaling,
					PASTEL_TAG(translation), translation,
					PASTEL_TAG(W), W);

				auto&& QE = lsMatch.Q;
				auto&& SE = lsMatch.S;
				auto&& tE = lsMatch.t;
    
				// Check that the errors are small.
				real qError = arma::norm(QE - Q, "fro");
				real sError = arma::norm(SE - S, "fro");
				real tError = arma::norm(tE - t, "fro");

				if (std::max(std::max(qError, sError), tError) > threshold ||
				   (orientation != 0 && sign(arma::det(QE * SE)) != sign(orientation)))
				{
					std::cout << orientation << " " << (integer)matrix << " " << (integer)scaling << " " << (integer)translation << std::endl;

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
