#ifndef PASTELGFX_COLORADAPTER_H
#define PASTELGFX_COLORADAPTER_H

#include "pastel/gfx/gfxlibrary.h"
#include "pastel/gfx/color.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/syscommon.h"

#include <boost/static_assert.hpp>

namespace Pastel
{

	template <typename Adapter>
	class ReverseAdapter
	{
	public:
		typedef typename Adapter::Physical Logical;
		typedef typename Adapter::Logical Physical;

		Logical toLogical(const Physical& physical) const
		{
			return adapter_.toPhysical(physical);
		}

		Physical toPhysical(const Logical& logical) const
		{
			return adapter_.toLogical(logical);
		}

	private:
		Adapter adapter_;
	};

	template <typename Type>
	class IdentityAdapter
	{
	public:
		typedef Type Logical;
		typedef Type Physical;

		const Logical& toLogical(const Physical& physical) const
		{
			return physical;
		}

		const Physical& toPhysical(const Logical& logical) const
		{
			return logical;
		}
	};

	class PASTELGFX Color_Component_Adapter
	{
	public:
		typedef real32 Logical;
		typedef Color Physical;

		Color_Component_Adapter()
			: component_(0)
		{
		}

		explicit Color_Component_Adapter(
			integer component)
			: component_(component)
		{
			ENSURE1(component >= 0 && component < 3, component);
		}

		Logical toLogical(const Physical& physical) const
		{
			return physical[component_];
		}

		Physical toPhysical(const Logical& logical) const
		{
			return Physical(logical);
		}

	private:
		integer component_;
	};

	template <typename Real, typename Integer>
	class Real_Integer_Adapter
	{
	public:
		typedef Real Logical;
		typedef Integer Physical;

		Real_Integer_Adapter(
			const Integer& max)
			: max_(max)
		{
		}

		Physical toPhysical(const Real& logical) const
		{
			return quantizeUnsigned(logical, max_);
		}

		Logical toLogical(const Real& physical) const
		{
			return dequantizeUnsigned(physical, max_);
		}

	private:
		Integer max_;
	};

	template <typename Integer>
	class Bool_Integer_Adapter
	{
	public:
		typedef bool Logical;
		typedef Integer Physical;

		Logical toLogical(const Physical& physical) const
		{
			return (physical != 0);
		}

		Physical toPhysical(const Logical& logical) const
		{
			return logical ? 1 : 0;
		}
	};

	class PASTELGFX Color_ByteColor_Adapter
	{
	public:
		typedef Color Logical;
		typedef ByteColor Physical;

		Logical toLogical(const Physical& physical) const
		{
			return Logical(
				dequantizeUnsigned(physical[0], 255),
				dequantizeUnsigned(physical[1], 255),
				dequantizeUnsigned(physical[2], 255));
		}

		Physical toPhysical(const Logical& logical) const
		{
			return Physical(
				quantizeUnsigned(logical[0], 255),
				quantizeUnsigned(logical[1], 255),
				quantizeUnsigned(logical[2], 255));
		}
	};

	typedef ReverseAdapter<Color_ByteColor_Adapter> ByteColor_Color_Adapter;

	template <typename Integer,
		int RedBits, int GreenBits, int BlueBits,
		int RedFrom = 0,
		int GreenFrom = RedFrom + RedBits,
		int BlueFrom = GreenFrom + GreenBits>
	class ByteColor_Integer_Adapter
	{
	public:
		typedef ByteColor Logical;
		typedef Integer Physical;

		~ByteColor_Integer_Adapter()
		{
			BOOST_STATIC_ASSERT(RedFrom >= 0);
			BOOST_STATIC_ASSERT(GreenFrom >= 0);
			BOOST_STATIC_ASSERT(BlueFrom >= 0);
			BOOST_STATIC_ASSERT(RedBits > 0);
			BOOST_STATIC_ASSERT(GreenBits > 0);
			BOOST_STATIC_ASSERT(BlueBits > 0);
		}

		Logical toLogical(const Physical& physical) const
		{
			const uint8 red = (uint8)scaleInteger<RedBits, 8>(
				extractBits<RedFrom, RedBits>(physical));
			const uint8 green = (uint8)scaleInteger<GreenBits, 8>(
				extractBits<GreenFrom, GreenBits>(physical));
			const uint8 blue = (uint8)scaleInteger<BlueBits, 8>(
				extractBits<BlueFrom, BlueBits>(physical));

			return Logical(red, green, blue);
		}

		Physical toPhysical(const Logical& logical) const
		{
			const Integer red = scaleInteger<8, RedBits, uint32>(logical[0]);
			const Integer green = scaleInteger<8, GreenBits, uint32>(logical[1]);
			const Integer blue = scaleInteger<8, BlueBits, uint32>(logical[2]);

			return (red << RedFrom) + (green << GreenFrom) + (blue << BlueFrom);
		}
	};

	template <typename Integer,
		int RedBits, int GreenBits, int BlueBits,
		int RedFrom = 0,
		int GreenFrom = RedFrom + RedBits,
		int BlueFrom = GreenFrom + GreenBits>
	class Color_Integer_Adapter
	{
	private:
		enum
		{
			RedMax = (1 << RedBits) - 1,
			GreenMax = (1 << GreenBits) - 1,
			BlueMax = (1 << BlueBits) - 1
		};

	public:
		typedef Color Logical;
		typedef Integer Physical;

		Color_Integer_Adapter()
			: redScaling_((real32)1 / RedMax)
			, greenScaling_((real32)1 / GreenMax)
			, blueScaling_((real32)1 / BlueMax)
		{
		}

		~Color_Integer_Adapter()
		{
			BOOST_STATIC_ASSERT(RedFrom >= 0);
			BOOST_STATIC_ASSERT(GreenFrom >= 0);
			BOOST_STATIC_ASSERT(BlueFrom >= 0);
			BOOST_STATIC_ASSERT(RedBits > 0);
			BOOST_STATIC_ASSERT(GreenBits > 0);
			BOOST_STATIC_ASSERT(BlueBits > 0);
			BOOST_STATIC_ASSERT(RedFrom + RedBits <= 32);
			BOOST_STATIC_ASSERT(GreenFrom + GreenBits <= 32);
			BOOST_STATIC_ASSERT(BlueFrom + BlueBits <= 32);
		}

		Logical toLogical(const Physical& physical) const
		{
			return Logical(
				extractBits<RedFrom, RedBits>(physical) * redScaling_,
				extractBits<GreenFrom, GreenBits>(physical) * greenScaling_,
				extractBits<BlueFrom, BlueBits>(physical) * blueScaling_);
		}

		Physical toPhysical(const Logical& logical) const
		{
			const Integer red = quantizeUnsigned(logical[0], RedMax);
			const Integer green = quantizeUnsigned(logical[1], GreenMax);
			const Integer blue = quantizeUnsigned(logical[2], BlueMax);

			return (red << RedFrom) + (green << GreenFrom) + (blue << BlueFrom);
		}

	private:
		real32 redScaling_;
		real32 greenScaling_;
		real32 blueScaling_;
	};

}

#endif
