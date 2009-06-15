#ifndef PASTEL_RANDOMDISTRIBUTION_H
#define PASTEL_RANDOMDISTRIBUTION_H

#include "pastel/sys/countedptr.h"
#include "pastel/sys/point.h"

#include "pastel/sys/random_vector.h"

namespace Pastel
{

	template <int N, typename Real>
	class RandomDistribution
		: public ReferenceCounted
	{
	public:
		typedef CountedPtr<RandomDistribution> Ptr;

		explicit RandomDistribution(integer dimension)
			: dimension_(dimension)
		{
			ENSURE1(dimension > 0, dimension);
		}

		virtual ~RandomDistribution()
		{
		}

		virtual TemporaryPoint<N, Real> sample() const = 0;
		virtual std::string name() const = 0;

		integer dimension() const
		{
			return dimension_;
		}

	private:
		integer dimension_;
	};

}

#include "pastel/sys/random.h"

namespace Pastel
{

	template <int N, typename Real>
	class Gaussian_RandomDistribution
		: public RandomDistribution<N, Real>
	{
	private:
		typedef RandomDistribution<N, Real> Base;

	public:
		typedef CountedPtr<Gaussian_RandomDistribution> Ptr;

		Gaussian_RandomDistribution()
			: Base(N)
		{
			ENSURE(N != Dynamic);
		}

		explicit Gaussian_RandomDistribution(integer dimension)
			: Base(dimension)
		{
		}

		virtual ~Gaussian_RandomDistribution()
		{
		}

		virtual TemporaryPoint<N, Real> sample() const
		{
			Point<N, Real> result(
				randomGaussianVector<N, Real>(Base::dimension()));
			
			return result.asTemporary();
		}

		virtual std::string name() const
		{
			return std::string("gaussian");
		}
	};

	template <int N, typename Real>
	CountedPtr<Gaussian_RandomDistribution<N, Real> >
		gaussianRandomDistribution(integer dimension)
	{
		return CountedPtr<Gaussian_RandomDistribution<N, Real> >(
			new Gaussian_RandomDistribution<N, Real>(dimension));
	}

	template <int N, typename Real>
	CountedPtr<Gaussian_RandomDistribution<N, Real> >
		gaussianRandomDistribution()
	{
		BOOST_STATIC_ASSERT(N != Dynamic);

		return Pastel::gaussianRandomDistribution<N, Real>(N);
	}

}

namespace Pastel
{

	template <int N, typename Real>
	class Exponential_RandomDistribution
		: public RandomDistribution<N, Real>
	{
	private:
		typedef RandomDistribution<N, Real> Base;

	public:
		typedef CountedPtr<Exponential_RandomDistribution> Ptr;

		Exponential_RandomDistribution()
			: Base(N)
		{
			ENSURE(N != Dynamic);
		}

		explicit Exponential_RandomDistribution(integer dimension)
			: Base(dimension)
		{
		}

		virtual ~Exponential_RandomDistribution()
		{
		}

		virtual TemporaryPoint<N, Real> sample() const
		{
			Point<N, Real> result(
				randomExponentialVector<N, Real>(Base::dimension()));
			
			return result.asTemporary();
		}

		virtual std::string name() const
		{
			return std::string("exponential");
		}
	};

	template <int N, typename Real>
	CountedPtr<Exponential_RandomDistribution<N, Real> >
		exponentialRandomDistribution(integer dimension)
	{
		return CountedPtr<Exponential_RandomDistribution<N, Real> >(
			new Exponential_RandomDistribution<N, Real>(dimension));
	}

	template <int N, typename Real>
	CountedPtr<Exponential_RandomDistribution<N, Real> >
		exponentialRandomDistribution()
	{
		BOOST_STATIC_ASSERT(N != Dynamic);

		return Pastel::exponentialRandomDistribution<N, Real>(N);
	}

}

namespace Pastel
{

