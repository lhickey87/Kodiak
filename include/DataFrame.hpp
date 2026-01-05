#include <unordered_map>
#include "Column.hpp"
#include "DataVector.hpp"

namespace Kodiak {

    template <typename I, typename H>
    class DataFrame {
        public:

        DataFrame();

        template<typename T,size_t S>
        auto load_column(const char* name,DataVector<S>&& cols);

        private:

        using Columns = std::vector<DataVector<>>;
        using ColDict = std::unordered_map<std::string,size_t>;

        ColDict nameIndMap_{};
        Columns data_{};
    };
}
