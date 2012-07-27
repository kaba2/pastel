// Description: Gradient field for noise algorithms

#ifndef PASTEL_GRADIENTFIELD_H
#define PASTEL_GRADIENTFIELD_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

#include <vector>

namespace Pastel
{

		template <typename Real, int N = 2>
		class GradientField
		{
		public:
			static GradientField<Real, N>& create();

			Real operator()(
				const Vector<integer, N>& position,
				const Vector<Real, N>& delta) const;

		private:
			GradientField();
			explicit GradientField(integer n);
			~GradientField();

			void initialize(integer n);

			GradientField(const GradientField& that) PASTEL_DELETE;
			GradientField<Real, N>& operator=(
				const GradientField& that) PASTEL_DELETE;

			std::vector<integer> permutation_;
			integer permutationMask_;
			Real normalizationFactor_;
		};

		template <typename Real, int N>
		GradientField<Real, N>& gradientField();

}

#include "pastel/gfx/gradientfield.hpp"

#endif
