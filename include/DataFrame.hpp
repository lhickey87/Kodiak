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
        auto load_column(std::string_view name,DataVector&& cols);

        auto getColNames(std::string_view str);

        void setColumnIndexMap(const std::string& str);

        bool isCSV(const fs::path& p);
        auto read_csv(const fs::path& filePath);


        template<typename ColType>
        void setColumn(const StringVector& strVector, size_t colInd);

        auto getColumnImpl(const StringVector& strVector, Kodiak::DataType columnType, size_t colInd);

        template<typename ColType>
        auto load_column(const StringVector& str);

        private:

        using Columns = std::vector<DataVector>;
        using ColDict = std::unordered_map<std::string,size_t>;

        ColDict nameIndMap_{};
        size_t numColumns_;
        Columns data_{};
    };
}
