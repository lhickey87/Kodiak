#include "DataFrame.hpp"
#include "DataVector.hpp"
#include <filesystem>

namespace Kodiak {

    template<typename I, typename H>
    template<typename T, size_t S>
    auto DataFrame<I,H>::load_column(std::string_view name, DataVector<S>&& col){

    }

    template<typename I, typename H>
    bool DataFrame<I,H>::isCSV(const fs::path& p){
        return fs::is_regular_file(p) && p.extension() == ".csv";
    }

    template<typename I, typename H>
    void DataFrame<I,H>::setColumnIndexMap(const std::string& line) {
        size_t columnIndex = 0;
        size_t start = 0;
        while (start < line.size()) {

            size_t commaPos = line.find(',', start);
            if (commaPos == std::string::npos) commaPos = line.size();

            nameIndMap_.emplace(line.substr(start, commaPos - start), columnIndex++);
            start = commaPos + 1;
        }

        numColumns_ = columnIndex;
    }

    template<typename I, typename H>
    auto DataFrame<I,H>::read_csv(const fs::path& filePath)
    {
        std::ifstream inputFile{filePath, std::ios::in};
        std::string str(static_cast<size_t>(fs::file_size(filePath)),0);

        inputFile.read(str.data(),str.size());
        // 1. Names of the Columns (then store the name to index in our map)
        // 2. Initialize each of the columns and store them in our map
        size_t endOfRow = str.find('\n');
        if (endOfRow == std::string::npos){ std::cout << "CSV File has no header" << std::endl;}

        std::string header = str.substr(0, endOfRow);

        setColumnIndexMap(header);

    }
};
