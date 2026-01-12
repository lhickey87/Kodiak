#include "CSVReader.hpp"
#include "Column.hpp"
#include <string_view>

namespace CSV {

    bool isInteger(std::string_view str)
    {
        int result{};
        auto [ptr,ec] = std::from_chars(str.data(), str.data()+str.size(),result);
        return (ec == std::errc{} && ptr == (str.data()+str.size()));
    }

    bool isFloat(std::string_view str)
    {
        double value;
        auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value);
        return ec == std::errc() && ptr == str.data() + str.size();
    }

    bool isBoolean(std::string_view str)
    {
        if (str.empty()) return false;

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
    bool getElement<bool>(std::string_view str)
    {
        return (str[0] == 'T' || str[0] == 't' || str[0] == '1');
    }

    Kodiak::DataType getType(std::string_view str)
    {
        if (isInteger(str)){
            return Kodiak::DataType::Integer;
        } else if (isBoolean(str)){
            return Kodiak::DataType::Bool;
        } else if (isFloat(str)){
            return Kodiak::DataType::Float;
        } else {
            return Kodiak::DataType::String;
        }
    }

    // This is the function implementation found in C++ 17 in Detail
    [[nodiscard]] std::vector<std::string_view> splitString(std::string_view str, char delim)
    {
        std::vector<std::string_view> output;
        for (auto first = str.begin(), second = str.begin(), last = str.end(); first != last && second != last; first = std::next(second)){
            second = std::find(first,last, delim);

            output.emplace_back(&*first, std::distance(first,second));
            if (second == last) break;
        }

        return output;
    }

    template <typename ColType>
    inline ColType convertString(std::string_view str)
    {
        auto [_, val] = getElement<ColType>(str);
        return std::get<ColType>(val);
    }

    //this will be called as we move through the actual csv
    template<typename ColType>
    std::vector<ColType> getColumn(const std::vector<std::string_view>& strVector, size_t colInd, size_t numColumns)
    {
        std::vector<size_t> indices;
        for (size_t i = colInd; i < strVector.size(); i += numColumns){ indices.push_back(i);}

        std::vector<ColType> out(indices.size());

        std::transform(std::execution::par,
                    indices.begin(),indices.end(),
                    out.begin(),
                    [&](size_t i){ return convertString<ColType>(strVector[i]);});
        return out;
    }

}