	template <int N, typename Real>
	class Uniform_RandomDistribution
		: public RandomDistribution<N, Real>
	{
	private:
		typedef RandomDistribution<N, Real> Base;

	public:
		typedef CountedPtr<Uniform_RandomDistribution> Ptr;

		Uniform_RandomDistribution()
			: Base(N)
		{
			ENSURE(N != Dynamic);
		}

		explicit Uniform_RandomDistribution(integer dimension)
			: Base(dimension)
		{
		}

		virtual ~Uniform_RandomDistribution()
		{
		}

		virtual TemporaryPoint<N, Real> sample() const
		{
			Point<N, Real> result(
				randomVectorCube<N, Real>(Base::dimension()));
			
			return result.asTemporary();
		}

		virtual std::string name() const
		{
			return std::string("uniform");
		}
	};

	template <int N, typename Real>
	CountedPtr<Uniform_RandomDistribution<N, Real> >
		uniformRandomDistribution(integer dimension)
	{
		return CountedPtr<Uniform_RandomDistribution<N, Real> >(
			new Uniform_RandomDistribution<N, Real>(dimension));
	}

	template <int N, typename Real>
	CountedPtr<Uniform_RandomDistribution<N, Real> >
		uniformRandomDistribution()
	{
		BOOST_STATIC_ASSERT(N != Dynamic);

		return Pastel::uniformRandomDistribution<N, Real>(N);
	}

}

namespace Pastel
{

	template <int N, typename Real>
	class GeneralizedGaussian_RandomDistribution
		: public RandomDistribution<N, Real>
	{
	private:
		typedef RandomDistribution<N, Real> Base;

	public:
		typedef CountedPtr<GeneralizedGaussian_RandomDistribution> Ptr;

		GeneralizedGaussian_RandomDistribution(
			const Real& shape,
			const Real& scale)
			: Base(N)
			, shape_(shape)
			, scale_(scale)
		{
			ENSURE(N != Dynamic);
		}

		explicit GeneralizedGaussian_RandomDistribution(
			integer dimension,
			const Real& shape,
			const Real& scale)
			: Base(dimension)
			, shape_(shape)
			, scale_(scale)
		{
		}

		virtual ~GeneralizedGaussian_RandomDistribution()
		{
		}

		virtual TemporaryPoint<N, Real> sample() const
		{
			Point<N, Real> result(
				randomGeneralizedGaussianVector<N, Real>(
				Base::dimension(), shape_, scale_));
			
			return result.asTemporary();
		}

		virtual std::string name() const
		{
			return std::string("generalizedGaussian");
		}

	private:
		Real shape_;
		Real scale_;
	};

	template <int N, typename Real>
	CountedPtr<GeneralizedGaussian_RandomDistribution<N, Real> >
		generalizedGaussianRandomDistribution(
		integer dimension,
		const PASTEL_NO_DEDUCTION(Real)& shape,
		const PASTEL_NO_DEDUCTION(Real)& scale)
	{
		return CountedPtr<GeneralizedGaussian_RandomDistribution<N, Real> >(
			new GeneralizedGaussian_RandomDistribution<N, Real>(
			dimension, shape, scale));
	}

	template <int N, typename Real>
	CountedPtr<GeneralizedGaussian_RandomDistribution<N, Real> >
		generalizedGaussianRandomDistribution(
		const PASTEL_NO_DEDUCTION(Real)& shape,
		const PASTEL_NO_DEDUCTION(Real)& scale)
	{
		BOOST_STATIC_ASSERT(N != Dynamic);

		return Pastel::generalizedGaussianRandomDistribution<N, Real>(
			N, shape, scale);
	}

}

namespace Pastel
{

