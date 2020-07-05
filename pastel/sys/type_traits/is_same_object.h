#ifndef PASTELSYS_IS_SAME_OBJECT_H
#define PASTELSYS_IS_SAME_OBJECT_H

#include "pastel/sys/type_traits.h"

namespace Pastel {

    template <typename T>
    constexpr bool IsPlain = 
        std::is_object_v<T> && !std::is_const_v<T> && !std::is_volatile_v<T>;

}

namespace Pastel {

    template <typename... Ts>
    struct IsSameObject_;
    
    template <typename T, typename S, typename... Ts>
    struct IsSameObject_<T, S, Ts...> {
        static constexpr const bool value = 
            std::is_same<RemoveCvRef<T>, RemoveCvRef<S>>::value && 
            IsSameObject_<S, Ts...>::value; 
    };
    
    template <typename T>
    struct IsSameObject_<T> : std::true_type {};

    template <>
    struct IsSameObject_<> : std::true_type {};

    template <typename... Ts>
    constexpr bool IsSameObject = IsSameObject_<Ts...>::value;

}

#endif
