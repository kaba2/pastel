#ifndef PASTEL_NOISE_HPP
#define PASTEL_NOISE_HPP

#include "pastel/gfx/noise.h"

#include "pastel/sys/syscommon.h"
#include "pastel/sys/random.h"
#include "pastel/sys/tristate.h"

#include "pastel/sys/vector.h"
#include "pastel/math/smoothstep.h"
#include "pastel/math/linear.h"

#include <vector>

namespace Pastel
{

	namespace Detail
	{

		// Noise1

		template <typename Real>
		class Noise1
		{
		public:
			static Noise1<Real>& create();

			Real operator()(const Real& position) const;

		private:
			Noise1();
			~Noise1();

			Real latticeValue(
				integer x, const Real& xDeltaMin) const;

			// Prohibited
			Noise1(
				const Noise1<Real>& that);
			// Prohibited
			Noise1<Real>& operator=(
				const Noise1<Real>& that);

			std::vector<integer> permutation_;
			std::vector<Real> gradients_;
		};

		template <typename Real>
		Noise1<Real>&
			Noise1<Real>::create()
		{
			static Noise1 theNoise1;
			return theNoise1;
		}

		template <typename Real>
		Noise1<Real>::Noise1()
			: permutation_()
			, gradients_()
		{
			gradients_.reserve(8);

			gradients_.push_back(0.875);
			gradients_.push_back(0.625);
			gradients_.push_back(0.375);
			gradients_.push_back(0.125);
			gradients_.push_back(-0.125);
			gradients_.push_back(-0.375);
			gradients_.push_back(-0.625);
			gradients_.push_back(-0.875);

			// Generate random permutation table

			permutation_.reserve(512);

			for (integer i = 0;i < 256;++i)
			{
				permutation_.push_back(i);
			}

			for (integer i = 0;i < 256;++i)
			{
				integer u = randomInteger() & 0xFF;
				std::swap(permutation_[u], permutation_[i]);
			}

			// Double permutation table so
			// we needn't use modulus later

			for (integer i = 0;i < 256;++i)
			{
				permutation_.push_back(permutation_[i]);
			}
		}

		template <typename Real>
		Real Noise1<Real>::operator()(const Real& pos) const
		{
			const Real x(pos);

			const integer xFloor = (integer)std::floor(x);
			const Real xDeltaMin(x - xFloor);
			const Real xDeltaMax(xDeltaMin - 1);
			const Real xWeight(quinticSmoothStep(xDeltaMin));

			const Real vx0(latticeValue(xFloor, xDeltaMin));
			const Real vx1(latticeValue(xFloor + 1, xDeltaMax));

			return linear(vx0, vx1, xWeight) + 0.5;
		}

		// Private

		template <typename Real>
		Real Noise1<Real>::latticeValue(
			integer x, const Real& xDeltaMin) const
		{
			const integer index = permutation_[x & 0xFF] & 0x7;

			return gradients_[index] * xDeltaMin;
		}

		template <typename Real>
		Noise1<Real>::~Noise1()
		{
		}

		template <typename Real>
		Noise1<Real>& noise1()
		{
			return Noise1<Real>::create();
		}

		// Noise2

		template <typename Real>
		class Noise2
		{
		public:
			static Noise2<Real>& create();

			Real operator()(const Point<2, Real>& position) const;

		private:
			Noise2();
			~Noise2();

			Real latticeValue(
				integer x, integer y,
				const Real& xDeltaMin, const Real& yDeltaMin) const;

			// Prohibited
			Noise2(
				const Noise2<Real>& that);
			// Prohibited
			Noise2<Real>& operator=(
				const Noise2<Real>& that);

			std::vector<integer> permutation_;
			std::vector<Vector<2, Real> > gradients_;
		};

		template <typename Real>
		Noise2<Real>&
			Noise2<Real>::create()
		{
			static Noise2<Real> theNoise2;
			return theNoise2;
		}

		template <typename Real>
		Noise2<Real>::Noise2()
			: permutation_()
			, gradients_()
		{
			gradients_.reserve(4);

			gradients_.push_back(Vector<2, Real>( 1, 1));
			gradients_.push_back(Vector<2, Real>( 1,-1));
			gradients_.push_back(Vector<2, Real>(-1, 1));
			gradients_.push_back(Vector<2, Real>(-1,-1));

			// Generate random permutation table

			permutation_.reserve(512);

			for (integer i = 0;i < 256;++i)
			{
				permutation_.push_back(i);
			}

			for (integer i = 0;i < 256;++i)
			{
				integer u = randomInteger() & 0xFF;
				std::swap(permutation_[u], permutation_[i]);
			}

			// Double permutation table so
			// we needn't use modulus later

			for (integer i = 0;i < 256;++i)
			{
				permutation_.push_back(permutation_[i]);
			}
		}

