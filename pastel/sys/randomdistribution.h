#ifndef PASTEL_RANDOMDISTRIBUTION_H
#define PASTEL_RANDOMDISTRIBUTION_H

#include "pastel/sys/countedptr.h"
#include "pastel/sys/point.h"

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
