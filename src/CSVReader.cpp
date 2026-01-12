#include "CSVReader.hpp"
#include "Column.hpp"
#include <string_view>

std::optional<int> getInteger(std::string_view str)
{
    int result{};
    auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result);

    bool success = (ec == std::errc{} && ptr == str.data() + str.size());
    return success ? std::optional<int>{result} : std::nullopt;
}


inline std::optional<bool> getBoolean(std::string_view str)
{
    if (str == "true") return true;
    else if (str == "false") return false;
    else return std::nullopt;
}

std::tuple<Kodiak::DataType, DataValue> getElement(std::string_view str)
{
    if (auto i = getInteger(str)){
        return {Kodiak::DataType::Integer, *i};
    } else if (auto b = isBool(str)){
        return {Kodiak::DataType::Bool, *b};
    } else {
        return {Kodiak::DataType::String, str};
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
    auto [_, val] = getElement(str);
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
