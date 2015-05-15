// Description: Retrieves the settings type
// Documentation: settings_types.txt

#ifndef PASTELSYS_SETTINGS_TYPE_H
#define PASTELSYS_SETTINGS_TYPE_H

namespace Pastel
{

	template <typename Type>
	struct Settings_For_F;

	template <
		template <typename, template <typename> class> class Template,
		typename Settings_,
		template <typename> class Customization_>
	struct Settings_For_F<Template<Settings_, Customization_>>
	: Identity_F<Settings_>
	{};

	template <typename Type>
	using Settings_For = 
		typename Settings_For_F<Type>::type;

}

namespace Pastel
{

	template <typename Type, typename New_Settings>
	struct Replace_Settings_F;

	template <
		template <typename, template <typename> class> class Template,
		typename Settings_,
		template <typename> class Customization_,
		typename New_Settings>
	struct Replace_Settings_F<Template<Settings_, Customization_>, New_Settings>
	: Identity_F<Template<New_Settings, Customization_>>
	{};

	template <
		typename Type, 
		typename New_Settings>
	using Replace_Settings = 
		typename Replace_Settings_F<Type, New_Settings>::type;

}

#endif
