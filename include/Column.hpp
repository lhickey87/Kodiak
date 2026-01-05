#pragma once
#include "Types.h"
#include <vector>

namespace Kodiak {

    template<typename T>
    class Column;

    template<Kodiak::Numerical T>
    class Column<T> {
        public:

        Column(size_t size) : data_(size){}

        Column(Column<T>&& other) noexcept : data_(std::exchange(other.data_,nullptr), std::exchange(other.size,0)){}
        Column(const Column<T>& other) noexcept : data_(other.data_), size(other.size){}

        Column<T> operator=(const Column<T>& rhs){}
        Column<T> operator=(Column<T>&& rhs){}

        auto sum();
        auto filter(bool cond);
        auto getColumn();

        template<Kodiak::Numerical U>
        void push_back(U& v){
            data_.push_back(v);
        }

        template<typename Func>
        Column<T> transform(Func&& func) noexcept {
            Column<T> result(size);
            for (const auto& elt : data_){
                result.push_back(func(elt));
            }
            return result;
        }

        template<Kodiak::Numerical U>
        Column<T> operator+(const U& value);

        template<Kodiak::Numerical U>
        Column<T> operator+(const Column<U>& value);

        template<Kodiak::Numerical U>
        Column<bool> operator>(const U& cmp);

        template<Kodiak::Numerical U>
        Column<bool> operator>(const Column<U>& cmp);

        template<Kodiak::Numerical U>
        Column<bool> operator>=(const U& cmp);

        template<Kodiak::Numerical U>
        Column<bool> operator<(const U& cmp);

        template<typename U>
        Column<T> operator<=(const U& value);

        template<typename U>
        void operator==(const U& cmp);

        template<typename U>
        Column<bool> operator!=(const U& cmp);

        private:
        std::vector<T> data_;
        size_t size;
    };

    template<Kodiak::Categorical T>
    class Column<T> {
        public:
        auto filter();
        auto getColumn();
        Column<T> operator+(const std::string_view);

        private:
        std::vector<T> data;
    };

    template<>
    class Column<bool> {
        public:

        private:
        std::vector<uint64_t> data;
    };
};
