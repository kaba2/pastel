#ifndef PASTEL_RANDOMDISTRIBUTION_H
#define PASTEL_RANDOMDISTRIBUTION_H

#include "pastel/sys/countedptr.h"

#include "pastel/math/uniform_sampling.h"

namespace Pastel
{

	template <typename Real, int N>
	class RandomDistribution
		: public ReferenceCounted
	{
	public:
		typedef CountedPtr<RandomDistribution> Ptr;

		explicit RandomDistribution(integer dimension)
			: dimension_(dimension)
		{
			ENSURE_OP(dimension, >, 0);
		}

		virtual ~RandomDistribution()
		{
		}

		virtual Vector<Real, N> sample() const = 0;
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

	template <typename Real, int N>
	class Gaussian_RandomDistribution
		: public RandomDistribution<Real, N>
	{
	private:
		typedef RandomDistribution<Real, N> Base;

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

		virtual Vector<Real, N> sample() const
		{
			Vector<Real, N> result(
				randomGaussianVector<Real, N>(Base::dimension()));
			
			return result;
		}

		virtual std::string name() const
		{
			return std::string("gaussian");
		}
	};

	template <typename Real, int N>
	CountedPtr<Gaussian_RandomDistribution<Real, N> >
		gaussianRandomDistribution(integer dimension)
	{
		return CountedPtr<Gaussian_RandomDistribution<Real, N> >(
			new Gaussian_RandomDistribution<Real, N>(dimension));
	}

	template <typename Real, int N>
	CountedPtr<Gaussian_RandomDistribution<Real, N> >
		gaussianRandomDistribution()
	{
		BOOST_STATIC_ASSERT(N != Dynamic);

		return Pastel::gaussianRandomDistribution<Real, N>(N);
	}

}

namespace Pastel
{

	template <typename Real, int N>
	class Exponential_RandomDistribution
		: public RandomDistribution<Real, N>
	{
	private:
		typedef RandomDistribution<Real, N> Base;

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

		virtual Vector<Real, N> sample() const
		{
			Vector<Real, N> result(
				randomExponentialVector<Real, N>(Base::dimension()));
			
			return result;
		}

		virtual std::string name() const
		{
			return std::string("exponential");
		}
	};

	template <typename Real, int N>
	CountedPtr<Exponential_RandomDistribution<Real, N> >
		exponentialRandomDistribution(integer dimension)
	{
		return CountedPtr<Exponential_RandomDistribution<Real, N> >(
			new Exponential_RandomDistribution<Real, N>(dimension));
	}

	template <typename Real, int N>
	CountedPtr<Exponential_RandomDistribution<Real, N> >
		exponentialRandomDistribution()
	{
		BOOST_STATIC_ASSERT(N != Dynamic);

		return Pastel::exponentialRandomDistribution<Real, N>(N);
	}

}

namespace Pastel
{

	template <typename Real, int N>
	class Uniform_RandomDistribution
		: public RandomDistribution<Real, N>
	{
	private:
		typedef RandomDistribution<Real, N> Base;

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

		virtual Vector<Real, N> sample() const
		{
			Vector<Real, N> result(
				randomVectorCube<Real, N>(Base::dimension()));
			
			return result;
		}

		virtual std::string name() const
		{
			return std::string("uniform");
		}
	};

	template <typename Real, int N>
	CountedPtr<Uniform_RandomDistribution<Real, N> >
		uniformRandomDistribution(integer dimension)
	{
		return CountedPtr<Uniform_RandomDistribution<Real, N> >(
			new Uniform_RandomDistribution<Real, N>(dimension));
	}

	template <typename Real, int N>
	CountedPtr<Uniform_RandomDistribution<Real, N> >
		uniformRandomDistribution()
	{
		BOOST_STATIC_ASSERT(N != Dynamic);

		return Pastel::uniformRandomDistribution<Real, N>(N);
	}

}

namespace Pastel
{

