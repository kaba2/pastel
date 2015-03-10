// Description: Named parameters

#ifndef PASTELSYS_NAMED_PARAMETER_H
#define PASTELSYS_NAMED_PARAMETER_H

#define PASTEL_PARAMETER(Type, name) \
	Self& name(Type name##__) \
	{ \
		name##_ = std::move(name##__); \
		return *this; \
	} \
	Type name##_

#define PASTEL_PARAMETER_DEFAULT(Type, name, defaultValue) \
	Self& name(Type name##__) \
	{ \
		name##_ = std::move(name##__); \
		return *this; \
	} \
	\
	Type name##_ = defaultValue

#endif
