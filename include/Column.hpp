#pragma once
#include "Types.h"
#include <stdexcept>
#include <type_traits>

#include <ranges>
#include <algorithm>
#include <vector>
#include <execution>

namespace Kodiak {


    template<typename T>
    class Column;

    template<>
    class Column<bool> {
        public:
        Column(size_t size) : data_(size), size_(size){}

        private:
        std::vector<uint64_t> data_;
        size_t size_;
    };

    template<typename T> struct is_column : std::false_type {};
    template<typename T> struct is_column<Column<T>> : std::true_type {};

    template<typename T>
    concept Column_T = is_column<Column<T>>::value;

    template<Kodiak::Numerical T>
    class Column<T> {
        public:
        using value_type = T;
        Column(size_t size) : data_(size){}

        Column(Column<T>&& other) noexcept : data_(std::exchange(other.data_,nullptr), std::exchange(other.size,0)){}
        Column(const Column<T>& other) noexcept : data_(other.data_), size(other.size){}

        Column<T> operator=(const Column<T>& rhs) noexcept {
            if (this != &rhs){
                data_ = rhs.data_;
                size = rhs.size;
            }
            return *this;
        }

        Column<T> operator=(Column<T>&& rhs) noexcept {
            if (this != &rhs){
                data_ = std::exchange(rhs.data_,nullptr);
                size = std::exchange(rhs.size,0);
            }
            return *this;
        }

        auto sum();
        auto filter(bool cond);
        auto getColumn();

        size_t getSize() const noexcept {
            return size;
        }

        const T* data() const {
            return data_.data();
        }

        const auto begin() {
            return data_.begin();
        }

        const auto end() {
            return data_.end();
        }

        template<Kodiak::Numerical U>
        void push_back(const U& v){
            data_.push_back(v);
        }

        template<Column_T LHS, Column_T RHS, typename Op>
        static auto binary_dispatch(const LHS& lhs, const RHS& rhs, Op op) {

            using Ret_T = std::common_type<typename LHS::value_type, typename RHS::value_type>;

            if (lhs.size() != rhs.size()) throw std::invalid_argument("Size mismatch");

            size_t n = lhs.size();

            Column<Ret_T> result(n);

            auto idxView = std::views::iota(size_t{0}, n);

            auto* res_ptr = result.data();

            std::transform(std::execution::par_unseq,
                idxView.begin(), idxView.end(),
                result.data(),
                [=](size_t i) {
                    return op(lhs[i], rhs[i]);
                }
            );

            return result;
        }

        template<Column_T LHS, typename RHS, typename OP>
        static auto binaryOperation(const LHS& lhs, const RHS& rhs, OP op){

            using Ret_T = std::common_type<typename LHS::value_type, RHS>;

            size_t n = lhs.size();
            Column<Ret_T> result(n);

            auto idxView = std::views::iota(size_t{0},n);

            std::transform(std::execution::par_unseq,
                idxView.begin(), idxView.end(),
                result.data(),
                [=](size_t i){ return op(lhs[i],rhs);});
        }


        template<typename LHS, Column_T RHS, typename OP>
        static auto binaryOperation(const LHS& lhs, const RHS& rhs, OP op){

            using Ret_T = std::common_type<typename LHS::value_type, RHS>;

        }

        template<Kodiak::Numerical U>
        Column<bool> operator>(const U& cmp)
        {

        }

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
        size_t size_;
    };

};
