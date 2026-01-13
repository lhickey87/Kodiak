#include <filesystem>
#include <string_view>
#include "Column.hpp"
#include <charconv>
#include <variant>


namespace fs = std::filesystem;

namespace CSV {

    using StringVector = Kodiak::StringVector;
    using DataValue = std::variant<int,double, bool, std::string_view>;

    template<typename ColType>
    bool isType(std::string_view str);

    int getInteger(std::string_view str);

    bool getBoolean(std::string_view str);

    template<typename ColType>
    DataValue getElement(std::string_view str);

    Kodiak::DataType getType(std::string_view str);

    StringVector splitString(std::string_view str, char delim);

    //returns a variant, first we find the type of
    inline DataValue convertString(std::string_view str);

    template <typename T>
    Kodiak::Column<T> getColumn(const StringVector& str, size_t colInd, size_t numColumns);

}

//split string
