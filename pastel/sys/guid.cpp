#include "pastel/sys/guid.h"

#include <algorithm>

namespace Pastel
{

	Guid::Guid()
		: a_(0)
		, b_(0)
		, c_(0)
		, d_(0)
	{
	}

	Guid::Guid(
		uint32 a, uint32 b,
		uint32 c, uint32 d)
		: a_(a)
		, b_(b)
		, c_(c)
		, d_(d)
	{
	}

	Guid::Guid(const Guid& that)
		: a_(that.a_)
		, b_(that.b_)
		, c_(that.c_)
		, d_(that.d_)
	{
	}

	void Guid::swap(Guid& that)
	{
		std::swap(a_, that.a_);
		std::swap(b_, that.b_);
		std::swap(c_, that.c_);
		std::swap(d_, that.d_);
	}

	bool Guid::operator==(const Guid& that) const
	{
		return (a_ == that.a_ && b_ == that.b_ &&
			c_ == that.c_ && d_ == that.d_);
	}

	bool Guid::operator<(const Guid& that) const
	{
		if (a_ < that.a_)
		{
			return true;
		}
		if (a_ > that.a_)
		{
			return false;
		}
		if (b_ < that.b_)
		{
			return true;
		}
		if (b_ > that.b_)
		{
			return false;
		}
		if (c_ < that.c_)
		{
			return true;
		}
		if (c_ > that.c_)
		{
			return false;
		}
		if (d_ < that.d_)
		{
			return true;
		}
		if (d_ > that.d_)
		{
			return false;
		}

		// The guids are equal.
		return false;
	}

	void Guid::set(uint32 a, uint32 b,
		uint32 c, uint32 d)
	{
		a_ = a;
		b_ = b;
		c_ = c;
		d_ = d;
	}

}
