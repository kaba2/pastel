// Description: Testing for coherent point-drift
// DocumentationOf: coherent_point_drift.h

#include "test/test_init.h"

#include <pastel/geometry/pattern_matching/coherent_point_drift.h>
#include <pastel/math/sampling/random_orthogonal.h>
#include <pastel/sys/random.h>

namespace
{

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
	void testCase(
		MatrixView<Real> Ps,
		MatrixView<Real> Qs,
		MatrixView<Real> Ss,
		MatrixView<Real> ts,
		std::initializer_list<Cpd_Matrix> matrixSet,
		std::initializer_list<Cpd_Scaling> scalingSet,
		std::initializer_list<Cpd_Translation> translationSet)
	{
		Real threshold = 1e-4;

		MapMatrix<Real> P(Ps.data(), Ps.rows(), Ps.cols());
		MapMatrix<Real> Q(Qs.data(), Qs.rows(), Qs.cols());
		MapMatrix<Real> S(Ss.data(), Ss.rows(), Ss.cols());
		MapColMatrix<Real> t(ts.data(), ts.rows(), ts.cols());

		Matrix<Real> R = (Q * S * P).colwise() + t;
		MatrixView<Real> Rs(R.data(), R.rows(), R.cols());

		REQUIRE(Ps.rows() == 2);

		auto deltaNorm = [&]()
		{
			Matrix<Real> delta = 
				((Q * S * P).colwise() + t) - R;

			return maxNorm(delta);
		};

		RANGES_FOR(auto scaling, scalingSet)
		{
			RANGES_FOR(auto translation, translationSet)
			{
				RANGES_FOR(auto matrix, matrixSet)
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

					coherentPointDrift(
						Ps, Rs, Qs, Ss,	ts,
						PASTEL_TAG(scaling), scaling,
						PASTEL_TAG(translation), translation,
						PASTEL_TAG(matrix), matrix,
						PASTEL_TAG(orientation), orientation);

					if (deltaNorm() >= threshold)
					{
						std::cout << "Matrix " << (integer)matrix
							<< " Scaling " << (integer)scaling
							<< " Orientation " << (integer)orientation
							<< " Translation " << (integer)translation
							<< std::endl;
					}

					REQUIRE(deltaNorm() < threshold);
				}
			}
		}
	}

	template <typename Real>
	void testRotation()
	{
		Matrix<Real> S = Matrix<Real>::Identity(2, 2);
		ColMatrix<Real> t = Matrix<Real>::Zero(2, 1);

		Matrix<Real> P(2, 3);
		P <<
			0, 1, 0,
			0, 0, 1;

		// Angle pi / 3 often matches to another minimum.
		Real maxAngle = constantPi<Real>() / 4;
		for (Real alpha = 0; alpha <= maxAngle; alpha += maxAngle / 10)
		{
			Matrix<Real> Q(2, 2);
			Q <<
				std::cos(alpha), -std::sin(alpha),
				std::sin(alpha), std::cos(alpha);

			testCase(
				view(P), view(Q), view(S), view(t), 
				{Cpd_Matrix::Free},
				{Cpd_Scaling::Rigid, Cpd_Scaling::Conformal, Cpd_Scaling::Free},
				{Cpd_Translation::Free, Cpd_Translation::Identity});
		}
	}

	template <typename Real>
	void testTranslation()
	{
		Matrix<Real> Q = Matrix<Real>::Identity(2, 2);
		Matrix<Real> S = Matrix<Real>::Identity(2, 2);

		Matrix<Real> fromSet(2, 7);
		fromSet <<
			0, 5, 10, 11, 12, 13, 15,
			0, 6, 3, 7, 5, 6, -4;

		Real offsetMin = -1;
		Real offsetMax = 1;
		for (Real alpha = offsetMin; alpha <= offsetMax; alpha += (offsetMax - offsetMin) / 10)
		{
			ColMatrix<Real> t(2, 1);
			t << alpha, alpha;

			testCase(
				view(fromSet), view(Q), view(S), view(t), 
				{Cpd_Matrix::Free, Cpd_Matrix::Identity},
				{Cpd_Scaling::Rigid, Cpd_Scaling::Conformal, Cpd_Scaling::Free},
				{Cpd_Translation::Free});
		}
	}

}

TEST_CASE("translation (coherent_point_drift)")
{
	testTranslation<float>();
	testTranslation<double>();
}

TEST_CASE("rotation (coherent_point_drift)")
{
	testRotation<float>();
	testRotation<double>();
}
