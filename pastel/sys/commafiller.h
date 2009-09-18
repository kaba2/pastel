// Description: CommaFiller class
// Detail: Assigns values to an iterator range using the comma operator.

#ifndef PASTEL_COMMAFILLER_H
#define PASTEL_COMMAFILLER_H

namespace Pastel
{

	template <typename Type, typename Iterator>
	class CommaFiller
	{
	public:
		CommaFiller(
			const Iterator& iter,
			const Iterator& iterEnd)
			: iter_(iter)
			, iterEnd_(iterEnd)
		{
		}

		CommaFiller& operator=(
			const Type& that)
		{
			if (iter_ != iterEnd_)
			{
				*iter_ = that;
				++iter_;
			}

			return *this;
		}

		CommaFiller& operator,(
			const Type& that)
		{
			return (*this = that);
		}

	private:
		Iterator iter_;
		Iterator iterEnd_;
	};

	template <typename Type, typename Iterator>
	CommaFiller<Type, Iterator> commaFiller(
		const Iterator& begin,
		const Iterator& end)
	{
		return CommaFiller<Type>(begin, end);
	}

	template <typename Type, typename Iterator>
	CommaFiller<Type, Iterator> commaFiller(
		const Iterator& begin,
		const Iterator& end,
		const PASTEL_NO_DEDUCTION(Type)& that)
	{
		CommaFiller<Type, Iterator> result(
			begin, end);

		return result = that;
	}

}

#endif
