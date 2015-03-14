// Description: Testing for type traits
// DocumentationOf: is_template_instance.h

#include "test_pastelsys.h"

#include "pastel/sys/generic/is_template_instance.h"

using namespace Pastel;

namespace
{

	class Test
		: public TestSuite
	{
	public:
		Test()
			: TestSuite(&testReport())
		{
		}

		virtual void run()
		{
			test();
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

		void test()
		{
			PASTEL_STATIC_ASSERT((IsTemplateInstance<A0, A_Template>::value));
			PASTEL_STATIC_ASSERT((IsTemplateInstance<A1, A_Template>::value));
			PASTEL_STATIC_ASSERT((IsTemplateInstance<A2, A_Template>::value));

			PASTEL_STATIC_ASSERT((!IsTemplateInstance<B0, A_Template>::value));
			PASTEL_STATIC_ASSERT((!IsTemplateInstance<B1, A_Template>::value));
			PASTEL_STATIC_ASSERT((!IsTemplateInstance<B2, A_Template>::value));

			PASTEL_STATIC_ASSERT((IsTemplateInstance<B0, B_Template>::value));
			PASTEL_STATIC_ASSERT((IsTemplateInstance<B1, B_Template>::value));
			PASTEL_STATIC_ASSERT((IsTemplateInstance<B2, B_Template>::value));

			PASTEL_STATIC_ASSERT((!IsTemplateInstance<A0, B_Template>::value));
			PASTEL_STATIC_ASSERT((!IsTemplateInstance<A1, B_Template>::value));
			PASTEL_STATIC_ASSERT((!IsTemplateInstance<A2, B_Template>::value));
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("type_traits", test);
	}

	CallFunction run(addTest);

}
