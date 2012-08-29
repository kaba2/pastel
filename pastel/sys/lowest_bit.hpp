#ifndef PASTEL_LOWEST_BIT_HPP
#define PASTEL_LOWEST_BIT_HPP

#include "pastel/sys/lowest_bit.h"

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	inline integer lowestBit(uint8 data)
	{
		if (data == 0)
		{
			return -1;
		}

		if (data & 0x0F)
		{
			if (data & 0x03)
			{
				if (data & 0x01) return 0;
				else return 1;
			}
			else
			{
				if (data & 0x04) return 2;
				else return 3;
			}
		}
		else
		{
			if (data & 0x30)
			{
				if (data & 0x10) return 4;
				else return 5;
			}
			else
			{
				if (data & 0x40) return 6;
				else return 7;
			}
		}
	}

	inline integer lowestBit(uint16 data)
	{
		if (data == 0)
		{
			return -1;
		}

		if (data & 0x00FF)
		{
			if (data & 0x000F)
			{
				if (data & 0x0003)
				{
					if (data & 0x0001) return 0;
					else return 1;
				}
				else
				{
					if (data & 0x0004) return 2;
					else return 3;
				}
			}
			else
			{
				if (data & 0x0030)
				{
					if (data & 0x0010) return 4;
					else return 5;
				}
				else
				{
					if (data & 0x0040) return 6;
					else return 7;
				}
			}
		}
		else
		{
			if (data & 0x0F00)
			{
				if (data & 0x0300)
				{
					if (data & 0x0100) return 8;
					else return 9;
				}
				else
				{
					if (data & 0x0400) return 10;
					else return 11;
				}
			}
			else
			{
				if (data & 0x3000)
				{
					if (data & 0x1000) return 12;
					else return 13;
				}
				else
				{
					if (data & 0x4000) return 14;
					else return 15;
				}
			}
		}
	}

	inline integer lowestBit(uint32 data)
	{
		if (data == 0)
		{
			return -1;
		}

		if (data & 0x0000FFFF)
		{
			if (data & 0x000000FF)
			{
				if (data & 0x0000000F)
				{
					if (data & 0x00000003)
					{
						if (data & 0x00000001) return 0;
						else return 1;
					}
					else
					{
						if (data & 0x00000004) return 2;
						else return 3;
					}
				}
				else
				{
					if (data & 0x00000030)
					{
						if (data & 0x00000010) return 4;
						else return 5;
					}
					else
					{
						if (data & 0x00000040) return 6;
						else return 7;
					}
				}
			}
			else
			{
				if (data & 0x00000F00)
				{
					if (data & 0x00000300)
					{
						if (data & 0x00000100) return 8;
						else return 9;
					}
					else
					{
						if (data & 0x00000400) return 10;
						else return 11;
					}
				}
				else
				{
					if (data & 0x00003000)
					{
						if (data & 0x00001000) return 12;
						else return 13;
					}
					else
					{
						if (data & 0x00004000) return 14;
						else return 15;
					}
				}
			}
		}
		else
		{
			if (data & 0x00FF0000)
			{
				if (data & 0x000F0000)
				{
					if (data & 0x00030000)
					{
						if (data & 0x00010000) return 16;
						else return 17;
					}
					else
					{
						if (data & 0x00040000) return 18;
						else return 19;
					}
				}
				else
				{
					if (data & 0x00300000)
					{
						if (data & 0x00100000) return 20;
						else return 21;
					}
					else
					{
						if (data & 0x00400000) return 22;
						else return 23;
					}
				}
			}
			else
			{
				if (data & 0x0F000000)
				{
					if (data & 0x03000000)
					{
						if (data & 0x01000000) return 24;
						else return 25;
					}
					else
					{
						if (data & 0x04000000) return 26;
						else return 27;
					}
				}
				else
				{
					if (data & 0x30000000)
					{
						if (data & 0x10000000) return 28;
						else return 29;
					}
					else
					{
						if (data & 0x40000000) return 30;
						else return 31;
					}
				}
			}
		}
	}

}

#endif
