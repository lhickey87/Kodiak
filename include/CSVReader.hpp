#include <filesystem>
#include <string_view>
#include "Column.hpp"
#include <charconv>
#include <variant>

namespace fs = std::filesystem;

namespace CSV {

    using StringVector = Kodiak::StringVector;

    template<typename ColType>
    bool isType(std::string_view str);

    int getInteger(std::string_view str);

    bool getBoolean(std::string_view str);

    template<typename ColType>
    ColType getElement(std::string_view str);

    Kodiak::DataType getColumnType(std::string_view str);

    StringVector getColumnNames(std::string_view str);

    StringVector getDataCells(std::string_view str);

    template <typename ColType>
    Kodiak::Column<ColType> getColumn(const StringVector& str, size_t colInd, size_t numColumns);
}

//split string
