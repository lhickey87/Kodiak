#include <filesystem>
#include <string_view>
#include "Column.hpp"
#include <charconv>
#include <tuple>
#include <variant>


namespace fs = std::filesystem;

using DataValue = std::variant<int,double, bool, std::string_view>;

std::optional<int> isInteger(std::string_view str);
std::optional<bool> isFloat(std::string_view str);

auto convertBool(std::string_view str);
std::optional<bool> isBool(std::string_view str);
//still fairly important as it will allow us to more easily form columns
std::vector<std::string_view> splitString(std::string_view str, char delim);

template<typename T>
auto convertString(std::string_view str);
//why would this be templated is the qustion?
template<typename T>
[[nodiscard]] Kodiak::Column<T> getColumn(const std::vector<std::string_view>& str, size_t colInd, size_t numColumns);


//split string
