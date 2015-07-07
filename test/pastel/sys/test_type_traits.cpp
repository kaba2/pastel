// Description: Testing for type traits
// DocumentationOf: type_traits.h

#include "test/test_init.h"

#include "pastel/sys/type_traits.h"

namespace
{

	class Test
	{
	public:
		virtual void run()
		{
			testCompiles();
			testFold();
			testIsTemplateBaseOf();
			testTemplateBase(); 
			testIsTemplateInstance();
			testAnd();
			testOr();
			testNot();
		}

		template <
			typename Type,
			typename = typename std::enable_if<!std::is_same<Type, Type>::value>::type>
		struct FailingTest;

		template <
			typename Type,
			typename = typename std::enable_if<std::is_same<Type, integer>::value>::type>
		struct IntegerTest;

		void testCompiles()
		{
			PASTEL_STATIC_ASSERT(!(Compiles<FailingTest, integer>::value));
			PASTEL_STATIC_ASSERT((Compiles<IntegerTest, integer>::value));
			PASTEL_STATIC_ASSERT(!(Compiles<IntegerTest, float>::value));
		}

		template <
			typename Left,
			typename Right>
		struct Plus
		: std::integral_constant<integer, Left::value + Right::value>
		{};

		void testFold()
		{
			using Identity = std::integral_constant<integer, 0>;
			{
				using Sum = Fold<Plus, Identity>;
			}
			{
				using Sum = Fold<Plus, Identity>;
				PASTEL_STATIC_ASSERT(Sum::value == Identity::value);
			}
			{
				using Sum = 
					Fold<Plus,
						Identity,
						std::integral_constant<integer, 1>
					>;

				PASTEL_STATIC_ASSERT(Sum::value == 1);
			}
			{
				using Sum = 
					Fold<
						Plus,
						Identity,
						std::integral_constant<integer, 1>,
						std::integral_constant<integer, 2>,
						std::integral_constant<integer, 3>
					>;

				PASTEL_STATIC_ASSERT(Sum::value == 6);
			}
		}

		template <typename...>
		class A_Template
		{};

		using A0 = A_Template<>;
		using A1 = A_Template<int>;
		using A2 = A_Template<int, float>;

		template <typename...>
		class B_Template
		{};

		using B0 = B_Template<>;
		using B1 = B_Template<int>;
		using B2 = B_Template<int, float>;

		void testIsTemplateInstance()
		{
			PASTEL_STATIC_ASSERT((IsTemplateInstance<A0, A_Template>::value));
			PASTEL_STATIC_ASSERT((IsTemplateInstance<A1, A_Template>::value));
			PASTEL_STATIC_ASSERT((IsTemplateInstance<A2, A_Template>::value));

			PASTEL_STATIC_ASSERT((IsTemplateInstance<const volatile A0&, A_Template>::value));
			PASTEL_STATIC_ASSERT((IsTemplateInstance<const volatile A1&, A_Template>::value));
			PASTEL_STATIC_ASSERT((IsTemplateInstance<const volatile A2&, A_Template>::value));

			PASTEL_STATIC_ASSERT((!IsTemplateInstance<B0, A_Template>::value));
			PASTEL_STATIC_ASSERT((!IsTemplateInstance<B1, A_Template>::value));
			PASTEL_STATIC_ASSERT((!IsTemplateInstance<B2, A_Template>::value));

			PASTEL_STATIC_ASSERT((!IsTemplateInstance<const volatile B0&, A_Template>::value));
			PASTEL_STATIC_ASSERT((!IsTemplateInstance<const volatile B1&, A_Template>::value));
			PASTEL_STATIC_ASSERT((!IsTemplateInstance<const volatile B2&, A_Template>::value));

			PASTEL_STATIC_ASSERT((IsTemplateInstance<B0, B_Template>::value));
			PASTEL_STATIC_ASSERT((IsTemplateInstance<B1, B_Template>::value));
			PASTEL_STATIC_ASSERT((IsTemplateInstance<B2, B_Template>::value));

			PASTEL_STATIC_ASSERT((IsTemplateInstance<const volatile B0&&, B_Template>::value));
			PASTEL_STATIC_ASSERT((IsTemplateInstance<const volatile B1&&, B_Template>::value));
			PASTEL_STATIC_ASSERT((IsTemplateInstance<const volatile B2&&, B_Template>::value));

			PASTEL_STATIC_ASSERT((!IsTemplateInstance<A0, B_Template>::value));
			PASTEL_STATIC_ASSERT((!IsTemplateInstance<A1, B_Template>::value));
			PASTEL_STATIC_ASSERT((!IsTemplateInstance<A2, B_Template>::value));
		}

		class C0
			: public B0
		{};

		class C1
			: public B1
		{};

		class C2
			: public B2
		{};

