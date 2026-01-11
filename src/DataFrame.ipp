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
    auto DataFrame<I,H>::read_csv(std::string_view fileName){
        //we will need a method that actually constructs the Columns based on the csv file
    }
};
