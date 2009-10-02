#ifndef PASTEL_PERLIN_NOISE_HPP
#define PASTEL_PERLIN_NOISE_HPP

#include "pastel/gfx/perlin_noise.h"

#include "pastel/sys/syscommon.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/math_functions.h"
#include "pastel/sys/random_uniform.h"

#include "pastel/math/smoothstep.h"

#include <vector>

namespace Pastel
{

	namespace Detail
	{

		template <typename Real, int N = 2>
		class Noise
		{
		public:
			static Noise<Real, N>& create();

			Real operator()(const Vector<Real, N>& position) const;

			// The evaluation of Perlin noise is
			// exponential in dimension, so let's
			// be realistic and prohibit instantiation
			// on dimensions that are too high.
			enum
			{
				MaxDimension = 8
			};

		private:
			Real contribution(
				const Vector<integer, N>& position,
				const Vector<Real, N>& minDelta) const;

			Noise();
			explicit Noise(integer n);
			~Noise();

			void initialize(integer n);

			// Prohibited
			Noise(const Noise& that);
			// Prohibited
			Noise<Real, N>& operator=(
				const Noise& that);

			std::vector<integer> permutation_;
			integer permutationMask_;
		};

		template <typename Real, int N>
		Noise<Real, N>&
			Noise<Real, N>::create()
		{
			static Noise<Real, N> theNoise;
			return theNoise;
		}

		template <typename Real, int N>
		Noise<Real, N>::Noise()
			: permutation_()
			, permutationMask_(0)
		{
			BOOST_STATIC_ASSERT(N != Dynamic);

			initialize(N);
		}

		template <typename Real, int N>
		Noise<Real, N>::Noise(integer n)
			: permutation_()
			, permutationMask_(0)
		{
			ENSURE(N == Dynamic || N == n);

			ENSURE_OP(n, <=, MaxDimension);

			initialize(n);
		}

		template <typename Real, int N>
		void Noise<Real, N>::initialize(integer n)
		{
			const integer basicGradients = (1 << (n - 1));
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
			// also determines the size of the noise tile that is to be 
			// repeated (in RR^n, the size of the tile is 
			// [0, permutationSize[^n).

			const integer permutationSize = 
				std::max(roundUpToPowerOf2(gradients), 256);
			permutationMask_ = permutationSize - 1;
			
			// Generate the standard permutation.
			permutation_.reserve(permutationSize * 2);
			for (integer i = 0;i < permutationSize;++i)
			{
				permutation_.push_back(i);
			}

			// Shuffle it to a random permutation.
			for (integer i = 0;i < permutationSize;++i)
			{
				const integer u = randomInteger() & permutationMask_;
				std::swap(permutation_[u], permutation_[i]);
			}

			// Repeat the permutation table so
			// we needn't use modulus later
			for (integer i = 0;i < permutationSize;++i)
			{
				permutation_.push_back(permutation_[i]);
			}
		}

		template <typename Real, int N>
		Real Noise<Real, N>::operator()(const Vector<Real, N>& position) const
		{
			const integer n = position.size();

			const Vector<integer, N> floorPosition = floor(position);
			const Vector<Real, N> minDelta = position - Vector<Real, N>(floorPosition);
			
			// Find out the contribution of each vertex of the
			// containing cube.

			std::vector<Real> vertexSet;
			vertexSet.reserve(1 << n);
			
			Vector<integer, N> p = floorPosition;
			Vector<Real, N> f = minDelta;
			uint32 state = 0;
			while(true)
			{
				vertexSet.push_back(contribution(p, f));

				integer axis = 0;
				uint32 mask = 1;
				while((state & mask) && axis < n)
				{
					state -= mask;
					--p[axis];
					f[axis] = minDelta[axis];
					
					++axis;
					mask <<= 1;
				}
				if (axis == n)
				{
					break;
				}

				state += mask;
				++p[axis];
				--f[axis];
			}

			// The interpolation coefficients are modified
			// by a quintic smoothstep function.
			Vector<Real, N> t(ofDimension(n));
			for (integer i = 0;i < n;++i)
			{
				t[i] = quinticSmoothStep(minDelta[i]);
			}

			// Linearly interpolate between the contributions
			// of the cube vertices.
			const Real value = linear(t, randomAccessRange(
				vertexSet.begin(), vertexSet.end()));

			const Real maxValue = (Real)n / 2;

			return (value / maxValue + 1) / 2;
		}

		// Private

		template <typename Real, int N>
		Real Noise<Real, N>::contribution(
			const Vector<integer, N>& position,
			const Vector<Real, N>& delta) const
		{
			const integer n = position.size();

			if (n == 1)
			{
				// The 1-dimensional noise has to be handled
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
				// cubes (intervals).
				//
				// Let us target 256 different kinds of cubes,
				// that is, 16 different gradients.

				enum
				{
					Gradients = 1 << 4,
					GradientMask = Gradients - 1
				};

				const integer index = permutation_[position[0] & permutationMask_];
				const Real gradient = (((Real)(2 * (index & GradientMask))) / (Gradients - 1)) - 1;

				return delta[0] * gradient;
			}

			const integer basicGradients = (1 << (n - 1));
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

			const integer zeroAt = index / basicGradients;
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

			return dotProduct;
		}

		template <typename Real, int N>
		Noise<Real, N>::~Noise()
		{
			BOOST_STATIC_ASSERT(N == Dynamic || N <= MaxDimension);
		}

		template <typename Real, int N>
		Noise<Real, N>& noise()
		{
			return Noise<Real, N>::create();
		}

	}

	template <typename Real>
	Real noise(const PASTEL_NO_DEDUCTION(Real)& position)
	{
		return Detail::noise<Real, 1>()(position);
	}

	template <typename Real, int N>
	Real noise(const Vector<Real, N>& position)
	{
		return Detail::noise<Real, N>()(position);
	}

}

#endif
