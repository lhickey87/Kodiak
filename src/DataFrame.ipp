#include "DataFrame.hpp"
#include "DataVector.hpp"
#include <filesystem>

namespace Kodiak {

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
    template<typename ColType>
    void DataFrame<I,H>::setColumn(const StringVector& strVector, size_t colInd)
    {
        DataVector dataVec;
        size_t size = (strVector.size() / numColumns_)+1;
        dataVec.reserve<ColType>(size);

        for (size_t i = colInd; i < strVector.size(); i += numColumns_) {
            dataVec.push_back(getElement<ColType>(strVector[i]));
        }

        data_[colInd] = std::move(dataVec);
    }

    template<typename I, typename H>
    auto DataFrame<I,H>::getColumnImpl(const StringVector& strVector, Kodiak::DataType columnType, size_t colInd)
    {
        switch(columnType){
            case Kodiak::DataType::Integer:{
                setColumn<int>(strVector, colInd);
            }
            break;
            case Kodiak::DataType::Bool:{
                setColumn<bool>(strVector,colInd);
            }
            break;
            case Kodiak::DataType::Float: {
                setColumn<double>(strVector, colInd);
            }
            break;
            case Kodiak::DataType::String:{
                setColumn<std::string>(strVector, colInd);
            }
            break;
        }
    }



    template<typename I, typename H>
    auto DataFrame<I,H>::read_csv(const fs::path& filePath)
    {
        std::ifstream inputFile{filePath, std::ios::in};
        std::string str(static_cast<size_t>(fs::file_size(filePath)),0);

        inputFile.read(str.data(),str.size());
        // 2. Initialize each of the columns and store them in our map
        size_t endOfRow = str.find('\n');
        if (endOfRow == std::string::npos){ std::cout << "CSV File has no header" << std::endl;}

        std::string header = str.substr(0, endOfRow);
        setColumnIndexMap(header);

        StringVector strVector = CSV::splitString(str,',');

        for (size_t i = 0; i < numColumns_; ++i){
            Kodiak::DataType colType = CSV::getType(strVector[i]);
            //call Set Column Helper with type, this will initialize our data_{}
            getColumnImpl(strVector, colType, i);
        }

    }
};
