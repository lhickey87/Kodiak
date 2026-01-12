#include <filesystem>
#include <string_view>
#include "Column.hpp"
#include <charconv>
#include <variant>


namespace fs = std::filesystem;

namespace CSV {

    using DataValue = std::variant<int,double, bool, std::string_view>;

    bool isInteger(std::string_view str);
    bool isBoolean(std::string_view str);
    bool isFloat(std::string_view str);

    int getInteger(std::string_view str);

    bool getBoolean(std::string_view str);

    template<typename ColType>
    DataValue getElement(std::string_view str);

    Kodiak::DataType getType(std::string_view str);

    std::vector<std::string_view> splitString(std::string_view str, char delim);

    //returns a variant, first we find the type of
    inline DataValue convertString(std::string_view str);

    template <typename T>
    Kodiak::Column<T> getColumn(const std::vector<std::string_view>& str, size_t colInd, size_t numColumns);

}

//split string