	template <typename Real, int N>
	class GeneralizedGaussian_RandomDistribution
		: public RandomDistribution<Real, N>
	{
	private:
		typedef RandomDistribution<Real, N> Base;

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

		virtual Vector<Real, N> sample() const
		{
			Vector<Real, N> result(
				randomGeneralizedGaussianVector<N, Real>(
				Base::dimension(), shape_, scale_));
			
			return result;
		}

		virtual std::string name() const
		{
			return std::string("generalized_gaussian");
		}

	private:
		Real shape_;
		Real scale_;
	};

	template <typename Real, int N>
	CountedPtr<GeneralizedGaussian_RandomDistribution<Real, N> >
		generalizedGaussianRandomDistribution(
		integer dimension,
		const PASTEL_NO_DEDUCTION(Real)& shape,
		const PASTEL_NO_DEDUCTION(Real)& scale)
	{
		return CountedPtr<GeneralizedGaussian_RandomDistribution<Real, N> >(
			new GeneralizedGaussian_RandomDistribution<Real, N>(
			dimension, shape, scale));
	}

	template <typename Real, int N>
	CountedPtr<GeneralizedGaussian_RandomDistribution<Real, N> >
		generalizedGaussianRandomDistribution(
		const PASTEL_NO_DEDUCTION(Real)& shape,
		const PASTEL_NO_DEDUCTION(Real)& scale)
	{
		BOOST_STATIC_ASSERT(N != Dynamic);

		return Pastel::generalizedGaussianRandomDistribution<Real, N>(
			N, shape, scale);
	}

}

namespace Pastel
{

	template <typename Real, int N>
	class Gamma_RandomDistribution
		: public RandomDistribution<Real, N>
	{
	private:
		typedef RandomDistribution<Real, N> Base;

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

		virtual Vector<Real, N> sample() const
		{
			Vector<Real, N> result(
				randomGammaVector<Real, N>(
				Base::dimension(), shape_));
			
			return result;
		}

		virtual std::string name() const
		{
			return std::string("gamma");
		}

	private:
		Real shape_;
	};

	template <typename Real, int N>
	CountedPtr<Gamma_RandomDistribution<Real, N> >
		gammaRandomDistribution(
		integer dimension,
		const PASTEL_NO_DEDUCTION(Real)& shape)
	{
		return CountedPtr<Gamma_RandomDistribution<Real, N> >(
			new Gamma_RandomDistribution<Real, N>(
			dimension, shape));
	}

	template <typename Real, int N>
	CountedPtr<Gamma_RandomDistribution<Real, N> >
		gammaRandomDistribution(
		const PASTEL_NO_DEDUCTION(Real)& shape)
	{
		BOOST_STATIC_ASSERT(N != Dynamic);

		return Pastel::gammaRandomDistribution<Real, N>(
			N, shape);
	}

}

namespace Pastel
{

