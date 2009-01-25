#ifndef PASTELSYS_COMPILETIME_H
#define PASTELSYS_COMPILETIME_H

namespace Pastel
{

	template <int N>
	class Factorial
	{
	public:
		enum
		{
			Result = Factorial<N - 1>::Result * N
		};
	};

	template <>
	class Factorial<0>
	{
	public:
		enum
		{
			Result = 1
		};
	};

	template <int N>
	class DoubleFactorial
	{
	public:
		enum
		{
			Result = DoubleFactorial<N - 2>::Result * N
		};
	};

	template <>
	class DoubleFactorial<0>
	{
	public:
		enum
		{
			Result = 1
		};
	};

	template <>
	class DoubleFactorial<-1>
	{
	public:
		enum
		{
			Result = 1
		};
	};

}

#endif
