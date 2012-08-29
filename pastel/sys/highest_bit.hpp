#ifndef PASTEL_HIGHEST_BIT_HPP
#define PASTEL_HIGHEST_BIT_HPP

#include "pastel/sys/highest_bit.h"

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	inline integer highestBit(uint8 data)
	{
		if (data == 0)
		{
			return -1;
		}

		if (data & 0xF0)
		{
			if (data & 0xC0)
			{
				if (data & 0x80) return 7;
				else return 6;
			}
			else
			{
				if (data & 0x20) return 5;
				else return 4;
			}
		}
		else
		{
			if (data & 0x0C)
			{
				if (data & 0x08) return 3;
				else return 2;
			}
			else
			{
				if (data & 0x02) return 1;
				else return 0;
			}
		}
	}

	inline integer highestBit(uint16 data)
	{
		if (data == 0)
		{
			return -1;
		}

		if (data & 0xFF00)
		{
			if (data & 0xF000)
			{
				if (data & 0xC000)
				{
					if (data & 0x8000) return 15;
					else return 14;
				}
				else
				{
					if (data & 0x2000) return 13;
					else return 12;
				}
			}
			else
			{
				if (data & 0x0C00)
				{
					if (data & 0x0800) return 11;
					else return 10;
				}
				else
				{
					if (data & 0x0200) return 9;
					else return 8;
				}
			}
		}
		else
		{
			if (data & 0x00F0)
			{
				if (data & 0x00C0)
				{
					if (data & 0x0080) return 7;
					else return 6;
				}
				else
				{
					if (data & 0x0020) return 5;
					else return 4;
				}
			}
			else
			{
				if (data & 0x000C)
				{
					if (data & 0x0008) return 3;
					else return 2;
				}
				else
				{
					if (data & 0x0002) return 1;
					else return 0;
				}
			}
		}
	}

	inline integer highestBit(uint32 data)
	{
		if (data == 0)
		{
			return -1;
		}

		if (data & 0xFFFF0000)
		{
			if (data & 0xFF000000)
			{
				if (data & 0xF0000000)
				{
					if (data & 0xC0000000)
					{
						if (data & 0x80000000) return 31;
						else return 30;
					}
					else
					{
						if (data & 0x20000000) return 29;
						else return 28;
					}
				}
				else
				{
					if (data & 0x0C000000)
					{
						if (data & 0x08000000) return 27;
						else return 26;
					}
					else
					{
						if (data & 0x02000000) return 25;
						else return 24;
					}
				}
			}
			else
			{
				if (data & 0x00F00000)
				{
					if (data & 0x00C00000)
					{
						if (data & 0x00800000) return 23;
						else return 22;
					}
					else
					{
						if (data & 0x00200000) return 21;
						else return 20;
					}
				}
				else
				{
					if (data & 0x000C0000)
					{
						if (data & 0x00080000) return 19;
						else return 18;
					}
					else
					{
						if (data & 0x00020000) return 17;
						else return 16;
					}
				}
			}
		}
		else
		{
			if (data & 0x0000FF00)
			{
				if (data & 0x0000F000)
				{
					if (data & 0x0000C000)
					{
						if (data & 0x00008000) return 15;
						else return 14;
					}
					else
					{
						if (data & 0x00002000) return 13;
						else return 12;
					}
				}
				else
				{
					if (data & 0x00000C00)
					{
						if (data & 0x00000800) return 11;
						else return 10;
					}
					else
					{
						if (data & 0x00000200) return 9;
						else return 8;
					}
				}
			}
			else
			{
				if (data & 0x000000F0)
				{
					if (data & 0x000000C0)
					{
						if (data & 0x00000080) return 7;
						else return 6;
					}
					else
					{
						if (data & 0x00000020) return 5;
						else return 4;
					}
				}
				else
				{
					if (data & 0x0000000C)
					{
						if (data & 0x00000008) return 3;
						else return 2;
					}
					else
					{
						if (data & 0x00000002) return 1;
						else return 0;
					}
				}
			}
		}
	}

}

#endif