	template <int N, typename Real>
	class Gamma_RandomDistribution
		: public RandomDistribution<N, Real>
	{
	private:
		typedef RandomDistribution<N, Real> Base;

	public:
		typedef CountedPtr<Gamma_RandomDistribution> Ptr;

		explicit Gamma_RandomDistribution(
			const Real& shape)
			: Base(N)
			, shape_(shape)
			, scale_(scale)
		{
			ENSURE(N != Dynamic);
		}

		explicit Gamma_RandomDistribution(
			integer dimension,
			const Real& shape)
			: Base(dimension)
			, shape_(shape)
		{
		}

		virtual ~Gamma_RandomDistribution()
		{
		}

		virtual TemporaryPoint<N, Real> sample() const
		{
			Point<N, Real> result(
				randomGammaVector<N, Real>(
				Base::dimension(), shape_));
			
			return result.asTemporary();
		}

		virtual std::string name() const
		{
			return std::string("gamma");
		}

	private:
		Real shape_;
	};

	template <int N, typename Real>
	CountedPtr<Gamma_RandomDistribution<N, Real> >
		gammaRandomDistribution(
		integer dimension,
		const PASTEL_NO_DEDUCTION(Real)& shape)
	{
		return CountedPtr<Gamma_RandomDistribution<N, Real> >(
			new Gamma_RandomDistribution<N, Real>(
			dimension, shape));
	}

	template <int N, typename Real>
	CountedPtr<Gamma_RandomDistribution<N, Real> >
		gammaRandomDistribution(
		const PASTEL_NO_DEDUCTION(Real)& shape)
	{
		BOOST_STATIC_ASSERT(N != Dynamic);

		return Pastel::gammaRandomDistribution<N, Real>(
			N, shape);
	}

}

namespace Pastel
{

	template <int N, typename Real>
	class Clustered_RandomDistribution
		: public RandomDistribution<N, Real>
	{
	private:
		typedef RandomDistribution<N, Real> Base;

	public:
		typedef CountedPtr<Clustered_RandomDistribution> Ptr;

		Clustered_RandomDistribution()
			: Base(N)
			, distributionSet_()
		{
			ENSURE(N != Dynamic);
		}

		explicit Clustered_RandomDistribution(integer dimension)
			: Base(dimension)
			, distributionSet_()
		{
		}

		virtual ~Clustered_RandomDistribution()
		{
		}

		virtual TemporaryPoint<N, Real> sample() const
		{
			if (distributionSet_.empty())
			{
				return TemporaryPoint<N, Real>(
					ofDimension(Base::dimension()));
			}

			const integer cluster = 
				randomInteger() % distributionSet_.size();

			return distributionSet_[cluster]->sample();
		}

		virtual std::string name() const
		{
			return std::string("clustered");
		}

		void add(
			const CountedPtr<RandomDistribution<N, Real> >& distribution)
		{
			distributionSet_.push_back(distribution);
		}

	private:
		std::vector<
			CountedPtr<RandomDistribution<N, Real> > >
			distributionSet_;
	};

	template <int N, typename Real>
	CountedPtr<Clustered_RandomDistribution<N, Real> >
		clusteredRandomDistribution(integer dimension)
	{
		return CountedPtr<Clustered_RandomDistribution<N, Real> >(
			new Clustered_RandomDistribution<N, Real>(dimension));
	}

	template <int N, typename Real>
	CountedPtr<Clustered_RandomDistribution<N, Real> >
		clusteredRandomDistribution()
	{
		BOOST_STATIC_ASSERT(N != Dynamic);

		return Pastel::clusteredRandomDistribution<N, Real>(N);
	}

}

namespace Pastel
{

	template <int N, typename Real>
	class Scaled_RandomDistribution
		: public RandomDistribution<N, Real>
	{
	private:
		typedef RandomDistribution<N, Real> Base;

	public:
		typedef CountedPtr<Scaled_RandomDistribution> Ptr;

		explicit Scaled_RandomDistribution(
			const CountedPtr<RandomDistribution<N, Real> >& distribution,
			const Vector<N, Real>& scaling)
			: Base(scaling.dimension())
			, distribution_(distribution)
			, scaling_(scaling)
		{
			ENSURE2(distribution->dimension() == scaling.dimension(),
				distribution->dimension(), scaling.dimension());
		}

		virtual ~Scaled_RandomDistribution()
		{
		}

		virtual TemporaryPoint<N, Real> sample() const
		{
			Point<N, Real> result = distribution_->sample();
			asVector(result) *= scaling_;
			return result.asTemporary();
		}

		virtual std::string name() const
		{
			return std::string("scaled");
		}

	private:
		CountedPtr<RandomDistribution<N, Real> > distribution_;
		Vector<N, Real> scaling_;
	};

