#pragma once
#include <unordered_map>
#include <fstream>
#include "Column.hpp"
#include "DataVector.hpp"
#include "CSVReader.hpp"
#include <filesystem>
#include <string_view>
#include <iostream>

namespace fs = std::filesystem;

namespace Kodiak {

    template <typename I, typename H>
    class DataFrame {
        public:

        DataFrame();

        //WHEN read_csv is called would this be called? Likely not
        template<typename T,size_t S>
        auto load_column(std::string_view name,DataVector<S>&& cols);

        auto getColNames(std::string_view str);

        void setColumnIndexMap(const std::string& str);

        bool isCSV(const fs::path& p);
        auto read_csv(const fs::path& filePath);

        private:

        using Columns = std::vector<DataVector<>>;
        using ColDict = std::unordered_map<std::string_view,size_t>;

        ColDict nameIndMap_{};
        size_t numColumns_;
        Columns data_{};
    };
}

#include "DataFrame.ipp"
