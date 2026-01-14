#include "DataFrame.hpp"
#include "DataVector.hpp"
#include <filesystem>
 namespace Kodiak {

    template<typename I, typename H>
    DataFrame<I,H>::DataFrame(size_t size)
        : numColumns_(size),
          data_(size)
        {
            nameIndMap_.reserve(size);
        }

    template<typename I, typename H>
    DataFrame<I,H>::DataFrame(std::initializer_list<ColumnEntry> columns)
        : numColumns_(columns.size())
        {
            data_.reserve(numColumns_);
            nameIndMap_.reserve(numColumns_);

            size_t index = 0;
            for (auto&& [colName, column] : columns){
                data_.emplace_back(std::move(column));
                nameIndMap_.emplace(colName, index++);
            }
        }

    template<typename I, typename H>
    DataFrame<I,H>::DataFrame(const DataFrame& other)
    {
        if (this != &other){
            nameIndMap_ = other.nameIndMap_;
            data_ = other.data_;
            numColumns_ = other.numColumns_;
        }
    }

    template<typename I, typename H>
    DataFrame<I,H>::DataFrame(DataFrame&& other)
    {
        if (this != &other){
            nameIndMap_ = std::move(other.nameIndMap_);
            data_ = std::move(other.data_);
            numColumns_ = std::move(other.numColumns_);
        }
    }

    template<typename I, typename H>
    DataFrame<I,H>& DataFrame<I,H>::operator=(const DataFrame& other)
    {
        if (this != &other){
            nameIndMap_ = other.nameIndMap_;
            data_ = other.data_;
            numColumns_ = other.numColumns_;
        }
        return *(this);
    }

    template<typename I, typename H>
    DataFrame<I,H>& DataFrame<I,H>::operator=(DataFrame&& other){
        if (this != &other){
            nameIndMap_ = std::move(other.nameIndMap_);
            data_ = std::move(other.data_);
            numColumns_ = std::move(other.numColumns_);
        }
        return *(this);
    }

    template<typename I, typename H>
    void DataFrame<I,H>::setColumnIndexMap(const std::string& line)
    {
        size_t index = 0;

        //this could be made much easier if we just split it FROM the read_csv
        auto colNames = line | std::views::split(',');

        for (auto&& field : colNames){
            nameIndMap_.emplace(std::string(field.begin(),field.end()), index++);
        }

        numColumns_ = index;
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
    DataFrame<I,H> DataFrame<I,H>::read_csv(const fs::path& filePath)
    {
        if (!CSV::isCSV(filePath)){ throw std::invalid_argument("File is Not a CSV File, cannot parse");}

        std::ifstream inputFile{filePath, std::ios::in};
        std::string str(static_cast<size_t>(fs::file_size(filePath)),0);

        inputFile.read(str.data(),str.size());

        StringVector headerRow = CSV::getColumnNames(str);

        DataFrame df(headerRow.size());

        df.setColumnIndexMap(headerRow);

        StringVector elements = CSV::getDataCells(str);

        for (size_t i = 0; i < df.numColumns_; ++i){
            Kodiak::DataType colType = CSV::getColumnType(elements[i]);
            df.getColumnImpl(elements, colType, i);
        }

        return df;
    }
};
