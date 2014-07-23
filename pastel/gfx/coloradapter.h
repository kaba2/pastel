// Description: Some adapters for colors

#ifndef PASTELGFX_COLORADAPTER_H
#define PASTELGFX_COLORADAPTER_H

#include "pastel/gfx/gfxlibrary.h"
#include "pastel/gfx/color.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/extract_bits.h"

namespace Pastel
{

	template <typename Adapter>
	class ReverseAdapter
	{
	public:
		using Logical = typename Adapter::Physical;
		using Physical = typename Adapter::Logical;

		ReverseAdapter()
			: adapter_()
		{
		}

		explicit ReverseAdapter(
			const Adapter& adapter)
			: adapter_(adapter)
		{
		}

		Logical convert(const Physical& physical) const
		{
			return adapter_.revert(physical);
		}

		Physical revert(const Logical& logical) const
		{
			return adapter_.convert(logical);
		}

	private:
		Adapter adapter_;
	};

	template <typename Type>
	class IdentityAdapter
	{
	public:
		using Logical = Type;
		using Physical = Type;

		const Logical& convert(const Physical& physical) const
		{
			return physical;
		}

		const Physical& revert(const Logical& logical) const
		{
			return logical;
		}
	};

	class PASTELGFX Color_To_Component
	{
	public:
		using Logical = real32;
		using Physical = Color;

		Color_To_Component()
			: component_(0)
		{
		}

		explicit Color_To_Component(
			integer component)
			: component_(component)
		{
			ENSURE1(component >= 0 && component < 3, component);
		}

		Logical convert(const Physical& physical) const
		{
			return physical[component_];
		}

		Physical revert(const Logical& logical) const
		{
			return Physical(logical);
		}

	private:
		integer component_;
	};

	template <typename Real, typename Integer>
	class Integer_To_Real
	{
	public:
		using Logical = Real;
		using Physical = Integer;

		explicit Integer_To_Real(
			integer numbers,
			Real maxValue = 1)
			: numbers_(numbers)
			, maxValue_(maxValue)
		{
		}

		Logical convert(const Physical& physical) const
		{
			return dequantizeUnsigned(physical, numbers_) * maxValue_;
		}

		Physical revert(const Logical& logical) const
		{
			return quantizeUnsigned(logical / maxValue_, numbers_);
		}

	private:
		integer numbers_;
		Real maxValue_;
	};

	template <typename Integer>
	class Integer_To_Bool
	{
	public:
		using Logical = bool;
		using Physical = Integer;

		static Logical convert(const Physical& physical)
		{
			return (physical != 0);
		}

		static Physical revert(const Logical& logical)
		{
			return logical ? 1 : 0;
		}
	};

	class PASTELGFX ByteColor_To_Color
	{
	public:
		using Logical = Color;
		using Physical = ByteColor;

		static Logical convert(const Physical& physical)
		{
			return Color(
				dequantizeUnsigned(physical[0], 256),
				dequantizeUnsigned(physical[1], 256),
				dequantizeUnsigned(physical[2], 256));
		}

		static Physical revert(const Logical& logical)
		{
			return ByteColor(
				quantizeUnsigned(logical[0], 256),
				quantizeUnsigned(logical[1], 256),
				quantizeUnsigned(logical[2], 256));
		}
	};

	using ByteColor_Color_Adapter = ReverseAdapter<ByteColor_To_Color>;

	template <typename Integer,
		int RedBits, int GreenBits, int BlueBits,
		int RedFrom = 0,
		int GreenFrom = RedFrom + RedBits,
		int BlueFrom = GreenFrom + GreenBits>
	class Integer_To_ByteColor
	{
	public:
		using Logical = ByteColor;
		using Physical = Integer;

		~Integer_To_ByteColor()
		{
			PASTEL_STATIC_ASSERT(RedFrom >= 0);
			PASTEL_STATIC_ASSERT(GreenFrom >= 0);
			PASTEL_STATIC_ASSERT(BlueFrom >= 0);
			PASTEL_STATIC_ASSERT(RedBits > 0);
			PASTEL_STATIC_ASSERT(GreenBits > 0);
			PASTEL_STATIC_ASSERT(BlueBits > 0);
		}

