#include "CSVReader.hpp"
#include "Column.hpp"
#include <string_view>


std::optional<int> parseInt(std::string_view str)
{
    int result{};
    auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result);

    bool success = (ec == std::errc{} && ptr == str.data() + str.size());
    return success ? std::optional<int>{result} : std::nullopt;
}

auto convertBool(std::string_view str){
    bool retVal;
    retVal = (str == "true") ? true : false;
    return retVal;
}

std::optional<bool> isBool(std::string_view str)
{
    bool isbool = (str == "true" || str == "false");
    return (isbool) ? std::optional<bool>(convertBool(str)) : std::nullopt;
}

std::tuple<Kodiak::DataType, DataValue> getValue(std::string_view str)
{
    if (auto i = parseInt(str)){
        return {Kodiak::DataType::Integer, *i};
    } else if (auto b = isBool(str)){
        return {Kodiak::DataType::Bool, *b};
    } else {
        return {Kodiak::DataType::String, str};
    }
}

[[nodiscard]] std::vector<std::string_view> splitString(std::string_view str, char delim)
{

}

//question is, would we be able to do this in parallel
// could possibly use for each, with some const
auto convertString(std::string_view str)
{
    auto [type, value] = getValue(str);
    return value;
}

template<typename T>
[[nodiscard]] std::vector<T> getColumn(const std::vector<std::string_view>& str, size_t colInd, size_t numColumns)
{
    std::vector<size_t> indices;
    for (size_t i = colInd; i < str.size(); i += numColumns){ indices.push_back(i);}

    std::vector<T> out(indices.size());

    std::transform(std::execution::par,
                indices.begin(),indices.end(),
                out.begin(),
                [&](size_t i){return convertString(str[i]);});
    return out;
}
