#ifndef PASTELGFX_GRADIENTFIELD_HPP
#define PASTELGFX_GRADIENTFIELD_HPP

#include "pastel/gfx/gradientfield.h"

#include "pastel/sys/ensure.h"
#include "pastel/sys/random_uniform.h"

namespace Pastel
{

		template <typename Real, int N>
		GradientField<Real, N>&
			GradientField<Real, N>::create()
		{
			static GradientField<Real, N> thePerlinNoise;
			return thePerlinNoise;
		}

		template <typename Real, int N>
		GradientField<Real, N>::GradientField()
			: permutation_()
			, permutationMask_(0)
			, normalizationFactor_(0)
		{
			PASTEL_STATIC_ASSERT(N != Dynamic);

			initialize(N);
		}

		template <typename Real, int N>
		GradientField<Real, N>::GradientField(integer n)
			: permutation_()
			, permutationMask_(0)
			, normalizationFactor_(0)
		{
			ENSURE(N == Dynamic || N == n);

			initialize(n);
		}

		template <typename Real, int N>
		void GradientField<Real, N>::initialize(integer n)
		{
			integer basicGradients = (1 << (n - 1));

			const integer gradients = n * basicGradients;
			
			// The size of the permutation:
			//
			// * must be a power of two so that the modulo operation 
			// can be replaced by a bit-wise and. 
			//
			// * must be larger than the number of gradients
			// to be able to generate all of them.
			//
			// * must be at least 256 (arbitrarily) because this size 
			// also determines the size of the tile that is to be 
			// repeated (in RR^n, the size of the tile is 
			// [0, permutationSize[^n).

			integer permutationSize = 
				std::max(roundUpToPowerOfTwo(gradients), (integer)256);
			
			// Generate the standard permutation.

			permutation_.reserve(permutationSize * 2);
			for (integer i = 0;i < permutationSize;++i)
			{
				permutation_.push_back(i);
			}

			// Shuffle it to a random permutation.
			for (integer i = 0;i < permutationSize;++i)
			{
				integer u = randomInteger(permutationSize);
				std::swap(permutation_[u], permutation_[i]);
			}

			// Repeat the permutation table so
			// we needn't use modulus later
			for (integer i = 0;i < permutationSize;++i)
			{
				permutation_.push_back(permutation_[i]);
			}

			// The dot products in the contributions of the vertices
			// use un-normalized gradient vectors with length sqrt(n - 1).
			// We take care of the normalization by dividing the
			// result with that value. The 1-d case is not scaled because
			// it uses a different set of gradients.

			normalizationFactor_ = (n == 1) ? 1 : inverse(std::sqrt((Real)(n - 1)));
		}

		// Private

		template <typename Real, int N>
		Real GradientField<Real, N>::operator()(
			const Vector<integer, N>& position,
			const Vector<Real, N>& delta) const
		{
			integer n = position.size();

			if (n == 1)
			{
				// The 1-dimensional gradient field has to be handled
				// as a special case. The reasons are that in RR^1:
				//

				// * there are no gradients of the form which we
				// are going to pick.
				//
				// * a small number of gradients will not do.
				//
				// There has to be a larger number of gradients
				// to make the noise interesting. The difference
				// to higher dimensions is the number of surrounding
				// cube points. In nD, if there are m different
				// gradients, then there are m^{2^n} different kinds of
				// cubes. Thus, in 2D, 4 gradients are able to generate
				// 4^4 = 256 different kinds of cubes. But, in 1D,
				// 4 gradients generate only 4^2 = 16 different kinds
				// of cubes (intervals).
				//
				// Let us target 256 different kinds of cubes,
				// that is, 16 different gradients.

				static PASTEL_CONSTEXPR int Gradients = 1 << 4;
				static PASTEL_CONSTEXPR uint32 GradientMask = Gradients - 1;

				const integer index = permutation_[position[0] & permutationMask_];
				const Real gradient = (((Real)(2 * (index & GradientMask))) / (Gradients - 1)) - 1;

				return delta[0] * gradient;
			}
			else if (n == 2)
			{
				// The 2-dimensional gradient field also has to be
				// handled a special case. We want to choose gradient
				// vectors from the set {-1, 1}^2.

				static PASTEL_CONSTEXPR int Gradients = 1 << 3;
				static PASTEL_CONSTEXPR uint32 GradientMask = Gradients - 1;

				integer index = 

					permutation_[(position[1] & permutationMask_) +
					permutation_[position[0] & permutationMask_]];
				const uint32 gradient = ((uint32)index) & GradientMask;

				Real dotProduct = 0;
				switch(gradient)
				{
				case 0:
					// (1, 0)
					dotProduct = delta[0];
					break;
				case 1:
					// (1, 1)
					dotProduct = delta[0] + delta[1];
					break;
				case 2:
					// (0, 1)
					dotProduct = delta[1];
					break;
				case 3:
					// (-1, 1)
					dotProduct = delta[1] - delta[0];
					break;
				case 4:
					// (-1, 0)
					dotProduct = -delta[0];
					break;
				case 5:
					// (-1, -1)
					dotProduct = -delta[0] - delta[1];
					break;
				case 6:
					// (0, -1)
					dotProduct = -delta[1];
					break;
				case 7:
					// (1, -1)
					dotProduct = delta[0] - delta[1];
					break;
				};

				if (gradient & 1)
				{
					dotProduct /= std::sqrt((Real)2);
				}

				return dotProduct;
			}

			integer basicGradients = (1 << (n - 1));

			const integer gradients = n * basicGradients;

			// Each point in the integer lattice is associated
			// with a gradient. This association is made by
			// hashing the position vector into an index which
			// selects a gradient from a certain set.

			integer index = 0;
			for (integer i = 0;i < n;++i)
			{
				index = permutation_[(position[i] & permutationMask_) + index];
			}
			index %= gradients;

			// As gradients we select those vectors of {-1, 0, 1}^n sub RR^n
			// which have norm sqrt(n - 1). I.e. those vectors which have
			// exactly one zero component. 

			integer zeroAt = index / basicGradients;

			uint32 gradient = (uint32)(index & (basicGradients - 1));

			// The 'gradient' variable is a 32-bit integer consisting
			// of the bits {b_0, ..., b_31}.
			// A gradient vector 'g' is packed in 'gradient' as
			// follows:
			// g = [s_0, ..., s_{k - 1}, 0, s_{k}, ..., s_{n - 1}].
			// where
			// s_i = (-1)^{b_i}
			//
			// Here 'k' denotes the position of the zero in the
			// gradient vector, which is currently stored in 'zeroAt'
			// variable.

			// Evaluate <x, g>, the dot product between the 
			// gradient vector g and the delta vector x.

			Real dotProduct = 0;
			for (integer i = 0;i < n;++i)
			{
				if (i != zeroAt)
				{
					// The magnitude is 1.

					if (gradient & 0x1)
					{
						// The sign is -.
						dotProduct -= delta[i];
					}
					else
					{
						// The sign is +.
						dotProduct += delta[i];
					}
					
					// Next component.
					gradient >>= 1;
				}
			}

			return dotProduct * normalizationFactor_;
		}

		template <typename Real, int N>
		GradientField<Real, N>::~GradientField()
		{
		}

		template <typename Real, int N>
		GradientField<Real, N>& gradientField()
		{
			return GradientField<Real, N>::create();
		}

}

#endif