		template <typename Real>
		Real Noise2<Real>::operator()(const Point<2, Real>& pos) const
		{
			const Real x(pos[0]);
			const Real y(pos[1]);

			const integer xFloor = (integer)std::floor(x);
			const Real xDeltaMin(x - xFloor);
			const Real xDeltaMax(xDeltaMin - 1);
			const Real xWeight(quinticSmoothStep(xDeltaMin));

			const integer yFloor = (integer)std::floor(y);
			const Real yDeltaMin(y - yFloor);
			const Real yDeltaMax(yDeltaMin - 1);
			const Real yWeight(quinticSmoothStep(yDeltaMin));

			// Bilinear interpolation between 4
			// surrounding square latticepoint values

			// W.r.t. x-axis.

			const Real v00(latticeValue(
				xFloor, yFloor, xDeltaMin, yDeltaMin));
			const Real v10(latticeValue(
				xFloor + 1, yFloor, xDeltaMax, yDeltaMin));
			const Real vx0(linear(v00, v10, xWeight));

			const Real v01(latticeValue(
				xFloor, yFloor + 1, xDeltaMin, yDeltaMax));
			const Real v11(latticeValue(
				xFloor + 1, yFloor + 1, xDeltaMax, yDeltaMax));
			const Real vx1(linear(v01, v11, xWeight));

			// W.r.t. y-axis.

			const Real vxy(linear(vx0, vx1, yWeight));

			return (vxy + 1) * Real(0.5);
		}

		// Private

		template <typename Real>
		Real Noise2<Real>::latticeValue(
			integer x, integer y,
			const Real& xDeltaMin, const Real& yDeltaMin) const
		{
			const integer index =
				permutation_[(x & 0xFF) +
				permutation_[(y & 0xFF)]] & 0x3;

			const Vector<2, Real>& gradient(gradients_[index]);

			return gradient[0] * xDeltaMin +
				gradient[1] * yDeltaMin;
		}

		template <typename Real>
		Noise2<Real>::~Noise2()
		{
		}

		template <typename Real>
		Noise2<Real>& noise2()
		{
			return Noise2<Real>::create();
		}

		// Noise3

		template <typename Real>
		class Noise3
		{
		public:
			static Noise3<Real>& create();

			Real operator()(const Point<3, Real>& pos) const;

		private:
			Real latticeValue(
				integer x, integer y, integer z,
				const Real& xDeltaMin, const Real& yDeltaMin,
				const Real& zDeltaMin) const;

			Noise3();
			~Noise3();

			// Prohibited
			Noise3(
				const Noise3<Real>& that);
			// Prohibited
			Noise3<Real>& operator=(
				const Noise3<Real>& that);

			std::vector<integer> permutation_;
			std::vector<Vector<3, Real> > gradients_;
		};

		template <typename Real>
		Noise3<Real>&
			Noise3<Real>::create()
		{
			static Noise3<Real> theNoise3;
			return theNoise3;
		}

		template <typename Real>
		Noise3<Real>::Noise3()
			: permutation_()
			, gradients_()
		{
			gradients_.reserve(16);

			gradients_.push_back(Vector<3, Real>(1, 1, 0));
			gradients_.push_back(Vector<3, Real>(-1, 1, 0));
			gradients_.push_back(Vector<3, Real>( 1,-1, 0));
			gradients_.push_back(Vector<3, Real>(-1,-1, 0));

			gradients_.push_back(Vector<3, Real>( 1, 0, 1));
			gradients_.push_back(Vector<3, Real>(-1, 0, 1));
			gradients_.push_back(Vector<3, Real>( 1, 0,-1));
			gradients_.push_back(Vector<3, Real>(-1, 0,-1));

			gradients_.push_back(Vector<3, Real>( 0, 1, 1));
			gradients_.push_back(Vector<3, Real>(0,-1, 1));
			gradients_.push_back(Vector<3, Real>( 0, 1,-1));
			gradients_.push_back(Vector<3, Real>(0,-1,-1));

			gradients_.push_back(Vector<3, Real>( 1, 1, 0));
			gradients_.push_back(Vector<3, Real>(-1, 1, 0));
			gradients_.push_back(Vector<3, Real>( 0,-1, 1));
			gradients_.push_back(Vector<3, Real>( 0,-1,-1));

			// Generate random permutation table

			permutation_.reserve(512);

			for (integer i = 0;i < 256;++i)
			{
				permutation_.push_back(i);
			}

			for (integer i = 0;i < 256;++i)
			{
				integer u = randomInteger() & 0xFF;
				std::swap(permutation_[u], permutation_[i]);
			}

			// Double permutation table so
			// we needn't use modulus later

			for (integer i = 0;i < 256;++i)
			{
				permutation_.push_back(permutation_[i]);
			}
		}

