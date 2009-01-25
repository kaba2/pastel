#ifndef PASTELRAY_SPECTRUM_H
#define PASTELRAY_SPECTRUM_H

#include "pastel/ray/raylibrary.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/lineararray.h"

namespace Pastel
{

	namespace Yun
	{

		class PASTELRAY Spectrum
		{
		public:
			Spectrum();
			Spectrum(const Spectrum& that);
			explicit Spectrum(real intensity);
			~Spectrum();
			Spectrum& operator=(const Spectrum& that);

			Spectrum& operator+=(const Spectrum& that);
			Spectrum& operator-=(const Spectrum& that);
			Spectrum& operator*=(const Spectrum& that);
			Spectrum& operator/=(const Spectrum& that);

			Spectrum& operator+=(real that);
			Spectrum& operator-=(real that);
			Spectrum& operator*=(real that);
			Spectrum& operator/=(real that);

			Spectrum operator+(const Spectrum& that) const;
			Spectrum operator-(const Spectrum& that) const;
			Spectrum operator*(const Spectrum& that) const;
			Spectrum operator/(const Spectrum& that) const;

			Spectrum operator+(real that) const;
			Spectrum operator-(real that) const;
			Spectrum operator*(real that) const;
			Spectrum operator/(real that) const;

			real intensity() const;
			Vector<3, float> toXyz() const;
			static Spectrum fromXyz(const Vector<3, float>& xyz);

			friend Spectrum operator+(real left, const Spectrum& right)
			{
				return right + left;
			}

			friend Spectrum operator-(real left, const Spectrum& right)
			{
				return Spectrum(left) - right;
			}

			friend Spectrum operator/(real left, const Spectrum& right)
			{
				return Spectrum(left) / right;
			}

		private:
			float aSample_;
			float bSample_;
			float cSample_;
		};

		inline Spectrum operator*(real left, const Spectrum& right)
		{
			return right * left;
		}

		typedef LinearArray<2, Spectrum> SpectrumImage;

	}

}

#include "pastel/ray/spectrum.hpp"

#endif
