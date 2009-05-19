#ifndef PASTEL_FUNCTIONAL_H
#define PASTEL_FUNCTIONAL_H

#include <utility>
#include <functional>
#include <string>

namespace Pastel
{

	template <typename Type>
	struct identity
		: public std::unary_function<Type, Type>
	{
		const Type& operator()(const Type& val) const
		{
			return val;
		}
	};

	template <typename Type, typename Other>
	struct select1st
		: public std::unary_function<std::pair<Type, Other>, Type>
	{
		const Type& operator()(
			const std::pair<Type, Other>& val) const
		{
			return val.first;
		}
	};

	template <typename Type>
	struct equal_to
		: public std::binary_function<Type, Type, bool>
	{
		bool operator()(const Type& left,
			const Type& right) const
		{
			return left == right;
		}
	};

	template <class T>
	struct hash
		: public std::unary_function<T, std::size_t>
	{
		std::size_t operator()(const T& val) const
		{
			return (std::size_t)val;
		}
	};

	template<class T>
	struct hash<T*>
		: public std::unary_function<T*, std::size_t>
	{
		std::size_t operator()(const T* val) const;
	};

	template <>
	struct hash<float>
		: public std::unary_function<float, std::size_t>
	{
		std::size_t operator()(float val) const;
	};

	template <>
	struct hash<double>
		: public std::unary_function<double, std::size_t>
	{
		std::size_t operator()(double val) const;
	};

	template <>
	struct hash<long double>
		: public std::unary_function<long double, std::size_t>
	{
		std::size_t operator()(long double val) const;
	};

	template <>
	struct hash<std::string>
		: public std::unary_function<std::string, std::size_t>
	{
		std::size_t operator()(const std::string& val) const;
	};

	template <>
	struct hash<std::wstring>
		: public std::unary_function<std::wstring, std::size_t>
	{
		std::size_t operator()(const std::wstring& val) const;
	};

}

#include "pastel/sys/functional.hpp"

#endif
