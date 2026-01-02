#include <string>
#include <concepts>
#include <type_traits>
#include <algorithm>

namespace Kodiak {

    template <typename T>
    concept Numerical = (std::is_integral_v<T> || std::is_floating_point_v<T>)
                         && !std::same_as<T, char>
                         && !std::same_as<T, bool>;

    template <typename T>
    concept Categorical = std::is_same_v<std::string, T> ||
                          std::is_same_v<std::string_view, T>;
}