	template <typename Real, int N>
	class Clustered_RandomDistribution
		: public RandomDistribution<Real, N>
	{
	private:
		typedef RandomDistribution<Real, N> Base;

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

		virtual Vector<Real, N> sample() const
		{
			if (distributionSet_.empty())
			{
				return Vector<Real, N>(
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
			const CountedPtr<RandomDistribution<Real, N> >& distribution)
		{
			distributionSet_.push_back(distribution);
		}

	private:
		std::vector<
			CountedPtr<RandomDistribution<Real, N> > >
			distributionSet_;
	};

	template <typename Real, int N>
	CountedPtr<Clustered_RandomDistribution<Real, N> >
		clusteredRandomDistribution(integer dimension)
	{
		return CountedPtr<Clustered_RandomDistribution<Real, N> >(
			new Clustered_RandomDistribution<Real, N>(dimension));
	}

	template <typename Real, int N>
	CountedPtr<Clustered_RandomDistribution<Real, N> >
		clusteredRandomDistribution()
	{
		BOOST_STATIC_ASSERT(N != Dynamic);

		return Pastel::clusteredRandomDistribution<Real, N>(N);
	}

}

namespace Pastel
{

	template <typename Real, int N>
	class Scaled_RandomDistribution
		: public RandomDistribution<Real, N>
	{
	private:
		typedef RandomDistribution<Real, N> Base;

	public:
		typedef CountedPtr<Scaled_RandomDistribution> Ptr;

		explicit Scaled_RandomDistribution(
			const CountedPtr<RandomDistribution<Real, N> >& distribution,
			const Vector<Real, N>& scaling)
			: Base(scaling.dimension())
			, distribution_(distribution)
			, scaling_(scaling)
		{
			ENSURE_OP(distribution->dimension(), ==, scaling.dimension());
		}

		virtual ~Scaled_RandomDistribution()
		{
		}

		virtual Vector<Real, N> sample() const
		{
			Vector<Real, N> result = distribution_->sample();
			result *= scaling_;
			return result;
		}

		virtual std::string name() const
		{
			return std::string("scaled_") + distribution_->name();
		}

	private:
		CountedPtr<RandomDistribution<Real, N> > distribution_;
		Vector<Real, N> scaling_;
	};

	template <typename Real, int N>
	CountedPtr<Scaled_RandomDistribution<Real, N> >
		scale(
		const PASTEL_NO_DEDUCTION((CountedPtr<RandomDistribution<Real, N> >))& distribution,
		const Vector<Real, N>& scaling)
	{
		return CountedPtr<Scaled_RandomDistribution<Real, N> >(
			new Scaled_RandomDistribution<Real, N>(
			distribution, scaling));
	}

}

namespace Pastel
{

	template <typename Real, int N>
	class Transformed_RandomDistribution
		: public RandomDistribution<Real, N>
	{
	private:
		typedef RandomDistribution<Real, N> Base;

	public:
		typedef CountedPtr<Transformed_RandomDistribution> Ptr;

		explicit Transformed_RandomDistribution(
			const CountedPtr<RandomDistribution<Real, N> >& distribution,
			const Matrix<Real, N, N>& transform)
			: Base(transform.width())
			, distribution_(distribution)
			, transform_(transform)
		{
			ENSURE_OP(transform.width(), ==, transform.height());
			ENSURE_OP(distribution->dimension(), ==, transform.width());
		}

		virtual ~Transformed_RandomDistribution()
		{
		}

		virtual Vector<Real, N> sample() const
		{
			Vector<Real, N> result(
				distribution_->sample() * transform_);

			return result;
		}

		virtual std::string name() const
		{
			return std::string("transformed_") + distribution_->name();
		}

	private:
		CountedPtr<RandomDistribution<Real, N> > distribution_;
		Matrix<Real, N, N> transform_;
	};

	template <typename Real, int N>
	CountedPtr<Transformed_RandomDistribution<Real, N> >
		transform(
		const PASTEL_NO_DEDUCTION((CountedPtr<RandomDistribution<Real, N> >))& distribution,
		const Matrix<Real, N, N>& transform)
	{
		return CountedPtr<Transformed_RandomDistribution<Real, N> >(
			new Transformed_RandomDistribution<Real, N>(
			distribution, transform));
	}

}

namespace Pastel
{

	template <typename Real, int N>
	class Translated_RandomDistribution
		: public RandomDistribution<Real, N>
	{
	private:
		typedef RandomDistribution<Real, N> Base;

	public:
		typedef CountedPtr<Translated_RandomDistribution> Ptr;

		explicit Translated_RandomDistribution(
			const CountedPtr<RandomDistribution<Real, N> >& distribution,
			const Vector<Real, N>& translation)
			: Base(translation.dimension())
			, distribution_(distribution)
			, translation_(translation)
		{
			ENSURE_OP(distribution->dimension(), ==, translation.dimension());
		}

		virtual ~Translated_RandomDistribution()
		{
		}

		virtual Vector<Real, N> sample() const
		{
			Vector<Real, N> result = distribution_->sample();
			result += translation_;
			return result;
		}

		virtual std::string name() const
		{
			return std::string("translated_") + distribution_->name();
		}

	private:
		CountedPtr<RandomDistribution<Real, N> > distribution_;
		Vector<Real, N> translation_;
	};

	template <typename Real, int N>
	CountedPtr<Translated_RandomDistribution<Real, N> >
		translate(
		const PASTEL_NO_DEDUCTION((CountedPtr<RandomDistribution<Real, N> >))& distribution,
		const Vector<Real, N>& translation)
	{
		return CountedPtr<Translated_RandomDistribution<Real, N> >(
			new Translated_RandomDistribution<Real, N>(
			distribution, translation));
	}

}

#endif
