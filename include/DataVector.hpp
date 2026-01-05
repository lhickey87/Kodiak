#pragma once
#include "Column.hpp"
#include <unordered_map>

namespace Kodiak {

    //this will be our heterogenous vector, we will have a map of const DataVector to a std::vector of type T
    template<size_t S = 0>
    class DataVector {

        DataVector();
        DataVector(DataVector&& other);

        ~DataVector();

        template<typename T>
        auto push_back(const T& v);

        template <typename T>
        [[nodiscard]] Column<T>& get_column();

        template <typename T>
        [[nodiscard]] const Column<T>& get_column() const;

        private:

        template<typename T>
        inline static std::unordered_map<const DataVector*, Column<T>> vectors_{};

    };
};
