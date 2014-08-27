// Description: A real number which collects statistics of its use

#ifndef PASTELSYS_STATREAL_H
#define PASTELSYS_STATREAL_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

#include <boost/operators.hpp>

namespace Pastel
{

	namespace StatReal
	{

		class Stat
		{
		public:
			enum Enum
			{
				Operations,
				Comparisons,
				Conversions,
				Size
			};
		};

		typedef Vector<integer, Stat::Enum::Size>
			StatSet;

		class GlobalStats
		{
		public:
			GlobalStats()
				: statSet_(0)
				, constructed_(0)
				, destructed_(0)
			{
			}

			void clear()
			{
#				pragma omp critical (StatReal)
				{
					statSet_.set(0);
					constructed_ = 0;
					destructed_ = 0;
				}
			}

			void report(
				const StatSet& statSet)
			{
#				pragma omp critical (StatReal)
				{
					statSet_ += statSet;
				}
			}

			void reportConstructed()
			{
#			pragma omp critical (StatReal)
				{
					++constructed_;
				}
			}

			void reportDestructed()
			{
#			pragma omp critical (StatReal)
				{
					++destructed_;
				}
			}

			integer constructed() const
			{
				return constructed_;
			}

			integer destructed() const
			{
				return destructed_;
			}

			template <Stat::Enum stat>
			integer statistic() const
			{
				return statSet_[stat];
			}

			integer statistic(integer stat) const
			{
				return statSet_[stat];
			}

			const StatSet& statistics() const
			{
				return statSet_;
			}

		private:
			StatSet statSet_;
			integer constructed_;
			integer destructed_;
		};

		GlobalStats& globalStats();

		template <typename Real>
		class Number
			: boost::ordered_euclidian_ring_operators<Number<Real> >
		{
		public:
			Number()
				: data_()
				, statSet_(0)
			{
				globalStats().reportConstructed();
			}

			Number(const Number& that)
				: data_(that.data_)
				, statSet_(0)
			{
				globalStats().reportConstructed();
			}

			Number(const Real& that)
				: data_(that)
				, statSet_(0)
			{
				globalStats().reportConstructed();
			}

			Number& operator=(const Number& that)
			{
				add<Stat::Enum::Operations>();

				data_ = that.data_;
				return *this;
			}

			~Number()
			{
				globalStats().report(statSet_);
				globalStats().reportDestructed();
			}

			operator const Real&() const
			{
				add<Stat::Enum::Conversions>();

				return data_;
			}

			Number& operator+=(const Number& that)
			{
				add<Stat::Enum::Operations>();

				data_ += that.data_;
				return *this;
			}

			Number& operator-=(const Number& that)
			{
				add<Stat::Enum::Operations>();

				data_ -= that.data_;
				return *this;
			}

			Number& operator*=(const Number& that)
			{
				add<Stat::Enum::Operations>();

				data_ *= that.data_;
				return *this;
			}

			Number& operator/=(const Number& that)
			{
				add<Stat::Enum::Operations>();

				data_ /= that.data_;
				return *this;
			}

			bool operator<(const Number& that) const
			{
				add<Stat::Enum::Comparisons>();

				return data_ < that.data_;
			}

			bool operator==(const Number& that) const
			{
				add<Stat::Enum::Comparisons>();

				return data_ == that.data_;
			}

			template <Stat::Enum stat>
			integer statistic() const
			{
				return statSet_[stat];
			}

			const StatSet& statistics() const
			{
				return statSet_;
			}

		private:
			template <int stat>
			void add() const
			{
#				pragma omp atomic
				++statSet_[stat];
			}

			Real data_;
			mutable StatSet statSet_;
		};

	}

}

#endif