		template <typename Real>
		Real Noise3<Real>::operator()(const Point<3, Real>& pos) const
		{
			const Real x(pos[0]);
			const Real y(pos[1]);
			const Real z(pos[2]);

			const integer xFloor = (integer)std::floor(x);
			const Real xDeltaMin(x - xFloor);
			const Real xDeltaMax(xDeltaMin - 1);
			const Real xWeight(quinticSmoothStep(xDeltaMin));

			const integer yFloor = (integer)std::floor(y);
			const Real yDeltaMin(y - yFloor);
			const Real yDeltaMax(yDeltaMin - 1);
			const Real yWeight(quinticSmoothStep(yDeltaMin));

			const integer zFloor = (integer)std::floor(z);
			const Real zDeltaMin(z - zFloor);
			const Real zDeltaMax(zDeltaMin - 1);
			const Real zWeight(quinticSmoothStep(zDeltaMin));

			// Trilinear interpolation between 8 surrounding cube
			// latticepoint values

			// W.r.t. x-axis.

			const Real v000(latticeValue(
				xFloor, yFloor, zFloor,
				xDeltaMin, yDeltaMin, zDeltaMin));
			const Real v100(latticeValue(
				x + 1, yFloor, zFloor,
				xDeltaMax, yDeltaMin, zDeltaMin));
			const Real vx00(linear(v000, v100, xWeight));

			const Real v010(latticeValue(
				xFloor, yFloor + 1, zFloor,
				xDeltaMin, yDeltaMax, zDeltaMin));
			const Real v110(latticeValue(
				xFloor + 1, yFloor + 1, zFloor,
				xDeltaMax, yDeltaMax, zDeltaMin));
			const Real vx10(linear(v010, v110, xWeight));

			const Real v001(latticeValue(
				xFloor, yFloor, zFloor + 1,
				xDeltaMin, yDeltaMin, zDeltaMax));
			const Real v101(latticeValue(
				xFloor + 1, yFloor, zFloor + 1,
				xDeltaMax, yDeltaMin, zDeltaMax));
			const Real vx01(linear(v001, v101, xWeight));

			const Real v011(latticeValue(
				xFloor, yFloor + 1, zFloor + 1,
				xDeltaMin, yDeltaMax, zDeltaMax));
			const Real v111(latticeValue(
				xFloor + 1, yFloor + 1, zFloor + 1,
				xDeltaMax, yDeltaMax, zDeltaMax));
			const Real vx11(linear(v011, v111, xWeight));

			// W.r.t. y-axis.

			const Real vxy0(linear(vx00, vx10, yWeight));
			const Real vxy1(linear(vx01, vx11, yWeight));

			// W.r.t. z-axis.

			const Real vxyz(linear(vxy0, vxy1, zWeight));

			return (vxyz + 2) * Real(0.25);
		}

		// Private

		template <typename Real>
		Real Noise3<Real>::latticeValue(
			integer x, integer y, integer z,
			const Real& xDeltaMin, const Real& yDeltaMin,
			const Real& zDeltaMin) const
		{
			// FIX: What if x, y or z < 0?

			const integer index =
				permutation_[(x & 0xFF) +
				permutation_[(y & 0xFF) +
				permutation_[(z & 0xFF)]]] & 0xF;

			const Vector3& gradient = gradients_[index];

			return
				gradient[0] * xDeltaMin +
				gradient[1] * yDeltaMin +
				gradient[2] * zDeltaMin;
		}

		template <typename Real>
		Noise3<Real>::~Noise3()
		{
		}

		template <typename Real>
		Noise3<Real>& noise3()
		{
			return Noise3<Real>::create();
		}

	}

	template <typename Real>
	Real noise(const Real& position)
	{
		return Detail::noise1<Real>()(position);
	}

	template <typename Real>
	Real noise(const Point<2, Real>& position)
	{
		return Detail::noise2<Real>()(position);
	}

	template <typename Real>
	Real noise(const Point<3, Real>& position)
	{
		return Detail::noise3<Real>()(position);
	}

}

#endif
