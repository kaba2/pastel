// Description: Light spectrum
// Documentation: pastelray.txt

#ifndef PASTEL_SPECTRUM_H
#define PASTEL_SPECTRUM_H

#include "pastel/ray/raylibrary.h"

#include "pastel/gfx/color.h"

#include "pastel/sys/vector.h"
#include "pastel/sys/array.h"

namespace Pastel
{

	typedef Color Spectrum;

	/*
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
		Color toXyz() const;
		static Spectrum fromXyz(const Color& xyz);

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

	typedef Array<Spectrum, 2> SpectrumImage;

	*/

}

#include "pastel/ray/spectrum.hpp"

#endif
