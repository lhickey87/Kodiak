#include <filesystem>
#include <string_view>
#include "Column.hpp"
#include <charconv>
#include <variant>


namespace fs = std::filesystem;

using DataValue = std::variant<int,double, bool, std::string_view>;

std::optional<int> isInteger(std::string_view str);

std::optional<bool> isFloat(std::string_view str);

std::optional<bool> isBool(std::string_view str);

inline auto convertBool(std::string_view str);

std::vector<std::string_view> splitString(std::string_view str, char delim);

//returns a variant, first we find the type of
inline DataValue convertString(std::string_view str);


template <typename T>
Kodiak::Column<T> getColumn(const std::vector<std::string_view>& str, size_t colInd, size_t numColumns);


//split string
