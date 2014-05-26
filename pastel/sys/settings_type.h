// Description: Retrieves the settings type
// Documentation: settings_types.txt

#ifndef PASTELSYS_SETTINGS_TYPE_H
#define PASTELSYS_SETTINGS_TYPE_H

namespace Pastel
{

	namespace Settings_Type_
	{

		template <typename Type>
		class Settings;

		template <
			template <typename, template <typename> class> class Template,
			typename Settings_,
			template <typename> class Customization_>
		class Settings<Template<Settings_, Customization_>>
		{
		public:
			using type = Settings_;
		};

	}

	template <typename Type>
	using Settings_For = typename Settings_Type_::Settings<Type>::type;


}

#endif
