#pragma once
#include "Column.hpp"
#include <unordered_map>

namespace Kodiak {

    //this will be our heterogenous vector, we will have a map of const DataVector to a std::vector of type T
    template<size_t S = 0>
    class DataVector {

        DataVector();
        //should we be able to copy/move
        //given we don't actually dynamically manage objects this may not be the best idea
        DataVector(const DataVector& other) = delete;
        DataVector operator=(const DataVector& other) = delete;

        ~DataVector();

        template<typename T>
        auto push_back(const T& v){
            get_column<T>().push_back(v);
        }

        template<typename T>
        auto size(){
            get_column<T>().getSize();
        }

        template<typename T>
        auto emplace_back(const T& v){
            get_column<T>().push_back(v);
        }

        template <typename T>
        [[nodiscard]] Column<T>& get_column(){
            Column<T> col = vectors_<T>[this];
            return col;
        }

        template <typename T>
        [[nodiscard]] const Column<T>& get_column() const {
            const Column<T> col = vectors_<T>[this];
            return col;
        }

        private:

        template<typename T>
        inline static std::unordered_map<const DataVector*, Column<T>> vectors_{};

    };
};
