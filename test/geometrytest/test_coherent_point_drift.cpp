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
			testTranslation<float>();
			testTranslation<double>();

			testRotation<float>();
			testRotation<double>();
		}

		struct Report
		{
			template <typename State>
			void operator()(State&& state)
			{
				std::cout << "Q" << std::endl;
				std::cout << state.Q << std::endl;

				std::cout << "S" << std::endl;
				std::cout << state.S << std::endl;

				std::cout << "t" << std::endl;
				std::cout << state.t << std::endl;

				std::cout << "sigma2" << std::endl;
				std::cout << state.sigma2 << std::endl;

				std::cout << "W" << std::endl;
				std::cout << state.W << std::endl;
			};
		};

		template <typename Real>
		void testRotation()
		{
			arma::Mat<Real> S(2, 2, arma::fill::eye);
			arma::Col<Real> t(2, arma::fill::zeros);

			arma::Mat<Real> fromSet = 
			{
				{0, 1, 0},
				{0, 0, 1} 
			};

			// Angle pi / 3 often matches to another minimum.
			Real maxAngle = constantPi<Real>() / 4;
			for (Real alpha = 0; alpha <= maxAngle; alpha += maxAngle / 10)
			{
				arma::Mat<Real> Q = 
				{
					{std::cos(alpha), -std::sin(alpha)},
					{std::sin(alpha), std::cos(alpha)}
				};

				testCase(
					Q, S, t, fromSet,
					{Cpd_Matrix::Free},
					{Cpd_Scaling::Rigid, Cpd_Scaling::Conformal, Cpd_Scaling::Free},
					{Cpd_Translation::Free, Cpd_Translation::Identity});
			}
		}

		template <typename Real>
		void testTranslation()
		{
			arma::Mat<Real> Q(2, 2, arma::fill::eye);
			arma::Mat<Real> S(2, 2, arma::fill::eye);

			arma::Mat<Real> fromSet = 
			{
				{0, 1, 0},
				{0, 0, 1} 
			};

			// Angle pi / 3 often matches to another minimum.
			Real offsetMin = -2;
			Real offsetMax = 2;
			for (Real alpha = offsetMin; alpha <= offsetMax; alpha += (offsetMax - offsetMin) / 10)
			{
				arma::Col<Real> t = 
				{
					alpha, alpha
				};

				testCase(
					Q, S, t, fromSet,
					{Cpd_Matrix::Free, Cpd_Matrix::Identity},
					{Cpd_Scaling::Rigid, Cpd_Scaling::Conformal, Cpd_Scaling::Free},
					{Cpd_Translation::Free});
			}
		}

		template <typename Real>
		void testCase(
			arma::Mat<Real> Q,
			arma::Mat<Real> S,
			arma::Col<Real> t,
			arma::Mat<Real> fromSet,
			std::initializer_list<Cpd_Matrix> matrixSet,
			std::initializer_list<Cpd_Scaling> scalingSet,
			std::initializer_list<Cpd_Translation> translationSet)
		{
			Real threshold = 1e-5;

			arma::Mat<Real> toSet = Q * S * fromSet + 
				t * arma::ones<arma::Mat<Real>>(1, fromSet.n_cols);

			TEST_ENSURE_OP(fromSet.n_cols, ==, 3);
			TEST_ENSURE_OP(fromSet.n_rows, ==, 2);

			Cpd_Return<Real> match;
			auto deltaNorm = [&]()
			{
				arma::Mat<Real> delta = 
					(match.Q * match.S * fromSet + match.t * arma::ones<arma::Mat<Real>>(1, fromSet.n_cols)) - toSet;

				return arma::norm(delta, "inf");
			};

			for (auto scaling : scalingSet)
			{
				for (auto translation : translationSet)
				{
					for (auto matrix : matrixSet)
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

						TEST_ENSURE_OP(deltaNorm(), <, threshold);
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