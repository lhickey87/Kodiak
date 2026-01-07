#include <unordered_map>
#include <filesystem>
#include "Column.hpp"
#include "DataVector.hpp"


namespace Kodiak {

    using fs = std::filesystem;
    template <typename I, typename H>
    class DataFrame {
        public:

        DataFrame();

        template<typename T,size_t S>
        auto load_column(const char* name,DataVector<S>&& cols);

        bool isCSV(const fs::path& p);
        auto read_csv(const char* fileName);

        private:

        using Columns = std::vector<DataVector<>>;
        using ColDict = std::unordered_map<std::string,size_t>;

        ColDict nameIndMap_{};
        Columns data_{};
    };
}
