#include "pastel/ray/spectrum.h"

namespace Pastel
{

	namespace Yun
	{

		Spectrum::Spectrum()
			: aSample_(0)
			, bSample_(0)
			, cSample_(0)
		{
		}

		Spectrum::Spectrum(const Spectrum& that)
			: aSample_(that.aSample_)
			, bSample_(that.bSample_)
			, cSample_(that.cSample_)
		{
		}

		Spectrum::Spectrum(real intensity)
			: aSample_(intensity)
			, bSample_(intensity)
			, cSample_(intensity)
		{
		}

		Spectrum::~Spectrum()
		{
		}

		Spectrum& Spectrum::operator=(const Spectrum& that)
		{
			aSample_ = that.aSample_;
			bSample_ = that.bSample_;
			cSample_ = that.cSample_;
			return *this;
		}

		Spectrum& Spectrum::operator+=(const Spectrum& that)
		{
			aSample_ += that.aSample_;
			bSample_ += that.bSample_;
			cSample_ += that.cSample_;

			return *this;
		}

		Spectrum& Spectrum::operator-=(const Spectrum& that)
		{
			aSample_ -= that.aSample_;
			bSample_ -= that.bSample_;
			cSample_ -= that.cSample_;

			return *this;
		}

		Spectrum& Spectrum::operator*=(const Spectrum& that)
		{
			aSample_ *= that.aSample_;
			bSample_ *= that.bSample_;
			cSample_ *= that.cSample_;

			return *this;
		}

		Spectrum& Spectrum::operator/=(const Spectrum& that)
		{
			aSample_ /= that.aSample_;
			bSample_ /= that.bSample_;
			cSample_ /= that.cSample_;

			return *this;
		}

		Spectrum& Spectrum::operator+=(real that)
		{
			real32 that32 = (real32)that;

			aSample_ += that32;
			bSample_ += that32;
			cSample_ += that32;

			return *this;
		}

		Spectrum& Spectrum::operator-=(real that)
		{
			real32 that32 = (real32)that;

			aSample_ -= that32;
			bSample_ -= that32;
			cSample_ -= that32;

			return *this;
		}

		Spectrum& Spectrum::operator*=(real that)
		{
			real32 that32 = (real32)that;

			aSample_ *= that32;
			bSample_ *= that32;
			cSample_ *= that32;

			return *this;
		}

		Spectrum& Spectrum::operator/=(real that)
		{
			real32 that32 = (real32)1 / (real32)that;

			aSample_ *= that32;
			bSample_ *= that32;
			cSample_ *= that32;

			return *this;
		}

		Spectrum Spectrum::operator+(
			const Spectrum& that) const
		{
			Spectrum result(*this);
			result += that;
			return result;
		}

		Spectrum Spectrum::operator-(
			const Spectrum& that) const
		{
			Spectrum result(*this);
			result -= that;
			return result;
		}

		Spectrum Spectrum::operator*(
			const Spectrum& that) const
		{
			Spectrum result(*this);
			result *= that;
			return result;
		}

		Spectrum Spectrum::operator/(
			const Spectrum& that) const
		{
			Spectrum result(*this);
			result /= that;
			return result;
		}

		Spectrum Spectrum::operator+(
			real that) const
		{
			Spectrum result(*this);
			result += that;
			return result;
		}

		Spectrum Spectrum::operator-(
			real that) const
		{
			Spectrum result(*this);
			result -= that;
			return result;
		}

		Spectrum Spectrum::operator*(
			real that) const
		{
			Spectrum result(*this);
			result *= that;
			return result;
		}

		Spectrum Spectrum::operator/(
			real that) const
		{
			Spectrum result(*this);
			result /= that;
			return result;
		}

		real Spectrum::intensity() const
		{
			return
				0.212571 * aSample_ +
				0.715160 * bSample_ +
				0.072169 * cSample_;
		}

		Vector<3, float> Spectrum::toXyz() const
		{
			return Vector<3, float>(
				0.412453 * aSample_ +
				0.357580 * bSample_ +
				0.180423 * cSample_,
				0.212671 * aSample_ +
				0.715160 * bSample_ +
				0.072169 * cSample_,
				0.019334 * aSample_ +
				0.119193 * bSample_ +
				0.950227 * cSample_);
		}

		Spectrum Spectrum::fromXyz(const Vector<3, float>& xyz)
		{
			Spectrum spectrum;

			spectrum.aSample_ =
				3.240479 * xyz[0] -
				1.537150 * xyz[1] -
				0.498535 * xyz[2];
			spectrum.bSample_ =
				-0.969256 * xyz[0] +
				1.875991 * xyz[1] +
				0.041556 * xyz[2];
			spectrum.cSample_ =
				0.055648 * xyz[0] -
				0.204043 * xyz[1] +
				1.057311 * xyz[2];

			return spectrum;
		}

	}

}