		static Logical convert(const Physical& physical)
		{
			uint8 red = (uint8)scaleInteger<RedBits, 8>(
				extractBits<RedFrom, RedBits>(physical));
			uint8 green = (uint8)scaleInteger<GreenBits, 8>(
				extractBits<GreenFrom, GreenBits>(physical));
			const uint8 blue = (uint8)scaleInteger<BlueBits, 8>(
				extractBits<BlueFrom, BlueBits>(physical));

			return Logical(red, green, blue);
		}


		static Physical revert(const Logical& logical)
		{
			Integer red = scaleInteger<8, RedBits, uint32>(logical[0]);
			Integer green = scaleInteger<8, GreenBits, uint32>(logical[1]);
			const Integer blue = scaleInteger<8, BlueBits, uint32>(logical[2]);

			return (red << RedFrom) + (green << GreenFrom) + (blue << BlueFrom);
		}
	};

	template <typename Integer,
		int RedBits, int GreenBits, int BlueBits,
		int RedFrom = 0,
		int GreenFrom = RedFrom + RedBits,
		int BlueFrom = GreenFrom + GreenBits>
	class Integer_To_Color
	{
	private:
		static PASTEL_CONSTEXPR int RedNumbers = 1 << RedBits;
		static PASTEL_CONSTEXPR int GreenNumbers = 1 << GreenBits;
		static PASTEL_CONSTEXPR int BlueNumbers = 1 << BlueBits;

	public:
		using Logical = Color;
		using Physical = Integer;

		Integer_To_Color()
		{
		}

		~Integer_To_Color()
		{
			PASTEL_STATIC_ASSERT(RedFrom >= 0);
			PASTEL_STATIC_ASSERT(GreenFrom >= 0);
			PASTEL_STATIC_ASSERT(BlueFrom >= 0);
			PASTEL_STATIC_ASSERT(RedBits > 0);
			PASTEL_STATIC_ASSERT(GreenBits > 0);
			PASTEL_STATIC_ASSERT(BlueBits > 0);
			PASTEL_STATIC_ASSERT(RedFrom + RedBits <= 32);
			PASTEL_STATIC_ASSERT(GreenFrom + GreenBits <= 32);
			PASTEL_STATIC_ASSERT(BlueFrom + BlueBits <= 32);
		}


		static Logical convert(const Physical& physical)
		{
			return Color(
				dequantizeUnsigned(extractBits<RedFrom, RedBits>(physical), RedNumbers),
				dequantizeUnsigned(extractBits<GreenFrom, GreenBits>(physical), GreenNumbers),
				dequantizeUnsigned(extractBits<BlueFrom, BlueBits>(physical), BlueNumbers));
		}

		static Physical revert(const Logical& logical)
		{
			Integer red = quantizeUnsigned(logical[0], RedNumbers);
			Integer green = quantizeUnsigned(logical[1], GreenNumbers);
			const Integer blue = quantizeUnsigned(logical[2], BlueNumbers);

			return (red << RedFrom) + (green << GreenFrom) + (blue << BlueFrom);
		}
	};

	template <typename Integer>
	Color integerToColor(

		const PASTEL_NO_DEDUCTION(Integer)& packedColor,
		integer redBits, integer greenBits, integer blueBits,
		integer redFrom, integer greenFrom, integer blueFrom);

	template <typename Integer>
	Color integerToColor(
		const PASTEL_NO_DEDUCTION(Integer)& packedColor,
		integer redBits, integer greenBits, integer blueBits);

	template <typename Integer>
	Integer colorToInteger(const Color& color,
		integer redBits, integer greenBits, integer blueBits,
		integer redFrom, integer greenFrom, integer blueFrom);

	template <typename Integer>
	Integer colorToInteger(const Color& color,
		integer redBits, integer greenBits, integer blueBits);

}

#include "pastel/gfx/coloradapter.hpp"

#endif