		void testIsTemplateBaseOf()
		{
			PASTEL_STATIC_ASSERT((IsTemplateBaseOf<B_Template, C0>::value));
			PASTEL_STATIC_ASSERT((IsTemplateBaseOf<B_Template, C1>::value));
			PASTEL_STATIC_ASSERT((IsTemplateBaseOf<B_Template, C2>::value));

			PASTEL_STATIC_ASSERT((!IsTemplateBaseOf<A_Template, C0>::value));
			PASTEL_STATIC_ASSERT((!IsTemplateBaseOf<A_Template, C1>::value));
			PASTEL_STATIC_ASSERT((!IsTemplateBaseOf<A_Template, C2>::value));

			PASTEL_STATIC_ASSERT((!IsTemplateBaseOf<A_Template, A0>::value));
			PASTEL_STATIC_ASSERT((!IsTemplateBaseOf<A_Template, A1>::value));
			PASTEL_STATIC_ASSERT((!IsTemplateBaseOf<A_Template, A2>::value));
		}

		void testTemplateBase()
		{
			PASTEL_STATIC_ASSERT((
				std::is_same<TemplateBase<B_Template, C0>, B0>::value));
			PASTEL_STATIC_ASSERT((
				std::is_same<TemplateBase<B_Template, C1>, B1>::value));
			PASTEL_STATIC_ASSERT((
				std::is_same<TemplateBase<B_Template, C2>, B2>::value));
			PASTEL_STATIC_ASSERT((
				std::is_same<TemplateBase<A_Template, C0>, void>::value));
			PASTEL_STATIC_ASSERT((
				std::is_same<TemplateBase<B_Template, A0>, void>::value));
		}

		void testAnd()
		{
			PASTEL_STATIC_ASSERT((And<>::value));

			PASTEL_STATIC_ASSERT((And<std::true_type>::value));
			PASTEL_STATIC_ASSERT((!And<std::false_type>::value));

			PASTEL_STATIC_ASSERT((!And<std::false_type, std::false_type>::value));
			PASTEL_STATIC_ASSERT((!And<std::true_type, std::false_type>::value));
			PASTEL_STATIC_ASSERT((!And<std::false_type, std::true_type>::value));
			PASTEL_STATIC_ASSERT((And<std::true_type, std::true_type>::value));

			PASTEL_STATIC_ASSERT((!And<std::false_type, std::false_type, std::false_type>::value));
			PASTEL_STATIC_ASSERT((!And<std::false_type, std::false_type, std::true_type>::value));
			PASTEL_STATIC_ASSERT((!And<std::false_type, std::true_type, std::false_type>::value));
			PASTEL_STATIC_ASSERT((!And<std::false_type, std::true_type, std::true_type>::value));
			PASTEL_STATIC_ASSERT((!And<std::true_type, std::false_type, std::false_type>::value));
			PASTEL_STATIC_ASSERT((!And<std::true_type, std::false_type, std::true_type>::value));
			PASTEL_STATIC_ASSERT((!And<std::true_type, std::true_type, std::false_type>::value));
			PASTEL_STATIC_ASSERT((And<std::true_type, std::true_type, std::true_type>::value));

			PASTEL_STATIC_ASSERT(!(And<std::is_same<void, void>, std::is_same<void, int>>::value));
		}

		void testOr()
		{
			PASTEL_STATIC_ASSERT((!Or<>::value));

			PASTEL_STATIC_ASSERT((Or<std::true_type>::value));
			PASTEL_STATIC_ASSERT((!Or<std::false_type>::value));

			PASTEL_STATIC_ASSERT((!Or<std::false_type, std::false_type>::value));
			PASTEL_STATIC_ASSERT((Or<std::true_type, std::false_type>::value));
			PASTEL_STATIC_ASSERT((Or<std::false_type, std::true_type>::value));
			PASTEL_STATIC_ASSERT((Or<std::true_type, std::true_type>::value));

			PASTEL_STATIC_ASSERT((!Or<std::false_type, std::false_type, std::false_type>::value));
			PASTEL_STATIC_ASSERT((Or<std::false_type, std::false_type, std::true_type>::value));
			PASTEL_STATIC_ASSERT((Or<std::false_type, std::true_type, std::false_type>::value));
			PASTEL_STATIC_ASSERT((Or<std::false_type, std::true_type, std::true_type>::value));
			PASTEL_STATIC_ASSERT((Or<std::true_type, std::false_type, std::false_type>::value));
			PASTEL_STATIC_ASSERT((Or<std::true_type, std::false_type, std::true_type>::value));
			PASTEL_STATIC_ASSERT((Or<std::true_type, std::true_type, std::false_type>::value));
			PASTEL_STATIC_ASSERT((Or<std::true_type, std::true_type, std::true_type>::value));
		}

		void testNot()
		{
			PASTEL_STATIC_ASSERT((Not<std::true_type>::value == false));
			PASTEL_STATIC_ASSERT((Not<std::false_type>::value == true));
		}
	};

	TEST_CASE("type_traits", "[type_traits]")
	{
	}

}