	template <int N, typename Real>
	CountedPtr<Scaled_RandomDistribution<N, Real> >
		scale(
		const PASTEL_NO_DEDUCTION((CountedPtr<RandomDistribution<N, Real> >))& distribution,
		const Vector<N, Real>& scaling)
	{
		return CountedPtr<Scaled_RandomDistribution<N, Real> >(
			new Scaled_RandomDistribution<N, Real>(
			distribution, scaling));
	}

}

namespace Pastel
{

	template <int N, typename Real>
	class Transformed_RandomDistribution
		: public RandomDistribution<N, Real>
	{
	private:
		typedef RandomDistribution<N, Real> Base;

	public:
		typedef CountedPtr<Transformed_RandomDistribution> Ptr;

		explicit Transformed_RandomDistribution(
			const CountedPtr<RandomDistribution<N, Real> >& distribution,
			const Matrix<N, N, Real>& transform)
			: Base(transform.width())
			, distribution_(distribution)
			, transform_(transform)
		{
			ENSURE2(transform.width() == transform.height(),
				transform.width(), transform.height());
			ENSURE2(distribution->dimension() == transform.width(),
				distribution->dimension(), transform.width());
		}

		virtual ~Transformed_RandomDistribution()
		{
		}

		virtual TemporaryPoint<N, Real> sample() const
		{
			Point<N, Real> result(
				distribution_->sample() * transform_);

			return result.asTemporary();
		}

		virtual std::string name() const
		{
			return std::string("transformed");
		}

	private:
		CountedPtr<RandomDistribution<N, Real> > distribution_;
		Matrix<N, N, Real> transform_;
	};

	template <int N, typename Real>
	CountedPtr<Transformed_RandomDistribution<N, Real> >
		transform(
		const PASTEL_NO_DEDUCTION((CountedPtr<RandomDistribution<N, Real> >))& distribution,
		const Matrix<N, N, Real>& transform)
	{
		return CountedPtr<Transformed_RandomDistribution<N, Real> >(
			new Transformed_RandomDistribution<N, Real>(
			distribution, transform));
	}

}

namespace Pastel
{

	template <int N, typename Real>
	class Translated_RandomDistribution
		: public RandomDistribution<N, Real>
	{
	private:
		typedef RandomDistribution<N, Real> Base;

	public:
		typedef CountedPtr<Translated_RandomDistribution> Ptr;

		explicit Translated_RandomDistribution(
			const CountedPtr<RandomDistribution<N, Real> >& distribution,
			const Vector<N, Real>& translation)
			: Base(translation.dimension())
			, distribution_(distribution)
			, translation_(translation)
		{
			ENSURE2(distribution->dimension() == translation.dimension(),
				distribution->dimension(), translation.dimension());
		}

		virtual ~Translated_RandomDistribution()
		{
		}

		virtual TemporaryPoint<N, Real> sample() const
		{
			Point<N, Real> result = distribution_->sample();
			result += translation_;
			return result.asTemporary();
		}

		virtual std::string name() const
		{
			return std::string("translated");
		}

	private:
		CountedPtr<RandomDistribution<N, Real> > distribution_;
		Vector<N, Real> translation_;
	};

	template <int N, typename Real>
	CountedPtr<Translated_RandomDistribution<N, Real> >
		translate(
		const PASTEL_NO_DEDUCTION((CountedPtr<RandomDistribution<N, Real> >))& distribution,
		const Vector<N, Real>& translation)
	{
		return CountedPtr<Translated_RandomDistribution<N, Real> >(
			new Translated_RandomDistribution<N, Real>(
			distribution, translation));
	}

}

#endif
