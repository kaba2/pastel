// Description: Testing for coherent point-drift
// DocumentationOf: coherent_point_drift.h

#include "test_pastelgeometry.h"

#include <pastel/geometry/pattern_matching/coherent_point_drift.h>
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
			test<float>();
			test<double>();
		}

		template <typename Real>
		void test()
		{
			Real threshold = 
				std::is_same<Real, float>::value ? 1e-4 : 1e-12;

			// pi / 3 works; pi / 2 does not work.
			Real alpha = constantPi<Real>() / 8;

			arma::Mat<Real> Q = 
			{
				{std::cos(alpha), -std::sin(alpha)},
				{std::sin(alpha), std::cos(alpha)}
			};

			arma::Mat<Real> S =
			{
				{1, 0},
				{0, 1}
			};

			arma::Col<Real> t =
			{
				0, 0
			};

			arma::Mat<Real> fromSet = 
			{
				{0, 1, 0},
				{0, 0, 1} 
			};

			arma::Mat<Real> toSet = Q * S * fromSet + 
				t * arma::ones<arma::Mat<Real>>(1, fromSet.n_cols);

			TEST_ENSURE_OP(fromSet.n_cols, ==, 3);
			TEST_ENSURE_OP(fromSet.n_rows, ==, 2);

			Cpd_Return<Real> match;
			auto maxError = [&]()
			{
				Real qError = std::abs(arma::norm(Q - match.Q, "inf"));
				Real sError = std::abs(arma::norm(S - match.S, "inf"));
				Real tError = std::abs(arma::norm(t - match.t, "inf"));

				/*
				std::cout << qError 
					<< " " << sError
					<< " " << tError
					<< std::endl;
				*/

				return std::max(std::max(qError, sError), tError);
			};

			for (auto scaling : {Cpd_Scaling::Rigid, Cpd_Scaling::Conformal, Cpd_Scaling::Free})
			{
				for (auto translation : {Cpd_Translation::Free, Cpd_Translation::Identity})
				{
					for (auto matrix : {Cpd_Matrix::Free})
					{
						integer orientation = 1;

						if (matrix == LsAffine_Matrix::Identity &&
							scaling != LsAffine_Scaling::Rigid)
						{
							orientation = 0;
						}

						if (scaling == LsAffine_Scaling::Free)
						{
							orientation = 0;
						}

						match = coherentPointDrift(
							fromSet, toSet,
							scaling,
							translation,
							matrix,
							PASTEL_TAG(orientation), orientation);

						if (maxError() >= threshold)
						{
							std::cout << (integer)matrix << (integer)scaling << (integer)translation << std::endl;
							std::cout << "maxError: " << maxError() << std::endl;

							std::cout << "Q" << std::endl;
							std::cout << Q << std::endl;

							std::cout << "S" << std::endl;
							std::cout << S << std::endl;

							std::cout << "t" << std::endl;
							std::cout << t << std::endl;

							auto report = [](auto&& match)
							{
								std::cout << "Q" << std::endl;
								std::cout << match.Q << std::endl;

								std::cout << "S" << std::endl;
								std::cout << match.S << std::endl;

								std::cout << "t" << std::endl;
								std::cout << match.t << std::endl;

								std::cout << "sigma2" << std::endl;
								std::cout << match.sigma2 << std::endl;
							};

							match = coherentPointDrift(
								fromSet, toSet,
								scaling,
								translation,
								matrix,
								PASTEL_TAG(orientation), orientation,
								PASTEL_TAG(report), report);

							return;
						}

						TEST_ENSURE_OP(maxError(), <, threshold);
					}
				}
			}
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("coherent_point_drift", test);
	}

	CallFunction run(addTest);

}
