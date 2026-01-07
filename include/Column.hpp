#pragma once
#include "Types.h"
#include <type_traits>

#include <execution>
#include <ranges>
#include <iterator>
#include <algorithm>
#include <vector>

using std::views = std::ranges::views;

namespace Kodiak {


    template<typename T>
    class Column;

    template<Kodiak::Numerical T>
    class Column<T> {
        public:

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

        template<typename LHS, typename RHS, typename OP>
        auto binaryOperation(const LHS& u, const RHS& v, OP&& op)
        {
            //LHS and RHS NEED to be containers, might have to make these a concept here
            //size_t size = std::min(u.size,v.size);
            auto idxView = std::views::iota(size_t{0},100);
            std::transform(std::execution::par_unseq,
                           idxView.begin(), idxView.end(),
                           )
        }


        template<Kodiak::Numerical U>
        void push_back(const U& v){
            data_.push_back(v);
        }

        template<Kodiak::Numerical U, typename Func>
        auto transform(const Column<U>& rhs, Func&& func) {
            Column<U> result(size);
            U* resultPtr = result.data();

            const U* rhsData = rhs.data();
            const U* data = data_.data();

            #pragma omp parallel for simd if(n > 1000)
            for (size_t i = 0; i < data_.size();++i){
               resultPtr[i] = func(rhsData[i],data[i]);
            }

            return result;
        }

        template<Kodiak::Numerical U,typename Func>
        auto transform(Func&& func) {
            Column<U> result(size);

            T* resultPtr = result.data();
            const T* colPtr = data_.data();
            #pragma omp parallel for simd if(n > 1000)
            for (size_t i = 0; i < size; ++i){
                resultPtr[i] = func(colPtr[i]);
            }
            return result;
        }

        template<Kodiak::Numerical U>
        friend auto operator+(const Column<T>& lhs, const U& rhs) {
            if (lhs.size() != rhs.size()){
                throw std::invalid_argument("Columns are not equal in Dimension");
            }
            auto binaryOp = [rhs](auto elt){return rhs+elt;};
            return lhs. template transform<U>(binaryOp);
        }

        template<Kodiak::Numerical U>
        friend auto operator+(const U& lhs, const Column<T>& rhs) {
            return rhs + lhs;
        }

        template<Kodiak::Numerical V, Kodiak::Numerical U>
        friend auto operator+(const Column<V>& lhs, const Column<U>& rhs) {
            if (lhs.size() != rhs.size()){
                throw std::invalid_argument("Columns are not equal in Dimension");
            }
            using res_t = std::common_type_t<U,V>;
            auto op = [](const res_t& lhs, const res_t& rhs){return lhs+rhs;};
            return lhs. template transform<res_t>(rhs,op);
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

    template<>
    class Column<bool> {
        public:
        Column(size_t size) : data_(size), size_(size){}

        private:
        std::vector<uint64_t> data_;
        size_t size_;
    };
};
