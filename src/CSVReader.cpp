#include "CSVReader.hpp"
#include "Column.hpp"
#include <string_view>

namespace CSV {

    template<typename ColType>
    bool isType(std::string_view str)
    {
        ColType result{};
        auto [ptr,ec] = std::from_chars(str.data(), str.data()+str.size(),result);
        return (ec == std::errc{} && ptr == (str.data()+str.size()));
    }

    template<>
    bool isType<bool>(std::string_view str)
    {
        return (str == "false" || str == "true" ||
                str == "False" || str == "True");
    }

    template<typename ColType>
    ColType getElement(std::string_view str)
    {
        ColType result{};
        auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result);
        return result;
    }

    template<>
    std::string getElement<std::string>(std::string_view str)
    {
        return std::string(str);
    }

    template<>
    bool getElement<bool>(std::string_view str)
    {
        return (str[0] == 'T' || str[0] == 't' || str[0] == '1');
    }

    Kodiak::DataType getColumnType(std::string_view str)
    {
        if (isType<int>(str)){
            return Kodiak::DataType::Integer;
        } else if (isType<bool>(str)){
            return Kodiak::DataType::Bool;
        } else if (isType<double>(str)){
            return Kodiak::DataType::Float;
        } else {
            return Kodiak::DataType::String;
        }
    }

    StringVector getColumnNames(std::string_view str)
    {
        auto lines = str | std::views::split('\n');
        //now we will have to iterate over this, but only once?
        auto firstLine = *lines.begin();
        StringVector result;
        for (auto&& elt : firstLine | std::views::split(','))
        {
            result.emplace_back(elt.begin(),elt.end());
        }
        return result;
    }

    StringVector extractDataCells(std::string_view str)
    {
        StringVector strVector;
        auto rows = str | std::views::split('\n') | std::views::drop(1);

        for (auto&& row : rows){
            for (auto&& field : row | std::views::split(',')){
                strVector.emplace_back(field.begin(),field.end());
            }
        }

        return strVector;
    }

    //instead of this we should have a method that Does DataVector dataVec. THEN adds everything into the vector using push_back
    template<typename ColType>
    std::vector<ColType> getColumn(const StringVector& strVector, size_t colInd, size_t numColumns)
    {
        size_t size = (strVector.size() / numColumns)+1;

        std::vector<ColType> out;
        out.reserve(size);

        for (size_t i = colInd; i < strVector.size(); i += numColumns) {
            out.push_back(getElement<ColType>(strVector[i]));
        }

        return out;
    }
}
