#ifndef PASTEL_DIAGONAL_AXIS_H
#define PASTEL_DIAGONAL_AXIS_H

namespace Pastel
{

	typedef uint32 DiagonalAxis;

	namespace Detail_NearestDiagonalAxis
	{

		template <int N, typename Real>
		class Predicate
		{
		public:
			explicit Predicate(
				const Vector<N, Real>& data)
				: data_(data)
			{
			}

			bool operator()(integer left, integer right) const
			{
				return data_[right] < data_[left];
			}

		private:
			const Vector<N, Real>& data_;
		};

	}

	template <int N, typename Real>
	DiagonalAxis nearestDiagonalAxis(
		const Vector<N, Real>& that)
	{
		BOOST_STATIC_ASSERT(N == Dynamic || N <= 32);

		const integer dimension = that.dimension();

		PENSURE1(dimension <= 32, dimension);
		PENSURE(allGreaterEqual(that, 0));

		std::vector<integer> permutation;
		permutation.reserve(dimension);
		for (integer i = 0;i < dimension;++i)
		{
			permutation.push_back(i);
		}

		const Detail_NearestDiagonalAxis::Predicate<N, Real> 
			predicate(that);

		std::sort(permutation.begin(),
			permutation.end(), predicate);

		Real sum = that[permutation.front()];
		Real minimumCandidate2 = square(sum);
		integer minimumCandidate2Index = 0;

		for (integer i = 1;i < dimension;++i)
		{
			sum += that[permutation[i]];
			Real candidate2 = square(sum) / (i + 1);
			if (candidate2 < minimumCandidate2)
			{
				minimumCandidate2 = candidate2;
				minimumCandidateIndex = i;
			}
		}
		
		DiagonalAxis result = 0;
		for (integer i = 0;i < minimumCandidateIndex + 1;++i)
		{
			result |= (1 << permutation[i]);
		}

		return result;
	}

}

#endif
