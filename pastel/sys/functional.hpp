#ifndef PASTEL_FUNCTIONAL_HPP
#define PASTEL_FUNCTIONAL_HPP

#include "pastel/sys/functional.h"

namespace Pastel
{

	namespace Detail
	{

		inline std::size_t oneAtATimeHash(
			const char* key, std::size_t len)
		{
			// One-at-a-time hash function by Bob Jenkins,
			// http://www.burtleburtle.net/bob/hash/doobs.html.

			std::size_t hash = 0;

			for(std::size_t i = 0; i < len; i++)
			{
				hash += key[i];
				hash += (hash << 10);
				hash ^= (hash >> 6);
			}

			hash += (hash << 3);
			hash ^= (hash >> 11);
			hash += (hash << 15);

			return hash;
		}

	}

	template <class T>
	std::size_t hash<T*>::operator()(const T* val) const
	{
		return (std::size_t)val;
	}

	inline std::size_t hash<float>::operator()(float val) const
	{
		return Detail::oneAtATimeHash(
			(char*)&val, sizeof(float));
	}

	inline std::size_t hash<double>::operator()(double val) const
	{
		return Detail::oneAtATimeHash(
			(char*)&val, sizeof(double));
	}

	inline std::size_t
		hash<long double>::operator()(long double val) const
	{
		return Detail::oneAtATimeHash(
			(char*)&val, sizeof(long double));
	}

	inline std::size_t
		hash<std::string>::operator()(const std::string& val) const
	{
		return Detail::oneAtATimeHash(
			(char*)val.data(), val.size());
	}

	inline std::size_t
		hash<std::wstring>::operator()(const std::wstring& val) const
	{
		return Detail::oneAtATimeHash(
			(char*)val.data(),
			val.size() * sizeof(wchar_t));
	}

}

#endif
