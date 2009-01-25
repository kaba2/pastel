#ifndef PASTELSYS_TABLEMODIFY_H
#define PASTELSYS_TABLEMODIFY_H

// TableModify is used to unroll loops when modifying
// constant-length tables for example in vector member functions.
// Idea originated from Jukka Liimatta.

namespace Pastel
{

	template <int Size, typename Modifier>
	class TableModify
	{
	public:
		template <
			typename LeftTable,
			typename RightOperand>
			static void workSingle(
			LeftTable& leftTable,
			const RightOperand& operand)
		{
			TableModify<Size - 1, Modifier>::workSingle(
				leftTable, operand);
			Modifier::work(
				leftTable[Size - 1], operand);
		}

		template <
			typename LeftTable,
			typename RightTable>
			static void workTable(
			LeftTable& leftTable,
			const RightTable& rightTable)
		{
			TableModify<Size - 1, Modifier>::workTable(
				leftTable, rightTable);
			Modifier::work(
				leftTable[Size - 1], rightTable[Size - 1]);
		}
	};

	template <typename Modifier>
	class TableModify<1, Modifier>
	{
	private:
		enum
		{
			Size = 1
		};
	public:
		template <
			typename LeftTable,
			typename RightOperand>
			static void workSingle(
			LeftTable& leftTable,
			const RightOperand& operand)
		{
			Modifier::work(
				leftTable[Size - 1], operand);
		}

		template <
			typename LeftTable,
			typename RightTable>
			static void workTable(
			LeftTable& leftTable,
			const RightTable& rightTable)
		{
			Modifier::work(
				leftTable[Size - 1], rightTable[Size - 1]);
		}
	};

	template <typename Modifier>
	class TableModify<0, Modifier>
	{
	private:
		enum
		{
			Size = 0
		};
	public:
		template <
			typename LeftTable,
			typename RightOperand>
			static void workSingle(
			LeftTable& leftTable,
			const RightOperand& operand)
		{
		}

		template <
			typename LeftTable,
			typename RightTable>
			static void workTable(
			LeftTable& leftTable,
			const RightTable& rightTable)
		{
		}
	};

	// Modifiers

	class TableModifierAssign
	{
	public:
		template <
			typename LeftOperand,
			typename RightOperand>
			static void work(
			LeftOperand& left,
			const RightOperand& right)
		{
			left = right;
		}
	};

	class TableModifierAdd
	{
	public:
		template <
			typename LeftOperand,
			typename RightOperand>
			static void work(
			LeftOperand& left,
			const RightOperand& right)
		{
			left += right;
		}
	};

	class TableModifierSubtract
	{
	public:
		template <
			typename LeftOperand,
			typename RightOperand>
			static void work(
			LeftOperand& left,
			const RightOperand& right)
		{
			left -= right;
		}
	};

	class TableModifierMultiply
	{
	public:
		template <
			typename LeftOperand,
			typename RightOperand>
			static void work(
			LeftOperand& left,
			const RightOperand& right)
		{
			left *= right;
		}
	};

	class TableModifierDivide
	{
	public:
		template <
			typename LeftOperand,
			typename RightOperand>
			static void work(
			LeftOperand& left,
			const RightOperand& right)
		{
			left /= right;
		}
	};

}

#endif
