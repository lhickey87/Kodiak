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

    Kodiak::DataType getType(std::string_view str)
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

    StringVector splitString(std::string_view str, char delim)
    {
        StringVector strVector;
        auto rows = str | std::views::split('\n') | std::views::drop(1);

        for (auto&& row : rows){
            for (auto&& field : row | std::views::split(delim)){
                strVector.emplace_back(&*field.begin(),std::ranges::distance(field));
            }
        }
        return strVector;
    }

    //instead of this we should have a method that Does DataVector dataVec. THEN adds everything into the vector using push_back
    //this will be called as we move through the actual csv
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
