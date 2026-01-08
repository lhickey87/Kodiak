#include <unordered_map>
#include "Column.hpp"
#include "DataVector.hpp"
#include <filesystem>
#include <string_view>

namespace fs = std::filesystem;

namespace Kodiak {

    template <typename I, typename H>
    class DataFrame {
        public:

        DataFrame();

        template<typename T,size_t S>
        auto load_column(std::string_view name,DataVector<S>&& cols){

        }

        bool isCSV(const fs::path& p);
        auto read_csv(std::string_view fileName);

        private:

        using Columns = std::vector<DataVector<>>;
        using ColDict = std::unordered_map<std::string,size_t>;

        ColDict nameIndMap_{};
        Columns data_{};
    };
}

#include "DataFrame.ipp"
